#version 460 core

in VSOutput
{
    vec2 texCoords;
    vec3 worldPos;
    vec3 normal;
} fInput;

out vec4 fragColour;

struct Material
{
    sampler2D albedo0;
    sampler2D normal0;
    sampler2D metallic0;
    sampler2D roughness0;
    sampler2D ao0;
    float test;
};

uniform Material material;

// Light params
uniform vec3 lightPositions[4];
uniform vec3 lightColours[4];
uniform uint noLights;

uniform vec3 camPos;

const float pi = 3.14159265359;

float DistributionGGX(vec3 normal, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float aSq = a * a;
    float NdotH = max(dot(normal, H), 0.0);
    float NdotHSq = NdotH * NdotH;

    float numerator = aSq;
    float denominator = (NdotHSq * (aSq - 1.0) + 1.0);
    denominator = pi * denominator * denominator;
    return numerator / denominator;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = r * r / 8.0;

    float numerator = NdotV;
    float denominator = NdotV * (1.0 - k) + k;
    return numerator / denominator;
}

float GeometrySmith(vec3 normal, vec3 vertexToCamera, vec3 vertexToLight, float roughness)
{
    float NdotV = max(dot(normal, vertexToCamera), 0.0);
    float NdotL = max(dot(normal, vertexToLight), 0.0);

    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CookTorranceBRDF(vec3 normal, float roughness, vec3 vertexToCamera, vec3 vertexToLight, vec3 F0, out vec3 F)
{
    vec3 halfway = normalize(vertexToCamera + vertexToLight);
    
    float NDF = DistributionGGX(vertexToCamera, halfway, roughness);
    float G = GeometrySmith(normal, vertexToCamera, vertexToLight, roughness);
    F = FresnelSchlick(max(dot(halfway, vertexToCamera), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, vertexToCamera), 0.0) * max(dot(normal, vertexToLight), 0.0) + 0.0001; // Adding 0.0001 prevents division by 0
    return numerator / denominator;
}

vec3 ReflectanceEquation(vec3 albedo, float metallic, float roughness, float ao, vec3 normal)
{
    vec3 vertexToCamera = normalize(camPos - fInput.worldPos);

    // Reflectance changes based on metallicity:
    // if metal, use albedo colour as F0
    // if dia-electric, use 0.04 as F0
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for (int i = 0; i < noLights; i++) // For each light
    {
        // Calculate per-light radiance
        vec3 vertexToLight = normalize(lightPositions[i] - fInput.worldPos);
        float distance = length(vertexToLight);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColours[i] * attenuation;

        // Cook-Torrance BRDF
        vec3 F;
        vec3 specular = CookTorranceBRDF(normal, roughness, vertexToCamera, vertexToLight, F0, F);

        vec3 kS = F; // kS == Fresnel

        // PBR needs to adhere to energy conservation laws, so diffuse + specular can't exceed 1.0 (excluding light emitters)
        // This means we can find the diffuse by subtraction the specular from 1.0
        vec3 kD = vec3(1.0) - kS;

        // Multiplying diffuse by the inverse metallic value means only non-metals have diffuse lighting,
        // or a linear blend if partly metal. Pure metals have no diffuse
        kD *= 1.0 - metallic;

        // Scale light by NdotL
        float NdotL = max(dot(normal, vertexToLight), 0.0);

        // Do final reflecatance equation
        // Add result to final value, outgoing radiance Lo
        Lo += (kD * albedo / pi + specular) * radiance * NdotL;
    }

    return Lo;
}

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(material.normal0, fInput.texCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(fInput.worldPos);
    vec3 Q2  = dFdy(fInput.worldPos);
    vec2 st1 = dFdx(fInput.texCoords);
    vec2 st2 = dFdy(fInput.texCoords);

    vec3 N   = normalize(fInput.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
	vec3 albedo = pow(texture(material.albedo0, fInput.texCoords).rgb, vec3(2.2));
	float metallic = texture(material.metallic0, fInput.texCoords).r;
	float roughness = texture(material.roughness0, fInput.texCoords).r;
	float ao = texture(material.ao0, fInput.texCoords).r;
	vec3 normal = GetNormalFromMap();

    vec3 Lo = ReflectanceEquation(albedo, metallic, roughness, ao, normal);

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 colour = ambient + Lo;

    colour = colour / (colour + vec3(1.0)); // HDR
    colour = pow(colour, vec3(1.0 / 2.2)); // Gamma correction

	fragColour = vec4(colour, 1.0);

    if (material.test == 1.0)
    {
        fragColour = vec4(0.1, 0.1, 0.1, 1.0);
    }
}