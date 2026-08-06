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

using namespace DirectX;

// コンストラクタ
GamePlayScene::GamePlayScene() 
    : m_camera(SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.1f, 0.0f))
{
    
}

// 更新
void GamePlayScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	Imase::DebugRenderer& debugRenderer = gameContext.debugRenderer;
	// 経過時間を取得する
	float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    //ステータスがクリアを示していたなら
    if (m_status->IsClear())
    {
        //クリアシーンに切り替える
        sceneController.RequestSwitch(SceneId::ClearScene);
    }

    //タイマーがゼロになったら
    if (m_timer <= 0)
    {
        //ステータスを表示する
        m_status->Update(m_timer);

        return;
    }

    //カメラの設定
    GamePlayCamera(elapsedTime);

    //プレイヤーの更新
    m_player->Update(elapsedTime);

    //ステージとの当たり判定
    for (int i=0;i<m_stageManager->GetNumStages();i++)
    {
        Stage* Stage = m_stageManager->GetStage(i);
        //もしプレイヤーとステージがぶつかったら
        if (m_collisionChecker->CheckCollision(*m_player->GetCollider(), *Stage->GetCollider()))
        {
            //押し出し判定を行う
            CheckCollider(m_player->GetCollider(), Stage->GetCollider());
        }

    }
    //オーブとの当たり判定
    for (int i = 0; i < m_orbManager->GetNumOrbs(); i++)
    {
        Orb* pOrb = m_orbManager->GetOrb(i);
        // もしプレイヤーとオーブがぶつかったら
        if (m_collisionChecker->CheckCollision(*m_player->GetCollider(), *pOrb->GetCollider()))
        {
            //音を鳴らす
            gameContext.audio.PlayOneShot("GetOrb");
            // 持ってるオーブの数の追加
            m_status->AddOrbCount(pOrb->GetOrbValue());
            //位置を変更する
            pOrb->SetRandom();

        }
    }

    //時間を減らす
    m_timer -= elapsedTime;

    //テキストの設定
    std::wstring text = L"Time: " + std::to_wstring(m_timer);
    std::wstring OrbCounttext = L"Orb: " + std::to_wstring(m_status->GetOrbCount());

    //テキストの描画
    debugRenderer.DrawText({500.0f, 0.0f}, text);
    debugRenderer.DrawText({.0f, 100.0f}, OrbCounttext);

    //debugRenderer.DrawText({0.0f, 0.0f}, L"GamePlayScene");

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

    //制限時間が0になったら
    if (m_timer <= 0)
    {
        //ステータスの描画を行う
        m_status->Render();

         return;
    }

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

    //for (int i = 0; i < m_orbManager->GetNumOrbs(); i++)
    //{
    //    Orb* pOrb = m_orbManager->GetOrb(i);
    //    m_renderer->Render(context, m_view, m_projection, *pOrb->GetCollider());
    //}

    //目線で手前側のものを前にする
    m_orbManager->Sort(eye);

    for (int i = 0; i < m_orbManager->GetNumOrbs(); i++)
    {
        Orb* pOrb = m_orbManager->GetOrb(i);
        pOrb->Render(context, m_view, m_projection, eye, target);
    }

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

    // ---テクスチャのロード---//
    DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/Orb.dds", nullptr,
                                               m_orbTexture.ReleaseAndGetAddressOf()));

    //DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Resources/Textures/.dds", nullptr,
    //                                           m_timerTexture.ReleaseAndGetAddressOf()));

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


	// デバッグカメラの作成
    m_debugCamera = std::make_unique<Imase::DebugCamera>(rect.right, rect.bottom);

	// エフェクトを作成する工場
    EffectFactory fx(device);
    fx.SetDirectory(L"Resources/Models");	// <- ddsのフォルダ

	// モデルの読み込み
	m_model = Model::CreateFromCMO(device, L"Resources/Models/Dammy.cmo", fx);

	// プレイヤーの作成
    m_player = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());
    //m_player2 = std::make_unique<Player>(gameContext, m_view, m_projection, m_model.get());

    //レンダラーの読み込み
    m_renderer = std::make_unique<Itsuki::ColliderRenderer>(device, context, &gameContext.commonStates);

    //コリジョンチェッカーの作成
    m_collisionChecker = std::make_unique<Itsuki::CollisionChecker>();

    // ーーーーーーーーーーーーーーーーーーーーーーガチでやむを得ず制作確実に変更するーーーーーーーーーーーーーーーーーーーーーーーーーー

    //箱の座標
    //床
    DirectX::SimpleMath::Vector3 pos{.0f,0.0f,.0f};
    DirectX::SimpleMath::Vector3 min{-2.5f,-11.0f,-2.5f};
    DirectX::SimpleMath::Vector3 max{2.5f,.0f,2.5f};
     
    //壁
    DirectX::SimpleMath::Vector3 pos2{-10.0f, 0.0f, .0f};
    DirectX::SimpleMath::Vector3 min2{-.1f, -11.0f, -10.0f};
    DirectX::SimpleMath::Vector3 max2{0.0f, 10.0f, 10.0f};

    DirectX::SimpleMath::Vector3 pos3{10.0f, 0.0f, .0f};
    DirectX::SimpleMath::Vector3 min3{.0f, -11.0f, -10.0f};
    DirectX::SimpleMath::Vector3 max3{0.1f, 10.0f, 10.0f};

    DirectX::SimpleMath::Vector3 pos4{0.0f, 0.0f, -10.0f};
    DirectX::SimpleMath::Vector3 min4{-10.0f, -11.0f, -.1f};
    DirectX::SimpleMath::Vector3 max4{10.0f, 10.0f, .0f};

    DirectX::SimpleMath::Vector3 pos5{0.0f, 0.0f, 10.0f};
    DirectX::SimpleMath::Vector3 min5{-10.0f, -11.0f, .0f};
    DirectX::SimpleMath::Vector3 max5{10.0f, 10.0f, .1f};

    //床
    DirectX::SimpleMath::Vector3 pos6{8.0f, 0.0f, .0f};
    DirectX::SimpleMath::Vector3 min6{-1.5f, -11.0f, -1.5f};
    DirectX::SimpleMath::Vector3 max6{1.5f, .0f, 1.5f};

    DirectX::SimpleMath::Vector3 pos7{-8.0f, 5.0f, .0f};
    DirectX::SimpleMath::Vector3 min7{-1.5f, -16.0f, -1.5f};
    DirectX::SimpleMath::Vector3 max7{1.5f, .0f, 1.5};

    //一番下の床
    DirectX::SimpleMath::Vector3 pos8{.0f, -11.0f, .0f};
    DirectX::SimpleMath::Vector3 min8{-10.0f, -.1f, -10.0f};
    DirectX::SimpleMath::Vector3 max8{10.0f, .0f, 10.0};



    // ーーーーーーーーーーーーーーーーーーーーーーガチでやむを得ず制作確実に変更するーーーーーーーーーーーーーーーーーーーーーーーーーー

    m_stageManager = std::make_unique<StageManager>(&gameContext, m_CprimitiveBatch.get());

    m_stageManager->AddStage(min,max,pos);
    m_stageManager->AddStage(min2,max2,pos2);
    m_stageManager->AddStage(min3,max3,pos3);
    m_stageManager->AddStage(min4,max4,pos4);
    m_stageManager->AddStage(min5,max5,pos5);
    m_stageManager->AddStage(min6,max6,pos6);
    m_stageManager->AddStage(min7,max7,pos7);
    m_stageManager->AddStage(min8,max8,pos8);

    m_orbManager = std::make_unique<OrbManager>(&gameContext, m_primitiveBatch.get(), m_orbTexture.Get() ,FIRST_ORB);

    //bgmの設定
    gameContext.audio.LoadSound("Bgm", "Resources/Audio/Bgm/GameBgm.wav");
    gameContext.audio.LoadSound("GetOrb", "Resources/Audio/Se/se_hirameki02.wav");
    gameContext.audio.LoadSound("Jump", "Resources/Audio/Se/se_bound1.wav");
    gameContext.audio.LoadSound("Stop", "Resources/Audio/Se/電子ルーレットの出目が点滅.wav");
    gameContext.audio.LoadSound("Buy", "Resources/Audio/Se/5ac8850c.wav");

    SuzukiLib::Audio::AudioPlayDesc desc;
    desc.channel = SuzukiLib::Audio::AudioChannel::Bgm;
    desc.loop = true;
    m_bgmHandle = gameContext.audio.Play("Bgm", desc);
    gameContext.audio.SetVolume(m_bgmHandle, 1.0f);


    m_status = std::make_unique<Status>(gameContext, m_player.get(), m_orbManager.get());

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

