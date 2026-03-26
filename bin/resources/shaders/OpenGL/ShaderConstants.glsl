#define PI 3.14159265358979323f
#define RECIPROCAL_PI 1.0f / PI
#define RECIPROCAL_2PI 1.0f / (2.0f * PI)
#define INV_PI 0.31830988618379067239521257108191f

// ----------------------------------------------------------------------------

layout(std140, binding = 0) uniform MainBuffer
{
  mat4 matView;
  mat4 matViewTranspose;
  mat4 matViewInverse;
  mat4 matViewInverseTranspose;
  
  mat4 matProjection;
  mat4 matProjectionTranspose;
  mat4 matProjectionInverse;
  mat4 matProjectionInverseTranspose;

  mat4 matViewProjInverse;
  mat4 matViewProjInverseTranspose;

  vec2 screenSize;
  float nearClip;
  float farClip;

  vec4 viewPos;
  vec4 viewTarget;

  float time;
  float deltaTime;
  float cosTime;
  float sinTime;
};

// ----------------------------------------------------------------------------

layout(std140, binding = 1) uniform ShaderData
{
  float sampleRad;
  float aoScale;
  float aoBias;
  float aoIntensity;

  float shadowMapSize;

  float toneMapIndex;
  float lutSize;
  float whitePoint;
  float bloomMultiplier;
  float brightThreshold;

  float minR;
  float maxR;
  float minG;
  float maxG;
  float minB;
  float maxB;

  float mipLevel0;
  float mipLevel1;

  float ssaoEnabled;
  float middleGrey;

  float cubeRoughness;
  uint cubeFaceIndex;
  float emmisiveIntensity;
  float sdPadding;
};

// ----------------------------------------------------------------------------

struct MaterialProperties
{
  int bHasBaseColor;
  int bHasSpecular;
  int bHasNormalMap;
  int bHasMetallicMap;
  int bHasRoughnessMap;
  int bHasAmbientOcclusion;
  int bHasEmissiveMap;
  int bIsOpaque;
  int bHasAlphaTest;
  int bHasAlphaBlend;
  int bIsDoubleSided;
  int bWireframeEnabled;
  int bCanCastShadow;
  int bCanReceiveShadow;
  int bInvertRoughness;
};

MaterialProperties getMaterialProperties(int materialProps)
{
  MaterialProperties props;

  props.bHasBaseColor = (materialProps & 1);
  props.bHasSpecular = (materialProps & 2);
  props.bHasNormalMap = (materialProps & 4);
  props.bHasMetallicMap = (materialProps & 8);
  props.bHasRoughnessMap = (materialProps & 16);
  props.bHasAmbientOcclusion = (materialProps & 32);
  props.bHasEmissiveMap = (materialProps & 64);
  props.bIsOpaque = (materialProps & 128);
  props.bHasAlphaTest = (materialProps & 256);
  props.bHasAlphaBlend = (materialProps & 512);
  props.bIsDoubleSided = (materialProps & 1024);
  props.bWireframeEnabled = (materialProps & 2048);
  props.bCanCastShadow = (materialProps & 4096);
  props.bCanReceiveShadow = (materialProps & 8192);
  props.bInvertRoughness = (materialProps & 16384);

  return props;
}

// ----------------------------------------------------------------------------

vec4 sRGBToLinear(vec4 color)
{
  return vec4(pow(color.rgb, vec3(1.0 / 2.2)), color.a);
}

vec4 linearToSRGB(vec4 color)
{
  return vec4(pow(color.rgb, vec3(2.2)), color.a);
}

// ----------------------------------------------------------------------------

vec2 getSkyBoxUV(vec3 dir)
{
  float u = atan(dir.z, dir.x) * RECIPROCAL_2PI + 0.5;
  float v = acos(dir.y) * RECIPROCAL_PI;
  return vec2(u, v);
}

float clampedDot(vec3 a, vec3 b)
{
  return max(0.0, dot(a, b));
}

// ----------------------------------------------------------------------------

