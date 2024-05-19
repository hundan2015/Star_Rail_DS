// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2008-2024
//
// This file is part of Nitro Engine
#include <cstdint>

#include "nds/arm9/input.h"
#define LANG_ENGLISH
#include <iostream>

#include "BattleCore.h"
#include "Characters/Herta.h"
#include "Characters/Monsters/SilvermaneSoldier.h"
#include "Characters/TrailBlazerPhysic.h"
#include "Constants.h"

extern "C" {
#include <NEMain.h>

#include "NEGeneral.h"
}

using std::cout;
using std::string;
string endl = "\n";
typedef struct {
    int placeholder;
} SceneData1;

typedef struct {
    int placeholder;
} SceneData2;

void Draw3DScene1(void* arg) {
    SceneData1* Scene = (SceneData1*)arg;

    (void)Scene;  // Silence unused variable warning
}

void Draw3DScene2(void* arg) {
    SceneData2* Scene = (SceneData2*)arg;

    (void)Scene;  // Silence unused variable warning
}

void singleAction(int action, int target) {
    BattleCore& battleCore = BattleCore::getInstance();
    int nextActionCharacterId = battleCore.getNextAction();
    string name = characterStrings[battleCore.characters[nextActionCharacterId]
                                       ->characterGlobalId];
    cout << "Current action character id:" << nextActionCharacterId << endl;
    cout << "Name:" << name << endl;
    if (battleCore.characterBattleStates[target] == nullptr) {
        cout << "Bad target!" << endl;
        return;
    }
    if (battleCore.characters[nextActionCharacterId]->skills[action] ==
        nullptr) {
        cout << "Bad action!" << endl;
        return;
    }
    battleCore.tick(nextActionCharacterId, action,
                    battleCore.characters[nextActionCharacterId]
                        ->skills[action]
                        ->getTargets(battleCore.characterBattleStates, target));
    auto& hitInfos = battleCore.getHitInfoInTick();
    for (auto& i : hitInfos) {
        auto& targetCharacter = battleCore.characters[i.target];
        auto& targetCharacterState = battleCore.characterBattleStates[i.target];
        string criticalString = i.isCritical ? ",critical!" : "";
        cout << name << " hit "
             << characterStrings[targetCharacter->characterGlobalId]
             << criticalString << endl;
        cout << characterStrings[targetCharacter->characterGlobalId]
             << " HP:" << targetCharacterState->characterProperty->hp << endl;
    }
    if (battleCore.getGameState() != GOING) return;
    battleCore.resetHitInfoInTick();

    if (battleCore.battleCoreState == BEFORE_APPEND) {
        auto appendATKInfo = battleCore.appendATKs.front();
        auto tempName = characterStrings[appendATKInfo.attacker];
        battleCore.tick(0, 0, {});
        auto& appendHitInfos = battleCore.getHitInfoInTick();
        for (auto& i : appendHitInfos) {
            auto& targetCharacter = battleCore.characters[i.target];
            auto& targetCharacterState =
                battleCore.characterBattleStates[i.target];
            string criticalString = i.isCritical ? ",critical!" : "";
            cout << tempName << " have a append hit on "
                 << characterStrings[targetCharacter->characterGlobalId]
                 << criticalString << endl;
            cout << characterStrings[targetCharacter->characterGlobalId]
                 << " HP:" << targetCharacterState->characterProperty->hp
                 << endl;
        }
        if (battleCore.getGameState() != GOING) return;
        battleCore.resetHitInfoInTick();
    }
    if (battleCore.battleCoreState == BEFORE_ROUND) {
        battleCore.tick(0, 0, {});
        auto& appendHitInfos = battleCore.getHitInfoInTick();
        for (auto& i : appendHitInfos) {
            auto& attacker = battleCore.characters[i.attacker];
            auto tempName = characterStrings[attacker->characterGlobalId];
            auto& targetCharacter = battleCore.characters[i.target];
            auto& targetCharacterState =
                battleCore.characterBattleStates[i.target];
            string criticalString = i.isCritical ? ",critical!" : "";
            cout << tempName << " have a before round hit on "
                 << characterStrings[targetCharacter->characterGlobalId]
                 << criticalString << endl;
            cout << characterStrings[targetCharacter->characterGlobalId]
                 << " HP:" << targetCharacterState->characterProperty->hp
                 << endl;
        }
        if (battleCore.getGameState() != GOING) return;
        battleCore.resetHitInfoInTick();
    }
}

