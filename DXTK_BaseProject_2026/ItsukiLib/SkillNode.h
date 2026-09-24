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
        : m_matrix{},
        m_parentNode{nullptr},
        m_skillUp{},
        m_steak{0},
        m_isGet{false}
        {
        }

        ~SkillNode()
        {
        }

        // 更新関数
        void Update(float elapsedtime, GameContext& gameContext, Player& player)
        {

            auto mouse = Mouse::Get().GetState();

            auto kb = Keyboard::Get().GetState();

            // 対応したキーごとに移動する方向を変える
            if (kb.W)
            if (kb.S)
            if (kb.A)
            if (kb.D)



            if (GetIsGet())
                return;

            if (m_imageButton.IsCursored(mouse))
            {
                m_imageButton.SetColor(Colors::White);
            }
            else
            {
                m_imageButton.SetColor(Colors::Gray);
            }

            if (m_imageButton.IsPushed(mouse))
            {
                if (player.GetHaveOrb() >= GetSteak())
                {
                    gameContext.audio.PlayOneShot("Buy");

                    GetSkillUp();
                    SetIsGet(true);
                    m_imageButton.SetColor(Colors::White);

                }
            }
        }

        void Render()
        {
            auto mouse = Mouse::Get().GetState();

            m_imageButton.SpriteBegin();
            m_imageButton.Render();
            m_imageButton.SpriteEnd();

            if (m_imageButton.IsCursored(mouse))
            {
                m_spriteBatch->Begin();
                m_spriteFont->DrawString(
                    m_spriteBatch.get(),
                    m_text.c_str(),
                    DirectX::SimpleMath::Vector2{20.0f, 600.0f},
                    DirectX::Colors::White,
                    0.0,
                    {0,0},
                    {1.2,1.2});

                m_spriteBatch->End();

            }
        }

        void SetNode(GameContext& gameContext, SkillNode* parent, DirectX::SimpleMath::Vector2 position,
                     std::function<void()> skillup, int steak = 10,
                     std::wstring skillimage = L"Resources/Textures/テスト六角型.png", std::wstring text = L"データ無し")
        {
            m_parentNode = parent;
            m_skillUp = skillup;
            m_steak = steak;

                // テキストの設定
            std::wstring alltext = text + L" コスト:" + std::to_wstring(m_steak);


            m_text = alltext;

            m_imageButton.Initialize(gameContext, position, skillimage, IMAGE_MAGNI, BUTTON_MAGNI);

            auto device = gameContext.deviceResources.GetD3DDevice();
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            // テキストの初期化
            m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Font/Meiryo.spritefont");
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
            if (m_skillUp)
            {
                m_skillUp();
            }
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
        // 画像の大きさ
        static constexpr float IMAGE_MAGNI = 0.8f;
        // ボタンの大きさ
        static constexpr float BUTTON_MAGNI = 0.7f;

    private:

        //カメラ移動のマトリクス
        DirectX::SimpleMath::Matrix m_matrix;

        //
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        // スプライトフォント
        std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

        // 親のノード
        SkillNode* m_parentNode;
        // 関数
        std::function<void()> m_skillUp;
        // 必要なオーブ数
        int m_steak;
        // 取得したか
        bool m_isGet = false;

        // 説明文
        std::wstring m_text;

        // イメージボタン
        ImageButton m_imageButton;
    };

}