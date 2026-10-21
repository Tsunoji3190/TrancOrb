///////////////////////////////
//
// スキルツリーを作成するクラス
// 2026/10/21更新 宮本樹
///////////////////////////////

#pragma once
#include "SkillNode.h"

#include"../Scene/GamePlayScene/Player.h"
#include"../Scene/GamePlayScene/Manager/OrbManager.h"
#include "GameTimer.h"
#include <cmath>

namespace Itsuki
{
	using namespace Itsuki;

    //------------スキルツリー設計図クラス------------//
    class SkillTreeDefinition
    {

        // ノードのデータ（ノードそのものじゃない）
        struct NodeData
        {
            // 自分のid
            std::string id;

            // 親のid
            std::string parentId;

            // 位置
            DirectX::SimpleMath::Vector2 pos;

            // スキルの種類
            SkillEffect type;

            // スキルの効果をどれだけ適応するか
            float num;

            // 必要オーブ数
            int cost = 0;       

            // 画像
            std::wstring image;

            // 画面の相対位置か絶対位置か決める　true=親からの相対位置 false=絶対位置
            bool relative;
        };

        // 直線状にノードを追加する際の一時的にまとめるデータ
        struct Step
        {
            float num;
            int cost;
        };

    public:
        // ノードを追加する
        void AddNode(const std::string& cid, const std::string& parentId, const DirectX::SimpleMath::Vector2& pos,
                     SkillEffect effect, float num, int cost, const std::wstring& image = STAR_IMAGE,
                     bool relative = true)
        {
            m_data.push_back({cid, parentId, pos, effect, num, cost, image, relative});
        }

        // 直線状にノードを追加する
        void AddChain(const std::string& cid, const std::string& parentId, const DirectX::SimpleMath::Vector2& step,
                      SkillEffect effect, std::initializer_list<Step> steps)
        {
            // 親を格納しておく
            std::string prev = parentId;

            //
            int n = 1;
            for (const Step& s : steps)
            {
                // ○○_1,○○_2となるように名前を変える
                std::string id = cid + "_" + std::to_string(n++);

                // ノードを追加する
                AddNode(id, prev, step, effect, s.num, s.cost);

                // 次の親になる
                prev = id;
            }
        }

        // 円状にノードを追加する
        void AddRing(const std::string& cid, const std::string& parentId, const DirectX::SimpleMath::Vector2& center,
                     float radius, int count, SkillEffect effect, std::initializer_list<Step> steps)
        {
            for (size_t i = 0; i < count; i++)
            {
                // 中心角の大きさ
                auto rad = DirectX::XMConvertToRadians(360 / count * i);

                // radiusを用意することでノード同士をどの程度離すか決められる
                const DirectX::SimpleMath::Vector2 p{center.x + radius * std::cosf(rad),
                                                     center.y + radius * std::sinf(rad)};

                // リストの始まりからi番目のデータを取る
                float num = (steps.begin() + i)->num;
                int cost = (steps.begin() + i)->cost;

                // ○○_1,○○_2となるようにノードを追加していく
                AddNode(cid + "_" + std::to_string(i + 1), parentId, p, effect, num, cost);
            }
        }

