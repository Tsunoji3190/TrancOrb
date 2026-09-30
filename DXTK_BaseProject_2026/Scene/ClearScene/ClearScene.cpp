//--------------------------------------------------------------------------------------
// File: ClearScene.cpp
//
// 新規シーン作成時の元にするファイル
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "pch.h"
#include "ClearScene.h"

using namespace DirectX;

// 更新
void ClearScene::Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext)
{
	//キーボードの取得
	auto kb = Keyboard::Get().GetState();


	if (gameContext.keyboardTracker.pressed.Space)
	{
        gameContext.audio.Stop(m_bgmHandle);
        sceneController.RequestSwitch(SceneId::TitleScene);
	}

}

// 描画
void ClearScene::Render(GameContext& gameContext)
{
	gameContext;

    m_spriteBatch->Begin();

    // クリア画像の描画
    m_spriteBatch->Draw(m_texture.Get(), {0, 0}, nullptr, m_color, 0.0f, g_XMZero, {1, 1});

    m_spriteBatch->End();
}

// シーン切り替え時に呼び出される関数
void ClearScene::OnEnter(GameContext& gameContext)
{
    // DirectX3Dのデバイスを取得する
    auto device = gameContext.deviceResources.GetD3DDevice();

    // DirectX3Dのデバイスコンテキストを取得する
    auto context = gameContext.deviceResources.GetD3DDeviceContext();

    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(context);

    //画像の設定
    DirectX::CreateWICTextureFromFile(device, L"Resources/Textures/TRANC CLEAR.png", nullptr, m_texture.ReleaseAndGetAddressOf());

    // bgmの設定
    gameContext.audio.LoadSound("ClearBgm", "Resources/Audio/Bgm/A_Sanctuary_of_Healing.wav");

    SuzukiLib::Audio::AudioPlayDesc desc;
    desc.channel = SuzukiLib::Audio::AudioChannel::Bgm;
    desc.loop = true;

    //Bgmの情報を入れる
    m_bgmHandle = gameContext.audio.Play("ClearBgm", desc);
    //音の大きさを変える
    gameContext.audio.SetVolume(m_bgmHandle, 0.65f);
}
