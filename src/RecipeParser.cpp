#include "RecipeParser.h"
#include <map>

RecipeParser::RecipeParser(AnimationManager *animManager_, const std::string &filepath_)
{
    m_animManager = animManager_;
    std::ifstream finp(filepath_);
    if (!finp.is_open())
    {
        std::cout << "Can't open file\n";
    }
    nlohmann::json data = nlohmann::json::parse(finp);

    //auto hbv = parseHurtboxFramesVec(data["hurtboxvecexample"]);
    //auto hbv = parseHitboxes(data["activeframesexample"]);

    nlohmann::json particles = data["ParticlesSpawnData"];
    for (auto &el : particles)
    {
        parseParticlesSpawnData(el);
    }

    nlohmann::json hits = data["Hits"];
    for (auto &el : hits)
    {
        parseHitData(el);
    }

    nlohmann::json chars = data["Characters"];
    for (auto &el : chars)
    {
        parseCharacter(el);
    }
}

void RecipeParser::parseCharacter(const nlohmann::json &json_)
{
    std::map<std::string, int> states;
    int i = 0;
    nlohmann::json actions = json_["Actions"];
    for (auto &action : actions)
    {
        if (!states.contains(action["State"]))
            states[action["State"]] = i++;
    }

    std::cout << i << " states in total\n";

    CharacterRecipe crp;
    crp.states = states;
    m_characterRecipes.push_back(crp);
    m_currentCharacterRecipe = &m_characterRecipes[m_characterRecipes.size() - 1];

    for (auto &action : actions)
    {
        parseAction(action);
    }
}

HurtboxFramesVec RecipeParser::parseHurtboxFramesVec(const nlohmann::json &json_)
{
    HurtboxFramesVec vec;
    for (auto &el : json_)
    {
        auto frames = parseTimelineProperty<bool>(el["frames"]);
        auto hurtbox = parseCollider(el["hurtbox"]);
        vec.push_back({frames, hurtbox});
    }

    return vec;
}

std::vector<std::pair<FrameWindow, Collider>> RecipeParser::parseHitboxes(const nlohmann::json &json_)
{
    std::vector<std::pair<FrameWindow, Collider>> vec;

    for (auto &el : json_)
    {
        auto beginFrame = el["BeginFrame"];
        auto endFrame = el["EndFrame"];
        auto hitbox = parseCollider(el["Hitbox"]);
        vec.push_back({{beginFrame, endFrame}, hitbox});
    }

    return vec;
}

PostHitProperties RecipeParser::parsePostHitProperties(const nlohmann::json &json_)
{
    PostHitProperties php;

    if (json_.contains("HardKnd"))
        php.hardKnd = json_["HardKnd"];
    if (json_.contains("GroundBounce"))
        php.groundBounce = json_["GroundBounce"];
    if (json_.contains("GroundBounceStrength"))
        php.groundBounceStrength = json_["GroundBounceStrength"];
    if (json_.contains("Wallbounce"))
        php.wallBounce = json_["Wallbounce"];
    if (json_.contains("WallbounceInertiaMultiplierX"))
        php.wallbounceInertiaMultiplierX = json_["WallbounceInertiaMultiplierX"];
    if (json_.contains("WallbounceInertiaMultiplierY"))
        php.wallbounceInertiaMultiplierY = json_["WallbounceInertiaMultiplierY"];
    if (json_.contains("Hitstun"))
        php.hitstun = json_["Hitstun"];
    if (json_.contains("Hitstop"))
        php.hitstop = json_["Hitstop"];
    if (json_.contains("Proratio"))
        php.proratio = json_["Proratio"];
    if (json_.contains("ForceCrouch"))
        php.forceCrouch = json_["ForceCrouch"];
    if (json_.contains("OpponentImpulseOnHit"))
        php.opponentImpulseOnHit = parseVector2<float>(json_["OpponentImpulseOnHit"]);
    if (json_.contains("OpponentImpulseOnAirhit"))
        php.opponentImpulseOnAirHit = parseVector2<float>(json_["OpponentImpulseOnAirhit"]);
    if (json_.contains("GroundHitstunAnimation"))
        php.groundHitstunAnimation = strToHitstunAnimation(json_["GroundHitstunAnimation"]);
    if (json_.contains("AirHitstunAnimation"))
        php.airHitstunAnimation = strToHitstunAnimation(json_["AirHitstunAnimation"]);

    return php;
}

