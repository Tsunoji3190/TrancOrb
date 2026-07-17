#pragma once
#include "Player.h"
#include "Factory/OrbManager.h"
class Status
{
public:

	Status(GameContext& gamecontext, Player * player, OrbManager* manager);

	void Initialize();

	~Status();

	void Update(float& timer);

	void Render();

    void AddOrbCount(int orb)
    {
        m_OrbCount += orb;
    }

    int GetOrbCount()
    {
        return m_OrbCount;
    }

    bool IsAboveSteak(float steak)
    {
        return (steak <= m_OrbCount);
    }

    bool IsClear()
    {
        return m_isClear;
    }

private:

    int FIRST_TIMER = 10;

    //ゲームコンテキスト
    GameContext& m_gameContext;

	//プレイヤー
	Player* m_player;

	//オーブマネージャー
	OrbManager* m_orbManager;

	    // 持ってる星の数
    int m_OrbCount;

    //アップグレードするのに必要なオーブ数
    int m_JumpSteak = 5;
    int m_OrbNumberSteak = 10;
    int m_OrbValueSteak = 20;
    int m_timerSteak = 10;

    //追加する数
    float m_JumpAdd = 0.005;
    int m_OrbNumberAdd = 1;
    int m_OrbValueAdd = 1;
    int m_timerAdd = 5;

    //クリアかどうか判定する
    bool m_isClear;

    //現在のレベル
    int m_JumpLevel = 1;
    int m_OrbNumberLevel = 1;
    int m_OrbValueLevel = 1;
    int m_timerLevel = 1;
};
