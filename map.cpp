//
//  map.cpp
//  cse 167
//
//  Created by Ryan Jackson on 12/3/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#include "map.h"
#include <iostream>
#include "map.h"
#include <vector>
#include <time.h>


map::map(int x, int y)
{
    this->x = x;
    this->y = y;
    totalsize = x* y;

    std::vector<float> tempvec;

    for(int i = 0; i != y; i++)
    {
        elements.push_back(tempvec);

        for(int j = 0; j != x; j++)
        {
            elements[i].push_back(0);
        }
    }
}

int map::getX()
{
    return x;
}

int map::getY()
{
    return y;
}

 std::vector<std::vector<float> > map::getElements()
{
    return elements;

}
void map::setElement(int x, int y, int val)
{
    elements[y][x] = val;
}
float map::getElement(int x, int y)
{
    return elements[y][x];
}

void map::square(int x, int y, int size, int offset)
{
    elements[y][x] = (elements[y + size][x + size] +
        elements[y - size][x - size] +
        elements[y + size][x - size] +
        elements[y - size][x + size]) / 4 + offset;
}

void map::diamond(int x, int y, int size, int offset)
{
    if (x - size < 0)
    {
        elements[y][x] = (elements[y][x + size] +
            elements[y + size][x] +
            elements[y - size][x]) / 3 + offset;
    }
    else if (x + size > this->x - 1)
    {
        elements[y][x] = (elements[y][x - size] +
            elements[y + size][x] +
            elements[y - size][x]) / 3 + offset;
    }
    else if (y - size < 0)
    {
        elements[y][x] = (elements[y][x + size] +
            elements[y][x - size] +
            elements[y + size][x]) / 3 + offset;
    }
    else if (y + size > this->y - 1)
    {
        elements[y][x] = (elements[y][x + size] +
            elements[y][x - size] +
            elements[y - size][x]) / 3 + offset;
    }
    else
    {
        elements[y][x] = (elements[y ][x + size] +
            elements[y][x - size] +
            elements[y + size][x] +
            elements[y - size][x]) / 4 + offset;
    } // optomize so not going through all 4 before this

}

void map::generate(int size, float mag, float roughness) //size is just x but make x better first, sidelength
{
    srand(time(NULL));
    int squaresize = (size - 1) / 2;

    while (squaresize>0)//for(int i = 0; i < (size); i = i+2) //fix to a for loop
    {
        for (int y = 0; y < size / (squaresize * 2); y++)
        {
            for (int x = 0; x < size / (squaresize * 2); x++)
            {
                square((x*squaresize * 2) + squaresize,
                    (y*squaresize * 2) + squaresize,
                    squaresize,
                    rand() % int(2 * mag) - mag);
            }
        }

        int rowtype = 1;

        for (int y = 0; y < size; y += squaresize)//follows diff for loop logic then square
        {
            if (rowtype == 1)
            {
                for (int x = squaresize; x < size; x += 2 * squaresize)
                {
                    diamond(x, y, squaresize, rand() % int(2 * mag) - mag);
                }
            }
            if (rowtype == -1)
            {
                for (int x = 0; x < size; x += squaresize * 2)
                {
                    diamond(x, y, squaresize, rand() % int(2 * mag) - mag);
                }
            }
            rowtype *= -1;
        }

        mag *= roughness;
        squaresize *= 0.5;
    }
}

void map::capData(int lower, int upper)
{
    for (int y = 0; y < this->y; y++)
    {
        for (int x = 0; x < this->x; x++)
        {
            if (elements[y][x] > upper) elements[y][x] = upper;
            if (elements[y][x] < lower) elements[y][x] = lower;
        }
    }
}


void map::smooth(int filterSize, int passes)
{
    std::vector<std::vector<float>> newelements = elements;

    for (int i = 0; i != passes; i++)
    {
        for (int Y = 0; Y != this->y; Y++)
        {
            for (int X = 0; X != this->x; X++)
            {
                float surroundingTotal = -elements[Y][X];
                int aroundNum = -1;

                for (int y = -filterSize; y <= filterSize; y++)
                {
                    for (int x = -filterSize; x <= filterSize; x++)
                    {
                        if (X + x > -1 && X + x < this->x && Y + y > -1 && Y + y < this->y)
                        {
                            surroundingTotal += elements[Y + y][X + x];
                            aroundNum++;
                        }
                    }
                }
                newelements[Y][X] = (elements[Y][X] + (surroundingTotal / aroundNum)) / 2;
            }
        }
        elements = newelements;
    }
}