float radicalInverse_VdC(uint bits)
{
  bits = (bits << 16u) | (bits >> 16u);
  bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
  bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
  bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
  bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
  return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley(uint i, uint N)
{
  return vec2(float(i) / float(N), radicalInverse_VdC(i));
}

// ----------------------------------------------------------------------------

vec3 importanceSampleDiffuse(vec2 Xi, vec3 N)
{
  float CosTheta = 1.0 - Xi.y;
  float SinTheta = sqrt(1.0 - CosTheta * CosTheta);
  float Phi = 2.0 * PI * Xi.x;

  vec3 H;
  H.x = SinTheta * cos(Phi);
  H.y = SinTheta * sin(Phi);
  H.z = CosTheta;

  vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
  vec3 TangentX = normalize(cross(UpVector, N));
  vec3 TangentY = cross(N, TangentX);

  return TangentX * H.x + TangentY * H.y + N * H.z;
}

// ----------------------------------------------------------------------------

vec3 importanceSampleGGX(vec2 Xi, float roughness, vec3 N)
{
  float a = roughness * roughness;

  float Phi = 2.0 * PI * Xi.x;
  float CosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
  float SinTheta = sqrt(1.0 - CosTheta * CosTheta);

  vec3 H;
  H.x = SinTheta * cos(Phi);
  H.y = SinTheta * sin(Phi);
  H.z = CosTheta;

  vec3 UpVector = abs(N.z) < 0.999 ? vec3(0,0,1) : vec3(1,0,0);
  vec3 TangentX = normalize(cross(UpVector, N));
  vec3 TangentY = cross(N, TangentX);

  return TangentX * H.x + TangentY * H.y + N * H.z;
}

float specularD(float roughness, float NoH)
{
  float r2 = roughness * roughness;
  float NoH2 = NoH * NoH;
  float a = 1.0f / (PI * r2 * pow(NoH, 4.0f));
  float b = exp((NoH2 - 1.0f) / (r2 * NoH2));

  return a * b;
}

vec4 sumSpecular(vec3 hdrPixel, float NoL, vec4 result)
{
  result.xyz += hdrPixel * NoL;
  result.w += NoL;
  return result;
}

vec4 sumDiffuse(vec3 hdrPixel, float NoL, vec4 result)
{
  result.xyz += hdrPixel * NoL;
  result.w += NoL;
  return result;
}

float t2p(float t, int nOfPixels)
{
  return (t * float(nOfPixels) - 0.5);
}

vec3 sphericalEnvMapToDirection(vec2 uv)
{
  float theta = PI * (1.0f - uv.y);
  float phi = 2.0f * PI * (0.5f - uv.x);
  return vec3(
    sin(theta) * cos(phi),
    sin(theta) * sin(phi),
    cos(theta)
  );
}

vec2 directionToSphericalEnvMap(vec3 dir)
{
  float phi = -atan(dir.y, dir.x);
  float theta = acos(clamp(dir.z, -1.0, 1.0));
  float s = 0.5 - phi * RECIPROCAL_2PI;
  float t = 1.0 - theta * RECIPROCAL_PI;

  return vec2(s, t);
}

vec3 random_pcg3d(uvec3 v)
{
  v = v * 1664525u + 1013904223u;
  v.x += v.y * v.z;
  v.y += v.z * v.x;
  v.z += v.x * v.y;
  v ^= (v >> 16u);
  v.x += v.y * v.z;
  v.y += v.z * v.x;
  v.z += v.x * v.y;
  return vec3(v) / float(0xffffffffu);
}

mat3 getNormalFrame(vec3 normal)
{
  vec3 someVec = vec3(1.0, 0.0, 0.0);
  float dd = dot(someVec, normal);

  vec3 tangent = (1.0 - abs(dd) > 1e-6) ?
                 normalize(cross(someVec, normal)) :
                 vec3(0.0, 1.0, 0.0);

  vec3 bitangent = cross(normal, tangent);

  return mat3(tangent, bitangent, normal);
}