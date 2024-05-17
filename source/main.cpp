// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Antonio Niño Díaz, 2008-2024
//
// This file is part of Nitro Engine
// #define LANG_ENGLISH
#include "iostream"
#include "BattleCore.h"

extern "C" {
#include "Constants.h"
#include "Characters/Herta.h"
#include "Characters/TrailBlazerPhysic.h"
#include "Characters/Monsters/SilvermaneSoldier.h"
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
    cout << "Name:" << name;
    battleCore.tick(nextActionCharacterId, action, {target});
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

    while (1) {
        NE_WaitForVBL(NE_UPDATE_GUI);
        NE_ProcessDualArg(Draw3DScene1, Draw3DScene2, &Scene1, &Scene2);

        scanKeys();

        // Your code goes here
    }

    return 0;
}
