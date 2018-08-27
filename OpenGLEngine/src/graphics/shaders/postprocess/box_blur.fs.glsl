#version 420 core

in vec2 TexCoord;

out vec4 out_FragColor;

layout(binding = 0) uniform sampler2D InputTexture;

void main()
{
    vec3 Color = vec3(0.0, 0.0, 0.0);
    
    vec2 size = 1.0 / textureSize(InputTexture, 0);
    
    for (int y = -2 ; y < 2 ; ++y) 
	{
        for (int x = -2 ; x < 2 ; ++x) 
		{
            vec2 offset = vec2(float(x), float(y)) * size;
            Color += texture(InputTexture, TexCoord + offset).rgb;
        }
    }
    
    Color /= 16.0;
    
    out_FragColor = vec4(Color, 1.0);
	
	//out_FragColor = vec4(texture(InputTexture, TexCoord).rgb, 1.0);
	
}