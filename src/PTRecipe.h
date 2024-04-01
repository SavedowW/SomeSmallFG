#ifndef PT_RECIPE_H_
#define PT_RECIPE_H_
#include "Vector2.h"

struct PTRecipe
{
    int m_ptType;
    ORIENTATION m_starterOrientation;
    Vector2<float> m_starterPos;

    Vector2<float> m_minPos;
    Vector2<float> m_maxPos;

};

#endif