#include "RecipeParser.h"

RecipeParser::RecipeParser()
{
    nlohmann::json j;
    j["a"] = "test";
    std::cout << j["a"] << std::endl;
}