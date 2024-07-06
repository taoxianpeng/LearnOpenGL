#version 330 core

struct Light{ //带衰减的点光源
	vec3 postion;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

uniform Material material;
uniform Light light;

//uniform Light light;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;


void main()
{    
    vec3 ambient = light.ambient * texture(material.texture_diffuse1, TexCoords).rgb;
    // diffuse 
    float dist = length(light.postion - FragPos);
    ////衰减算法
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.postion - FragPos); 
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * texture(material.texture_diffuse1, TexCoords).rgb);
    
    // specular
    vec3 viewDir = normalize(light.postion - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * texture(material.texture_specular1, TexCoords).rgb);  
        
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
    // vec3 result = diffuse;
    // FragColor = vec4(result, 1.0); 
    // gl_FragColor = texture(material.texture_diffuse1, TexCoords);
}