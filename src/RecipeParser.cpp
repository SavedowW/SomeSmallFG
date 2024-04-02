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
    m_currentActionRecipe->ActionType = actType;
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
    std::cout << json_["InputComparator"] << std::endl;
    std::cout << json_["Animation"] << std::endl;
    auto hbs = parseHurtboxFramesVec(json_["Hurtboxes"]);
    auto counterWindow = parseTimelineProperty<bool>(json_["CounterWindow"]);
    auto gravityWindow = parseTimelineProperty<bool>(json_["GravityWindow"]);
    auto blockWindow = parseTimelineProperty<bool>(json_["BlockWindow"]);
    std::vector<int> statesFrom;
    for (auto &el : json_["TransitionableFrom"].get<std::vector<std::string>>())
        statesFrom.push_back(m_currentCharacterRecipe->states[el]);
    StateMarker transitionableFrom(m_currentCharacterRecipe->states.size(), statesFrom);

    m_currentActionRecipe->InputComparator = json_["InputComparator"];
    m_currentActionRecipe->state = m_currentCharacterRecipe->states[json_["State"]];
    m_currentActionRecipe->hurtboxes = hbs;
    // TODO: calculate animation
    m_currentActionRecipe->counterWindow = counterWindow;
    m_currentActionRecipe->gravityWindow = gravityWindow;
    m_currentActionRecipe->blockWindow = blockWindow;
    m_currentActionRecipe->transitionableFrom = transitionableFrom;
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
