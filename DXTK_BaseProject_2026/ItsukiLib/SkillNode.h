///////////////////////////////
//
// スキルツリーに使われる押すことで各々の効果を発揮するクラス
// 2026/10/21更新 宮本樹
///////////////////////////////

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

        enum class SkillEffect
    {
        Time,
        Speed,
        OrbValue,
        Interval,
        Luck,
        FirstOrb,
        Skill,
        Clear
    };

    class SkillNode
    {
    public:
        SkillNode() 
        :m_parent{nullptr},
        m_effect{}, 
        m_value{},
        m_cost{0},
        m_isGet{false}
        {
        }

        ~SkillNode()
        {
        }

        // 更新関数
        bool Update(GameContext& gameContext, Player& player, const DirectX::SimpleMath::Vector2& pos, bool canClick)
        {
            //取得済みなら何もしない
            if (IsGet())
                return false;

            auto mouse = Mouse::Get().GetState();

            auto kb = Keyboard::Get().GetState();

            //カーソルが触れたら
            if (m_imageButton.IsCursored(mouse, {pos.x, -pos.y}))
            {
                //色を明るくする
                m_imageButton.SetColor(Colors::White);
            }
            else
            {
                //触れてないなら色を暗くする
                m_imageButton.SetColor(Colors::Gray);
            }

            //
            if (!m_imageButton.IsPushed(mouse, {pos.x, -pos.y}) || !canClick)
                return false;


            // オーブが足りているのなら
            if (player.GetStatus().haveOrb >= GetSteak())
            {
                // Steak分持ってるオーブを減らす
                player.GetStatus().haveOrb -= m_cost;

                gameContext.audio.PlayOneShot("Buy",        
                    SuzukiLib::Audio::AudioPlayDesc{SuzukiLib::Audio::AudioChannel::Se, 0.3});

                m_isGet = true;
                m_imageButton.SetColor(Colors::White);

                return true;
            }

            return false;
        }

        void Render(DirectX::SimpleMath::Vector2 pos)
        {


            SimpleMath::Matrix trans =
                SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(pos.x, -pos.y, 0.0f));

            m_imageButton.SpriteBegin(SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, trans);
            m_imageButton.Render();
            m_imageButton.SpriteEnd();

        }

        void TextRender(DirectX::SimpleMath::Vector2 pos)
        {
            auto mouse = Mouse::Get().GetState();

            if (m_imageButton.IsCursored(mouse, {pos.x, -pos.y}))
            {
                m_spriteBatch->Begin();
                m_spriteFont->DrawString(m_spriteBatch.get(), m_text.c_str(),
                                         DirectX::SimpleMath::Vector2{20.0f, 600.0f}, DirectX::Colors::White, 0.0,
                                         {0, 0}, {1.2, 1.2});
                m_spriteBatch->End();
            }

        }

        void SetNode(GameContext& gameContext, const SkillNode* parent, const DirectX::SimpleMath::Vector2& position,
                     SkillEffect effect, float value, int cost, const std::wstring& image, const std::wstring& text)
        {
            //初期化設定
            m_parent = parent;
            m_effect = effect;
            m_value = value;
            m_cost = cost;
            m_text = text + L"\nコスト : " + std::to_wstring(m_cost);

            m_imageButton.Initialize(gameContext, position, image, IMAGE_MAGNI, BUTTON_MAGNI);

            auto device = gameContext.deviceResources.GetD3DDevice();
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            // テキストの初期化
            m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Font/Meiryo.spritefont");
        }

        // カーソルが乗っているか(説明文の表示用)
        bool IsCursored(const DirectX::SimpleMath::Vector2& pos)
        {
            auto mouse = DirectX::Mouse::Get().GetState();
            return m_imageButton.IsCursored(mouse, {pos.x, -pos.y});
        }

        // 取得できるか
        bool IsUnlockable() const
        {
            //親がいないか、親が入手されていたらtrue
            return m_parent == nullptr || m_parent->IsGet();
        }

        //スキルの種類を返す関数
        SkillEffect GetEffect()
        {
            return m_effect;
        }

        //どれだけの効果か返す
        float GetValue()
        {
            return m_value;
        }

        // 必要なオーブ数を返す
        int GetSteak()
        {
            return m_cost;
        }

        // 取得済みか返す
        const bool IsGet() const
        {
            return m_isGet;
        }

        //説明文の取得
        const std::wstring& GetText() const
        {
            return m_text;
        }

        // 位置の取得
        DirectX::SimpleMath::Vector2 GetPosition() const
        {
            return m_imageButton.GetPosition();
        }

    private:
        // 画像の大きさ
        static constexpr float IMAGE_MAGNI = 0.7f;
        // ボタンの大きさ
        static constexpr float BUTTON_MAGNI = 0.6f;

    private:


        //
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        // スプライトフォント
        std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

        // 親のノード
        const SkillNode* m_parent;

        // 効果と効果量
        SkillEffect m_effect;
        float m_value = 0.0f;

        // 必要なオーブ数
        int m_cost;

        // 取得したか
        bool m_isGet = false;

        // 説明文
        std::wstring m_text;

        // イメージボタン
        ImageButton m_imageButton;


    };

}