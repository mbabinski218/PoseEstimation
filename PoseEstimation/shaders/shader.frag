//#version 440
//
//out vec4 FragColor;
//in vec3 WorldPos;
//in vec3 Normal;
//
//// material parameters
//uniform vec3 albedo;
//uniform float metallic;
//uniform float roughness;
//uniform float ao;
//
//// lights
//uniform vec3 lightPosition;
//uniform vec3 lightColor;
//
//uniform vec3 camPos;
//
//const float PI = 3.14159265359;
//// ----------------------------------------------------------------------------
//float DistributionGGX(vec3 N, vec3 H, float roughness)
//{
//  float a = roughness * roughness;
//  float a2 = a * a;
//  float NdotH = max(dot(N, H), 0.0);
//  float NdotH2 = NdotH * NdotH;
//
//  float nom = a2;
//  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
//  denom = PI * denom * denom;
//
//  return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
//}
//// ----------------------------------------------------------------------------
//float GeometrySchlickGGX(float NdotV, float roughness)
//{
//  float r = (roughness + 1.0);
//  float k = (r * r) / 8.0;
//
//  float nom = NdotV;
//  float denom = NdotV * (1.0 - k) + k;
//
//  return nom / denom;
//}
//// ----------------------------------------------------------------------------
//float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
//{
//  float NdotV = max(dot(N, V), 0.0);
//  float NdotL = max(dot(N, L), 0.0);
//  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
//  float ggx1 = GeometrySchlickGGX(NdotL, roughness);
//
//  return ggx1 * ggx2;
//}
//// ----------------------------------------------------------------------------
//vec3 fresnelSchlick(float cosTheta, vec3 F0)
//{
//  return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
//}
//// ----------------------------------------------------------------------------
//void main()
//{
//  vec3 N = normalize(Normal);
//  vec3 V = normalize(camPos - WorldPos);
//
//  // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
//  // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
//  vec3 F0 = vec3(0.04);
//  F0 = mix(F0, albedo, metallic);
//
//  // reflectance equation
//  vec3 Lo = vec3(0.0);
//
//  // calculate per-light radiance
//  vec3 L = normalize(lightPosition - WorldPos);
//  vec3 H = normalize(V + L);
//  float distance = length(lightPosition - WorldPos);
//  float attenuation = 1.0 / (distance * distance);
//  vec3 radiance = lightColor * attenuation;
//
//  // Cook-Torrance BRDF
//  float NDF = DistributionGGX(N, H, roughness);
//  float G = GeometrySmith(N, V, L, roughness);
//  vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
//
//  vec3 nominator = NDF * G * F;
//  float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
//  vec3 specular = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
//
//  // kS is equal to Fresnel
//  vec3 kS = F;
//  // for energy conservation, the diffuse and specular light can't
//  // be above 1.0 (unless the surface emits light); to preserve this
//  // relationship the diffuse component (kD) should equal 1.0 - kS.
//  vec3 kD = vec3(1.0) - kS;
//  // multiply kD by the inverse metalness such that only non-metals 
//  // have diffuse lighting, or a linear blend if partly metal (pure metals
//  // have no diffuse light).
//  kD *= 1.0 - metallic;
//
//  // scale light by NdotL
//  float NdotL = max(dot(N, L), 0.0);
//
//  // add to outgoing radiance Lo
//  Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
//
//
//  // ambient lighting (note that the next IBL tutorial will replace 
//  // this ambient lighting with environment lighting).
//  vec3 ambient = vec3(0.03) * albedo * ao;
//
//  vec3 color = ambient + Lo;
//
//  // HDR tonemapping
//  color = color / (color + vec3(1.0));
//
//  // gamma correct
//  color = pow(color, vec3(1.0 / 2.2));
//
//  FragColor = vec4(color, 1.0);
//}

#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

//Uniforms
uniform Material material;
uniform PointLight pointLight;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

//Functions
vec3 calculateAmbient(Material material)
{
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	//vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;
	vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}

void main()
{
	//fs_color = vec4(vs_color, 1.f);
	//fs_color = texture(texture0, vs_texcoord) * texture(texture1, vs_texcoord) * vec4(vs_color, 1.f);

	//Ambient light
	vec3 ambientFinal = calculateAmbient(material);

	//Diffuse light
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);

	//Specular light
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, cameraPos);

	//Attenuation
	float distance = length(pointLight.position - vs_position);
	//constant linear quadratic
	float attenuation = pointLight.constant / (1.f + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

	//Final light
	ambientFinal *= attenuation;
	diffuseFinal *= attenuation;
	specularFinal *= attenuation;

	fs_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));

	fs_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}

