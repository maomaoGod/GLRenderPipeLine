#version 330 core
out vec4 FragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    //基础参数
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    //探照灯参数
    vec3  direction;
    float cutOff;
    vec3 position;
    float outerCutOff;


    //点光源参数
    float constant;
    float linear;
    float quadratic;

    sampler2D lightMark;
};



in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 ProjTexCoord;

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec2 viewPort;

uniform Material material;
uniform Light pointLight;
uniform Light directLight;
uniform Light spotLight;

vec3 calc_light_imp(Light light){
     //环境光
    vec3 ambient = light.ambient * vec3( texture(material.diffuse,TexCoord));

    //漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position- FragPos);
    float diff = max(dot(norm, lightDir), 0.0);                     //漫反射系数，光线在物体表面进行漫反射时的衰减系数

    //镜面反射
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse,TexCoord));
    vec3 viewDir = normalize(viewPos-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);     //光线在物体表面进行镜面反射时的衰减系数
    vec3 speccoord = vec3(texture(material.specular, TexCoord));
    vec3 specular = light.specular*spec*speccoord;

    //高光部分
    vec3 emission = texture(material.emission, TexCoord).rgb;

    return (ambient+diffuse+specular+emission);
}

vec3 calc_directLight(){
    return calc_light_imp(directLight);
}

vec3 calc_pointLight(){
    // attenuation
    float distance    = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    return calc_light_imp(pointLight)*attenuation;
}

vec3 calc_spotlight(){
    // ambient
    vec3 ambient = spotLight.ambient * texture(material.diffuse, TexCoord).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(spotLight.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * texture(material.diffuse, TexCoord).rgb;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * texture(material.specular, TexCoord).rgb;

    // spotlight (soft edges)
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = (spotLight.cutOff - spotLight.outerCutOff);
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0)*5;
    //float projColor = textureProj(spotLight.lightMark, ProjTexCoord).r;
    //vec2 fragCoord = gl_FragCoord.xy / viewPort * vec2(1.0, -1.0);
    //if(ProjTexCoord.z>0.0)
    //    intensity *= (length(vec3(textureProj(spotLight.lightMark, ProjTexCoord))));
    //intensity *= projColor;
    diffuse  *= intensity;
    specular *= intensity;

    // attenuation
    float distance    = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
    ambient  *= attenuation;
    diffuse   *= attenuation;
    specular *= attenuation;

    //实现projective texture mapping
    vec4 projColor;
    if( ProjTexCoord.z > 0.0 )
    		projColor= textureProj(spotLight.lightMark,ProjTexCoord);
    vec3 result =mix((ambient + diffuse + specular), projColor.rgb,0.4f);
    //vec3 result = ambient + diffuse + specular;
    return result;
}

void main()
{
    //vec3 ambient = light.ambient * vec3( mix(texture(material.diffuse, TexCoord),texture(material.emission,TexCoord),0.2f);

    vec3 ltcolor = calc_spotlight();
	vec4 mixcolor = vec4(ltcolor , 1.0);

	FragColor = mixcolor;// mixcolor*mixcoord;
}