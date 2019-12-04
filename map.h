//
//  map.hpp
//  cse 167
//
//  Created by Ryan Jackson on 12/3/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#ifndef map_h
#define map_h
#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <stdio.h>
class map
{
        int totalsize, x, y;
        std::vector<std::vector<float> > elements;

    public:

        map(int x, int y);
        map(std::vector<std::vector<float> > input, int x, int y);

        void display(); //Output data to console
        void clearElements();
        bool isedge(int x, int y);
        int getX();
        int getY();
        int getTotalsize();
        float getElement(int x, int y);
        std::vector<std::vector<float> > getElements();
        void setElement(int x, int y, int val);
        void loadElements(std::vector<std::vector<float> > input);

        //variouse overloaded functions to generate the heightmap
        //based on various parameters.
        void generate(int size, int roughness, int min, int max);
        void generate(int size, int roughness);
        void generate(int size, float roughness);
        void generate(int size, float mag, float roughness);

        void square(int x, int y, int size, int offset);
        void diamond(int x, int y, int size, int offset);
        bool isPastEdge(int x, int y);

        //Caps the heightmap data by setting values greater than
        //upper equal to upper and vice versa
        void capData(int lower, int upper);
        void multiplyData(float multiple);

        //Smooths terrain using a mean/box filter
        void smooth(int filterSize, int passes);
        void dynamicHeightSmooth(int filterSize, int passMin, int passMax);



};
#endif /* map_hpp */
