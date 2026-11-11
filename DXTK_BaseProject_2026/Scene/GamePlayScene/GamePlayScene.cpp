//--------------------------------------------------------------------------------------
// File: GamePlayScene.cpp
//
// モデルテスト用シーン
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "GamePlayScene.h"
#include <ItsukiLib/SphereCollider.h>
#include <ItsukiLib/Collider.h>
#include"ItsukiLib/BoxCollider.h"
#include "Orb.h"
#include <ItsukiLib/ColliderFactory.h>
#include <random>

using namespace DirectX;
using CollFactory = Itsuki::ColliderFactory;

// コンストラクタ
GamePlayScene::GamePlayScene() 
    : m_camera(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.1f, 0.0f))
{
    // 当たり判定工場の作成
    CollFactory& colF = CollFactory::GetRefInstance();
    // 当たり判定を格納する
    CollFactory::GetRefInstance().AddCollider<Itsuki::BoxCollider>(Itsuki::BOX);
    CollFactory::GetRefInstance().AddCollider<Itsuki::SphereCollider>(Itsuki::SPHERE);
}

// 更新
void GamePlayScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{

    Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;
    // 経過時間を取得する
    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    // タイマーがゼロになったら
    if (m_timer->GetTimer() <= 0)
    {

        // マウスの表示
        m_player->SetMouseModeAbsolute();
        m_skilltree->Update(elapsedTime, gameContext);
        m_backButton.Update(elapsedTime, m_player.get(), m_orbManager.get(), m_timer.get());
        return;
    }

    // プレイヤーの更新
    m_player->Update(elapsedTime);

    // オブジェクトの更新
    for (auto& object : m_throwableObjects)
    {
        object->Update(elapsedTime);
    }

    GamePlayCamera(elapsedTime);

    // ステージとの当たり判定
    for (int i = 0; i < m_stageManager->GetNumStages(); i++)
    {
        Stage* Stage = m_stageManager->GetStage(i);

        SimpleMath::Vector3 separation;
        if (m_collisionChecker->GetSeparation(*m_player->GetCollider(), *Stage->GetCollider(), separation))
        {
            const auto correctedPosition = m_player->GetPosition() + separation;

            m_player->SetPosition(correctedPosition);
            m_player->GetCollider()->SetPosition(correctedPosition);

            // 重力加速でのめり込みが起きないようにする
            if (separation.y > 0)
            {
                m_player->SetIsGround(1);

                m_player->SetVelocity({m_player->GetVelocity().x, 0, m_player->GetVelocity().z});
            }
        }

        // 投げれるオブジェクトの更新
        for (auto& object : m_throwableObjects)
        {

            if (m_collisionChecker->GetSeparation(*object->GetCollider(), *Stage->GetCollider(), separation))
            {
                const auto correctedPosition = object->GetPosition() + separation;

                object->SetPosition(correctedPosition);
                object->GetCollider()->SetPosition(correctedPosition);

                // 重力加速でのめり込みが起きないようにする
                if (separation.y > 0)
                {
                    object->SetVelocity(SimpleMath::Vector3::Zero);
                }
            }
        }
    }


    // 何も持っていない状態でプレイヤーがマウスをクリックしたら
    if (m_player->IsMousePressed() && !m_player->GetIsHeld())
    {
        if (auto obj = FindThrowObj())
        {
            obj->PickUp();
            m_player->SetIsHeld();
        }

    }
    // 何か持った状態でプレイヤーがマウスをクリックしたら
    else if (m_player->IsMousePressed() && m_player->GetIsHeld())
    {
        // 目線の向き
        SimpleMath::Vector3 rayDirection = m_camera.GetRay();

        // 大きさは取らず向きだけにする
        rayDirection.Normalize();

        // オブジェクト分確認する
        for (auto& object : m_throwableObjects)
        {
            // もし持っているものだったら
            if (object->IsHeld())
            {
                const SimpleMath::Vector3 throwPosition = m_camera.GetEyePosition() + rayDirection * 1.25f;

                object->Throw(throwPosition, rayDirection * 8.0f + m_player->GetVelocity());
                return;
            }
        }
        
        m_player->SetIsHeld();

    }


    // もしクリア状態なら
    if (m_player->GetIsCleared())
    {
        // ロケットとの当たり判定を取りあたったらクリアシーンへ
        if (m_collisionChecker->CheckCollision(*m_player->GetCollider(), *m_rocket->GetCollider()))
        {
            // マウスの表示
            m_player->SetMouseModeAbsolute();
            gameContext.audio.Stop(m_bgmHandle);
            sceneController.RequestSwitch(SceneId::ClearScene);
        }

        return;
    }

    // オーブマネージャーの更新
    m_orbManager->Update(elapsedTime);

    auto collectOrb = [&](int index)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, 100);
        if (m_player->GetStatus().luck >= dist(gen))
        {
            gameContext.audio.PlayOneShot(
                "GetOrb", SuzukiLib::Audio::AudioPlayDesc{SuzukiLib::Audio::AudioChannel::Se, 0.3, 0.5});
            m_player->GetStatus().haveOrb += m_orbManager->GetOrbValue() * 2;
        }
        else
        {
            gameContext.audio.PlayOneShot("GetOrb",
                                          SuzukiLib::Audio::AudioPlayDesc{SuzukiLib::Audio::AudioChannel::Se, 0.3});
            m_player->GetStatus().haveOrb += m_orbManager->GetOrbValue();
        }
        m_orbManager->DeleteOrb(index);
    };

    // オーブとの当たり判定
    for (int i = m_orbManager->GetNumOrbs() - 1; i >= 0; i--)
    {

        Orb* pOrb = m_orbManager->GetOrb(i);
        bool collected = m_collisionChecker->CheckCollision(*m_player->GetCollider(), *pOrb->GetCollider());
        for (const auto& object : m_throwableObjects)
        {
            if (!collected && m_collisionChecker->CheckCollision(*object->GetCollider(), *pOrb->GetCollider()))
                collected = true;
        }
        if (collected)
        {
            collectOrb(i);
        }
    }

    // フィーバータイムに突入したら
    if (m_player->GetFever())
    {
        // オーブのカウントをめちゃ減らす
        m_orbManager->SetInterval(0.5);
    }
    else // 違うなら
    {
        // 戻す
        m_orbManager->ReturnInterval();
    }

    // 時間を減らす
    m_timer->RemoveTimer(elapsedTime);
}


