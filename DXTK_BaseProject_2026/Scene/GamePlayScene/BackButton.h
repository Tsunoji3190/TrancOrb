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
            m_imageButton.Initialize(gameContext, position, buttonimage, IMAGE_MAGNI, IMAGE_MAGNI);
        }

        void Update(float elapsedtime, Player* player)
        {

            auto mouse = Mouse::Get().GetState();
            m_imageButton.Update(mouse);

            if (m_imageButton.IsPushed(mouse))
            {
                player->SetTimer();
            }
        }

        void Render()
        {
            m_imageButton.Render();
        }

        // 色の設定
        void SetColor(DirectX::XMVECTORF32 color)
        {
            m_color = color;
        }


        
    private:
        // 画像の大きさの倍率
        static constexpr float IMAGE_MAGNI = 0.8;

    private:    

        // 中心点
        XMFLOAT2 m_origin;

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
        Itsuki::ImageButton m_imageButton;
    };

}