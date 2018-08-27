#version 420 core

layout (location = 0) in vec3 in_Position; 

out vec2 TexCoord;

void main()
{          
    gl_Position = vec4(in_Position, 1.0);
	
	TexCoord = (in_Position.xy + 1.0) / 2.0;
}
