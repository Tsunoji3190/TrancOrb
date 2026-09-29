#pragma once
#include "SkillNode.h"
#include"../Scene/GamePlayScene/Player.h"
#include"../Scene/GamePlayScene/Manager/OrbManager.h"
#include <cmath>
namespace Itsuki
{
	using namespace Itsuki;

	class SkillTree
    {
    public:

        SkillTree(Player& player, OrbManager& manager) 
            : ref_player{player}, ref_orbManager{manager}, m_mouse{}
        {
        }

        //更新処理
        void Update(float elapsedtime,GameContext& gameContext)
        {

                        
            MouseDragging();

            //押せるかどうかの距離の判断
            bool canClick = m_dragDistance < CLICK_DRAG_THRESHOLD;

            for (int i = 0; i < NODE_COUNT; i++)
            {
                if (IsCanGet(m_node[i]))
                {
                    m_node[i]->Update(elapsedtime, gameContext,ref_player,m_pos,canClick);

                }

            }
            
            auto kb = Keyboard::Get().GetState();


            // 対応したキーごとに移動する方向を変える
            if (kb.Up)
                m_pos.y -= NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Down)
                m_pos.y += NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Left)
                m_pos.x -= NODE_MOVE_SPEED * elapsedtime * 60;

            if (kb.Right)
                m_pos.x += NODE_MOVE_SPEED * elapsedtime * 60;


            if (m_pos.y > MAX_SKILLTREE_MOVE)
            {
                m_pos.y = MAX_SKILLTREE_MOVE;
            }
            if (m_pos.y < -MAX_SKILLTREE_MOVE)
            {
                m_pos.y = -MAX_SKILLTREE_MOVE;
            }
            if (m_pos.x < -MAX_SKILLTREE_MOVE)
            {
                m_pos.x = -MAX_SKILLTREE_MOVE;
            }
            if (m_pos.x > MAX_SKILLTREE_MOVE)
            {
                m_pos.x = MAX_SKILLTREE_MOVE;
            }
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

            // 描画したい四角形の範囲
            RECT rect = {0, 575, 1280, 720};

            // 塗りつぶしたい色
            XMVECTOR color = Colors::Black;

