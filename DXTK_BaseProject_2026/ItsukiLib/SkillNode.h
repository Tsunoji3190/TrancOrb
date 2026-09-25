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
        void Update(float elapsedtime, GameContext& gameContext, Player& player,DirectX::SimpleMath::Vector2 pos)
        {

            auto mouse = Mouse::Get().GetState();

            auto kb = Keyboard::Get().GetState();




            if (GetIsGet())
                return;

            if (m_imageButton.IsCursored(mouse, {pos.x, -pos.y}))
            {
                m_imageButton.SetColor(Colors::White);
            }
            else
            {
                m_imageButton.SetColor(Colors::Gray);
            }

            if (m_imageButton.IsPushed(mouse, {pos.x, -pos.y}))
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

        void Render(DirectX::SimpleMath::Vector2 pos)
        {
            auto mouse = Mouse::Get().GetState();


            SimpleMath::Matrix trans =
                SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(pos.x, -pos.y, 0.0f));

            m_imageButton.SpriteBegin(SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, trans);
            m_imageButton.Render();
            m_imageButton.SpriteEnd();

            m_spriteBatch->Begin();
            // 描画したい四角形の範囲（左, 上, 右, 下）
            RECT rect;
            rect.left = 0;
            rect.top = 575;
            rect.right = 1280; // 幅 300 ピクセル
            rect.bottom = 720; // 高さ 200 ピクセル

            // 塗りつぶしたい色（例：不透明な赤）
            XMVECTOR color = Colors::Black;

            // 1x1の白色テクスチャをRECTのサイズに拡大して描画
            m_spriteBatch->Draw(m_whiteTexture.Get(), rect, color * 0.5);

            if (m_imageButton.IsCursored(mouse, {pos.x, -pos.y}))
            {
                m_spriteFont->DrawString(
                    m_spriteBatch.get(),
                    m_text.c_str(),
                    DirectX::SimpleMath::Vector2{20.0f, 600.0f},
                    DirectX::Colors::White,
                    0.0,
                    {0,0},
                    {1.2,1.2});

            }
                            
            m_spriteBatch->End();

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
                
            // 文字の背景の初期化
            CreateWhiteTexture(device);

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

        // 説明時の文字の後ろの画像
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_whiteTexture;

        // ↑の初期化処理
        void CreateWhiteTexture(ID3D11Device* device)
        {
            UINT whitePixel = 0xFFFFFFFF; // RGBA(255, 255, 255, 255)

            D3D11_SUBRESOURCE_DATA initData = {};
            initData.pSysMem = &whitePixel;
            initData.SysMemPitch = sizeof(UINT);

            CD3D11_TEXTURE2D_DESC desc(DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, 1, 1, // 幅1, 高さ1, ミップレベル1, 配列サイズ1
                                       D3D11_BIND_SHADER_RESOURCE);

            Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
            device->CreateTexture2D(&desc, &initData, texture.GetAddressOf());
            device->CreateShaderResourceView(texture.Get(), nullptr, m_whiteTexture.GetAddressOf());
        }

    };

}