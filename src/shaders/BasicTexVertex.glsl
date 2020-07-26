#version 460 core

in vec2 position;
in vec2 texCoords;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = texCoords;

    gl_Position = vec4(position.x, position.y, 1.0f, 1.0f);
}
