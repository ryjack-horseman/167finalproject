#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in vec3 FragPos;

in vec3 NormalOut;
in vec3 FragPos;




//uniform vec3 color;
uniform int mode;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 matAmbient;
uniform vec3 matSpecular;
uniform vec3 matDiffuse;
uniform float matShine;
uniform vec3 eye;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcAttLight(vec3 lightColor, vec3 lightPos, vec3 currPos);


void main()
{
    float redVal,blueVal, greenVal,diffuseX,diffuseY,diffuseZ;
    if(mode == 1){
    redVal = (NormalOut.x+1)/2;
    greenVal = (NormalOut.y+1)/2;
    blueVal = (NormalOut.z+1)/2;
        fragColor = vec4(vec3(redVal, greenVal, blueVal), 1.0f);
    }
    //else if(mode == 2){
   //     fragColor = vec4(vec3(lightColor),1.0f);
  //  }
    else{
        
        //ambient
        vec3 tuning = CalcAttLight(lightColor, lightPos, FragPos);
       vec3 ambient = tuning * matAmbient;
       
       // diffuse

       vec3 lightDir = normalize(lightPos - FragPos);
       float diff = max(dot(NormalOut, lightDir), 0.0);
        vec3 diffuse = tuning * diff * matDiffuse;
 
       // specular
       vec3 viewDir = normalize(eye - FragPos);
      vec3 reflectDir = reflect(-lightDir, NormalOut);
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), matShine);
       vec3 specular = tuning * (spec * matSpecular);
         
        vec3 result = ambient + diffuse + specular;
      
       fragColor = vec4(result, 1.0);
    }
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
}
vec3 CalcAttLight(vec3 lightColor, vec3 lightPos, vec3 currPos){
    float kConst = 0.2f;
    vec3 diff = lightPos-currPos;
    float mag = length(diff) * kConst;
    vec3 toReturn = lightColor/mag;
    return toReturn;
}
