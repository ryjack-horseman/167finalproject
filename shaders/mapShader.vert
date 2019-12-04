#version 330 core

layout (location = 0) in vec3 in_cube;
layout(location = 2) in vec3 in_normal;

out vec3 normal;
out vec3 light;
//out float yval;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 in_light;

void main()
{
    
   // normal = normalize(normalMatrix * in_normal);
    
   // yval = in_cube.y;

    gl_Position = projection * view * model * vec4(in_cube,1.0f);
    light = in_light - vec3(model * vec4(in_cube, 1)); // makes light vec follow view
    
    normal = mat3(transpose(inverse(model))) * in_normal;

    
}
