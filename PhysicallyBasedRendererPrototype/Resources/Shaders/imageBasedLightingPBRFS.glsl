#version 430 core

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
};

uniform Material material;

// Image-based lighting maps
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

struct PointLight
{
    vec3 position;
    vec3 emittedColour;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 emittedColour;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    vec3 emittedColour;
};

#define MAX_POINT 4
#define MAX_DIR 2
#define MAX_SPOT 4

uniform uint noPoint;
uniform uint noDirectional;
uniform uint noSpot;

uniform PointLight pointLights[MAX_POINT];
uniform DirectionalLight directionalLights[MAX_DIR];
uniform SpotLight spotLights[MAX_SPOT];

uniform vec3 lightPositions[4];
uniform vec3 lightColours[4];
uniform uint noLights;

uniform vec3 camPos;

const float pi = 3.14159265359;

// Normal distribution function (Trowbridge-Reitz GGX): approximates the alignment of microfacets matching the halfway vector
// Equation: a^2 / pi * ((normal . halfway)^2 * (a^2 - 1) + 1)^2
float NormalDistributionGGX(vec3 normal, vec3 halfway, float roughness)
{
    float a = roughness * roughness; // Square the roughness before using it as 'a' for better results
    float aSq = a * a;
    float NdotH = max(dot(normal, halfway), 0.0);
    float NdotHSq = NdotH * NdotH;

    float numerator = aSq;
    float denominator = (NdotHSq * (aSq - 1.0) + 1.0);
    denominator = pi * denominator * denominator;
    return numerator / denominator;
}

// Geometry function (Schlick-GGX): approximates self-shadowing caused by misaligned microfacets
// Equation: normal . v / (normal . v) * (1 - k) + k
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = r * r / 8.0;

    float numerator = NdotV;
    float denominator = NdotV * (1.0 - k) + k;
    return numerator / denominator;
}

// Geometry function (Smith): takes both the view direction and light direction into account when approximating self-shadowing. Extends another geometry function
// Equation: Geometry(normal, vertexToCamera, k) * Geometry(normal, vertexToLight, k)
float GeometrySmith(vec3 normal, vec3 vertexToCamera, vec3 vertexToLight, float roughness)
{
    float NdotC = max(dot(normal, vertexToCamera), 0.0);
    float NdotL = max(dot(normal, vertexToLight), 0.0);

    float ggx2 = GeometrySchlickGGX(NdotC, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

// Fresnel equation (Schlick): Calculates ratio of light reflected vs refracted
// Equation: F0 + (1 - F0) * (1 - (halfway . v))^5
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 CookTorranceBRDF(vec3 normal, float roughness, vec3 vertexToCamera, vec3 vertexToLight, vec3 F0, out vec3 F)
{
    vec3 halfway = normalize(vertexToCamera + vertexToLight);
    
    float NDF = NormalDistributionGGX(normal, halfway, roughness);
    float G = GeometrySmith(normal, vertexToCamera, vertexToLight, roughness);
    F = FresnelSchlick(max(dot(halfway, vertexToCamera), 0.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, vertexToCamera), 0.0) * max(dot(normal, vertexToLight), 0.0) + 0.0001; // Adding 0.0001 prevents division by 0
    return numerator / denominator;
}

vec3 Reflectance(vec3 albedo, float metallic, float roughness, float ao, vec3 normal, vec3 F0, vec3 emittedColour, vec3 vertexToLight, float attenuation)
{
    vec3 vertexToCamera = normalize(camPos - fInput.worldPos);
    vec3 radiance = emittedColour * attenuation;

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

    // Do final reflectance equation
    // Add result to final value, outgoing radiance Lo
    return (kD * albedo / pi + specular) * radiance * NdotL;
}

vec3 ReflectanceEquation(vec3 albedo, float metallic, float roughness, float ao, vec3 normal)
{
    // Reflectance changes based on metallicity:
    // if metal, use albedo colour as F0
    // if dia-electric, use 0.04 as F0
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    for (int i = 0; i < noPoint; i++) // For each point light
    {
        // Precalculate values that differentiate based on light type
        vec3 vertexToLight = normalize(pointLights[i].position - fInput.worldPos);
        float distance = length(vertexToLight);
        float attenuation = 1.0 / (distance * distance);

        Lo += Reflectance(albedo, metallic, roughness, ao, normal, F0, pointLights[i].emittedColour, vertexToLight, attenuation);
    }

    for (int i = 0; i < noDirectional; i++) // For each directional light
    {
        // Precalculate values that differentiate based on light type
        vec3 vertexToLight = normalize(-directionalLights[i].direction);
        float attenuation = 1.0f; // Directional lights are infinite in length, so they don't fall off

        Lo += Reflectance(albedo, metallic, roughness, ao, normal, F0, directionalLights[i].emittedColour, vertexToLight, attenuation);
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

    // Construct TBN matrix
    vec3 N   = normalize(fInput.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    // Retrieve map values
	vec3 albedo = pow(texture(material.albedo0, fInput.texCoords).rgb, vec3(2.2));
	float metallic = texture(material.metallic0, fInput.texCoords).r;
	float roughness = texture(material.roughness0, fInput.texCoords).r;
	float ao = texture(material.ao0, fInput.texCoords).r;
	vec3 normal = GetNormalFromMap();

    // Do reflectance equation
    vec3 Lo = ReflectanceEquation(albedo, metallic, roughness, ao, normal);

    // Calculate ambient lighting    
    vec3 vertexToCamera = normalize(camPos - fInput.worldPos);
    vec3 F0 = vec3(0.04);
    vec3 F = FresnelSchlickRoughness(max(dot(normal, vertexToCamera), 0.0), F0, roughness);

    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse = irradiance * albedo;

    const float maxReflectionLOD = 4.0;
    vec3 R = reflect(-vertexToCamera, normal);
    vec3 prefilteredColour = textureLod(prefilterMap, R, roughness * maxReflectionLOD).rgb;
    vec2 brdf = texture(brdfLUT, vec2(max(dot(normal, vertexToCamera), 0.0), roughness)).rg;
    vec3 specular = prefilteredColour * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 colour = ambient + Lo;

    colour = colour / (colour + vec3(1.0)); // HDR
    colour = pow(colour, vec3(1.0 / 2.2)); // Gamma correction

	fragColour = vec4(colour, 1.0);
}