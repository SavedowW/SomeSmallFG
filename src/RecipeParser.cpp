#include "RecipeParser.h"
#include <map>

RecipeParser::RecipeParser(const std::string &filepath_)
{
    std::ifstream finp(filepath_);
    if (!finp.is_open())
    {
        std::cout << "Can't open file\n";
    }
    nlohmann::json data = nlohmann::json::parse(finp);

    //auto hbv = parseHurtboxFramesVec(data["hurtboxvecexample"]);

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

void RecipeParser::parseAction(const nlohmann::json &json_)
{
    std::cout << json_["State"] << ": " << m_currentCharacterRecipe->states[json_["State"]] << std::endl;
    std::string actType = json_["ActionType"];
    m_currentCharacterRecipe->actions.push_back({});
    m_currentActionRecipe = &m_currentCharacterRecipe->actions[m_currentCharacterRecipe->actions.size() - 1];
    m_currentActionRecipe->m_actionType = actType;
    if (actType == "ActionCharacter")
    {
        parseActionCharacter(json_);
    }
    else
    {
        std::cout << "Unknown action type: " << actType << std::endl;
    }
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
    // TODO: calculate animation
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

void RecipeParser::parseActionExtentions(const nlohmann::json &json_)
{
    if (json_.contains("extentionSwitchData"))
        parseExtentionSwitchData(json_["extentionSwitchData"]);

    if (json_.contains("extentionRealignData"))
        parseExtentionRealignData(json_["extentionRealignData"]);
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
