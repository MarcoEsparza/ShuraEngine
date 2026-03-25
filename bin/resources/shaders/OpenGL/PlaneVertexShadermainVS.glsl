#version 430 core

out vec2 Texcoord;

void main()
{
    vec2 position[3] = vec2[](
        vec2(-1.0, -1.0),
        vec2( 3.0, -1.0),
        vec2(-1.0,  3.0)
    );

    vec2 pos = position[gl_VertexID];

    gl_Position = vec4(pos, 0.0, 1.0);

    Texcoord = (pos + vec2(1.0)) * 0.5;
    Texcoord.y = 1.0 - Texcoord.y;
}