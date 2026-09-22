#pragma once
#include <functional>
#include <string>
#include <memory>
#include <Windows.h>
#include <GameContext.h>
#include <WICTextureLoader.h>
#include "Button.h"
#include <Scene/GamePlayScene/Player.h>

namespace Itsuki
{
    class SkillNode
    {
    public:
        SkillNode()
            : m_parentNode{nullptr},
            m_skillUp{},
            m_steak{0},
            m_isGet{false}
        {
        }

        ~SkillNode()
        {
        }

        //更新関数
        void Update(float elapsedtime, GameContext& gameContext, Player& player)
        {

            auto mouse = Mouse::Get().GetState();
            m_imageButton.Update(mouse);

            if (m_imageButton.IsPushed(mouse))
            {
                if (player.GetHaveOrb() >= GetSteak())
                {
                    gameContext.audio.PlayOneShot("Buy");

                    GetSkillUp();
                    SetIsGet(true);
                }
            }
        }

    void Render()
        {
            m_imageButton.Render();
        }

        void SetNode(GameContext& gameContext, SkillNode* parent, DirectX::SimpleMath::Vector2 position,
                     std::function<void()> skillup, int steak = 10,
                     std::wstring skillimage = L"Resources/Textures/テスト六角型.png",
                     std::wstring text = L"データ無し")
        {
            m_parentNode = parent;
            m_skillUp = skillup;
            m_steak = steak;
            m_text = text;

            m_imageButton.Initialize(gameContext, position, skillimage, IMAGE_MAGNI, BUTTON_MAGNI);
        }


        // 取得済みかどうか設定する
        void SetIsGet(bool isget)
        {
            m_isGet = isget;
        }

        // 親のノードを返す
        SkillNode* GetPrevNode()
        {
            return m_parentNode;
        }

        // 実行する関数を返す
        void GetSkillUp()
        {
            m_skillUp;
        }

        // 必要なオーブ数を返す
        int GetSteak()
        {
            return m_steak;
        }

        // 取得済みか返す
        bool GetIsGet()
        {
            return m_isGet;
        }

        // 位置の取得
        DirectX::SimpleMath::Vector2 GetPosition() const
        {
            return m_imageButton.GetPosition();
        }

        private:
        //画像の大きさ
        static constexpr float IMAGE_MAGNI = 0.8f;
        //ボタンの大きさ
        static constexpr float BUTTON_MAGNI = 0.7f;
    private:

        //親のノード
        SkillNode* m_parentNode;
        //関数
        std::function<void()> m_skillUp;
        //必要なオーブ数
        int m_steak;
        //取得したか
        bool m_isGet = false;
        //
        std::wstring m_text;

        //イメージボタン
        ImageButton m_imageButton;
    };

}