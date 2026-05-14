#pragma once

class BattleSystem {
public:
    void StartBattle();
    void StopBattle();

    void Update(float dt);

    bool IsBattling() const;

private:
    bool m_IsBattling = false;
};