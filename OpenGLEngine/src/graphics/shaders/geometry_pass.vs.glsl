#version 420 core

layout (location = 0) in vec3 in_Position; 
layout (location = 1) in vec3 in_Normal; 
layout (location = 2) in vec4 in_Color; 

uniform mat4 MVP;
uniform mat3 Normal;

//out vec3 fragPosition; 
out vec3 FragNormal; 
out vec4 FragColor; 

void main()
{ 

    gl_Position = MVP * vec4(in_Position, 1.0);
    
    FragNormal = Normal * in_Normal;
	//fragNormal = mat3(model) * normal;
	
    FragColor = in_Color;
	//FragColor = vec4(Normal * in_Normal, 1.0);

}