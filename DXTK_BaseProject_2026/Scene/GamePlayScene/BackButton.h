#pragma once
#include"pch.h"

#include <string>
#include <memory>
#include <Windows.h>
#include <GameContext.h>
#include <WICTextureLoader.h>

#include"../../ItsukiLib/Button.h"

namespace Itsuki
{
    class BackButton
    {
    public:

        void Initialize(GameContext& gameContext, DirectX::SimpleMath::Vector2 position,
                        std::wstring buttonimage = L"Resources/Textures/テスト六角型.png")
        {

            m_position = position;
            m_image = buttonimage;

            // DirectX3Dのデバイスを取得する
            auto device = gameContext.deviceResources.GetD3DDevice();

            // DirectX3Dのデバイスコンテキストを取得する
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            m_states = std::make_unique<DirectX::CommonStates>(device);

            DirectX::CreateWICTextureFromFile(device, m_image.c_str(), nullptr,
                                              m_texture.ReleaseAndGetAddressOf());

            // 画像のサイズ取得
            Microsoft::WRL::ComPtr<ID3D11Resource> resource;
            m_texture->GetResource(resource.GetAddressOf());

            // 画像の大きさを入れておく
            Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
            if (SUCCEEDED(resource.As(&tex2D)))
            {
                D3D11_TEXTURE2D_DESC desc = {};
                // tex2Dの大きさをdescに入れる
                tex2D->GetDesc(&desc);
                m_textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height) * IMAGE_MAGNI;
            }

            // ボタンの当たり判定を画像より少し小さくしておく
            m_button.SetRect(m_textureSize * BUTTON_MAGNI);

            // ボタンの位置を画像と同じにする
            m_button.Setpositon(m_position);

        }

        void Update(float elapsedtime, Player* player)
        {
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
                player->SetTimer();
            }

        }

        void Render()
        {
            m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());

            m_spriteBatch->Draw(m_texture.Get(), m_position, 0, m_color, 0, {0, 0}, IMAGE_MAGNI);

            m_spriteBatch->End();

        }

        // 色の設定
        void SetColor(DirectX::XMVECTORF32 color)
        {
            m_color = color;
        }


        
    private:
        // 画像の大きさの倍率
        static constexpr float IMAGE_MAGNI = 0.8;

        // ボタンの大きさの倍率
        static constexpr float BUTTON_MAGNI = 0.8;

    private:    
        // 画像名
        std::wstring m_image;

        // スプライトバッヂ
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        // コモンステート
        std::unique_ptr<DirectX::CommonStates> m_states;

        // 色
        DirectX::SimpleMath::Color m_color;

        // 画像
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

        // 画像サイズ
        DirectX::SimpleMath::Vector2 m_textureSize;

        // 位置
        DirectX::SimpleMath::Vector2 m_position;

        //ボタン
        Itsuki::Button m_button;
    };

}