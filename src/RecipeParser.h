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
#include <memory>

class ActionExtention
{
public:
    enum class ExtentionType {
        SWITCH_DATA,
        HITSTUN_ANIMATION,
        ANIM_RESET_DATA,
        REALIGN_DATA,
        AIR_ACTION_TIMER
    } m_extentionType;

    ActionExtention(ExtentionType extType_);

    virtual ~ActionExtention() = default;
};

class ActionExtentionSwitchData : public ActionExtention
{
public:
    ActionExtentionSwitchData();
    virtual ~ActionExtentionSwitchData() = default;

    bool realign = false;
    int timerValue = 0;
    bool velToInertia = false;
    bool resetDefenseState = false;
    bool setAirAttackFlag = false;
    bool resetPushback = false;
    bool callForPriority = false;
    Vector2<float> mulOwnVel = {1.0, 1.0};
    Vector2<float> mulOwnInr = {1.0, 1.0};
    Vector2<float> mulOwnDirVel = {0.0, 0.0};
    Vector2<float> mulOwnDirInr = {0.0, 0.0};
    Vector2<float> rawAddVel = {0.0, 0.0};
    Vector2<float> rawAddInr = {0.0, 0.0};
};

class ActionExtentionRealignData : public ActionExtention
{
public:
    ActionExtentionRealignData();
    virtual ~ActionExtentionRealignData() = default;

    TimelineProperty<bool> realignData;
};

class ActionExtentionAirActionTimer : public ActionExtention
{
public:
    ActionExtentionAirActionTimer();
    virtual ~ActionExtentionAirActionTimer() = default;

    int airjumpTimer = 5;
    int airdashTimer = 6;
};

struct ActionRecipe
{
    std::string m_actionType;
    std::string m_inputComparator;
    std::string m_inputComparatorProlong;
    int m_state;
    HurtboxFramesVec m_hurtboxes;
    int m_animation;
    TimelineProperty<bool> m_counterWindow;
    TimelineProperty<bool> m_gravityWindow;
    TimelineProperty<bool> m_blockWindow;
    StateMarker m_transitionableFrom;
    Vector2<float> m_impulse;
    int m_prejumpLen;
    float m_maxHorInertia;
    bool m_isAttack;
    bool m_isCrouchState;
    bool m_isThrowStartup;
    int m_consumeAirdash;
    int m_consumeAirjump;
    bool m_waitAirdashTimer;
    bool m_waitAirjumpTimer;
    bool m_isAirborne;

    // To use a unique_ptr I would need to manually write constructors and assignments which is too much work
    std::vector<ActionExtention*> m_extentions;

    void clear();
};

struct CharacterRecipe
{
    std::map<std::string, int> states;
    std::vector<ActionRecipe> actions;

    ~CharacterRecipe();
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
    void parseActionExtentions(const nlohmann::json &json_);
    void parseActionCharacter(const nlohmann::json &json_);
    void parseActionProlonged(const nlohmann::json &json_);
    void parseActionJump(const nlohmann::json &json_);
    
    void parseExtentionSwitchData(const nlohmann::json &json_);
    void parseExtentionRealignData(const nlohmann::json &json_);
    void parseExtentionAirActionTimer(const nlohmann::json &json_);

    std::vector<CharacterRecipe> m_characterRecipes;
    CharacterRecipe *m_currentCharacterRecipe = nullptr;
    ActionRecipe *m_currentActionRecipe = nullptr;
};

#endif