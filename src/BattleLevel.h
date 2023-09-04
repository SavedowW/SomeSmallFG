#ifndef BATTLE_LEVEL_H_
#define BATTLE_LEVEL_H_

#include "Level.h"
#include "Camera.h"
#include "Background.h"
#include "Character.h"
#include "Application.h"
#include <array>
#include "FrameTimer.h"
#include <SDL_ttf.h>
#include "ParticleManager.h"
#include "PriorityHandler.h"
#include <ranges>

template <typename BackType>
class BattleLevel : public Level
{
public:
    BattleLevel(Application *application_, const Vector2<float>& size_, int lvlId_) :
    	Level(application_, size_, lvlId_),
    	m_camera(gamedata::stages::startingCameraPos, {gamedata::global::cameraWidth, gamedata::global::cameraHeight}, m_size),
        m_frameTimer(gamedata::stages::framesBeforeZoom),
        m_maxCharRange(gamedata::stages::maxCharRange),
        m_particleManager(application_->getRenderer(), application_->getAnimationManager())
    {
        subscribe(EVENTS::FN2);
    }

    void receiveInput(EVENTS event, const float scale_) override
    {
        Level::receiveInput(event, scale_);
        if (scale_ > 0 && event == EVENTS::FN2)
        {
            std::cout << "EVENT\n";
            ParticleSpawnData spdata;
            spdata.m_pos = m_camera.getPos();
            spdata.m_particleType = PARTICLE_TYPES::BLOCK;
            spdata.m_flip = (rand() % 2 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            //spdata.m_scale = ((rand() % 101) + 20) / 100.0f;
            spdata.m_scale = 0.5f;
            spdata.m_angle = 45;
            //spdata.m_angle = rand() % 360;
            m_particleManager.spawnParticles(spdata);
        }
    }

    virtual void enter() override
    {
        Level::enter();
        m_camera.setPos(gamedata::stages::startingCameraPos);
        m_camera.setScale(gamedata::stages::startingCameraScale);

        auto &renderer = *m_application->getRenderer();

        m_shadowsLayer = renderer.createTexture(gamedata::global::cameraWidth, gamedata::global::cameraHeight);
        auto blendmode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
        SDL_SetTextureBlendMode(m_shadowsLayer, blendmode);
        SDL_SetTextureAlphaMod(m_shadowsLayer, 150);
    }

    void startFlash(int lockedDuration_, int alphaDuration_)
    {
        m_flashAlphaTimer.begin(alphaDuration_);
        m_flashLockedTimer.begin(lockedDuration_);
    }

    virtual ~BattleLevel()
    {
        SDL_DestroyTexture(m_shadowsLayer);
    }

protected:
    void update() override
    {
        //std::cout << m_camera.getPos() << std::endl;
        //system("cls");

        if (m_flashLockedTimer.update())
        {
            if (m_flashAlphaTimer.update())
            {
                m_flashAlphaTimer.begin(0);
                m_flashLockedTimer.begin(0);
            }
        }

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

        auto priorityList = m_priorityHandler.getUpdatedPriority();

        for (const auto &i : priorityList)
            results[i] = m_characters[i]->update();

        results[0].pushbox = m_characters[0]->getPushbox();
        results[1].pushbox = m_characters[1]->getPushbox();

        // Solve pushbox interactions

        // Check if comebody crossed level bounds
        std::array<bool, 2> hitsWall = {false, false};
        for (const auto &i : priorityList)
        {
            int stageBoundResult = results[i].pushbox.isWithinHorizontalBounds(0.0f, m_size.x);
            if (stageBoundResult < 0)
            {
                auto offset = Vector2<float>{results[i].pushbox.x, 0};
                m_characters[i]->setPos(m_characters[i]->getPos() - offset);
                float pushback = m_characters[i]->touchedWall(stageBoundResult);
                hitsWall[i] = true;

                const Vector2 dirFromWall{1, 0};
                m_characters[1-i]->takePushback(dirFromWall * pushback);
            }
            else if (stageBoundResult > 0)
            {
                auto offset = Vector2<float>{results[i].pushbox.x + results[i].pushbox.w - m_size.x, 0};
                m_characters[i]->setPos(m_characters[i]->getPos() - offset);
                float pushback = m_characters[i]->touchedWall(stageBoundResult);
                hitsWall[i] = true;

                const Vector2 dirFromWall{-1, 0};
                m_characters[1-i]->takePushback(dirFromWall * pushback);
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

            int p1wall = -horDirToEnemy / abs(horDirToEnemy);
            m_characters[0]->touchedWall(p1wall);
            m_characters[1]->touchedWall(-p1wall);
        }

        // Check if characters collided with each other
        auto pb1 = m_characters[0]->getPushbox();
        auto pb2 = m_characters[1]->getPushbox();
        auto pbres = pb1.getHorizontalOverlap(pb2);
        if (pb1.isCollideWith(pb2) && pbres.overlapWidth != 0 && !m_characters[0]->passableThrough() && !m_characters[1]->passableThrough())
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
        for (const auto &pid : priorityList)
            m_characters[pid]->updateBlockState();

        // Check for hitbox interactions
        bool noHit = true;
        std::array<HitsVec, 2> hits = {m_characters[0]->getHits(), m_characters[1]->getHits()};
        std::array<HurtboxVec, 2> hurtboxes = {m_characters[0]->getHurtboxes(), m_characters[1]->getHurtboxes()};

        //Check for forced clashes
        for (auto &hit1 : hits[0])
        {
            for (auto &hit2 : hits[1])
            {
                if (hit1.forcedClash || hit2.forcedClash)
                {
                    auto hboxes1 = hit1.getHitboxes();
                    for (auto &hbox1 : hboxes1)
                    {
                        auto hboxes2 = hit2.getHitboxes();
                        for (auto &hbox2 : hboxes2)
                        {
                            if (hbox1.second.isCollideWith(hbox2.second))
                            {
                                m_characters[0]->applyClash(hit1);
                                m_characters[1]->applyClash(hit2);
                                startFlash(10, 5);

                                ParticleSpawnData spdata;
                                auto pos = hbox1.second.getOverlapArea(hbox2.second).getCenter();
                                spdata.m_pos = pos;
                                spdata.m_particleType = PARTICLE_TYPES::CLASH;
                                spdata.m_scale = 0.65f;
                                m_particleManager.spawnParticles(spdata);

                                noHit = false;
                            }
                        }
                    }
                }
            }
        }

        if (noHit)
        {
            // Check for hits
            for (const auto &pid : priorityList)
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
                            if (hboxes[ihitbox].second.isCollideWith(hurtboxes[p2id][ihurtbox]))
                            {
                                hitFound = true;
                                HitEvent ev;
                                ev.m_hittingPlayerId = pid + 1;
                                ev.m_hitData = hits[pid][ihit].getHitData();
                                m_characters[p2id]->applyHit(ev);
                                m_characters[pid]->applyHit(ev);
                                m_camera.startShake(ev.m_hitData.hitBlockShakeAmp, ev.m_hitData.hitProps.hitstop + 1);

                                auto hitpos = hitutils::getHitPosition(hboxes, hurtboxes[p2id]);
                                m_hitpos = hitpos - m_hitsize / 2;
                                m_characters[pid]->generateHitParticles(ev, hitpos);

                                noHit = false;
                            }
                        }
                    }
                }
            }
        }

        // Check for possible throws
        for (const auto &i : priorityList)
        {
            m_characters[i]->attemptThrow();
            m_characters[i]->attemptThrow();
        }

        if (noHit)
        {
            // Check for clashes
            for (auto &hit1 : hits[0])
            {
                auto hboxes1 = hit1.getHitboxes();
                for (auto &hbox1 : hboxes1)
                {
                    for (auto &hit2 : hits[1])
                    {
                        auto hboxes2 = hit2.getHitboxes();
                        for (auto &hbox2 : hboxes2)
                        {
                            if (hbox1.second.isCollideWith(hbox2.second))
                            {
                                m_characters[0]->applyClash(hit1);
                                m_characters[1]->applyClash(hit2);
                                startFlash(10, 5);

                                ParticleSpawnData spdata;
                                auto pos = hbox1.second.getOverlapArea(hbox2.second).getCenter();
                                spdata.m_pos = pos;
                                spdata.m_particleType = PARTICLE_TYPES::CLASH;
                                spdata.m_scale = 0.65f;
                                m_particleManager.spawnParticles(spdata);
                            }
                        }
                    }
                }
            }

        }

        m_camera.update();
        m_hud.update();
        m_particleManager.update();
        std::cout << m_characters[0]->CharStateData() << " | " << m_characters[1]->CharStateData() << std::endl;
    }

    void draw() override
    {
    	auto &renderer = *m_application->getRenderer();
    	renderer.fillRenderer(SDL_Color{ 60, 197, 255, 255 });

    	if (m_background.get())
    		m_background->draw(renderer, m_camera);

        bool inFlash = m_flashLockedTimer.isActive() || m_flashAlphaTimer.isActive();
        if (inFlash)
        {
            float alpha = 1 - m_flashAlphaTimer.getProgressNormalized();
            renderer.fillRectangle({0, 0}, {gamedata::global::cameraWidth, gamedata::global::cameraHeight}, {m_flashColor.r, m_flashColor.g, m_flashColor.b, Uint8(m_flashColor.a * alpha)});
        }

        auto priorityList = m_priorityHandler.getCurrentPriority();

        // TODO: move shadow or reflection logic to specific stage
        renderer.setRenderTarget(m_shadowsLayer);
        renderer.fillRenderer({0, 0, 0, 0});
        for (auto &chr : m_characters)
        {
            float angle = (chr->getPos().x / m_size.x) * 70 - 35;
            angle = angle * 3.1415f / 180.0f;
            chr->drawGroundProjection(renderer, m_camera, angle);
        }
        renderer.setRenderTarget(nullptr);
        renderer.renderTexture(m_shadowsLayer, 0, 0, gamedata::global::cameraWidth, gamedata::global::cameraHeight);

        for (const auto &i : priorityList | std::views::reverse)
            m_characters[i]->draw(*m_application->getRenderer(), m_camera);

        m_hud.draw(*m_application->getRenderer(), m_camera);

        m_particleManager.draw(m_camera);

        renderer.fillRectangle(m_hitpos, m_hitsize, {255, 255, 255, 255}, m_camera);

    	renderer.updateScreen();
    }

    FrameTimer m_frameTimer;

    Camera m_camera;
    std::unique_ptr<BackType> m_background;

    std::array<std::unique_ptr<Character>, 2> m_characters;
    int m_maxCharRange;
    HUD m_hud;
    ParticleManager m_particleManager;

    PriorityHandler m_priorityHandler;

    SDL_Texture *m_shadowsLayer;

    SDL_Color m_flashColor = {0, 0, 0, 200};
    FrameTimer m_flashLockedTimer;
    FrameTimer m_flashAlphaTimer;

    Vector2<float> m_hitpos;
    Vector2<float> m_hitsize = {20.0f, 20.0f};
};

#endif