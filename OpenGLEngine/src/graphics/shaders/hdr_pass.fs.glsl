#version 420 core

const float Gamma = 2.2;

layout(binding = 3) uniform sampler2D HDRTexture;
layout(binding = 6) uniform sampler2D BloomTexture;
uniform float Exposure;

in vec2 TexCoord;

out vec4 out_FragColor;

void main()
{

    vec3 Color = texture(HDRTexture, TexCoord).rgb;
	vec3 Bloom = texture(BloomTexture, TexCoord).rgb;
	Color += Bloom;

    // Exposure tone mapping
    vec3 result = vec3(1.0) - exp(-Color * Exposure);
    
	// Gamma correction
    result = pow(result, vec3(1.0 / Gamma));
    out_FragColor = vec4(result, 1.0);
	
	
	
	//out_FragColor = vec4(Bloom, 1.0);
	
}