// ゲームプレイ用カメラ
void GamePlayScene::GamePlayCamera(float elapsedTime)
{
	// プレイヤーの位置からのカメラの相対位置
    SimpleMath::Vector3 cameraPosition(0.0f, .0f, .05f);

	// 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_player->GetFacingAngleRad().y);
    SimpleMath::Matrix rotX = SimpleMath::Matrix::CreateRotationX(m_player->GetFacingAngleRad().x);

	// カメラの相対位置をプレイヤーの向いている角度で回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(cameraPosition, rotX*rotY);

    //カメラの見る位置を設定
    m_camera.SetCamera(m_player->GetPosition() + v, m_player->GetPosition());

    //一人称のカメラの更新
    m_camera.Update(elapsedTime);

}

// タイトル用カメラ
void GamePlayScene::ThirdCamera(float elapsedTime)
{
    // プレイヤーの位置からのカメラの相対位置
    SimpleMath::Vector3 cameraPosition(0.0f, 2.5f, 4.0f);


	// 回転行列を作成
    SimpleMath::Matrix rotY = SimpleMath::Matrix::CreateRotationY(m_player->GetFacingAngleRad().y);

	// カメラの相対位置を回転させる
    SimpleMath::Vector3 v = SimpleMath::Vector3::Transform(cameraPosition, rotY);

    // バネカメラの見る位置を設定
    m_camera.SetCamera(m_player->GetPosition() + v, m_player->GetPosition());


    m_camera.Update(elapsedTime);

}

