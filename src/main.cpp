#include <iostream>
#include <string>

#include "ProjectileFactory.h"
#include "Stage1.h"
#include "Application.h"
#include "TimelineProperty.h"

#include "RecipeParser.h"

int main(int argc, char* args[])
{    
    Application app;
    std::cout << app.getBasePath() << std::endl;
    //RecipeParser pr(app.getAnimationManager(), app.getBasePath() + "/Resources/data.json");

    app.setLevel(gamedata::stages::stage1::stageID, new Stage1(&app, {gamedata::stages::stageWidth, gamedata::stages::stageHeight}, gamedata::stages::stage1::stageID));
    app.run();

    return 0;
}