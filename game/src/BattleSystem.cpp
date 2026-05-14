#include "BattleSystem.h"

void BattleSystem::StartBattle() {
    m_IsBattling = true;
}

void BattleSystem::StopBattle() {
    m_IsBattling = false;
}

void BattleSystem::Update(float) {
    if (!m_IsBattling) {
        return;
    }

    // TODO
    // Battle logic would go here
}

bool BattleSystem::IsBattling() const {
    return m_IsBattling;
}