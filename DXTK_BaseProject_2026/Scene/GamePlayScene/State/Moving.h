#pragma once
#include "Scene/GamePlayScene/Player.h"
#include "BaseState.h"
class Moving:public BaseState
{
public:

    // コンストラクタ
    Moving(Player* player);

    void Initialize() override;

    // デストラクタ
    ~Moving();

    void Update(float elapsedtime) override;

    void Render() override;

private:
    Player* m_player;
};
