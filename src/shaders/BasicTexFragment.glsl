#version 460 core

in vec2 v_TexCoords;

out vec4 outColor;

uniform sampler2D tex;

void main()
{
    outColor = texture(tex, v_TexCoords);
}