CancelWindow RecipeParser::parseCancelWindow(const nlohmann::json &json_)
{
    CancelWindow cw;

    cw.first.first = json_["BeginFrame"];
    cw.first.second = json_["EndFrame"];

    for (const auto &el : json_["Options"])
        cw.second.insert(m_currentCharacterRecipe->states[el]);

    return cw;
}

void RecipeParser::parseAction(const nlohmann::json &json_)
{
    std::cout << json_["State"] << ": " << m_currentCharacterRecipe->states[json_["State"]] << std::endl;
    std::string actType = json_["ActionType"];
    m_currentCharacterRecipe->actions.push_back({});
    m_currentActionRecipe = &m_currentCharacterRecipe->actions[m_currentCharacterRecipe->actions.size() - 1];
    m_currentActionRecipe->m_actionType = actType;
    if (actType == "ActionCharacter")
        parseActionCharacter(json_);
    else if (actType == "Action_prolonged")
        parseActionProlonged(json_);
    else if (actType == "Action_jump")
        parseActionJump(json_);
    else if (actType == "Action_airjump")
        parseActionAirjump(json_);
    else if (actType == "Action_attack")
        parseActionAttack(json_);
    else
        std::cout << "Unknown action type: " << actType << std::endl;
}

void RecipeParser::parseActionCharacter(const nlohmann::json &json_)
{
    std::cout << json_["ActionType"] << std::endl;

    // Build vector of state IDs
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    // Parsing regular data
    m_currentActionRecipe->m_inputComparator = json_["InputComparator"];
    m_currentActionRecipe->m_state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->m_hurtboxes = parseHurtboxFramesVec(json_["Hurtboxes"]);
    m_currentActionRecipe->m_animation = m_animManager->getAnimID(json_["Animation"]);
    m_currentActionRecipe->m_counterWindow = parseTimelineProperty<bool>(json_["CounterWindow"]);
    m_currentActionRecipe->m_gravityWindow = parseTimelineProperty<bool>(json_["GravityWindow"]);
    m_currentActionRecipe->m_blockWindow = parseTimelineProperty<bool>(json_["BlockWindow"]);
    m_currentActionRecipe->m_transitionableFrom = transitionableFrom;
    m_currentActionRecipe->m_isAttack = json_["isAttack"];
    m_currentActionRecipe->m_isCrouchState = json_["isCrouchState"];
    m_currentActionRecipe->m_isThrowStartup = json_["isThrowStartup"];
    m_currentActionRecipe->m_consumeAirdash = json_["consumeAirdash"];
    m_currentActionRecipe->m_consumeAirjump = json_["consumeAirjump"];
    m_currentActionRecipe->m_waitAirdashTimer = json_["waitAirdashTimer"];
    m_currentActionRecipe->m_waitAirjumpTimer = json_["waitAirjumpTimer"];
    m_currentActionRecipe->m_isAirborne = json_["isAirborne"];

    // Handle extentions
    parseActionExtentions(json_);
}

void RecipeParser::parseActionProlonged(const nlohmann::json &json_)
{
    std::cout << json_["ActionType"] << std::endl;

    // Build vector of state IDs
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    // Parsing regular data
    m_currentActionRecipe->m_inputComparator = json_["InputComparator"];
    m_currentActionRecipe->m_inputComparatorProlong = json_["InputComparatorProlong"];
    m_currentActionRecipe->m_state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->m_hurtboxes = parseHurtboxFramesVec(json_["Hurtboxes"]);
    m_currentActionRecipe->m_animation = m_animManager->getAnimID(json_["Animation"]);
    m_currentActionRecipe->m_counterWindow = parseTimelineProperty<bool>(json_["CounterWindow"]);
    m_currentActionRecipe->m_gravityWindow = parseTimelineProperty<bool>(json_["GravityWindow"]);
    m_currentActionRecipe->m_blockWindow = parseTimelineProperty<bool>(json_["BlockWindow"]);
    m_currentActionRecipe->m_transitionableFrom = transitionableFrom;
    m_currentActionRecipe->m_isCrouchState = json_["isCrouchState"];
    m_currentActionRecipe->m_consumeAirdash = json_["consumeAirdash"];
    m_currentActionRecipe->m_consumeAirjump = json_["consumeAirjump"];
    m_currentActionRecipe->m_waitAirdashTimer = json_["waitAirdashTimer"];
    m_currentActionRecipe->m_waitAirjumpTimer = json_["waitAirjumpTimer"];
    m_currentActionRecipe->m_isAirborne = json_["isAirborne"];

    // Handle extentions
    parseActionExtentions(json_);
}