// 描画
void GamePlayScene::Render(GameContext& gameContext)
{
    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    auto& states = gameContext.commonStates;

    auto device = gameContext.deviceResources.GetD3DDevice();

	SimpleMath::Matrix world;

	// 一人称のカメラからからビュー行列を取得する
    m_view = m_camera.GetViewMatrix();

    // カメラの位置
    SimpleMath::Vector3 eye = m_camera.GetEyePosition();

    // ターゲットの位置
    SimpleMath::Vector3 target = m_camera.GetTargetPosition();

	//m_player->Render(context, m_view, m_projection, eye, target);

    SimpleMath::Matrix SkyWorld = SimpleMath::Matrix::CreateScale(15);


    //制限時間が0になったら
    if (m_timer->GetTimer() <= 0)
    {

        m_background->Render();

        m_skilltree->Render();
        m_backButton.Render();
         return;
    }

    m_skyModelSun->Draw(context, gameContext.commonStates, SkyWorld, m_view, m_projection);

    //m_renderer->Render(context, m_view, m_projection, *m_rocket->GetCollider());

    //ステージの線形描画
    for (int i = 0; i < m_stageManager->GetNumStages(); i++)
    {
        Stage* pStage = m_stageManager->GetStage(i);
        m_renderer->Render(context, m_view, m_projection, *pStage->GetCollider());
    }
    //ステージの描画
    for (int i = 0; i < m_stageManager->GetNumStages(); i++)
    {
        Stage* pStage = m_stageManager->GetStage(i);
        pStage->Render(context, m_view, m_projection, eye, target);
    }

    // クリアしたなら
    if (m_player->GetIsCleared())
    {
        //ロケットの描画
        m_rocket->Render();

        return;
    }

    //当たり判定描画
    for (int i = 0; i < m_orbManager->GetNumOrbs(); i++)
    {
        Orb* pOrb = m_orbManager->GetOrb(i);
        m_renderer->Render(context, m_view, m_projection, *pOrb->GetCollider());
    }

    //目線で手前側のものを前にする
    m_orbManager->Sort(eye);

    for (int i = 0; i < m_orbManager->GetNumOrbs(); i++)
    {
        Orb* pOrb = m_orbManager->GetOrb(i);
        pOrb->Render(context, m_view, m_projection, eye, target);
    }


    bool isRay=false;
    
    SimpleMath::Vector3 cameraForward = target - eye;
    cameraForward.Normalize();

    // レイの作成
    const Itsuki::Ray viewRay{m_camera.GetEyePosition(), cameraForward};

    //投げるオブジェクトの描画
    for (const auto& object : m_throwableObjects)
    {
        object->Render(m_view, m_projection, eye, cameraForward, SimpleMath::Vector3::UnitY);

        float distance = 0.0f;
    }

        // テキストの設定
    std::wstring text = L"Time: " + std::to_wstring(static_cast<int>(m_timer->GetTimer()));
    std::wstring OrbCounttext = L"Orb: " + std::to_wstring(m_player->GetStatus().haveOrb);

    // 文字列の大きさを取得
    DirectX::SimpleMath::Vector2 size = m_spriteFont->MeasureString(text.c_str());

    // 横幅
    int width = m_windowSize.right - m_windowSize.left;

    // 縦幅
    int height = m_windowSize.bottom - m_windowSize.top; 

    m_spriteBatch->Begin();

    //もし掴めるオブジェクトを見ていたら
    if (auto obj = FindThrowObj())
    {
        
        // 画像のサイズ取得
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        m_aimingHit->GetResource(resource.GetAddressOf());

        Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
        if (!SUCCEEDED(resource.As(&tex2D)))
            false;

        D3D11_TEXTURE2D_DESC desc = {};
        tex2D->GetDesc(&desc);
        auto textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height);
        XMFLOAT2 origin = {textureSize.x / 2, textureSize.y / 2};

        m_spriteBatch->Draw(m_aimingHit.Get(), XMFLOAT2(width / 2, height / 2), nullptr, Colors::White, 0.0, origin, 0.5f);
    }
    else
    {
        // 画像のサイズ取得
        Microsoft::WRL::ComPtr<ID3D11Resource> resource;
        m_aimingHit->GetResource(resource.GetAddressOf());

        Microsoft::WRL::ComPtr<ID3D11Texture2D> tex2D;
        if (!SUCCEEDED(resource.As(&tex2D)))
            false;

        D3D11_TEXTURE2D_DESC desc = {};
        tex2D->GetDesc(&desc);
        auto textureSize = DirectX::SimpleMath::Vector2(desc.Width, desc.Height);
        XMFLOAT2 origin = {textureSize.x / 2, textureSize.y / 2};

        m_spriteBatch->Draw(m_aiming.Get(), XMFLOAT2(width / 2, height / 2), nullptr, Colors::White, 0.0, origin, 0.5f);
    }

    // テキストの描画
    m_spriteFont->DrawString(m_spriteBatch.get(), text.c_str(), DirectX::SimpleMath::Vector2{width / 2 - size.x, 0},
                             DirectX::Colors::MidnightBlue, 0.0, {0, 0}, {1.0, 1.0});

    m_spriteFont->DrawString(m_spriteBatch.get(), OrbCounttext.c_str(), DirectX::SimpleMath::Vector2{.0f, 100.0f},
                             DirectX::Colors::MidnightBlue, 0.0, {0, 0}, {1.0, 1.0});
    // debugRenderer.DrawText({0.0f, 0.0f}, L"GamePlayScene");

    m_spriteBatch->End();


}


