#version 420 core

struct PointLight
{
	vec3 Position;
    vec3 Diffuse;
    
	float AttenFactor;
};

layout(binding = 0) uniform sampler2D DepthTexture;
layout(binding = 1) uniform sampler2D NormalTexture;
layout(binding = 2) uniform sampler2D ColorTexture;
layout(binding = 5) uniform sampler1D MaterialsTexture;

uniform PointLight uPointLight;
uniform vec2 ScreenSize;
uniform vec4 InvertProj;


vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / ScreenSize;
}


vec3 ReconstructPosition(in float depth, in vec2 texCoord)
{ 
	vec4 clipSpacePosition = vec4(texCoord * 2.0 - 1.0, 2.0 * depth - 1.0, 1.0);
    vec4 position = vec4(InvertProj.xy * clipSpacePosition.xy, -1, InvertProj.z * clipSpacePosition.z + InvertProj.w);
    return (position.xyz / position.w);
}

out vec4 out_FragColor;

void main()
{

	vec2 TexCoord = CalcTexCoord();

	float Depth = texture(DepthTexture, TexCoord).x;
	vec3 ViewPos = ReconstructPosition(Depth, TexCoord);
	
	vec4 Color = texture(ColorTexture, TexCoord);
	
	vec3 Normal = texture(NormalTexture, TexCoord).rgb;
	Normal = normalize(Normal);
	
	// Red = Ambient
	// Green = Specular Intensity
	// Blue = Shininess
	vec3 Material = texture(MaterialsTexture, Color.a).rgb;
	
	// Diffuse
	vec3 lightDir = normalize(uPointLight.Position - ViewPos);
	vec3 diffuse = max(dot(Normal, lightDir), 0.0) * uPointLight.Diffuse;
	
	// Specular
	vec3 viewDir = normalize(-ViewPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(Normal, halfwayDir), 0.0), Material.b);
    vec3 specular = uPointLight.Diffuse * spec * Material.g;
	
	// Attenuation
	float distance = length(uPointLight.Position - ViewPos);
	//float attenuation = 1.0 / (uPointLight.Constant + uPointLight.Linear * distance + uPointLight.Quadratic * distance * distance);
	float maxLightBrightness = max(max(uPointLight.Diffuse.r, uPointLight.Diffuse.g), uPointLight.Diffuse.b);
	//float attenuation = max(maxLightBrightness - uPointLight.Quadratic * distance * distance, 0.0);
	//float attenuation = max(maxLightBrightness - uPointLight.Linear * distance, 0.0);
	float attenuation = max(maxLightBrightness * (1.25 / (1 + distance * distance / uPointLight.AttenFactor) - 0.25), 0.0);
	
	if (Material.r > 0.0)
	{
		//out_FragColor = vec4(Color.rgb, 1.0);
		out_FragColor = vec4(0, 0, 0, 1.0);
		//vec3 final = (diffuse + specular) * Color.rgb * attenuation;
		//out_FragColor = vec4(final, 1.0);
	}
	else
	{
		vec3 final = (diffuse + specular) * Color.rgb * attenuation;
		out_FragColor = vec4(final, 1.0);
	}
	
	//out_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	
}
