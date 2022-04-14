#type Vertex
#version 460 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 v_Color;
void main()
{
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
}

#type Fragment
#version 460 core
out vec4 color;
in vec4 v_Color;
void main()
{
    color = v_Color;// vec4(1.0f, 0.5f, 0.2f, 1.0f);
}