void RecipeParser::parseActionJump(const nlohmann::json &json_)
{
    std::cout << json_["ActionType"] << std::endl;

    // Build vector of state IDs
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    // Parsing regular data
    m_currentActionRecipe->m_state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->m_hurtboxes = parseHurtboxFramesVec(json_["Hurtboxes"]);
    m_currentActionRecipe->m_inputComparator = json_["InputComparator"];
    m_currentActionRecipe->m_impulse = parseVector2<float>(json_["Impulse"]);
    m_currentActionRecipe->m_duration = json_["PrejumpLen"];
    m_currentActionRecipe->m_maxHorInertia = json_["MaxHorInertia"];
    m_currentActionRecipe->m_animation = m_animManager->getAnimID(json_["Animation"]);
    m_currentActionRecipe->m_counterWindow = parseTimelineProperty<bool>(json_["CounterWindow"]);
    m_currentActionRecipe->m_blockWindow = parseTimelineProperty<bool>(json_["BlockWindow"]);
    m_currentActionRecipe->m_transitionableFrom = transitionableFrom;

    // Handle extentions
    parseActionExtentions(json_);
}

void RecipeParser::parseActionAirjump(const nlohmann::json &json_)
{
    std::cout << json_["ActionType"] << std::endl;

    // Build vector of state IDs
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    // Parsing regular data
    m_currentActionRecipe->m_state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->m_impulse = parseVector2<float>(json_["Impulse"]);
    m_currentActionRecipe->m_inputComparator = json_["InputComparator"];
    m_currentActionRecipe->m_hurtboxes = parseHurtboxFramesVec(json_["Hurtboxes"]);
    m_currentActionRecipe->m_animation = m_animManager->getAnimID(json_["Animation"]);
    m_currentActionRecipe->m_transitionableFrom = transitionableFrom;

    // Handle extentions
    parseActionExtentions(json_);
}

void RecipeParser::parseActionAttack(const nlohmann::json &json_)
{
    std::cout << json_["ActionType"] << std::endl;

    // Build vector of state IDs
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    // Parsing regular data
    m_currentActionRecipe->m_state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->m_inputComparator = json_["InputComparator"];
    m_currentActionRecipe->m_duration = json_["Duration"];
    for (const auto &el : json_["Hits"])
        m_currentActionRecipe->m_hits.emplace_back(m_hits.at(el));
    m_currentActionRecipe->m_hurtboxes = parseHurtboxFramesVec(json_["Hurtboxes"]);
    m_currentActionRecipe->m_animation = m_animManager->getAnimID(json_["Animation"]);
    m_currentActionRecipe->m_gravityWindow = parseTimelineProperty<bool>(json_["GravityWindow"]);
    m_currentActionRecipe->m_transitionableFrom = transitionableFrom;
    m_currentActionRecipe->m_isCrouchState = json_["isCrouchState"];
    m_currentActionRecipe->m_isAirborne = json_["isAirborne"];

    // Handle extentions
    parseActionExtentions(json_);
}

void RecipeParser::parseActionExtentions(const nlohmann::json &json_)
{
    if (json_.contains("extentionSwitchData"))
        parseExtentionSwitchData(json_["extentionSwitchData"]);

    if (json_.contains("extentionRealignData"))
        parseExtentionRealignData(json_["extentionRealignData"]);

    if (json_.contains("extentionAirActionTimers"))
        parseExtentionAirActionTimer(json_["extentionAirActionTimers"]);

    if (json_.contains("extentionUpdateMovementData"))
        parseExtentionUpdateMovementData(json_["extentionUpdateMovementData"]);

    if (json_.contains("extentionLandingRecoveryState"))
        parseExtentionLandingRecovery(json_["extentionLandingRecoveryState"]);

    if (json_.contains("extentionOutdatedTransition"))
        parseExtentionOutdatedTransition(json_["extentionOutdatedTransition"]);
}

