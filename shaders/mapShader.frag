#version 330 core

//interpolated values from vertex shaders


in vec3 normal;
in vec3 light;
//in float yval;

//output data

out vec3 color;

//valuesthat stay constant for whole mesh



void main()
{
    float lightPow = 6;
    float cosTheta = clamp( dot( normalize(normal),normalize(light)), 0,1 );

    color = vec3(0.7,0.7,0.7)*lightPow*(cosTheta+0.1)/(length(light)*length(light));
}
