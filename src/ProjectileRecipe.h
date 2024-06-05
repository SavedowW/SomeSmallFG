#ifndef RECIPE_PARSER_H_
#define RECIPE_PARSER_H_
#include "Vector2.h"

struct PTRecipe
{
    ORIENTATION m_starterOrientation;
    Vector2<float> m_starterPos;

    std::string m_ptType;
    int m_ptTypeID;
    int m_initialState;
    Vector2<float> m_mulOwnDirOffset;
    Vector2<float> m_minPos;
    Vector2<float> m_maxPos;
};

#endif