        // TODO:jsonでの読み込みに対応させる
        // ノードの作成(オブジェクトが無くても呼びたいためstatic)
        static SkillTreeDefinition Create(float width, float height)
        {
            // 中心部分を作っておく
            DirectX::SimpleMath::Vector2 center = {width / 2, height / 2};

            SkillTreeDefinition d;

            // ノードの作成//

            // 一番最初のノード
            d.AddNode("First", "", center, SkillEffect::Time, 5, 1, STAR_IMAGE, false);

            // 価値
            d.AddChain("Value", "First", {65, 65}, SkillEffect::OrbValue, {{1, 3}, {1, 20}, {1, 50}});

            // プレイヤーの速度
            d.AddChain("Speed", "First", {-65, 65}, SkillEffect::Speed, {{1, 3}, {1, 6}, {1, 9}});

            // オーブのインターバル
            d.AddChain("Interval", "First", {65, -65}, SkillEffect::Interval, {{1, 3}, {1, 20}, {1, 100}});

            // 制限時間の延長
            d.AddChain("Time", "First", {-65, -65}, SkillEffect::Interval, {{1, 3}, {1, 50}, {1, 200}});

            // 運
            std::initializer_list<Step> luck = {
                {5, 10}, {5, 10}, {5, 10}, {5, 10}, {10, 20}, {10, 20}, {10, 20}, {10, 20},
            };
            d.AddRing("Luck", "First", {-500,0}, 90, 8, SkillEffect::Luck, luck);

            // クリア
            d.AddNode("Clear", "", {center.x, 80}, SkillEffect::Clear, 5, 1, CLEAR_IMAGE, false);

            // 最初のオーブの数
            d.AddNode("FirstOrb", "First", {1000, 90}, SkillEffect::FirstOrb, 3, 20, STAR_IMAGE, false);
            d.AddChain("ChainFirstOrb", "FirstOrb", {0, 65}, SkillEffect::FirstOrb, {{3, 60}, {3, 100}, {5, 300}});

            return d;
        }

        // ノードデータを参照する関数
        const std::vector<NodeData>& RefData() const
        {
            return m_data;
        }

    private:
        // テクスチャ
        static inline const std::wstring STAR_IMAGE = L"Resources/Textures/Star.png";
        static inline const std::wstring CLEAR_IMAGE = L"Resources/Textures/ClearStar.png";

        std::vector<NodeData> m_data;
    };



    //------------スキルツリークラス------------//
	class SkillTree
    {

    public:

        SkillTree(Player& player, OrbManager& manager,GameTimer& timer) 
            : ref_player{player}, ref_orbManager{manager}, ref_gameTimer{timer},m_mouse{}
        {
        }

