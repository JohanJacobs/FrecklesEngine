#type Vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TexIndex;
layout(location = 4) in int a_TilingFactor;

out vec4 v_Color;
out vec2 v_TexCoords;

flat out int v_TexIndex;
flat out int v_TilingFactor;

void main()
{
    v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
    v_TexCoords =a_TexCoord;
	
    gl_Position = vec4(a_Position, 1.0);
}

#type Fragment
#version 460 core

out vec4 color;

in vec4 v_Color;
in vec2 v_TexCoords;
flat in int v_TexIndex;
flat in int v_TilingFactor;
uniform sampler2D u_Texture;
uniform float u_Offset;
void main()
{   
	
	vec2 offsets[9] = vec2[](
        vec2(-u_Offset	, u_Offset), // top-left
        vec2( 0.0f		, u_Offset), // top-center
        vec2( u_Offset	, u_Offset), // top-right
        vec2(-u_Offset	, 0.0f),   	 // center-left
        vec2( 0.0f		, 0.0f),   	 // center-center
        vec2( u_Offset	, 0.0f),   	 // center-right
        vec2(-u_Offset	, -u_Offset),// bottom-left
        vec2( 0.0f		, -u_Offset),// bottom-center
        vec2( u_Offset	, -u_Offset) // bottom-right    
    );
	
	float kernel[9] = float[](
        1.0/16,  2.0/16, 1.0/16,
        2.0/16,  4.0/16, 2.0/16,
        1.0/16,  2.0/16, 1.0/16
    );
	
	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_Texture, v_TexCoords.xy + offsets[i]));
    }
	
	vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

	color = vec4(col, 1.0);
}