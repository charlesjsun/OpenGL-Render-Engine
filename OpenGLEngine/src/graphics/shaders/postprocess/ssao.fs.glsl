#version 420 core


const int KERNEL_SIZE = 16;
const int NOISE_SIZE = 16;
const int NOISE_WIDTH = 4;
const float NEAR = 0.1;
const float FAR = 1000.0;

layout(binding = 0) uniform sampler2D DepthTexture;
layout(binding = 1) uniform sampler2D NormalTexture;
//layout(binding = 2) uniform sampler2D NoiseTexture;

uniform vec3 Kernel[KERNEL_SIZE];
uniform vec3 Noise[NOISE_SIZE];
uniform float SampleRadius;

uniform vec2 NoiseScale; // ScreenSize/NoiseSize

uniform mat4 Projection;

vec3 ReconstructPosition(in float depth, in vec2 texCoord)
{ 
	vec4 clipSpacePosition = vec4(texCoord * 2.0 - 1.0, 2.0 * depth - 1.0, 1.0);
    vec4 position = vec4(clipSpacePosition.x / Projection[0][0] , clipSpacePosition.y / Projection[1][1], -1, (clipSpacePosition.z + Projection[2][2]) / Projection[3][2]);
	return (position.xyz / position.w);
}


float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));	
}

in vec2 TexCoord;

out float out_FragColor;

void main()
{

    float Depth = texture(DepthTexture, TexCoord).r;
	vec3 ViewPos = ReconstructPosition(Depth, TexCoord);
	vec3 Normal = texture(NormalTexture, TexCoord).rgb;
	vec2 noiseCoord = TexCoord * NoiseScale;
	vec3 RandomVec = Noise[(int(noiseCoord.x) % NOISE_WIDTH) + (int(noiseCoord.y) % NOISE_WIDTH) * NOISE_WIDTH];
	//float scale = NoiseScale.x + Noise[0].x;
	//vec3 RandomVec = Noise[(int(gl_FragCoord.x) % NOISE_WIDTH) + (int(gl_FragCoord.y) % NOISE_WIDTH) * NOISE_WIDTH];
	
	
	// Create TBN change-of-basis matrix: from tangent-space to view-space
    vec3 Tangent = normalize(RandomVec - Normal * dot(RandomVec, Normal));
    vec3 Bitangent = cross(Normal, Tangent);
    mat3 TBN = mat3(Tangent, Bitangent, Normal);

	float AO = 0.0;

    for (int i = 0 ; i < KERNEL_SIZE ; i++) 
	{
	
		vec3 samplePos = TBN * Kernel[i]; // From tangent to view-space
        samplePos = ViewPos + samplePos * SampleRadius; 
        
        // project sample position (to sample texture) (to get position on screen/texture)
        vec4 offset = vec4(samplePos, 1.0);
        offset = Projection * offset; // from view to clip-space
        offset.xyz /= offset.w; // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

        //vec3 samplePos = ViewPos + Kernel[i];
        //vec4 offset = vec4(samplePos, 1.0);
        //offset = Projection * offset;
        //offset.xy /= offset.w;
        //offset.xy = offset.xy * 0.5 + 0.5;
            
        float sampleDepth = -LinearizeDepth(texture(DepthTexture, offset.xy).r);
		
		float rangeCheck = smoothstep(0.0, 1.0, SampleRadius / abs(ViewPos.z - sampleDepth));
        AO += (sampleDepth >= samplePos.z ? 1.0 : 0.0) * rangeCheck;  
    }

    AO = 1.0 - AO / KERNEL_SIZE;
	AO = pow(AO, 2.0);
	
    out_FragColor = AO;
	
	//Depth *= 77;
	//out_FragColor = vec4(Depth, Depth, Depth, 1.0);
	//out_FragColor = vec4(0.5, 0, 1, 1.0);
	
}