#pragma once

namespace Itsuki
{
    class Button
    {
    public:
        // 当たり判定の大きさを設定する
        void SetRect(DirectX::SimpleMath::Vector2 size)
        {
            m_size = size;
        }

        //大きさを返す関数
        DirectX::SimpleMath::Vector2 GetRect()
        {
            return m_size;
        }

        //位置を設定する
        void Setpositon(DirectX::SimpleMath::Vector2 position)
        {
            m_position = position;
        }

        //位置を返す関数
        DirectX::SimpleMath::Vector2 GetPosition()
        {
            return m_position;
        }

        // クリックされた瞬間
        bool IsPushed(DirectX::Mouse::State mouse)
        {
            //左クリックした際にマウスカーソルが重なっていたら
            if (mouse.leftButton&& IsCursored(mouse))
            {
                return true;
            }

            return false;
        }

        //カーソルが重なったか返す関数
        bool IsCursored(DirectX::Mouse::State mouse)
        {
            auto mx = mouse.x;
            auto my = mouse.y;

            // 中心から左上・右下を逆算する
            auto left = m_position.x - m_size.x * 0.5;
            auto top = m_position.y - m_size.y * 0.5;
            auto right = m_position.x + m_size.x * 0.5;
            auto bottom = m_position.y + m_size.y * 0.5;

            // ボタンの範囲内にカーソルがあったらtrueを返す
            return (mx > left && mx < right && my > top && my < bottom);
        }



    private:

        //大きさ
        DirectX::SimpleMath::Vector2 m_size;

        //位置
        DirectX::SimpleMath::Vector2 m_position;
    };

        class ImageButton
    {
    public:
        // 初期化関数
        void Initialize(GameContext& gameContext, DirectX::SimpleMath::Vector2 position,
                        std::wstring image = L"Resources/Textures/テスト六角型.png", float imageMagni = 0.8f,
                        float buttonMagni = 0.8f)
        {
            //パラメータを設定する
            m_position = position;
            m_image = image;
            m_imageMagni = imageMagni;

            auto device = gameContext.deviceResources.GetD3DDevice();
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);
            m_states = std::make_unique<DirectX::CommonStates>(device);

            DirectX::CreateWICTextureFromFile(device, m_image.c_str(), nullptr, m_texture.ReleaseAndGetAddressOf());

            // 画像のサイズ取得
            Microsoft::WRL::ComPtr<ID3D11Resource> resource;
            m_texture->GetResource(resource.GetAddressOf());

            Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
            if (SUCCEEDED(resource.As(&tex2D)))
            {
                D3D11_TEXTURE2D_DESC desc = {};
                tex2D->GetDesc(&desc);
                m_textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height);
                m_origin = {m_textureSize.x / 2, m_textureSize.y / 2};
            }

            // ボタンの当たり判定を表示サイズに合わせて設定
            m_button.SetRect(m_textureSize * buttonMagni);
            m_button.Setpositon(m_position);
        }

        // 更新(カーソル/クリック判定と色変更のみ担当)
        void Update(DirectX::Mouse::State mouse)
        {
            if (m_button.IsCursored(mouse))
            {
                SetColor(Colors::White);
            }
            else
            {
                SetColor(Colors::Gray);
            }
        }

        // 押されたかどうかを返す
        bool IsPushed(DirectX::Mouse::State mouse)
        {
            return m_button.IsPushed(mouse);
        }

        // カーソルが
        bool IsCursored(DirectX::Mouse::State mouse)
        {
            return m_button.IsCursored(mouse);
        }

        void Render()
        {
            m_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());
            m_spriteBatch->Draw(m_texture.Get(), m_position, nullptr, m_color, 0.0f, m_origin, m_imageMagni);
            m_spriteBatch->End();
        }

        void SetColor(DirectX::XMVECTORF32 color)
        {
            m_color = color;
        }

        DirectX::SimpleMath::Vector2 GetPosition() const
        {
            return m_position;
        }

        DirectX::SpriteBatch* GetSpriteBatch()
        {
            return m_spriteBatch.get();
        }

    private:
        XMFLOAT2 m_origin;
        std::wstring m_image;
        float m_imageMagni = 0.8f;

        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
        std::unique_ptr<DirectX::CommonStates> m_states;

        DirectX::SimpleMath::Color m_color = Colors::Gray;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

        // 画像の大きさ
        DirectX::SimpleMath::Vector2 m_textureSize;
        // 位置
        DirectX::SimpleMath::Vector2 m_position;
        //ボタン
        Itsuki::Button m_button;
    };

}
