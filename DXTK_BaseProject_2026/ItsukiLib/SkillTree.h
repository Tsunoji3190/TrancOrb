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

        SkillTree(Player& player, OrbManager& manager) 
            : ref_player{player}, ref_orbManager{manager}
        {
        }

        void Update(float elapsedtime,GameContext& gameContext)
        {
            for (int i = 0; i < NODE_COUNT; i++)
            {
                m_node[i]->Update(elapsedtime, gameContext);
            }

        }

        void Render()
        {
            for (int i = 0; i < NODE_COUNT; i++)
            {
                m_node[i]->Render();
            }
        }

        void SetTrees(GameContext& gameContext)
        {
            auto size = gameContext.deviceResources.GetOutputSize();
            float width = size.right;   // ウィンドウのクライアント幅
            float height = size.bottom; // ウィンドウのクライアント高さ

              for (auto& node : m_node)
            {
                node = new SkillNode();
            }

            // ノードを作っていく
            m_node[0]->SetNode(
                gameContext, nullptr, {width/2,height/2},[this] { AddOrb(1); }, 1, L"Resources/Textures/テストオーブ.png");

            //
            m_node[1]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{100, 100}},
                [this] { AddOrbCost(1); }, 10, L"Resources/Textures/テストオーブ.png");

            m_node[2]->SetNode(
                gameContext, m_node[1], {m_node[1]->GetPosition() + SimpleMath::Vector2{100, 100}},
                [this] { AddOrbCost(1); }, 20, L"Resources/Textures/テストオーブ.png");

            m_node[3]->SetNode(
                gameContext, m_node[2], {m_node[1]->GetPosition() + SimpleMath::Vector2{100, 100}},
                [this] { AddOrbCost(1); }, 50, L"Resources/Textures/テストオーブ.png");

            //
            m_node[4]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-100, 100}},
                [this] { Magnet(0.2); }, 10, L"Resources/Textures/テストオーブ.png");

            m_node[5]->SetNode(
                gameContext, m_node[4], {m_node[4]->GetPosition() + SimpleMath::Vector2{-100, 100}},
                [this] { Magnet(0.2); }, 15, L"Resources/Textures/テストオーブ.png");

            m_node[6]->SetNode(
                gameContext, m_node[5], {m_node[5]->GetPosition() + SimpleMath::Vector2{-100, 100}},
                [this] { Magnet(0.2); }, 20, L"Resources/Textures/テストオーブ.png");

            //
            m_node[7]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{100, -100}},
                [this] { IntervalDecrease(0.5); }, 10, L"Resources/Textures/テストオーブ.png");

            m_node[8]->SetNode(
                gameContext, m_node[7], {m_node[7]->GetPosition() + SimpleMath::Vector2{100, -100}},
                [this] { IntervalDecrease(0.5); }, 50, L"Resources/Textures/テストオーブ.png");

            m_node[9]->SetNode(
                gameContext, m_node[8], {m_node[8]->GetPosition() + SimpleMath::Vector2{100, -100}},
                [this] { IntervalDecrease(0.5); }, 100, L"Resources/Textures/テストオーブ.png");

            //
            m_node[10]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-100, -100}},
                [this] { Time(5); }, 10, L"Resources/Textures/テストオーブ.png");
            m_node[11]->SetNode(
                gameContext, m_node[10], {m_node[10]->GetPosition() + SimpleMath::Vector2{-100, -100}},
                [this] { Time(10); }, 100, L"Resources/Textures/テストオーブ.png");
            m_node[12]->SetNode(
                gameContext, m_node[11], {m_node[11]->GetPosition() + SimpleMath::Vector2{-100, -100}},
                [this] { Time(15); }, 1500, L"Resources/Textures/テストオーブ.png");

            //
            m_node[13]->SetNode(
                gameContext, nullptr, {50, 50}, [this] { GetSkill(0); }, 100, L"Resources/Textures/テストオーブ.png");

        }

        //オーブの数の追加
        void AddOrb(int num)
        {
            //num分だけオーブを追加する
            for (size_t i = 0; i < num; i++)
            {
                ref_orbManager.AddOrb();
            }
        }

        //オーブの価値増加
        void AddOrbCost(int num)
        {

            ref_orbManager.SetOrbValue(num);
        }

        //オーブを取得できる範囲増加
        void Magnet(float num)
        {

        }

        //価値が2倍オーブの追加
        void LuckOrb(float num)
        {

        }

        //いいことが起きる確率を増加させる
        void Luck(float num)
        {

        }

        //脚の速さ増加
        void Speed(int num)
        {
            ref_player.AddSpeed(num);
        }

        //時間の延長
        void Time(int num)
        {
            //現在の制限時間に追加する
            ref_player.AddMaxTimer(num);
        }

        //スキルを追加
        void GetSkill(int num)
        {

        }

        //オーブのインターバル減少
        void IntervalDecrease(float time)
        {

        }


    private:

        static constexpr int NODE_COUNT = 14;

    private:

        //スキルノードを数分入れる
        SkillNode* m_node[NODE_COUNT];

        //プレイヤー
        Player& ref_player;

        //オーブマネージャー
        OrbManager& ref_orbManager;
    };

}