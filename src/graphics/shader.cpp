#include <fluxgl/graphics/shader.h>
#include <fluxgl/core/error.h>
#include <fluxgl/utils/file.h>

#include <fluxgl/core/log.h>

#include <glad/glad.h>

namespace fluxgl {
    void Shader::compile(const std::string& vertexSrc, const std::string& fragmentSrc) {
        FLUXGL_LOG_DEBUG("Compiling shader program...");       
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSourceCStr = vertexSrc.c_str();
        glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
        glCompileShader(vertexShader);

        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Vertex shader compilation failed: " + std::string(infoLog)};
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSourceCStr = fragmentSrc.c_str();
        glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Fragment shader compilation failed: " + std::string(infoLog)};
        }

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertexShader);
        glAttachShader(m_ID, fragmentShader);
        glLinkProgram(m_ID);
        glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
            throw Error{ErrorCode::ShaderCompilationFailed, "Shader linking failed: " + std::string(infoLog)};
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        FLUXGL_LOG_DEBUG("Shader program linked (ID: " + std::to_string(m_ID) + ")");
    }

    Shader::~Shader() {
        if(m_ID) {
            FLUXGL_LOG_DEBUG("Deleting shader program (ID: " + std::to_string(m_ID) + ")");
            glDeleteProgram(m_ID);
        }
    }

    Shader::Shader(Shader&& other) noexcept {
        m_ID = other.m_ID; 
        other.m_ID = 0; // Prevent deletion of the moved-from shader
    }

    Shader& Shader::operator=(Shader&& other) noexcept {
        if (this != &other) {
            if(m_ID) {
                glDeleteProgram(m_ID);
            }
            
            m_ID = other.m_ID;
            other.m_ID = 0;
        }
        return *this;
    }

    void Shader::bind() const {
        glUseProgram(m_ID);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setUniform(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, unsigned int value) const {
        glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), value.x, value.y, value.z);
    }

    void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
    }

    Shader Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
        FLUXGL_LOG_DEBUG("Loading shader from files: " + vertexPath + ", " + fragmentPath);
        std::string vertexSrc = readFileToString(vertexPath);
        std::string fragmentSrc = readFileToString(fragmentPath);
        return Shader::loadFromSource(vertexSrc, fragmentSrc);
    }

    Shader Shader::loadFromSource(const std::string& vertexSrc, const std::string& fragmentSrc) {
        Shader shader; 
        shader.compile(vertexSrc, fragmentSrc); 
        return shader; 
    }

    Shader Shader::defaultLit() {
        const char *vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec3 aColor;
            layout (location = 3) in vec2 aUV;

            out vec2 TexCoord;
            out vec3 FragPos;
            out vec3 Normal;

            uniform mat4 u_Model;
            uniform mat4 u_View;
            uniform mat4 u_Projection;

            void main()
            {
                gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
                TexCoord = aUV;
                FragPos = vec3(u_Model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(u_Model))) * aNormal;
            }
        )";

        const char *fragmentSrc = R"(
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
                for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < u_DirectionalLightCount; i++) {
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
        )";

        return loadFromSource(vertexSrc, fragmentSrc);
    }

    Shader Shader::defaultUnlit() {
        const char *vertexSrc = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
            layout (location = 2) in vec3 aColor;
            layout (location = 3) in vec2 aUV;

            out vec2 TexCoord;
            out vec3 FragPos;
            out vec3 Normal;

            uniform mat4 u_Model;
            uniform mat4 u_View;
            uniform mat4 u_Projection;

            void main()
            {
                gl_Position = u_Projection * u_View * u_Model * vec4(aPos, 1.0);
                TexCoord = aUV;
                FragPos = vec3(u_Model * vec4(aPos, 1.0));
                Normal = mat3(transpose(inverse(u_Model))) * aNormal;
            }
        )";

        const char *fragmentSrc = R"(
            #version 330 core
            out vec4 FragColor;

            in vec2 TexCoord;

            uniform vec3 u_AlbedoColor;

            void main()
            {
                // Output fragment
                FragColor = vec4(u_AlbedoColor, 1.0);
            }
        )";

        return loadFromSource(vertexSrc, fragmentSrc);
    }
}