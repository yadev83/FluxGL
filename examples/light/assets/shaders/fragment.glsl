#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

// Camera position
uniform vec3 u_CameraPosition;

// Material colors
uniform vec3 u_AlbedoColor;
uniform vec3 u_SpecularColor;
uniform vec3 u_EmissionColor;
// Material properties
uniform float u_Shininess;

// Material albedo texture sampler
#define MAX_ALBEDO_TEXTURES 4
uniform sampler2D u_Albedo[MAX_ALBEDO_TEXTURES];
uniform int u_AlbedoTextureCount;
uniform sampler2D u_NormalMap;
uniform bool u_UseNormalMap;
uniform sampler2D u_SpecularMap;
uniform bool u_UseSpecularMap;
uniform sampler2D u_EmissionMap;
uniform bool u_UseEmissionMap;

// Lighting
#define MAX_AMBIENT_LIGHTS 4
struct AmbientLight { vec3 color; float intensity; };
uniform int u_AmbientLightCount;
uniform AmbientLight u_AmbientLights[MAX_AMBIENT_LIGHTS];

#define MAX_DIRECTIONAL_LIGHTS 4
struct DirectionalLight { vec3 color; float intensity; vec3 direction; };
uniform int u_DirectionalLightCount;
uniform DirectionalLight u_DirectionalLights[MAX_DIRECTIONAL_LIGHTS];

#define MAX_POINT_LIGHTS 16
struct PointLight { vec3 color; float intensity; vec3 position; };
uniform int u_PointLightCount;
uniform PointLight u_PointLights[MAX_POINT_LIGHTS];

void main()
{
	// Start by using the diffuse color as a base
	vec3 baseColor = u_AlbedoColor;
	if(u_AlbedoTextureCount > 0) {
		vec3 texColor = vec3(0.0);
		for(int i = 0; i < MAX_ALBEDO_TEXTURES && i < u_AlbedoTextureCount; i++) {
			vec4 tex = texture(u_Albedo[i], TexCoord);
			texColor = mix(texColor, tex.rgb, tex.a);
		}
		
		baseColor *= texColor;
	}

	// Compute specular color
	vec3 specularColor = u_SpecularColor;
	if(u_UseSpecularMap) {
		vec4 tex = texture(u_SpecularMap, TexCoord);
		specularColor *= tex.rgb;
	}

	// LIGHTING STACK
	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	// AMBIENT LIGHTS
	for(int i = 0; i < MAX_AMBIENT_LIGHTS && i < u_AmbientLightCount; i++) {
		ambient += u_AmbientLights[i].color * u_AmbientLights[i].intensity;
	}

	// DIRECTIONAL LIGHTS
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < u_PointLightCount; i++) {
		vec3 lightDir = normalize(-u_DirectionalLights[i].direction);
		vec3 viewDir = normalize(u_CameraPosition - FragPos);
		vec3 normal = normalize(Normal);

		// Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        diffuse += (diff * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity);

        // Specular (Phong)
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
		specular += (spec * specularColor * u_DirectionalLights[i].color * u_DirectionalLights[i].intensity);
	}

	// POINT LIGHTS
	for(int i = 0; i < MAX_POINT_LIGHTS && i < u_PointLightCount; i++) {
		vec3 lightDir = normalize(u_PointLights[i].position - FragPos);
		vec3 viewDir = normalize(u_CameraPosition - FragPos);
		vec3 normal = normalize(Normal);

		// Compute distance attenuation
		float distance = length(u_PointLights[i].position - FragPos);
		float attenuation = u_PointLights[i].intensity / (distance * distance);

		// Diffuse
        float diff = max(dot(normal, lightDir), 0.0);
        diffuse += (diff * u_PointLights[i].color * u_PointLights[i].intensity * attenuation);

        // Specular (Phong)
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Shininess);
		specular += (spec * specularColor * u_PointLights[i].color * u_PointLights[i].intensity * attenuation);
	}

	// Emission
	vec3 emissionColor = u_EmissionColor;
	if(u_UseEmissionMap) {
		vec4 tex = texture(u_EmissionMap, TexCoord);
		emissionColor *= tex.rgb;
	}

	// Compute final color
	vec3 finalColor = (baseColor * diffuse) + (specular) + (baseColor * ambient) + emissionColor;

	// Output fragment
    FragColor = vec4(finalColor, 1.0f);
}