void RecipeParser::parseExtentionSwitchData(const nlohmann::json &json_)
{
    std::cout << "Switch data\n";
    auto *extdata = new ActionExtentionSwitchData();

    if (json_.contains("realign"))
        extdata->realign = json_["realign"];
    if (json_.contains("timerValue"))
        extdata->timerValue = json_["timerValue"];
    if (json_.contains("velToInertia"))
        extdata->velToInertia = json_["velToInertia"];
    if (json_.contains("resetDefenseState"))
        extdata->resetDefenseState = json_["resetDefenseState"];
    if (json_.contains("setAirAttackFlag"))
        extdata->setAirAttackFlag = json_["setAirAttackFlag"];
    if (json_.contains("resetPushback"))
        extdata->resetPushback = json_["resetPushback"];
    if (json_.contains("callForPriority"))
        extdata->callForPriority = json_["callForPriority"];
    if (json_.contains("mulOwnVel"))
        extdata->mulOwnVel = parseVector2<float>(json_["mulOwnVel"]);
    if (json_.contains("mulOwnInr"))
        extdata->mulOwnInr = parseVector2<float>(json_["mulOwnInr"]);
    if (json_.contains("mulOwnDirVel"))
        extdata->mulOwnDirVel = parseVector2<float>(json_["mulOwnDirVel"]);
    if (json_.contains("mulOwnDirInr"))
        extdata->mulOwnDirInr = parseVector2<float>(json_["mulOwnDirInr"]);
    if (json_.contains("rawAddVel"))
        extdata->rawAddVel = parseVector2<float>(json_["rawAddVel"]);
    if (json_.contains("rawAddInr"))
        extdata->rawAddInr = parseVector2<float>(json_["rawAddInr"]);

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseExtentionRealignData(const nlohmann::json &json_)
{
    std::cout << "Realign data\n";
    auto *extdata = new ActionExtentionRealignData();

    if (json_.contains("realignData"))
        extdata->realignData = parseTimelineProperty<bool>(json_["realignData"]);

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseExtentionAirActionTimer(const nlohmann::json &json_)
{
    std::cout << "Air action timer data\n";
    auto *extdata = new ActionExtentionAirActionTimer();

    if (json_.contains("airjumpTimer"))
        extdata->airjumpTimer = json_["airjumpTimer"];

    if (json_.contains("airdashTimer"))
        extdata->airdashTimer = json_["airdashTimer"];

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseExtentionUpdateMovementData(const nlohmann::json &json_)
{
    std::cout << "Update movement data\n";
    auto *extdata = new ActionExtentionUpdateMovementData();

    if (json_.contains("mulOwnVel"))
        extdata->mulOwnVel = parseTimelineProperty<Vector2<float>>(json_["mulOwnVel"]);
    if (json_.contains("mulOwnInr"))
        extdata->mulOwnInr = parseTimelineProperty<Vector2<float>>(json_["mulOwnInr"]);
    if (json_.contains("mulOwnDirVel"))
        extdata->mulOwnDirVel = parseTimelineProperty<Vector2<float>>(json_["mulOwnDirVel"]);
    if (json_.contains("mulOwnDirInr"))
        extdata->mulOwnDirInr = parseTimelineProperty<Vector2<float>>(json_["mulOwnDirInr"]);
    if (json_.contains("rawAddVel"))
        extdata->rawAddVel = parseTimelineProperty<Vector2<float>>(json_["rawAddVel"]);
    if (json_.contains("rawAddInr"))
        extdata->rawAddInr = parseTimelineProperty<Vector2<float>>(json_["rawAddInr"]);

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseExtentionLandingRecovery(const nlohmann::json &json_)
{
    std::cout << "Landing recovery data\n";
    auto *extdata = new ActionExtentionLandingRecovery();

    if (json_.contains("recoveryState"))
        extdata->recoveryState = m_currentCharacterRecipe->states[json_["recoveryState"]];

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseExtentionOutdatedTransition(const nlohmann::json &json_)
{
    std::cout << "Outdated transition data\n";
    auto *extdata = new ActionExtentionOutdatedTransition();

    if (json_.contains("targetState"))
        extdata->targetState = m_currentCharacterRecipe->states[json_["targetState"]];

    m_currentActionRecipe->m_extentions.push_back(extdata);
}

void RecipeParser::parseParticlesSpawnData(const nlohmann::json &json_)
{
    std::string name = json_["SetName"];
    std::cout << "Parsing particle \"" << name << "\"\n";

    ParticlesSpawnData psd;

    if (json_.contains("Count"))
        psd.m_count = json_["Count"];

    if (json_.contains("Angle"))
        psd.m_angle = json_["Angle"];

    if (json_.contains("Scale"))
        psd.m_scale = json_["Scale"];

    if (json_.contains("BaseVelocity"))
        psd.m_baseVelocity = parseVector2<float>(json_["BaseVelocity"]);

    if (json_.contains("VelocityRange"))
        psd.m_velocityRange = parseVector2<float>(json_["VelocityRange"]);

    if (json_.contains("ReverseAccel"))
        psd.m_reverseAccel = json_["ReverseAccel"];

    if (json_.contains("AdditionalAccel"))
        psd.m_additionalAccel = parseVector2<float>(json_["AdditionalAccel"]);

    if (json_.contains("MinLifeTime"))
        psd.m_minLifeTime = json_["MinLifeTime"];

    if (json_.contains("MaxLifeTime"))
        psd.m_maxLifeTime = json_["MaxLifeTime"];

    if (json_.contains("Animation"))
        psd.m_animation = m_animManager->getAnimID(json_["Animation"]);

    if (json_.contains("LoopMethod"))
        psd.m_loopMethod = strToLoopMethod(json_["LoopMethod"]);

    if (json_.contains("BeginFrame"))
        psd.m_beginFrame = json_["BeginFrame"];

    if (json_.contains("BeginDirection"))
        psd.m_beginDirection = json_["BeginDirection"];

    m_particles[name] = psd;
}

void RecipeParser::parseHitData(const nlohmann::json &json_)
{
    std::string name = json_["HitName"];
    std::cout << "Parsing hit \"" << name << "\"\n";

    HitData hitdata;

    if (json_.contains("Damage"))
        hitdata.damage = json_["Damage"];
    if (json_.contains("ForcedClash"))
        hitdata.forcedClash = json_["ForcedClash"];
    if (json_.contains("OpponentPushbackOnBlock"))
        hitdata.opponentPushbackOnBlock = json_["OpponentPushbackOnBlock"];
    if (json_.contains("Blockstun"))
        hitdata.blockstun = json_["Blockstun"];
    if (json_.contains("ChipDamage"))
        hitdata.chipDamage = json_["ChipDamage"];
    if (json_.contains("HitBlockShakeAmp"))
        hitdata.hitBlockShakeAmp = json_["HitBlockShakeAmp"];
    if (json_.contains("CanBeBlockedAs"))
        for (auto &el : json_["CanBeBlockedAs"])
            hitdata.canBeBlockedAs.insert(strToBlockState(el));
    if (json_.contains("CancelsOnHit"))
        hitdata.cancelsOnHit = parseCancelWindow(json_["CancelsOnHit"]);
    if (json_.contains("CancelsOnBlock"))
        hitdata.cancelsOnBlock = parseCancelWindow(json_["CancelsOnBlock"]);
    if (json_.contains("HitProperties"))
        hitdata.hitProps = parsePostHitProperties(json_["HitProperties"]);
    if (json_.contains("CounterHitProperties"))
        hitdata.chProps = parsePostHitProperties(json_["CounterHitProperties"]);
    
    if (json_.contains("ParticlesOnBlock"))
        for (auto &el : json_["ParticlesOnBlock"])
            hitdata.particlesOnBlock.push_back(m_particles[el]);

    if (json_.contains("ParticlesOnHit"))
        for (auto &el : json_["ParticlesOnHit"])
            hitdata.particlesOnHit.push_back(m_particles[el]);

    if (json_.contains("ParticlesOnCH"))
        for (auto &el : json_["ParticlesOnCH"])
            hitdata.particlesOnCH.push_back(m_particles[el]);

    auto hboxes = parseHitboxes(json_["Hitboxes"]);
    
    Hit res(hitdata, hboxes);
    m_hits.emplace(name, std::move(res));
}

LOOPMETHOD RecipeParser::strToLoopMethod(const std::string &str_)
{
    if (str_ == "JUMP_LOOP")
        return LOOPMETHOD::JUMP_LOOP;

    if (str_ == "NOLOOP")
        return LOOPMETHOD::NOLOOP;

    if (str_ == "SWITCH_DIR_LOOP")
        return LOOPMETHOD::SWITCH_DIR_LOOP;

    throw std::string("Cannot identify loop method: " + str_);
}

BLOCK_STATE RecipeParser::strToBlockState(const std::string &str_)
{
    if (str_ == "HIGH")
        return BLOCK_STATE::HIGH;

    if (str_ == "LOW")
        return BLOCK_STATE::LOW;

    if (str_ == "AIR")
        return BLOCK_STATE::AIR;

    if (str_ == "AUTO")
        return BLOCK_STATE::AUTO;

    if (str_ == "NONE")
        return BLOCK_STATE::NONE;

    throw std::string("Cannot identify block state: " + str_);
}

HITSTUN_ANIMATION RecipeParser::strToHitstunAnimation(const std::string &str_)
{
    if (str_ == "HIGH")
        return HITSTUN_ANIMATION::HIGH;

    if (str_ == "MID")
        return HITSTUN_ANIMATION::MID;

    if (str_ == "LOW")
        return HITSTUN_ANIMATION::LOW;

    if (str_ == "CROUCH")
        return HITSTUN_ANIMATION::CROUCH;

    if (str_ == "FLOAT")
        return HITSTUN_ANIMATION::FLOAT;

    if (str_ == "NONE")
        return HITSTUN_ANIMATION::NONE;

    throw std::string("Cannot identify hitstun animation: " + str_);
}

template <typename T>
TimelineProperty<T> RecipeParser::parseTimelineProperty(const nlohmann::json &json_)
{
    return parseTimelinePropertyImpl(json_, static_cast<T*>(0) );
}

template <typename T>
TimelineProperty<Vector2<T>> RecipeParser::parseTimelinePropertyImpl(const nlohmann::json &json_, Vector2<T> *)
{
    TimelineProperty<Vector2<float>> tpt;

    for (auto &el : json_)
    {
        tpt.addPropertyValue(el["frame"], parseVector2<float>(el["value"]));
    }

    return tpt;
}

template <typename T>
TimelineProperty<T> RecipeParser::parseTimelinePropertyImpl(const nlohmann::json &json_, T*)
{
    TimelineProperty<T> tpt;

    for (auto &el : json_)
    {
        tpt.addPropertyValue(el["frame"], el["value"]);
    }

    return tpt;
}

template <typename T>
Vector2<T> RecipeParser::parseVector2(const nlohmann::json &json_)
{
    return Vector2<T>(json_["x"], json_["y"]);
}

Collider RecipeParser::parseCollider(const nlohmann::json &json_)
{
    return Collider(json_["x"], json_["y"], json_["w"], json_["h"]);
}

ActionExtention::ActionExtention(ExtentionType extType_) :
        m_extentionType(extType_)
{}

ActionExtentionSwitchData::ActionExtentionSwitchData() :
    ActionExtention(ExtentionType::SWITCH_DATA)
{}

void ActionRecipe::clear()
{
    for (auto *el : m_extentions)
        delete el;
}

CharacterRecipe::~CharacterRecipe()
{
    for (auto &el : actions)
    {
        el.clear();
    }
}

ActionExtentionRealignData::ActionExtentionRealignData() :
    ActionExtention(ExtentionType::REALIGN_DATA)
{
}

ActionExtentionAirActionTimer::ActionExtentionAirActionTimer() :
    ActionExtention(ExtentionType::AIR_ACTION_TIMER)
{
}

ActionExtentionLandingRecovery::ActionExtentionLandingRecovery() :
    ActionExtention(ExtentionType::LANDING_RECOVERY)
{
}

ActionExtentionOutdatedTransition::ActionExtentionOutdatedTransition() :
    ActionExtention(ExtentionType::OUTDATED_TRANSITION)
{
}

ActionExtentionUpdateMovementData::ActionExtentionUpdateMovementData() :
    ActionExtention(ExtentionType::UPDATE_MOVEMENT_DATA)
{
}