        //更新処理
        void Update(float elapsedtime,GameContext& gameContext)
        {

            MouseDragging();

            // ドラッグ量が小さいときだけクリックとみなす
            const bool canClick = m_dragDistance < CLICK_DRAG_THRESHOLD;

            for (auto& node : m_nodes)
            {
                //アンロックされていないなら何もしない
                if (!node->IsUnlockable())
                    continue;

                // 購入されたら効果を適用
                if (node->Update(gameContext, ref_player, m_pos, canClick))
                {
                    ApplySkill(node->GetEffect(), node->GetValue());
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
            for (auto& node : m_nodes)
            {
                if (node->IsUnlockable())
                    node->Render(m_pos);
            }


            
            m_spriteBatch->Begin();

            // 描画したい四角形の範囲
            RECT rect = {0, 575, 1280, 720};

            // 塗りつぶしたい色
            XMVECTOR color = Colors::Black;

            // 1x1の白色テクスチャをRECTのサイズに拡大して描画
            m_spriteBatch->Draw(m_whiteTexture.Get(), rect, color * 0.8);


            // テキストの設定
            std::wstring orb =
                L"現在持っているオーブ: " + std::to_wstring(static_cast<int>(ref_player.GetStatus().haveOrb)) + L"個";

            //プレイヤーの持っているオーブを出す  
            m_spriteFont->DrawString(m_spriteBatch.get(), orb.c_str(), DirectX::SimpleMath::Vector2{20.0f, 675.0f},
                                     DirectX::Colors::White, 0.0, {0, 0}, {1.2, 1.2});



            // カーソルが乗っているノードの説明文
            for (auto& node : m_nodes)
            {
                if (node->IsUnlockable() && node->IsCursored(m_pos))
                {
                    m_spriteFont->DrawString(m_spriteBatch.get(), node->GetText().c_str(),
                                             DirectX::SimpleMath::Vector2{20.0f, 600.0f}, DirectX::Colors::White, 0.0f,
                                             {0, 0}, {1.2f, 1.2f});
                }
            }

            m_spriteBatch->End();


        }

        void SetTrees(GameContext& gameContext)
        {
            auto size = gameContext.deviceResources.GetOutputSize();
            float width = size.right;   // ウィンドウのクライアント幅
            float height = size.bottom; // ウィンドウのクライアント高さ
            
            //ツリー設計図の読み込み
            const auto def =SkillTreeDefinition::Create(static_cast<float>(size.right), static_cast<float>(size.bottom));

            //変なものが入っていないようにする
            m_nodes.clear();

            std::unordered_map<std::string, SkillNode*> idToNode;

            for (const auto& d : def.RefData())
            {
                const SkillNode* parent = d.parentId.empty() ? nullptr : idToNode.at(d.parentId);
                // 相対指定なら親の位置からのオフセット
                DirectX::SimpleMath::Vector2 position = d.pos;
                if (d.relative && parent)
                    position += parent->GetPosition();

                auto node = std::make_unique<SkillNode>();
                node->SetNode(gameContext, parent, position, d.type, d.num, d.cost, d.image,
                              GetEffectText(d.type));

                idToNode[d.id] = node.get();
                m_nodes.push_back(std::move(node));
            }



            auto device = gameContext.deviceResources.GetD3DDevice();
            auto context = gameContext.deviceResources.GetD3DDeviceContext();

            m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

            // 文字の背景の初期化
            CreateWhiteTexture(device);

            // テキストの初期化
            m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Font/Meiryo.spritefont");

        }
      
        //スキルの追加
        void ApplySkill(SkillEffect type,float num = 0)
        {
            switch (type)
            {
                case SkillEffect::Time:
                    // 現在の制限時間に追加する
                    ref_gameTimer.AddMaxTimer(num);
                    break;

                case SkillEffect::Speed:
                    //プレイヤーの速度を上げる
                    ref_player.GetStatus().speed += num;
                    break;

                case SkillEffect::OrbValue:
                    //オーブの価値を上げる
                    ref_orbManager.SetOrbValue(num);
                    break;

                case SkillEffect::Interval:
                    //オーブが出る間隔を減少させる
                    ref_orbManager.Remomveinterval(num);
                    break;

                case SkillEffect::Luck:
                    ref_player.GetStatus().luck += num;
                    break;

                case SkillEffect::FirstOrb:
                    //最初に出てくるオーブの追加
                    ref_orbManager.AddFirst(num);
                    break;

                case SkillEffect::Skill:
                    //クリアする
                    ref_player.GetSkill((SkillID)num);
                    break;

                case SkillEffect::Clear:
                    //クリアする
                    ref_player.GetIsCleared();
                    break;

            }
        }

        //効果ごとの説明を行う関数
        std::wstring GetEffectText(SkillEffect type)
        {
            switch (type)
            {
                case SkillEffect::Time:
                    return L"オーブを収集できる時間が増えます";

                case SkillEffect::Speed:
                    return L"あなたの足が速くなります";

                case SkillEffect::OrbValue:
                    return L"オーブの価値が増えます";

                case SkillEffect::Interval:
                    return L"オーブが出てくる間隔が減ります";

                case SkillEffect::Luck:
                    return L"オーブを手に入れた際、そのオーブの価値が倍になる確率が増えます";

                case SkillEffect::FirstOrb:
                    return L"最初のオーブの数が増えます";

                case SkillEffect::Skill:
                    return L"Eキーを押すことでオーブの生成時間を大幅に短縮することが出来ます";

                case SkillEffect::Clear:
                    return L"ロケットを買うことが出来、クリアすることが出来ます！";
            }

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
        static constexpr int NODE_COUNT = 27;

        //スキルツリーを動かす速さ
        static constexpr float NODE_MOVE_SPEED = 4;

        //スキルツリーの最大可動範囲
        static constexpr int MAX_SKILLTREE_MOVE = 100;

        //ノードをクリックできる移動量
        static constexpr float CLICK_DRAG_THRESHOLD = 5.0f; 



    private:

        //
        std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

        // スプライトフォント
        std::unique_ptr<DirectX::SpriteFont> m_spriteFont;

        //スキルノードを数分入れる
        std::vector<std::unique_ptr<SkillNode>> m_nodes;

        //プレイヤー
        Player& ref_player;

        //オーブマネージャー
        OrbManager& ref_orbManager;

        //ゲームタイマー
        GameTimer& ref_gameTimer;

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
