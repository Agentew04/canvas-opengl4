#pragma once 

#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <math.h>
#include <stdio.h>

class Vector2
{
public:
    float x;
    float y;

    Vector2()
    {
        x = y = 0;
    }

	Vector2(float _x, float _y) : x(_x), y(_y) {}

    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void normalize()
    {
        float norm = (float)sqrt(x * x + y * y);

        if (norm == 0.0)
        {
            printf("\n\nNormalize::Divisao por zero");
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    Vector2 operator - (const Vector2& v)
    {
        Vector2 aux(x - v.x, y - v.y);
        return(aux);
    }

    Vector2 operator + (const Vector2& v)
    {
        Vector2 aux(x + v.x, y + v.y);
        return(aux);
    }

    //Adicionem os demais overloads de operadores aqui.


};

#endif
