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

        //更新処理
        void Update(float elapsedtime,GameContext& gameContext)
        {
            for (int i = 0; i < NODE_COUNT; i++)
            {
                if (IsCanGet(m_node[i]))
                {
                    m_node[i]->Update(elapsedtime, gameContext,ref_player,m_pos);

                }

            }
            
            auto kb = Keyboard::Get().GetState();

            // 対応したキーごとに移動する方向を変える
            if (kb.Up && m_pos.y<=MAX_SKILLTREE_MOVE)
                m_pos.y += NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Down && m_pos.y >= -MAX_SKILLTREE_MOVE)
                m_pos.y -= NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Left && m_pos.x >= -MAX_SKILLTREE_MOVE)
                m_pos.x -= NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Right && m_pos.x <= MAX_SKILLTREE_MOVE)
                m_pos.x += NODE_MOVE_SPEED * elapsedtime * 60;

        }

        //描画処理
        void Render()
        {
            for (int i = 0; i < NODE_COUNT; i++)
            {
                if (IsCanGet(m_node[i]))
                {
                    m_node[i]->Render(m_pos);
                }

            }

            
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

            m_spriteBatch->End();

            for (size_t i = 0; i < NODE_COUNT; i++)
            {
                if (IsCanGet(m_node[i]))
                {
                    m_node[i]->TextRender(m_pos);
                }
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
                gameContext, nullptr, {width / 2, height / 2}, [this] { Time(5); }, 1, L"Resources/Textures/Star.png",
                TIMER_TEXT);

            //
            m_node[1]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{80, 80}},
                [this] { AddOrbCost(1); }, 10, L"Resources/Textures/Star.png", VALUE_TEXT);

            m_node[2]->SetNode(
                gameContext, m_node[1], {m_node[1]->GetPosition() + SimpleMath::Vector2{80, 80}},
                [this] { AddOrbCost(1); }, 20, L"Resources/Textures/Star.png", VALUE_TEXT);

            m_node[3]->SetNode(
                gameContext, m_node[2], {m_node[2]->GetPosition() + SimpleMath::Vector2{80, 80}},
                [this] { AddOrbCost(1); }, 50, L"Resources/Textures/Star.png", VALUE_TEXT);

            //
            m_node[4]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-80, 80}},
                [this] { Speed(0.25); }, 10, L"Resources/Textures/Star.png", SPEED_TEXT);

            m_node[5]->SetNode(
                gameContext, m_node[4], {m_node[4]->GetPosition() + SimpleMath::Vector2{-80, 80}},
                [this] { Speed(0.25); }, 15, L"Resources/Textures/Star.png", SPEED_TEXT);

            m_node[6]->SetNode(
                gameContext, m_node[5], {m_node[5]->GetPosition() + SimpleMath::Vector2{-80, 80}},
                [this] { Speed(0.5); }, 20, L"Resources/Textures/Star.png", SPEED_TEXT);

            //
            m_node[7]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{80, -80}},
                [this] { IntervalDecrease(0.5); }, 10, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            m_node[8]->SetNode(
                gameContext, m_node[7], {m_node[7]->GetPosition() + SimpleMath::Vector2{80, -80}},
                [this] { IntervalDecrease(0.5); }, 50, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            m_node[9]->SetNode(
                gameContext, m_node[8], {m_node[8]->GetPosition() + SimpleMath::Vector2{80, -80}},
                [this] { IntervalDecrease(0.5); }, 100, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            //
            m_node[10]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-80, -80}}, [this] { Time(5); },
                50, L"Resources/Textures/Star.png", TIMER_TEXT);
            m_node[11]->SetNode(
                gameContext, m_node[10], {m_node[10]->GetPosition() + SimpleMath::Vector2{-80, -80}},
                [this] { Time(5); }, 300, L"Resources/Textures/Star.png", TIMER_TEXT);
            m_node[12]->SetNode(
                gameContext, m_node[11], {m_node[11]->GetPosition() + SimpleMath::Vector2{-80, -80}},
                [this] { Time(15); }, 1500, L"Resources/Textures/Star.png", TIMER_TEXT);

            //
            m_node[13]->SetNode(
                gameContext, m_node[0], {50, 50}, [this] { GetSkill(0); }, 100, L"Resources/Textures/Star.png");

                            
            
            auto device = gameContext.deviceResources.GetD3DDevice();
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

            // 文字の背景の初期化
            CreateWhiteTexture(device);


        }

        //取得可能かどうか判定する関数
        bool IsCanGet(SkillNode* node)
        {
            bool IsGet;

            //親がいないなら取得してもいい
            if (node->GetPrevNode() == nullptr)
            {
                return true;
            }

            //親が解放されているなら取得してもいい
            else if (node->GetPrevNode()->GetIsGet())
            {
                return true;
            }


            return false;
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
        void Speed(float num)
        {
            ref_player.AddSpeed(num);
        }

        //時間の延長
        void Time(float num)
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

            ref_orbManager.Remomveinterval(time);

        }


    private:

        //ノードの数
        static constexpr int NODE_COUNT = 14;

        //スキルツリーを動かす速さ
        static constexpr float NODE_MOVE_SPEED = 6;

        //スキルツリーの最大可動範囲
        static constexpr int MAX_SKILLTREE_MOVE = 100;

        //テキスト群
        std::wstring ADD_TEXT = L"オーブを増やすものが追加されます";
        std::wstring TIMER_TEXT = L"オーブを収集できる時間が増えます。";
        std::wstring VALUE_TEXT = L"オーブの価値が増えます";
        std::wstring MAGNET_TEXT = L"オーブを取得できる範囲が増えます";
        std::wstring INTERVAL_TEXT = L"オーブが出てくる間隔が減ります";
        std::wstring SPEED_TEXT = L"あなたの足が速くなります";

    private:

        //
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


        //スキルノードを数分入れる
        SkillNode* m_node[NODE_COUNT];

        //プレイヤー
        Player& ref_player;

        //オーブマネージャー
        OrbManager& ref_orbManager;

        //スキルツリーの位置
        DirectX::SimpleMath::Vector2 m_pos;

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