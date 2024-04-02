#ifndef RECIPE_PARSER_H_
#define RECIPE_PARSER_H_
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "TimelineProperty.h"
#include "Vector2.h"
#include "Hit.h"
#include "StateMarker.h"

struct ActionRecipe
{
    std::string ActionType;
    std::string InputComparator;
    int state;
    HurtboxFramesVec hurtboxes;
    int animation;

    TimelineProperty<bool> counterWindow;
    TimelineProperty<bool> gravityWindow;
    TimelineProperty<bool> blockWindow;

    StateMarker transitionableFrom;
};

struct CharacterRecipe
{
    std::map<std::string, int> states;
    std::vector<ActionRecipe> actions;
};

struct RecipeParser
{
public:
    RecipeParser(const std::string &filepath_);

private:
    template <typename T>
    TimelineProperty<T> parseTimelineProperty(const nlohmann::json &json_);

    template <typename T>
    TimelineProperty<T> parseTimelinePropertyImpl(const nlohmann::json &json_, T*);

    template <typename T>
    TimelineProperty<Vector2<T>> parseTimelinePropertyImpl(const nlohmann::json &json_, Vector2<T>*);

    template <typename T>
    Vector2<T> parseVector2(const nlohmann::json &json_);

    Collider parseCollider(const nlohmann::json &json_);

    HurtboxFramesVec parseHurtboxFramesVec(const nlohmann::json &json_);

    void parseCharacter(const nlohmann::json &json_);

    void parseAction(const nlohmann::json &json_);
    void parseActionCharacter(const nlohmann::json &json_);

    std::vector<CharacterRecipe> m_characterRecipes;
    CharacterRecipe *m_currentCharacterRecipe = nullptr;
    ActionRecipe *m_currentActionRecipe = nullptr;
};

#endif