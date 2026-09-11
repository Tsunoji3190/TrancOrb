#pragma once
#include "SkillNode.h"
#include"../Scene/GamePlayScene/Player.h"
#include"../Scene/GamePlayScene/Manager/OrbManager.h"
namespace Itsuki
{
	using namespace Itsuki;

	class SkillTree
    {
    public:

        SkillTree(GameContext& gameContext, Player& player, OrbManager manager) 
            : ref_player{player}, ref_orbManager{manager}
        {
            //ノードを作っていく
            m_node[0]->SetNode(gameContext, nullptr, [this] { AddOrb(1); }, 1, L"Resources/Textures/テストオーブ.png");

            //
            m_node[1]->SetNode(gameContext, m_node[0], [this] { AddOrbCost(1); }, 10, L"Resources/Textures/テストオーブ.png");
            m_node[2]->SetNode(gameContext, m_node[1], [this] { AddOrbCost(1); }, 20, L"Resources/Textures/テストオーブ.png");
            m_node[3]->SetNode(gameContext, m_node[2], [this] { AddOrbCost(1); }, 50, L"Resources/Textures/テストオーブ.png");

            //
            m_node[4]->SetNode(gameContext, m_node[0], [this] { Magnet(0.2); }, 10, L"Resources/Textures/テストオーブ.png");
            m_node[5]->SetNode(gameContext, m_node[4], [this] { Magnet(0.2); }, 15, L"Resources/Textures/テストオーブ.png");
            m_node[6]->SetNode(gameContext, m_node[5], [this] { Magnet(0.2); }, 20, L"Resources/Textures/テストオーブ.png");

            //
            m_node[7]->SetNode(gameContext, m_node[0], [this] { IntervalDecrease(0.5); }, 10, L"Resources/Textures/テストオーブ.png");
            m_node[8]->SetNode(gameContext, m_node[7], [this] { IntervalDecrease(0.5); }, 50, L"Resources/Textures/テストオーブ.png");
            m_node[9]->SetNode(gameContext, m_node[8], [this] { IntervalDecrease(0.5); }, 100, L"Resources/Textures/テストオーブ.png");

            //
            m_node[10]->SetNode(gameContext, m_node[0], [this] { Time(5); }, 10, L"Resources/Textures/テストオーブ.png");
            m_node[11]->SetNode(gameContext, m_node[10], [this] { Time(10); }, 100, L"Resources/Textures/テストオーブ.png");
            m_node[12]->SetNode(gameContext, m_node[11], [this] { Time(15); }, 1500, L"Resources/Textures/テストオーブ.png");

            //
            m_node[13]->SetNode(gameContext, nullptr, [this] { GetSkill(0); }, 50, L"Resources/Textures/テストオーブ.png");
        }

        //オーブの数の追加
        void AddOrb(int num);

        //オーブの価値増加
        void AddOrbCost(int num);

        //オーブを取得できる範囲増加
        void Magnet(float num);

        //価値が2倍オーブの追加
        void LuckOrb(float num);

        //いいことが起きる確率を増加させる
        void Luck(float num);

        //脚の速さ増加
        void Speed(int num)
        {

        }

        //時間の延長
        void Time(int num)
        {

        }

        //スキルを追加
        void GetSkill(int num);

        //オーブのインターバル減少
        void IntervalDecrease(float time);


    private:

        //スキルノードを数分入れる
        std::vector<SkillNode*> m_node;

        //プレイヤー
        Player& ref_player;

        //オーブマネージャー
        OrbManager& ref_orbManager;
    };

}