// シーン切り替え時に呼び出される関数
void GamePlayScene::OnEnter(GameContext& gameContext)
{
	// プロジェクション行列を設定する
    m_projection = CreateProjectionMatrix(gameContext);

	// DirectX3Dのデバイスを取得する
    auto device = gameContext.deviceResources.GetD3DDevice();

	// DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

	// 画面のサイズを取得する
    RECT rect = gameContext.deviceResources.GetOutputSize();

    // スプライトバッチの作成
    m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(context);
    m_CprimitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(context);
    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

    // テキストの初期化
    m_spriteFont = std::make_unique<DirectX::SpriteFont>(device, L"Resources/Font/Meiryo.spritefont");


    // ---テクスチャのロード---//
    DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/Orb.dds", nullptr,
                                               m_orbTexture.ReleaseAndGetAddressOf()));

    //照準
    DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/Aiming.png", nullptr,
                                      m_aiming.ReleaseAndGetAddressOf());
    DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/AimingHit.png", nullptr,
                                      m_aimingHit.ReleaseAndGetAddressOf());

    // ---テクスチャのロード---//


    // ベーシックエフェクトの作成
    m_basicEffect = std::make_unique<DirectX::BasicEffect>(device);
    m_basicEffect->SetVertexColorEnabled(true);
    m_basicEffect->SetLightingEnabled(false);
    m_basicEffect->SetTextureEnabled(true);
    m_basicEffect->SetTexture(m_orbTexture.Get());

    // 入力レイアウトの作成
    DX::ThrowIfFailed(CreateInputLayoutFromEffect<VertexPositionColorTexture>(device, m_basicEffect.get(),
                                                                              m_inputLayout.ReleaseAndGetAddressOf()));

    //モデルの作成
    m_backGroundModel = std::make_unique<DirectX::Model>();

	// デバッグカメラの作成
    m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// エフェクトを作成する工場
    EffectFactory fx(device);
    fx.SetDirectory(L"Resources/Models");	// <- ddsのフォルダ
    

    //レンダラーの読み込み
    m_renderer = std::make_unique<Itsuki::ColliderRenderer>(device, context, &gameContext.commonStates);

    //コリジョンチェッカーの作成
    m_collisionChecker = std::make_unique<Itsuki::CollisionChecker>();

    // ーーーーーーーーーーーーーーーーーーーーーーオブジェクトの当たり判定設定ーーーーーーーーーーーーーーーーーーーーーーーーーー

    //箱の座標
    //床
    Itsuki::CollisionParams floar1 = {{.0f, 0.0f, .0f}, {-4.0f, -11.0f, -4.0f}, {4.0f, .0f, 4.0f}, {0}};
    Itsuki::CollisionParams floar2 = {{8.0f, 0.0f, .0f}, {-1.5f, -11.0f, -1.5f}, {1.5f, .0f, 1.5f}, {0}};
    Itsuki::CollisionParams floar3 = {{-8.0f, 5.0f, .0f}, {-1.5f, -16.0f, -1.5f}, {1.5f, .0f, 1.5}, {0}};
    Itsuki::CollisionParams floar4 = {{.0f, -11.0f, .0f}, {-10.0f, -.1f, -10.0f}, {10.0f, .0f, 10.0}, {0}};

    //壁
    Itsuki::CollisionParams wall1 = {{-4.0f, 0.0f, .0f}, {-.1f, -11.0f, -10.0f}, {0.0f, 6.0f, 10.0f}, {0}};
    Itsuki::CollisionParams wall2 = {{4.0f, 0.0f, .0f}, {.0f, -11.0f, -10.0f}, {0.1f, 6.0f, 10.0f}, {0}};
    Itsuki::CollisionParams wall3 = {{0.0f, 0.0f, -4.0f}, {-10.0f, -11.0f, -.1f}, {10.0f, 6.0f, .0f}, {0}};
    Itsuki::CollisionParams wall4 = {{0.0f, 0.0f, 4.0f}, {-10.0f, -11.0f, .0f}, {10.0f, 6.0f, .1f}, {0}};

    ///プレイヤー
    Itsuki::CollisionParams player = {{0.0f, 0.5f, 3.0f}, {-.15f, -.15f, -.15f}, {0.15f, 0.15f, 0.15f}, {0.3}};

    //ロケット
    Itsuki::CollisionParams rocket = {{0.0f, .75f, 0.0f}, {-0.25, -.5, -0.25}, {0.25, .5, 0.25}, {0.5}};

    // ーーーーーーーーーーーーーーーーーーーーーーオブジェクトの当たり判定設定ーーーーーーーーーーーーーーーーーーーーーーーーーー

    // 当たり判定工場の作成
    CollFactory& colF = CollFactory::GetRefInstance();

	// モデルの読み込み
    m_model = Model::CreateFromCMO(device, L"Resources/Models/Dammy.cmo", fx);
    m_RocketModel = Model::CreateFromCMO(device, L"Resources/Models/Space_Rocket.cmo", fx);


    //空のモデルの読み込み
    m_skyModelSun = Model::CreateFromCMO(device, L"Resources/Models/Skydome.cmo", fx);



    for (auto& mesh : m_skyModelSun->meshes)
    {
        for (auto& parts:mesh->meshParts )
        {
            // IEffect から BasicEffect にキャストする
            auto lights = dynamic_cast<DirectX::BasicEffect*>(parts->effect.get());
            if (lights)
            {

                //自己発光させる
                lights->SetEmissiveColor({0.8f, 0.8f, 0.8f});
            }
        }
    }

        for (auto& mesh : m_RocketModel->meshes)
    {
        for (auto& parts : mesh->meshParts)
        {
            // IEffect から BasicEffect にキャストする
            auto lights = dynamic_cast<DirectX::BasicEffect*>(parts->effect.get());
            if (lights)
            {

                // 自己発光させる
                lights->SetEmissiveColor({0.8f, 0.8f, 0.8f});
            }
        }
    }



    // プレイヤーの作成
    m_player = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get(),
                                        colF.MakeCollider(Itsuki::SHAPE::SPHERE, player));
    m_player->Initialze();

    // 投げれるオブジェクトの作成
    auto addThrowable = [&](const SimpleMath::Vector3& position, float size, int carryPower)
    {
        //大きさの半分を取る
        const float half = size * 0.5f;

        //立方体を作成する
        Itsuki::CollisionParams box = {position, {-half, -half, -half}, {half, half, half}, 0.0f};
        m_throwableObjects.push_back(
            std::make_unique<ThrowableObject>(context, colF.MakeCollider(Itsuki::SHAPE::BOX, box), size, carryPower));
    };

    // オブジェクトの追加
    addThrowable({0.0f, 0.45f, 1.5f}, 0.25f, 1);
    addThrowable({1.4f, 0.5f, 0.5f}, 0.9f, 2);


    //Rocketの作成
    m_rocket = std::make_unique<Rocket>(gameContext, m_view, m_projection, m_RocketModel.get(),
                                        colF.MakeCollider(Itsuki::SHAPE::BOX, rocket));

    auto& dr = gameContext.deviceResources;
    m_windowSize = dr.GetOutputSize();

    // マウスの状態を変える
    m_player->SetMouseModeRelative();

    m_stageManager = std::make_unique<StageManager>(&gameContext, m_CprimitiveBatch.get());
    //ステージの追加
    m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX,floar1));
    //m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, floar2));
    //m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, floar3));
    //m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, floar4));
    m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, wall1));
    m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, wall2));
    m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, wall3));
    m_stageManager->AddStage(colF.MakeCollider(Itsuki::SHAPE::BOX, wall4));

    //オーブマネージャーの設定
    m_orbManager = std::make_unique<OrbManager>(&gameContext, m_primitiveBatch.get(), m_orbTexture.Get());
    m_orbManager->Initialize();

    // タイマーの設定
    m_timer = std::make_unique<Itsuki::GameTimer>();
    m_timer->Initialize();

    //bgmの設定
    gameContext.audio.LoadSound("GameBgm", "Resources/Audio/Bgm/GameBgm.wav");
    gameContext.audio.LoadSound("SkillBgm", "Resources/Audio/Bgm/クロスロード.wav");
    gameContext.audio.LoadSound("GetOrb", "Resources/Audio/Se/se_hirameki02.wav");
    gameContext.audio.LoadSound("Jump", "Resources/Audio/Se/se_bound1.wav");
    gameContext.audio.LoadSound("Stop", "Resources/Audio/Se/電子ルーレットの出目が点滅.wav");
    gameContext.audio.LoadSound("Buy", "Resources/Audio/Se/5ac8850c.wav");


    SuzukiLib::Audio::AudioPlayDesc desc;
    desc.channel = SuzukiLib::Audio::AudioChannel::Bgm;
    desc.loop = true;
    m_bgmHandle = gameContext.audio.Play("GameBgm", desc);
    gameContext.audio.SetVolume(m_bgmHandle, 0.8f);

    //スキルツリーの生成m_bgmHandle
    m_skilltree = std::make_unique<Itsuki::SkillTree>(*m_player, *m_orbManager, *m_timer);

    //スキルツリーの設定
    m_skilltree->SetTrees(gameContext);

    //バックボタンの設定
    m_backButton.Initialize(gameContext, {1200, 625}, L"Resources/Textures/NextButton.png");


    // 背景の作成
    m_background = std::make_unique<Effect3D::BackGround>();

    // 背景の初期化処理
    m_background->Initialize(&gameContext.deviceResources, (int)gameContext.deviceResources.GetScreenViewport().Width,
                             (int)gameContext.deviceResources.GetScreenViewport().Height);
    //背景の画像設定
    m_background->SetTexture(L"Resources/Textures/Grid2.png");

}


