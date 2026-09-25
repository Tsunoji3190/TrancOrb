//--------------------------------------------------------------------------------------
// File: GamePlayScene.h
//
// モデルテスト用シーン
//
// Date: 2026.4.13
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#pragma once

#include "ImaseLib/SceneManager.h"
#include "GameContext.h"
#include "../SceneId.h"

#include "ImaseLib/DebugCamera.h"

#include "Player.h"
#include "ItsukiLib/ColliderRenderer.h"
#include"ItsukiLib/CollisionChecker.h"
#include"ItsukiLib/ColliderFactory.h"

#include"Manager/StageManager.h"
#include"Manager/OrbManager.h"
#include"../../ItsukiLib/SkillTree.h"
#include"BackButton.h"

#include"ItsukiLib/Camera/NormalCamera.h"

#include "Effect3D/UI/BackGround.h"

class GamePlayScene : public Imase::SceneBase<SceneId, GameContext>
{
public:

	// コンストラクタ
    GamePlayScene();

	// 更新
	void Update(Imase::ISceneController<SceneId>& sceneController, GameContext& gameContext) override;

	// 描画
	void Render(GameContext& gameContext) override;

	// シーン切り替え時に呼び出される関数
	void OnEnter(GameContext& gameContext) override;


private:

	// ビュー行列
    DirectX::SimpleMath::Matrix m_view;

	// プロジェクション行列
    DirectX::SimpleMath::Matrix m_projection;

	// バッチ
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
    std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_CprimitiveBatch;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;


	// ベーシックエフェクトへのポインタ
    std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

    // 入力レイアウト
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

    // シェーダーリソースビュー
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_orbTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_stageTexture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_timerTexture;

	// デバッグカメラ
    std::unique_ptr<Imase::DebugCamera> m_debugCamera;

	// プロジェクション行列を作成する関数
    DirectX::SimpleMath::Matrix CreateProjectionMatrix(GameContext& gameContext);

	// ウインドウサイズ変更時に呼び出される関数
    void OnWindowSizeChanged(GameContext& gameContext) override;

	//当った際に当る前の状態に戻す関数（現在は球と立方体の形のみに適応）
    void ResolveCollision(Itsuki::Collider* col1, Itsuki::Collider* col2);

	// モデルハンドル
    std::unique_ptr<DirectX::Model> m_model;


	// 一人称カメラ
    void GamePlayCamera(float elapsedTime); 
	
	// カメラのY軸に対する回転角度（タイトル用）
    float m_titleAngleRad = 0.0f;

	// タイトル用カメラの回転角度（１秒間あたりの回転角度）
    static constexpr float TITLE_CAMERA_MOVE_ANGLE_DEG = 10.0f;

	// モデルハンドル（矢印）
    std::unique_ptr<DirectX::Model> m_arrowModel;

	// ---- オイラー角テスト用 ----- //
    float m_angleRad_X = 0.0f;
    float m_angleRad_Y = 0.0f;
    float m_angleRad_Z = 0.0f;

	// １秒間あたりの回転角度
    static constexpr float ROTATE_ANGLE_DEG = 90.0f;

	// クォータニオン
    DirectX::SimpleMath::Quaternion m_quaternion;

	// モデルハンドル（ターゲット）
    std::unique_ptr<DirectX::Model> m_targetModel;

	// ターゲットの位置
    DirectX::SimpleMath::Vector3 m_targetPosition = { 0.0f, 0.0f, -2.0f };

	//プレイヤーの作成
	std::unique_ptr<Player> m_player;


	//ステージを格納
    std::unique_ptr<StageManager> m_stageManager;

	//オーブを格納
    std::unique_ptr<OrbManager> m_orbManager;

	//当たり判定を描画するもの
	std::unique_ptr<Itsuki::ColliderRenderer> m_renderer;

	//当たり判定同士がぶつかったか判定するもの
	std::unique_ptr<Itsuki::CollisionChecker> m_collisionChecker;

	//一人称のカメラ
    Itsuki::NormalCamera m_camera;

	//BGM
	SuzukiLib::Audio::AudioHandle m_bgmHandle;

	//スキルツリー
    std::unique_ptr<Itsuki::SkillTree> m_skilltree;

	//バックボタン
    Itsuki::BackButton m_backButton;

	// 背景
    std::unique_ptr<Effect3D::BackGround> m_background;
};