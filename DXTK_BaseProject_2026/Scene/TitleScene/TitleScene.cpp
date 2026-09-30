//--------------------------------------------------------------------------------------
// File: TitleScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "TitleScene.h"

using namespace DirectX;

// 更新
void TitleScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
    //マウスの取得
    auto mouse = Mouse::Get().GetState();

    float elapsedTime = static_cast<float>(gameContext.timer.GetElapsedSeconds());

    m_title->Update(elapsedTime);
    m_menu->Update();

    m_state.Update(mouse);
    //STARTの状態で押されたら
    if (m_state.leftButton == Mouse::ButtonStateTracker::RELEASED &&
        m_menu->GetMenuState() == Effect3D::Menu::STATE::START)
    {
        gameContext.audio.Stop(m_bgmHandle);
        sceneController.RequestSwitch(SceneId::GamePlayScene);
    }


    // Exitの状態で押されたら
    if (m_state.leftButton == Mouse::ButtonStateTracker::RELEASED
        &&m_menu->GetMenuState() == Effect3D::Menu::STATE::EXIT)
    {
        // ゲームを終える
        PostQuitMessage(0);
    }
}

// 描画
void TitleScene::Render(GameContext& gameContext)
{
    m_background->Render();

    m_menu->Render();

    m_title->Render();
}

// シーン切り替え時に呼び出される関数
void TitleScene::OnEnter(GameContext& gameContext)
{
    //デバイスリソースの取得
    auto& deviceres = gameContext.deviceResources;

    // タイトルの作成
    m_title = std::make_unique<Effect3D::Title>();

    // メニューの作成
    m_menu = std::make_unique<Effect3D::Menu>();

    // 背景の作成
    m_background = std::make_unique<Effect3D::BackGround>();

    // メニューの初期化処理
    D3D11_VIEWPORT windowInfo = deviceres.GetScreenViewport();
    m_menu->Initialize(&deviceres, (int)windowInfo.Width, (int)windowInfo.Height);


    // タイトルの初期化処理
    m_title->Initialize(&deviceres, (int)windowInfo.Width, (int)windowInfo.Height);

    // 背景の初期化処理
    m_background->Initialize(&deviceres, (int)windowInfo.Width, (int)windowInfo.Height);

    // bgmの設定
    gameContext.audio.LoadSound("TitleBgm", "Resources/Audio/Bgm/忍び寄る邂逅.wav");

    SuzukiLib::Audio::AudioPlayDesc desc;
    desc.channel = SuzukiLib::Audio::AudioChannel::Bgm;
    desc.loop = true;
    m_bgmHandle = gameContext.audio.Play("TitleBgm", desc);
    gameContext.audio.SetVolume(m_bgmHandle, 0.65f);


}
