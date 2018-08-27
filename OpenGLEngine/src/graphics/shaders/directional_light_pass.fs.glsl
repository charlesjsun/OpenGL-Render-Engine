#version 420 core

struct DirectionalLight
{
	vec3 Direction;
  
    float Ambient;
    vec3 Diffuse;
};

layout(binding = 1) uniform sampler2D NormalTexture;
layout(binding = 2) uniform sampler2D ColorTexture;
layout(binding = 4) uniform sampler2D SSAOTexture;
layout(binding = 5) uniform sampler1D MaterialsTexture;

uniform DirectionalLight DirLight;

in vec2 TexCoord;

out vec4 out_FragColor;

//out vec4 out_FragColor;

void main()
{
	
    vec4 Color = texture(ColorTexture, TexCoord);
    vec3 Normal = texture(NormalTexture, TexCoord).xyz;
    Normal = normalize(Normal);
	float ssao = texture(SSAOTexture, TexCoord).r;
	//ssao = 1.0;
	
	// Red = Ambient
	// Green = Specular Intensity
	// Blue = Shininess
	vec3 Material = texture(MaterialsTexture, Color.a).rgb;
	
	// Ambient
    vec3 ambient = DirLight.Diffuse * (DirLight.Ambient * ssao);
  	
    // Diffuse 
    vec3 lightDir = normalize(-DirLight.Direction);  
    vec3 diffuse = max(dot(Normal, lightDir), 0.0) * DirLight.Diffuse * ssao;

	
	if (Material.r > 0.0)
	{
		out_FragColor = vec4(Color.rgb * (Material.r + 1.0), 1.0);
	}
	else
	{
		vec3 final = (ambient + diffuse) * Color.rgb;
		out_FragColor = vec4(final, 1.0);
	}
			
	//vec3 final = (ambient + diffuse + Material.r) * Color.rgb;
	//out_FragColor = vec4(final, 1.0);
	
	//out_FragColor = vec4(Normal, 1);
	//out_FragColor = vec4(1, 0, 1, 1);
	//out_FragColor = vec4(vec3(Color.a) * 100, 1.0);
	//out_FragColor = vec4(ssao, ssao, ssao, 1.0);
		
		
}