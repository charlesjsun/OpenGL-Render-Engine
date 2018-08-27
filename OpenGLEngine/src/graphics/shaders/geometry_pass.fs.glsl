#version 420 core

const float MAX_MATERIAL_SIZE = 256.0;

//layout (location = 0) out vec3 outPosition;
layout (location = 0) out vec3 out_Normal;
layout (location = 1) out vec4 out_Color;

//in vec3 fragPosition;
in vec3 FragNormal;
in vec4 FragColor;

void main()
{    
    // Store the fragment position vector in the first gbuffer texture
    //outPosition = fragPosition;
    // Also store the per-fragment normals into the gbuffer
    out_Normal = FragNormal;
    // And the diffuse per-fragment color
    out_Color.rgb = FragColor.rgb;
	out_Color.a = FragColor.a / MAX_MATERIAL_SIZE;
}