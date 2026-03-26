#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

#define DELTA 0.00000001f

// === Textures ===
layout(binding = 0) uniform sampler2D t_depthMap;
layout(binding = 1) uniform sampler2D t_normalMap;
layout(binding = 2) uniform sampler2D t_colorMap;
layout(binding = 3) uniform sampler2D t_propMap;
layout(binding = 4) uniform sampler2D t_ssaoMap;
layout(binding = 5) uniform sampler2D t_shadowMap;
layout(binding = 6) uniform sampler2D t_depthStencil;
layout(binding = 7) uniform sampler2D t_brdfLUT;
layout(binding = 8) uniform sampler2D t_diffIrr;
layout(binding = 9) uniform sampler2D t_skyReflect;

layout(rgba32f, binding = 0) uniform image2D t_outputMap;

// === Uniforms ===
layout(std140, binding = 2) uniform LightData
{
    vec4 LightPos;
    vec3 lightTarget;
    float lightIntensity;
    vec4 lightColor;
    mat4 lightView;
    mat4 lightProj;
};

layout(std140, binding = 3) uniform PrefilterConstants
{
    uint width;
    uint height;
    uint samples;
    float prefRoughness;
    float mipmapLevels;
    vec3 pcPadding;
};

float pcFiltering(vec2 uv, float currentDepth, float texelSize, float bias)
{
    float shadow = 0.0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(t_shadowMap, uv.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    return shadow /= 9.0f;
}

float shadowCalculation(vec4 fragPosLightSpace, float NdL)
{
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(t_shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - NdL), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    // PCF
    float texelSize = 1.0 / textureSize(t_shadowMap, 0).x;
    shadow = pcFiltering(projCoords.xy, currentDepth, texelSize, bias);

    // Check if fragment is outside the shadow map
    if(projCoords.z > 1.0) {
        return 0.0;
    }
    
    return shadow;
}

float saturate(float x) { return clamp(x, 0.0, 1.0); }

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float geomSmith(float nDotV, float nDotL, float roughness)
{
    float k = pow((roughness + 1.0), 2.0) * 0.125;
    float ik = 1.0 - k;

    nDotV = max(nDotV, DELTA);
    nDotL = max(nDotL, DELTA);

    float ggx1 = nDotV / (nDotV * ik + k);
    float ggx2 = nDotL / (nDotL * ik + k);
    return ggx1 * ggx2;
}

float D_Beckmann(float nDotH, float alpha)
{
    float alpha2 = alpha * alpha;
    float cos2 = nDotH * nDotH;
    float tan2 = (1.0 - cos2) / (cos2 + 1e-5);
    return exp(-tan2 / alpha2) / (PI * alpha2 * cos2 * cos2 + 1e-5);
}

vec3 cookTorrenceSpecular(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness, vec3 F0)
{
    vec3 H = normalize(viewDir + lightDir);

    float nDotL = saturate(dot(normal, lightDir));
    float nDotV = saturate(dot(normal, viewDir));
    float nDotH = saturate(dot(normal, H));
    float vDotH = saturate(dot(viewDir, H));

    float alpha = roughness * roughness;

    float D = D_Beckmann(nDotH, alpha);
    float G = geomSmith(nDotV, nDotL, roughness);
    vec3 F = fresnelSchlick(F0, vDotH);

    float denom = 4.0 * nDotV * nDotL + 1e-5;
    return (D * G * F) / denom;
}

vec4 getSpecularSample(vec3 reflection, float lod)
{
  float u_EnvIntensity = 1.0;

  vec2 uv = getSkyBoxUV(normalize(reflection));
  vec4 texSample = textureLod(t_skyReflect, uv, lod);

  texSample.rgb *= u_EnvIntensity;
  return vec4(texSample.rgb, 1.0);
}

vec3 getIBLRadianceGGX(vec3 n, vec3 v, float roughness)
{
  float NdotV = clamp(dot(n, v), 0.0, 1.0);

  int width = textureSize(t_skyReflect, 0).x;
  float levels = float(textureQueryLevels(t_skyReflect));

  float lod = min(roughness * float(width), levels);

  vec3 reflection = normalize(reflect(-v, n));
  vec4 specularSample = getSpecularSample(reflection, lod);

  return specularSample.rgb;
}

vec3 getIBLGGXFresnel(vec3 n, vec3 v, float roughness, vec3 F0, float specularWeight)
{
  // Roughness dependent Fresnel
  float nDotV = clamp(dot(n, v), 0.0, 1.0);

  vec2 brdfSamplePoint = clamp(vec2(nDotV, 1.0 - roughness), 0.0, 1.0);
  vec2 f_ab = textureLod(t_brdfLUT, brdfSamplePoint, 0.0).rg;

  vec3 Fr = max(vec3(1.0 - roughness), F0) - F0;
  vec3 kS = F0 + Fr * pow(1.0 - nDotV, 5.0);

  vec3 FssEss = specularWeight * (kS * f_ab.x + vec3(f_ab.y));

  // Multiple scattering
  float Ems = (1.0 - (f_ab.x + f_ab.y));
  vec3 F_avg = specularWeight * (F0 + (vec3(1.0) - F0) / 21.0);
  vec3 FmsEms = Ems * FssEss * F_avg / (vec3(1.0) - F_avg * Ems);

  return FssEss + FmsEms;
}

vec3 getDiffuseLight(vec3 n)
{
  float envIntensity = 1.0f;
  vec2 dir = getSkyBoxUV(n);
  vec3 texSample = texture(t_diffIrr, dir).rgb;
  texSample *= envIntensity;
  return texSample;
}

void main()
{
    uvec3 dtID = gl_GlobalInvocationID;

    if (dtID.x >= uint(screenSize.x) || dtID.y >= uint(screenSize.y))
        return;

    ivec2 coord = ivec2(dtID.xy);

    vec4 depth = texelFetch(t_depthMap, coord, 0);
    vec4 normalMap = texelFetch(t_normalMap, coord, 0);
    vec4 color = texelFetch(t_colorMap, coord, 0);
    vec4 propMap = texelFetch(t_propMap, coord, 0);
    vec4 ssaoMap = texelFetch(t_ssaoMap, coord, 0);

    vec3 albedo = color.rgb;
    vec3 normal = normalMap.xyz;
    float ao = propMap.r;
    float roughness = propMap.g;
    float metalness = propMap.b;

    if (color.a < 0.5)
    {
        imageStore(t_outputMap, coord, vec4(1.0, 1.0, 1.0, 0.0));
        return;
    }

    normal = normal * 2.0 - 1.0;

    vec3 posWorld = depth.xyz;
    vec3 viewDir = normalize(viewPos.xyz - posWorld.xyz);

    vec3 F0 = mix(vec3(0.04), albedo, metalness);

    // === IBL ===
    vec3 R = reflect(-viewDir, normal);
    float nDotV = saturate(dot(normal, viewDir));

    vec3 diffuseIBL = getDiffuseLight(normal) * (albedo / PI);

    vec3 specularMetal = getIBLRadianceGGX(normal, viewDir, roughness);
    vec3 metalFresnel = getIBLGGXFresnel(normal, viewDir, roughness, F0, 1.0);

    metalFresnel *= specularMetal;
    vec3 ambientLight = diffuseIBL + metalFresnel;

    // === Direct Light ===
    vec3 lightDir = normalize(LightPos.xyz - posWorld);
    float NdL = saturate(dot(normal, lightDir));

    vec3 specular = cookTorrenceSpecular(normal, viewDir, lightDir, roughness, F0);

    vec3 directLight = (specular + albedo) * NdL * lightIntensity;

    // === AO ===
    vec4 lightWorldPos = (lightView * lightProj) * vec4(posWorld, 1.0);
    float shadow = shadowCalculation(lightWorldPos, NdL);

    float ssao = 1.0;
    if(ssaoEnabled == 1.0)
    {
      ssao = ssaoMap.r;
    }
    float totalAO = ao * ssao;

    vec3 ambient = ambientLight * totalAO;
    vec3 direct = directLight * (1.0 - shadow);
    vec3 finalColor = ambient + direct;

    imageStore(t_outputMap, coord, vec4(finalColor, 1.0));
}