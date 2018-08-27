#version 420 core

in vec2 TexCoord;

out vec4 out_FragColor;

layout(binding = 0) uniform sampler2D InputTexture;

void main()
{
	
	vec3 Color = texture(InputTexture, TexCoord).rgb;
	
    float brightness = dot(Color, vec3(0.2126, 0.7152, 0.0722));
	out_FragColor = vec4(0, 0, 0, 1);
    if(brightness > 1.0)
        out_FragColor = vec4(Color, 1.0);
	
}