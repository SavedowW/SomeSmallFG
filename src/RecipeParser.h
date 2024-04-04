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
#include "AnimationManager.h"

class ActionExtention
{
public:
    enum class ExtentionType {
        SWITCH_DATA,
        REALIGN_DATA,
        AIR_ACTION_TIMER,
        UPDATE_MOVEMENT_DATA,
        LANDING_RECOVERY,
        OUTDATED_TRANSITION
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

class ActionExtentionUpdateMovementData : public ActionExtention
{
public:
    ActionExtentionUpdateMovementData();
    virtual ~ActionExtentionUpdateMovementData() = default;

    TimelineProperty<Vector2<float>> mulOwnVel;
    TimelineProperty<Vector2<float>> mulOwnInr;
    TimelineProperty<Vector2<float>> mulOwnDirVel;
    TimelineProperty<Vector2<float>> mulOwnDirInr;
    TimelineProperty<Vector2<float>> rawAddVel;
    TimelineProperty<Vector2<float>> rawAddInr;
};

class ActionExtentionLandingRecovery : public ActionExtention
{
public:
    ActionExtentionLandingRecovery();
    virtual ~ActionExtentionLandingRecovery() = default;

    int recoveryState;
};

class ActionExtentionOutdatedTransition : public ActionExtention
{
public:
    ActionExtentionOutdatedTransition();
    virtual ~ActionExtentionOutdatedTransition() = default;

    int targetState;
};

struct ActionRecipe
{
    std::string m_actionType;
    std::string m_inputComparator;
    std::string m_inputComparatorProlong;
    int m_state;
    int m_throwWhiffState;
    int m_throwHoldState;
    int m_throwID;
    float m_throwRange;
    FrameWindow m_activeWindow;
    bool m_requireAirborne;
    HurtboxFramesVec m_hurtboxes;
    int m_animation;
    TimelineProperty<bool> m_counterWindow;
    TimelineProperty<bool> m_gravityWindow;
    TimelineProperty<bool> m_blockWindow;
    StateMarker m_transitionableFrom;
    Vector2<float> m_impulse;
    int m_duration;
    float m_maxHorInertia;
    bool m_isAttack;
    bool m_isCrouchState;
    bool m_isThrowStartup;
    int m_consumeAirdash;
    int m_consumeAirjump;
    bool m_waitAirdashTimer;
    bool m_waitAirjumpTimer;
    bool m_isAirborne;

    std::vector<Hit> m_hits;

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
    RecipeParser(AnimationManager *animManager_, const std::string &filepath_);

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
    std::vector<std::pair<FrameWindow, Collider>> parseHitboxes(const nlohmann::json &json_);
    PostHitProperties parsePostHitProperties(const nlohmann::json &json_);
    CancelWindow parseCancelWindow(const nlohmann::json &json_);
    FrameWindow parseFrameWindow(const nlohmann::json &json_);

    void parseCharacter(const nlohmann::json &json_);

    void parseAction(const nlohmann::json &json_);
    void parseActionExtentions(const nlohmann::json &json_);
    void parseActionCharacter(const nlohmann::json &json_);
    void parseActionProlonged(const nlohmann::json &json_);
    void parseActionJump(const nlohmann::json &json_);
    void parseActionAirjump(const nlohmann::json &json_);
    void parseActionAttack(const nlohmann::json &json_);
    void parseActionThrowStartup(const nlohmann::json &json_);
    
    void parseExtentionSwitchData(const nlohmann::json &json_);
    void parseExtentionRealignData(const nlohmann::json &json_);
    void parseExtentionAirActionTimer(const nlohmann::json &json_);
    void parseExtentionUpdateMovementData(const nlohmann::json &json_);
    void parseExtentionLandingRecovery(const nlohmann::json &json_);
    void parseExtentionOutdatedTransition(const nlohmann::json &json_);

    void parseParticlesSpawnData(const nlohmann::json &json_);
    void parseHitData(const nlohmann::json &json_);

    LOOPMETHOD strToLoopMethod(const std::string &str_);
    BLOCK_STATE strToBlockState(const std::string &str_);
    HITSTUN_ANIMATION strToHitstunAnimation(const std::string &str_);
    int strToThrowID(const std::string &str_);

    std::vector<CharacterRecipe> m_characterRecipes;
    CharacterRecipe *m_currentCharacterRecipe = nullptr;
    ActionRecipe *m_currentActionRecipe = nullptr;
    AnimationManager *m_animManager;

    std::map<std::string, ParticlesSpawnData> m_particles;
    std::map<std::string, Hit> m_hits;
    std::map<std::string, int> m_throwIDs;
};

#endif