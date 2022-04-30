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
};

uniform Material material;

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

struct LightingResult
{
	vec4 Diffuse;
	vec4 Specular;
};

uniform vec3 camPos;

vec4 DoDiffuse(vec3 colour, vec3 vertexToLight, vec3 normal)
{
    float NdotL = max(0, dot(normal, vertexToLight));

    return vec4(colour * NdotL, 1.0);
}

vec4 DoSpecular(vec3 vertexToEye, vec3 vertexToLight, vec3 normal)
{
    float lightIntensity = clamp(dot(normal, vertexToLight), 0, 1);
    vec4 specular = vec4(0, 0, 0, 0);
    float spec;
    if (lightIntensity > 0.0)
    {
        vec3 reflection = normalize(2 * lightIntensity * normal - vertexToLight);
        spec = pow(clamp(dot(reflection, vertexToEye), 0, 1), 32); // 32 = specular power
    }

    specular = vec4(spec, spec, spec, spec);

    return specular;
}

float DoAttenuation(float distanceToLight)
{
    float constant = 0.9;
    float linear = 0.09;
    float quadratic = 0.0;

    return 1.0 / (constant + linear * distanceToLight + quadratic * distanceToLight * distanceToLight);
}

LightingResult DoPointLight(PointLight light, vec3 vertexToEye, vec3 vertexToLight, vec3 normal)
{
    LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

    float distanceToLight = length(vertexToLight);
    float attenuation = DoAttenuation(distanceToLight);

    vertexToLight = normalize(vertexToLight);

    result.Diffuse = DoDiffuse(light.emittedColour, vertexToLight, normal) * attenuation;
    result.Specular = DoSpecular(vertexToEye, vertexToLight, normal) * attenuation;

    return result;
}

LightingResult DoDirectionalLight(DirectionalLight light, vec3 vertexToEye, vec3 vertexToLight, vec3 normal)
{
    vertexToLight = -vertexToLight;

    LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
	
    float attenuation = 0.5;
	
    result.Diffuse = DoDiffuse(light.emittedColour, vertexToLight, normal) * attenuation;
    result.Specular = DoSpecular(vertexToEye, vertexToLight, normal) * attenuation;
	
    return result;
}

LightingResult DoSpotLight(SpotLight light, vec3 vertexToEye, vec3 vertexToLight, vec3 normal)
{
    LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
	
    float distanceToLight = length(vertexToLight);
    float attenuation = DoAttenuation(distanceToLight);
	
    float cutOff = 0.9;
    float outerCutOff = 1.0;
	
    float theta = dot(vertexToLight, normalize(-light.direction));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
	
    result.Diffuse = DoDiffuse(light.emittedColour, vertexToLight, normal) * attenuation * intensity;
    result.Specular = DoSpecular(vertexToEye, vertexToLight, normal) * attenuation * intensity;
	
    return result;
}

vec3 BlinnPhong(vec3 normal, vec3 vertexPos)
{
    LightingResult totalResult = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

    vec3 vertexToEye = normalize(camPos - vertexPos);

    for (int i = 0; i < noPoint; i++)
    {
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
		
        vec3 vertexToLight = pointLights[i].position - vertexPos;

        result = DoPointLight(pointLights[i], vertexToEye, vertexToLight, normal);

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    for (int i = 0; i < noDirectional; i++)
    {
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
		
        vec3 vertexToLight = pointLights[i].position - vertexPos;

        result = DoDirectionalLight(directionalLights[i], vertexToEye, vertexToLight, normal);

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    for (int i = 0; i < noSpot; i++)
    {
        LightingResult result = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };
		
        vec3 vertexToLight = pointLights[i].position - vertexPos;

        result = DoSpotLight(spotLights[i], vertexToEye, vertexToLight, normal);

        totalResult.Diffuse += result.Diffuse;
        totalResult.Specular += result.Specular;
    }

    vec4 ambient = vec4(0.15, 0.15, 0.15, 1.0);
    totalResult.Diffuse = clamp(totalResult.Diffuse, 0, 1);
    totalResult.Specular = clamp(totalResult.Specular, 0, 1);

    return (ambient + totalResult.Diffuse + totalResult.Specular).xyz;
}

vec3 GetNormalFromMap()
{
    vec3 normal = texture(material.normal0, fInput.texCoords).xyz;

    normal = normal * 2.0 - 1.0;

    // Parameters for TBN matrix construction
    vec3 Q1  = dFdx(fInput.worldPos);
    vec3 Q2  = dFdy(fInput.worldPos);
    vec2 st1 = dFdx(fInput.texCoords);
    vec2 st2 = dFdy(fInput.texCoords);

    // Construct TBN matrix; we use this to transform normal out of tangent space
    vec3 N   = normalize(fInput.normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * normal);
}

void main()
{
    // Retrieve map values
	vec3 albedo = texture(material.albedo0, fInput.texCoords).rgb;
	vec3 normal = GetNormalFromMap();

    vec3 colour = albedo * BlinnPhong(normal, fInput.worldPos);

	fragColour = vec4(colour, 1.0);
}