#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 2) in vec4 a_Color;

uniform mat4 u_ModelViewProjectionMatrix;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;
	gl_Position = u_ModelViewProjectionMatrix * position;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

in vec4 v_Color;

void main()
{
	color = v_Color;
};