void GamePlayScene::CheckCollider(Itsuki::SphereCollider* sphere, Itsuki::BoxCollider* aabb)
{
    using namespace DirectX;

    // どちらかがない場合は判定しない
    if (!aabb || !sphere)
    {
        return;
    }

    //もっとも近い座標の作成
    SimpleMath::Vector3 closestPoint = {0, 0, 0};

    // それぞれの座標で球に最も近い点を取る
    closestPoint.x = std::clamp(sphere->GetPosition().x, aabb->GetMin().x, aabb->GetMax().x);
    closestPoint.y = std::clamp(sphere->GetPosition().y, aabb->GetMin().y, aabb->GetMax().y);
    closestPoint.z = std::clamp(sphere->GetPosition().z, aabb->GetMin().z, aabb->GetMax().z);


    //球の中心点を取る
    SimpleMath::Vector3 center = sphere->GetPosition();

    //球から近い点のベクトル
    SimpleMath::Vector3 diff = center - closestPoint;

    // ベクトルの二乗を取る（平方根を取るより早い）
    float distSq = (closestPoint - sphere->GetPosition()).LengthSquared();

    //半径を取得
    float r = sphere->GetRadius();

    //平方根をとる（やむなし）
    float dist = sqrtf(distSq);

    // 球の中心が AABB の内部にある場合（dist = 0）
    if (dist == 0.0f)
    {
        // とりあえず 上方向に押し出す（簡易処理）
        diff = SimpleMath::Vector3(0, 1, 0);
        dist = 1.0f;
    }

    // 押し戻し方向
    SimpleMath::Vector3 pushDir = diff / dist;

    // 押し戻し量
    float penetration = r - dist;

    SimpleMath::Vector3 position = center + pushDir * penetration;

    //
    m_player->SetPosition(center + pushDir * penetration);
    m_player->GetCollider()->SetPosition(center + pushDir * penetration);        

    // プレイヤーの位置を修正
    if (pushDir.y >0)
    {
        m_player->SetIsGround(1);

        m_player->SetVelocityY(0);

    }


}


