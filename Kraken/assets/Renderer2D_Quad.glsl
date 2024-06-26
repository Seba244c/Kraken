#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in uint a_TexIndex;
layout(location = 4) in float a_TilingFactor;
layout(std140, binding = 0) uniform Camera
{
    mat4 u_mViewProjection;
};

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat uint v_TexIndex;

void main ()
{
    Output.Color = a_Color;
    Output.TexCoord = a_TexCoord;
    Output.TilingFactor = a_TilingFactor;
    v_TexIndex = a_TexIndex;
    gl_Position = u_mViewProjection * vec4(a_Position, 1.0);
}
#type fragment
#version 450 core

struct VertexOutput
{
    vec4 Color;
    vec2 TexCoord;
    float TilingFactor;
};

layout(location = 0) in VertexOutput Input;
layout(location = 3) in flat uint v_TexIndex;

layout(location = 0) out vec4 o_Color;

layout(binding = 0) uniform sampler2D u_Textures[32];

void main ()
{
    vec4 texColor = Input.Color;
    texColor *= texture(u_Textures[v_TexIndex], Input.TexCoord * Input.TilingFactor);

    if (texColor.a == 0.0)
        discard;

    o_Color = texColor;
}