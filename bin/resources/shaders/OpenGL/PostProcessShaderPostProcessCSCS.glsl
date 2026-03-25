#version 430 core

#include "ShaderConstants.glsl"

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform sampler2D t_inputMap;
layout(binding = 0, rgba32f) uniform image2D t_outputMap;

void main()
{
    uvec2 id = gl_GlobalInvocationID.xy;

    if (id.x >= uint(screenSize.x) || id.y >= uint(screenSize.y))
        return;

    vec3 color = texelFetch(t_inputMap, ivec2(id), 0).rgb;

    color.r = mix(minR, maxR, color.r);
    color.g = mix(minG, maxG, color.g);
    color.b = mix(minB, maxB, color.b);

    imageStore(t_outputMap, ivec2(id), vec4(color, 1.0));
}