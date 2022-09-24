#version 430 core

in VSOutput
{
	vec3 worldPos;
} fInput;

out vec4 fragColour;

uniform samplerCube environmentMap;
uniform float roughness;

const float pi = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float aSq = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotHSq = NdotH * NdotH;

	float numerator = aSq;
	float denominator = (NdotHSq * (aSq - 1.0) + 1.0);
	denominator = pi * denominator * denominator;

	return numerator / denominator;
}

float RadicalInverseVanDerCorpus(uint bits)
{
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
	return float(bits) * 2.3283064365386963e-10; // 0x100000000
}

vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i) / float(N), RadicalInverseVanDerCorpus(i));
}

vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness * roughness;

	float phi = 2.0 * pi * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;

	vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);

	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}

void main()
{
	vec3 N = normalize(fInput.worldPos);
	vec3 R = N;
	vec3 V = R;

	const uint sampleCount = 1024u;
	vec3 prefilteredColour = vec3(0.0);
	float totalWeight = 0.0;

	for (uint i = 0u; i < sampleCount; i++)
	{
		vec2 Xi = Hammersley(i, sampleCount);
		vec3 H = ImportanceSampleGGX(Xi, N, roughness);
		vec3 L = normalize(2.0 * dot(V, H) * H - V);

		float NdotL = max(dot(N, L), 0.0);
		if (NdotL > 0.0)
		{
			float D = DistributionGGX(N, H, roughness);
			float NdotH = max(dot(N, H), 0.0);
			float HdotV = max(dot(H, V), 0.0);
			float pdf = D * NdotH / (4.0 * HdotV) + 0.0001;

			float resolution = 512.0; // Height/width of cubemap face
			float saTexel = 4.0 * pi / (6.0 * resolution * resolution);
			float saSample = 1.0 / (float(sampleCount) * pdf + 0.0001);

			float mipLevel = roughness == 0 ? 0.0 : 0.5 * log2(saSample / saTexel);

			prefilteredColour += textureLod(environmentMap, L, mipLevel).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

	prefilteredColour = prefilteredColour / totalWeight;

	fragColour = vec4(prefilteredColour, 1.0);
}