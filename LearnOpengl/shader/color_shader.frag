#version 330 core

struct Light{ //带衰减的点光源
  vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

struct ColorMaterial
{
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
}

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform Material material;
uniform ColorMaterial colorMaterial;
uniform Light light;

//uniform Light light;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


void main()
{    
    vec3 ambient = light.ambient * (colorMaterial.ambient).rgb;
    // diffuse 
    float dist = length(light.position - FragPos);
    ////衰减算法
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = light.diffuse * (diff * (colorMaterial.diffuse).rgb);
    
    // specular
    vec3 viewDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * (colorMaterial.specular).rgb);  
        
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    // FragColor = vec4(result, 1.0);
    //vec3 result = diffuse;
    FragColor = vec4(result, 1.0); 
    //gl_FragColor = texture(material.texture_diffuse1, TexCoords);
}
