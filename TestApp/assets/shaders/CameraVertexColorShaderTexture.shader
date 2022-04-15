#type Vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_Scale;
layout(location = 3) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoords;

void main()
{
    v_Color = a_Color;
    v_TexCoords = a_TexCoord;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 460 core

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;

uniform sampler2D Texture;

void main()
{
    color = texture(Texture, v_TexCoords) * v_Color;
}