            // 1x1の白色テクスチャをRECTのサイズに拡大して描画
            m_spriteBatch->Draw(m_whiteTexture.Get(), rect, color * 0.8);

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
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{65, 65}},
                [this] { AddOrbCost(1); }, 10, L"Resources/Textures/Star.png", VALUE_TEXT);

            m_node[2]->SetNode(
                gameContext, m_node[1], {m_node[1]->GetPosition() + SimpleMath::Vector2{65, 65}},
                [this] { AddOrbCost(1); }, 20, L"Resources/Textures/Star.png", VALUE_TEXT);

            m_node[3]->SetNode(
                gameContext, m_node[2], {m_node[2]->GetPosition() + SimpleMath::Vector2{65, 65}},
                [this] { AddOrbCost(1); }, 50, L"Resources/Textures/Star.png", VALUE_TEXT);

            //
            m_node[4]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-65, 65}},
                [this] { Speed(0.25); }, 10, L"Resources/Textures/Star.png", SPEED_TEXT);

            m_node[5]->SetNode(
                gameContext, m_node[4], {m_node[4]->GetPosition() + SimpleMath::Vector2{-65, 65}},
                [this] { Speed(0.25); }, 15, L"Resources/Textures/Star.png", SPEED_TEXT);

            m_node[6]->SetNode(
                gameContext, m_node[5], {m_node[5]->GetPosition() + SimpleMath::Vector2{-65, 65}},
                [this] { Speed(0.5); }, 20, L"Resources/Textures/Star.png", SPEED_TEXT);

            //
            m_node[7]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{65, -65}},
                [this] { IntervalDecrease(0.5); }, 10, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            m_node[8]->SetNode(
                gameContext, m_node[7], {m_node[7]->GetPosition() + SimpleMath::Vector2{65, -65}},
                [this] { IntervalDecrease(0.5); }, 50, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            m_node[9]->SetNode(
                gameContext, m_node[8], {m_node[8]->GetPosition() + SimpleMath::Vector2{65, -65}},
                [this] { IntervalDecrease(0.5); }, 100, L"Resources/Textures/Star.png", INTERVAL_TEXT);

            //
            m_node[10]->SetNode(
                gameContext, m_node[0], {m_node[0]->GetPosition() + SimpleMath::Vector2{-65, -65}}, [this] { Time(5); },
                50, L"Resources/Textures/Star.png", TIMER_TEXT);
            m_node[11]->SetNode(
                gameContext, m_node[10], {m_node[10]->GetPosition() + SimpleMath::Vector2{-65, -65}},
                [this] { Time(5); }, 300, L"Resources/Textures/Star.png", TIMER_TEXT);
            m_node[12]->SetNode(
                gameContext, m_node[11], {m_node[11]->GetPosition() + SimpleMath::Vector2{-65, -65}},
                [this] { Time(15); }, 1500, L"Resources/Textures/Star.png", TIMER_TEXT);

            //
            m_node[13]->SetNode(
                gameContext, m_node[0], {50, 50}, [this] { GetSkill(0); }, 100, L"Resources/Textures/Star.png");

            //運
            for (int i = 0; i < 8; i++)
            {
                auto rad = DirectX::XMConvertToRadians(45 * i);

                auto sinx = (90 * std::cosf(rad));
                auto cosy = (90 * std::sinf(rad));

                m_node[14 + i]->SetNode(
                    gameContext, m_node[0], {200 + sinx, 500 + cosy}, [this] { Luck(10); }, 150,
                    L"Resources/Textures/Star.png", LUCK_TEXT);
            

            }



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

        //いいことが起きる確率を増加させる
        void Luck(float num)
        {
            ref_player.AddLuck(num);
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

        //マウスを掴んだ際の反応
        void MouseDragging()
        {
            m_mouse = Mouse::Get().GetState();
            m_stateTrack.Update(m_mouse);

            DirectX::SimpleMath::Vector2 cur{static_cast<float>(m_mouse.x), static_cast<float>(m_mouse.y)};

            //もしマウスが押されたら
            if (m_stateTrack.leftButton == Mouse::ButtonStateTracker::PRESSED)
            {
                m_isDragging = true;
                m_lastMousePos = cur;
                m_dragDistance = 0.0f;
            }
            else if (m_stateTrack.leftButton == Mouse::ButtonStateTracker::HELD && m_isDragging)    //ホールドされていたら
            {
                DirectX::SimpleMath::Vector2 delta = cur - m_lastMousePos;

                // ノードは (m_pos.x, -m_pos.y) で描画しているので、yだけ符号を反転
                m_pos.x += delta.x;
                m_pos.y -= delta.y;

                //長さを取得
                m_dragDistance += delta.Length();
                m_lastMousePos = cur;
            }
            else if (m_stateTrack.leftButton == Mouse::ButtonStateTracker::RELEASED)    //離されたら
            {
                m_isDragging = false;
            }
        }
    private:

        //ノードの数
        static constexpr int NODE_COUNT = 22;

        //スキルツリーを動かす速さ
        static constexpr float NODE_MOVE_SPEED = 6;

        //スキルツリーの最大可動範囲
        static constexpr int MAX_SKILLTREE_MOVE = 100;

        //ノードをクリックできる移動量
        static constexpr float CLICK_DRAG_THRESHOLD = 5.0f; 

        //テキスト群
        std::wstring ADD_TEXT = L"オーブを増やすものが追加されます";
        std::wstring TIMER_TEXT = L"オーブを収集できる時間が増えます。";
        std::wstring VALUE_TEXT = L"オーブの価値が増えます";
        std::wstring MAGNET_TEXT = L"オーブを取得できる範囲が増えます";
        std::wstring INTERVAL_TEXT = L"オーブが出てくる間隔が減ります";
        std::wstring SPEED_TEXT = L"あなたの足が速くなります";
        std::wstring LUCK_TEXT = L"オーブを手に入れた際、そのオーブの価値が倍になる確率が増えます";

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

        Mouse::State m_mouse;
        //
        Mouse::ButtonStateTracker m_stateTrack;

        //クリックの状態か
        bool m_isDragging = false;
        //マウスの位置（ドラッグを離した瞬間）
        DirectX::SimpleMath::Vector2 m_lastMousePos{};
        float m_dragDistance = 0.0f; 
    };

}