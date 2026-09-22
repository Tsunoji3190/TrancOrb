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
            m_isGet{false},
            m_color{Colors::Gray}
        {
        }

        ~SkillNode()
        {
        }

        //更新関数
        void Update(float elapsedtime, GameContext& gameContext, Player& player)
        {

            //取得してるならもう更新しない
            if (GetIsGet()) return;

            // マウスの取得
            auto mouse = Mouse::Get().GetState();

            // カーソルがボタンの上に乗ったら
            if (m_button.IsCursored(mouse))
            {
                SetColor(Colors::White);
            }
            else
            {
                SetColor(Colors::Gray);
            }

            // ボタンが押されたら
            if (m_button.IsPushed(mouse))
            {
                //プレイヤーのオーブが必要オーブ数より多かったら
                if (player.GetHaveOrb() >= GetSteak())
                {
                    gameContext.audio.PlayOneShot("Buy");

                    // ノードに応じたスキルを強化
                    GetSkillUp();
                    // 解放する
                    SetIsGet(true);

                }
                else
                {
                
                }
            }

        }

        void Render()
        {

                m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());

                m_spriteBatch->Draw(m_texture.Get(), m_position, 0, m_color, 0, {0, 0}, IMAGE_MAGNI);

                m_spriteBatch->End();


        }


        void SetNode(GameContext& gameContext, SkillNode* parent,
                     DirectX::SimpleMath::Vector2 position, std::function<void()> skillup, int steak=10,
                     std::wstring skillimage = L"Resources/Textures/テスト六角型.png")
        {

            //ノードの変数を設定する
            m_parentNode = parent;
            m_position = position;
            m_skillUp = skillup;
            m_steak = steak;
            m_skillImage = skillimage;


            // DirectX3Dのデバイスを取得する
            auto device = gameContext.deviceResources.GetD3DDevice();

            // DirectX3Dのデバイスコンテキストを取得する
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            m_states = std::make_unique<DirectX::CommonStates>(device);

            DirectX::CreateWICTextureFromFile(device, m_skillImage.c_str(), nullptr,
                                              m_texture.ReleaseAndGetAddressOf());


            // 画像のサイズ取得
            Microsoft::WRL::ComPtr<ID3D11Resource> resource;
            m_texture->GetResource(resource.GetAddressOf());

            //画像の大きさを入れておく
            Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
            if (SUCCEEDED(resource.As(&tex2D)))
            {
                D3D11_TEXTURE2D_DESC desc = {};
                //tex2Dの大きさをdescに入れる
                tex2D->GetDesc(&desc);
                m_textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height) * IMAGE_MAGNI;
            }


            //ボタンの当たり判定を画像より少し小さくしておく
            m_button.SetRect(m_textureSize * BUTTON_MAGNI);

            //ボタンの位置を画像と同じにする
            m_button.Setpositon(m_position);

        }

        //取得済みかどうか設定する
        void SetIsGet(bool isget)
        {
            m_isGet = isget;
        }

        //親のノードを返す
        SkillNode* GetPrevNode()
        {
            return m_parentNode;
        }

        //実行する関数を返す
        void GetSkillUp()
        {
            m_skillUp;
        }

        //必要なオーブ数を返す
        int GetSteak()
        {
            return m_steak;
        }

        //画像名を返す
        std::wstring GetImage()
        {
            return m_skillImage;
        }

        //取得済みか返す
        bool GetIsGet()
        {
            return m_isGet;
        }

        //位置の取得
        SimpleMath::Vector2 GetPosition()
        {
            return m_position;
        }

        //色の設定
        void SetColor(DirectX::XMVECTORF32 color)
        {
            m_color = color;
        }

    private:

        //画像の大きさの倍率
        static constexpr float IMAGE_MAGNI = 0.8;

        //ボタンの大きさの倍率
        static constexpr float BUTTON_MAGNI = 0.8;


    private:

        //自分の前となるノード
        SkillNode* m_parentNode;

        //実行する関数
        std::function<void()> m_skillUp;

        //獲得するのに必要なオーブ数
        int m_steak;

        //スキルの画像名
        std::wstring m_skillImage;

        //このスキルが既に取得済みか
        bool m_isGet;

        //スプライトバッヂ
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        //コモンステート
        std::unique_ptr<DirectX::CommonStates> m_states;

        //色
        DirectX::SimpleMath::Color m_color;

        //画像
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

        // 画像サイズ
        DirectX::SimpleMath::Vector2 m_textureSize;

        //位置
        DirectX::SimpleMath::Vector2 m_position;

        //ボタンの判定
        Itsuki::Button m_button;
    };

}