// プロジェクション行列を作成する関数
DirectX::SimpleMath::Matrix GamePlayScene::CreateProjectionMatrix(GameContext& gameContext)
{
    SimpleMath::Matrix m;

	// 画面のサイズを取得する
    RECT rect = gameContext.deviceResources.GetOutputSize();

	// プロジェクション行列を作成
	m = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),	// 画角
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),	// アスペクト比
		0.1f,	// Near Clip
		1000.0f	// Far Clip
	);

    return m;
}

// ウインドウサイズ変更時に呼び出される関数
void GamePlayScene::OnWindowSizeChanged(GameContext& gameContext)
{
    // プロジェクション行列を設定する
    m_projection = CreateProjectionMatrix(gameContext);
}

//視線に入っている持てるオブジェクトを見つける関数
ThrowableObject* GamePlayScene::FindThrowObj()
{
    // 目線の向き
    SimpleMath::Vector3 rayDirection = m_camera.GetRay();

    // 大きさは取らず向きだけにする
    rayDirection.Normalize();

    // レイの作成
    const Itsuki::Ray viewRay{m_camera.GetEyePosition(), rayDirection};

    // オブジェクト分確認する
    for (auto& object : m_throwableObjects)
    {
        //掴まれているオブジェクトは取らない
        if (object->IsHeld())
            continue;

        // もしプレイヤーが持てるものでなかったら次へ行く
        if (object->GetRequiredCarryPower() > m_player->GetCarryPower())
            continue;
        float distance = 0.0f;

        // もしも範囲内のRayに当たり、投げれる距離にいたら
        if (object->IsGetRay(viewRay, *m_collisionChecker, distance) && distance <= THROWABLE_RAY_DISTANCE)
        {
            //そのオブジェクトの情報を返す
            return object.get();
        }
        return nullptr;

    }

    return nullptr;
}

// ゲームプレイ用カメラ
void GamePlayScene::GamePlayCamera(float elapsedTime)
{
	// プレイヤーの位置からのカメラの相対位置
    SimpleMath::Vector3 cameraPosition(0.0f, .0f, .05f);

	// 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_player->GetFacingAngleRad().y);
    SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_player->GetFacingAngleRad().x);

	// カメラの相対位置をプレイヤーの向いている角度で回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(cameraPosition, rotX * rotY);

    //カメラの見る位置を設定
    m_camera.SetCamera(m_player->GetPosition() + v, m_player->GetPosition());

    //一人称のカメラの更新
    m_camera.Update(elapsedTime);

}



