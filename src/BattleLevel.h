#ifndef BATTLE_LEVEL_H_
#define BATTLE_LEVEL_H_

#include "Level.h"
#include "Camera.h"
#include "Background.h"
#include "Character.h"
#include "Application.h"
#include <array>
#include "FrameTimer.h"

template <typename BackType>
class BattleLevel : public Level
{
public:
    BattleLevel(Application *application_, const Vector2<float>& size_, int lvlId_) :
    	Level(application_, size_, lvlId_),
    	m_camera(gamedata::stages::startingCameraPos, {gamedata::global::cameraWidth, gamedata::global::cameraHeight}, m_size),
        m_frameTimer(gamedata::stages::framesBeforeZoom),
        m_maxCharRange(gamedata::stages::maxCharRange)
    {
    	std::cout << "m_background: " << m_background.get() << std::endl;
    }

    virtual void enter() override
    {
        Level::enter();
        m_camera.setPos(gamedata::stages::startingCameraPos);
        m_camera.setScale(gamedata::stages::startingCameraScale);
    }

    virtual ~BattleLevel()
    {

    }

protected:
    void update() override
    {
        //std::cout << m_camera.getPos() << std::endl;
        //system("cls");

        // Align camera with players
        if (m_frameTimer.update())
        {
            auto p1 = m_characters[0]->getPos();
            auto p2 = m_characters[1]->getPos();
            float charRangeForScale = abs((p1 - p2).mulComponents(Vector2{1.0f, 3.0f}).getLen());

            m_camera.smoothScaleTowards(utils::clamp((charRangeForScale + gamedata::stages::reservedScaleRange) / gamedata::stages::maxCharRange * gamedata::stages::maxCameraScale,
            gamedata::stages::minCameraScale, gamedata::stages::maxCameraScale));

            float x = (p1.x + p2.x) / 2;
            m_camera.smoothMoveTowards(Vector2{x, std::lerp(std::max(p1.y, p2.y), std::min(p1.y, p2.y), gamedata::stages::heightMovePriority)} - Vector2{0.0f,  gamedata::stages::verticalCameraOffset});
        }

        std::array<CharacterUpdateRes, 2> results;

        results[0] = m_characters[0]->update();
        results[1] = m_characters[1]->update();

        // Solve pushbox interactions

        // Check if comebody crossed level bounds
        std::array<bool, 2> hitsWall = {false, false};
        for (int i = 0; i < 2; ++i)
        {
            int stageBoundResult = results[i].pushbox.isWithinHorizontalBounds(0.0f, m_size.x);
            if (stageBoundResult < 0)
            {
                auto newPos = Vector2<float>{results[i].newPos.x - results[i].pushbox.x, results[i].newPos.y};
                std::cout << "Moving char " << i << " to " << newPos << std::endl;
                m_characters[i]->setPos(newPos);
                hitsWall[i] = true;
            }
            else if (stageBoundResult > 0)
            {
                auto newPos = Vector2<float>{m_size.x - (results[i].pushbox.x + results[i].pushbox.w - results[i].newPos.x), results[i].newPos.y};
                //td::cout << "Moving char " << i << " to " << newPos << std::endl;
                m_characters[i]->setPos(newPos);
                hitsWall[i] = true;
            }
        }

        // Check if range between characters is too high
        float rangeBetweenCharacters = abs(m_characters[0]->getPos().x - m_characters[1]->getPos().x);
        if (rangeBetweenCharacters > m_maxCharRange)
        {
            //std::cout << "Too far!\n";

            float exceedRange = rangeBetweenCharacters - m_maxCharRange;
            std::array<float, 2> movement = {0, 0};

            auto horDirToEnemy = m_characters[1]->getPos().x - m_characters[0]->getPos().x;

            if (utils::sameSign(-results[0].moveOffset.x, horDirToEnemy))
                movement[0] = abs(results[0].moveOffset.x);

            if (utils::sameSign(results[1].moveOffset.x, horDirToEnemy))
                movement[1] = abs(results[1].moveOffset.x);

            float totalMovement = movement[0] + movement[1];
            if (totalMovement <= 0)
                throw std::runtime_error("Total movement cannot be <= 0");

            m_characters[0]->setPos(m_characters[0]->getPos() + Vector2(horDirToEnemy, 0.0f).normalised() * (exceedRange * movement[0] / totalMovement));
            m_characters[1]->setPos(m_characters[1]->getPos() + Vector2(-horDirToEnemy, 0.0f).normalised() * (exceedRange * movement[1] / totalMovement));
        }

        // Check if characters collided with each other
        auto pb1 = m_characters[0]->getPushbox();
        auto pb2 = m_characters[1]->getPushbox();
        auto pbres = pb1.getHorizontalOverlap(pb2);
        if (pb1.isCollideWith(pb2) && pbres.overlapWidth != 0)
        {
            float approachSpd = (results[0].moveOffset.x * m_characters[0]->getHorDirToEnemy().x + results[1].moveOffset.x * m_characters[1]->getHorDirToEnemy().x);
            auto maxPushRange = gamedata::stages::maxPushRange;
            if (approachSpd >= (pb1.w + pb2.w) / gamedata::stages::maxPushRangeTreshold)
            {
                maxPushRange = approachSpd;
            }

            auto dirToP2 = m_characters[0]->getHorDirToEnemy();

            std::array<Vector2<float>, 2> directionToPush = {dirToP2.mulComponents(Vector2{-1.0f, 0.0f}), dirToP2};

            if (dirToP2.x == 0)
            {
                if (results[0].newPos.y > results[1].newPos.y && results[0].moveOffset.x != 0)
                {
                    auto p1dir = results[0].moveOffset;
                    p1dir.y = 0;
                    p1dir = p1dir.normalised();
                    directionToPush[0] = p1dir;
                    directionToPush[1] = p1dir * -1;
                }
                else if (results[1].newPos.y > results[0].newPos.y && results[1].moveOffset.x != 0)
                {
                    auto p2dir = results[1].moveOffset;
                    p2dir.y = 0;
                    p2dir = p2dir.normalised();
                    directionToPush[0] = p2dir * -1;
                    directionToPush[1] = p2dir;
                }
                else if (m_characters[0]->getFullVelocity().x != 0)
                {
                    auto p1dir = m_characters[0]->getFullVelocity();
                    p1dir.y = 0;
                    p1dir = p1dir.normalised();
                    directionToPush[0] = p1dir;
                    directionToPush[1] = p1dir * -1;
                }
                else if (m_characters[1]->getFullVelocity().x != 0)
                {
                    auto p2dir = m_characters[1]->getFullVelocity();
                    p2dir.y = 0;
                    p2dir = p2dir.normalised();
                    directionToPush[0] = p2dir * -1;
                    directionToPush[1] = p2dir;
                }
                else
                {
                    directionToPush[0].x = 1.0f;
                    directionToPush[1].x = -1.0f;
                }
            }

            float totalRangeToPush = std::min(pbres.overlapWidth, maxPushRange);
            std::array<float, 2> maxRangeToPush;
            maxRangeToPush[0] = ( directionToPush[0].x == -1 ? pb1.rangeToLeftBound(0) : pb1.rangeToRightBound(m_size.x));
            maxRangeToPush[1] = ( directionToPush[1].x == -1 ? pb2.rangeToLeftBound(0) : pb2.rangeToRightBound(m_size.x));

            std::array<float, 2> rangeToPush = {totalRangeToPush / 2.0f, totalRangeToPush / 2.0f};
            if (rangeToPush[0] > maxRangeToPush[0])
            {
                float exceed = rangeToPush[0] - maxRangeToPush[0];
                rangeToPush[0] -= exceed;
                rangeToPush[1] += exceed;
            }
            if (rangeToPush[1] > maxRangeToPush[1])
            {
                float exceed = rangeToPush[1] - maxRangeToPush[1];
                rangeToPush[1] -= exceed;
                rangeToPush[0] += exceed;
            }

            //std::cout << "rangeToPush: " << rangeToPush << std::endl;

            m_characters[0]->setPos(m_characters[0]->getPos() + directionToPush[0] * rangeToPush[0]);
            m_characters[1]->setPos(m_characters[1]->getPos() + directionToPush[1] * rangeToPush[1]);
        }


        // Update block state
        for (int pid = 0; pid <= 1; ++pid)
            m_characters[pid]->updateBlockState();

        // Check for hitbox interactions
        //auto hits1 = m_characters[0]->getHits();
        //auto hits2 = m_characters[1]->getHits();
        std::array<HitsVec, 2> hits = {m_characters[0]->getHits(), m_characters[1]->getHits()};
        //auto hurtboxes1 = m_characters[0]->getHurtboxes();
        //auto hurtboxes2 = m_characters[1]->getHurtboxes();
        std::array<HurtboxVec, 2> hurtboxes = {m_characters[0]->getHurtboxes(), m_characters[1]->getHurtboxes()};

        for (int pid = 0; pid <= 1; ++pid)
        {
            int p2id = 1 - pid;
            bool hitFound = false;
            for (int ihit = 0; ihit < hits[pid].size() && !hitFound; ++ihit)
            {
                auto hboxes = hits[pid][ihit].getHitboxes();
                for (int ihitbox = 0; ihitbox < hboxes.size() && !hitFound; ++ihitbox)
                {
                    for (int ihurtbox = 0; ihurtbox < hurtboxes[p2id].size() && !hitFound; ++ihurtbox)
                    {
                        if (hboxes[ihitbox].isCollideWith(hurtboxes[p2id][ihurtbox]))
                        {
                            hitFound = true;
                            HitEvent ev;
                            ev.m_hittingPlayerId = pid + 1;
                            ev.m_hitData = hits[pid][ihit].getHitData();
                            auto hitres = m_characters[p2id]->applyHit(ev);
                            m_characters[pid]->applyHit(ev, hitres);
                        }
                    }
                }
            }
        }
    }

    void draw() override
    {
    	auto &renderer = *m_application->getRenderer();
    	renderer.fillRenderer(SDL_Color{ 60, 197, 255, 255 });

    	if (m_background.get())
    		m_background->draw(renderer, m_camera);

        for (auto &el : m_characters)
            el->draw(*m_application->getRenderer(), m_camera);

    	renderer.updateScreen();
    }

    FrameTimer m_frameTimer;

    Camera m_camera;
    std::unique_ptr<BackType> m_background;

    std::array<std::unique_ptr<Character>, 2> m_characters;
    int m_maxCharRange;
};

#endif