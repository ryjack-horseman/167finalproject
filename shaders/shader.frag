#version 330 core
// This is a sample fragment shader.
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalOutput;
in vec3 posOutput;
in float sampleExtraOutput;


uniform vec3 viewPos;
uniform Light light;
uniform Material material;
uniform vec3 color;
uniform bool celFlag;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    if (celFlag) {
        // CEL SHADING
        vec3 norm = normalize(normalOutput);
        vec3 viewDir = normalize(viewPos - posOutput);
        vec3 lightDir = normalize(vec3(2.0f, 1.0f, 1.0f));
        vec3 reflectDir = reflect(-lightDir, norm);

        float distance = length(light.position - posOutput);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance  * distance));

        //vec3 ambient = light.ambient * material.ambient;
        //vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * material.diffuse;
        //vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.specular;

        // ambient *= attenuation;
        // diffuse *= attenuation;
        // specular *= attenuation;

        // Calculating weighted average for luminosity method
        vec3 weights = vec3(0.3f, 0.59f, 0.11f);
        float ambi = dot(material.ambient, weights);
        float diff = dot(material.diffuse, weights);
        float spec = dot(material.specular, weights);

        // ratios
        float diffAmt = 0.01;
        float specAmt = 0.99;
        float rimAmt = 0.701;

        // necessary dot products
        float NdotL = dot(lightDir, norm);
        float VdotR = dot(viewDir, reflectDir);
        float NdotR = dot(norm, viewDir);
        vec3 halfVec = normalize(light.position + viewDir);

        //vec3 ambient = light.ambient * ambi;
        vec3 ambient = light.ambient * material.ambient;


        float diffMask = smoothstep(diffAmt - 0.01, diffAmt + 0.01, NdotL);
        //vec3 diffuse = light.diffuse * diff * diffMask;
        vec3 diffuse = light.diffuse * material.diffuse * diffMask;

        float specMask =smoothstep(specAmt - 0.01, specAmt + 0.01, VdotR);
        //vec3 specular = light.specular * spec * pow(specMask, material.shininess);
        vec3 specular = light.specular * material.specular * pow(specMask, material.shininess);

        // TODO: need to figure out how to adjust threshold for rim
        float rimMask = smoothstep(rimAmt - 0.01, rimAmt + 0.01, (1 - NdotR)* pow(max(NdotL, 0.0), 0.1));
        //vec3 rimDetection = light.specular * spec * rimMask;
        vec3 rimDetection = light.specular * material.specular * rimMask;
    
       // fragColor = vec4((diffuse + specular + ambient) * color, sampleExtraOutput);
        fragColor = vec4((ambient + diffuse + specular + rimDetection) * color, sampleExtraOutput);

    } else {

        // NORMAL PHONG SHADING

        vec3 norm = normalize(normalOutput);
        vec3 viewDir = normalize(viewPos - posOutput);
        vec3 lightDir = normalize(vec3(2.0f, 1.0f, 1.0f));
        vec3 reflectDir = reflect(-lightDir, norm);

        float distance = length(light.position - posOutput);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance  * distance));

        vec3 ambient = light.ambient * material.ambient;
        vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) * material.diffuse;
        vec3 specular = light.specular * pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * material.specular;

    // ambient *= attenuation;
    // diffuse *= attenuation;
    // specular *= attenuation;

 
        fragColor = vec4((diffuse + specular + ambient) * color, sampleExtraOutput);
    }
}