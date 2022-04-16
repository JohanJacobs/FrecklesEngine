#type Vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TexIndex;
layout(location = 4) in int a_TilingFactor;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoords;
flat out int v_TexIndex;
flat out int v_TilingFactor;

void main()
{
    v_Color = a_Color;
    v_TexCoords = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type Fragment
#version 460 core

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;
flat in int v_TexIndex;
flat in int v_TilingFactor;

uniform sampler2D u_Texture;

void main()
{
	vec4 s = texture(u_Texture, v_TexCoords * v_TilingFactor) * v_Color;	
    float c = (0.299 * s.r) + (0.587 * s.g) + (0.114 * s.b);
	color = vec4(c,c,c,1.0f);
	
}