void registerPlayerToBattleCore(BattleCore& battleCore, Character* character,
                                int pos) {
    battleCore.characters[pos] = character;
    battleCore.characterBattleStates[pos] =
        std::unique_ptr<CharacterBattleState>(
            character->getInitCharacterBattleState());
    auto tmp = new CharacterProperty;
    *tmp = character->getInitProperty();
    battleCore.originalCharacterProperty[pos] = tmp;
    battleCore.characterBattleStates[pos]->characterLocalId = pos;
}

int getKeyNum(uint32_t key) {
    if (key & KEY_UP) return 0;
    if (key & KEY_DOWN) return 1;
    if (key & KEY_LEFT) return 2;
    if (key & KEY_RIGHT) return 3;
    if (key & KEY_A) return 4;
    if (key & KEY_B) return 5;
    if (key & KEY_X) return 6;
    if (key & KEY_Y) return 7;
    if (key & KEY_L) return 8;
    if (key & KEY_R) return 9;
    if (key & KEY_START) return 10;
    if (key & KEY_SELECT) return 11;
    return -1;
}

int main(int argc, char* argv[]) {
    SceneData1 Scene1 = {0};
    SceneData2 Scene2 = {0};

    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_VBLANK, NE_HBLFunc);

    NE_InitDual3D_DMA();
    NE_InitConsole();

    // Init mini

    BattleCore& battleCore = BattleCore::getInstance();
    // Add some players.
    auto p1 = std::make_unique<Character>(TrailBlazerPhysic());
    auto p2 = std::make_unique<Character>(SilvermaneSolder());
    auto p3 = std::make_unique<Character>(Herta());
    p3->eidolonLevel = 6;
    // p2->basicCharacterProperty->hp = 2000;
    registerPlayerToBattleCore(battleCore, p1.get(), 0);
    registerPlayerToBattleCore(battleCore, p3.get(), 1);
    registerPlayerToBattleCore(battleCore, p2.get(), 4);

    battleCore.initActionPoint();
    int action = 0;
    int target;
    int state = 0;

    while (1) {
        NE_WaitForVBL(NE_UPDATE_GUI);
        NE_ProcessDualArg(Draw3DScene1, Draw3DScene2, &Scene1, &Scene2);

        scanKeys();

        auto keys = keysHeld();
        int keyNum = getKeyNum(keys);
        if (state == 0) {
            static bool isFirst = true;
            if (isFirst) {
                cout << "input action:";
                isFirst = false;
            }
            if (keyNum != -1) {
                action = keyNum;
                cout << action << endl;
                isFirst = true;
                state = 1;
            }
        } else if (state == 1) {
            static int counter = 0;
            counter++;
            if (counter >= 60) {
                counter = 0;
                state = 2;
            }
        } else if (state == 2) {
            static bool isFirst = true;
            if (isFirst) {
                cout << "input target:";
                isFirst = false;
            }
            if (keyNum != -1) {
                target = keyNum;
                cout << target << endl;
                isFirst = true;
                singleAction(action, target);
                static auto& battleCore = BattleCore::getInstance();
                if (battleCore.getGameState() != GOING) {
                    return 0;
                }
                state = 3;
            }
        } else if (state == 3) {
            static int counter = 0;
            counter++;
            if (counter >= 60) {
                counter = 0;
                state = 0;
            }
        }
        // Your code goes here
    }

    return 0;
}
