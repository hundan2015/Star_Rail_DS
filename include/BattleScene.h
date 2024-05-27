#ifndef BATTLE_SCENE_H
#define BATTLE_SCENE_H

#include <memory>
#include <vector>

#include "Character.h"
#include "NitroEngineAdvance/NEA_2D.h"
#include "NitroEngineAdvance/NEA_Object.h"

class BattleScene {
   public:
    std::vector<std::unique_ptr<Sprite3D>> sprites;
    std::unique_ptr<NEA_Object> sceneObject;
    void initBattleScene() {}
    void drawBattleScene() const {
        // Draw scene.
        if (sceneObject != nullptr) {
            sceneObject->draw();
        }
        // Draw sprites
        for (const auto &i : sprites) {
            if (i != nullptr) i->draw();
        }
    }
};

#endif