#pragma once
#include"Obj.h"

#include"ItsukiLib/SphereCollider.h"
#include "GameContext.h"


class Player :public Obj
{

public:

	Player(const GameContext& gameContext, const DirectX::SimpleMath::Matrix& view,
           const DirectX::SimpleMath::Matrix& projection, DirectX::Model* pModel,
           std::unique_ptr<Itsuki::Collider> collider);

	~Player();

    void Initialze();

	void Update(float elapsedTime) override;

	void Render();

	// 位置を取得する関数
    DirectX::SimpleMath::Vector3 GetPosition() const
    {
        return m_position;
    }

    //プレイヤーと当たり判定の位置を設定する
    void SetPosition(DirectX::SimpleMath::Vector3 position)
    {
        //位置を変える
        m_position = position;
    }

    // 速度を取得する関数
    DirectX::SimpleMath::Vector3 GetVelocity() const
    {
        return m_velocity;
    }

    //速度を設定する関数
    void SetVelocityY(float velocity) 
    {
        m_velocity.y = velocity;
    }

    // 向いている角度を取得する関数
    DirectX::SimpleMath::Vector3 GetFacingAngleRad() const
    {
        return m_facingAngleRad;
    }

    void ResetAngle()
    {
        m_facingAngleRad = DirectX::SimpleMath::Vector3::Zero;
    }

    void SetIsGround(bool isGround)
    {
        m_isGround = isGround;
    }

    bool GetIsGround()
    {
        return m_isGround;
    }

    void SetSpeed(float speed)
    {
        m_speed += speed;
    }

    float GetSpeed()
    {
        return m_speed;
    }

    void SetJump(float jump)
    {
        m_jump += jump;
    }

    float GetJump()
    {
        return m_jump;
    }


private:

    //X軸回転の最大までむける角度
    static constexpr float MAX_ROTATE_X = 85.0f;

    // 回転の速さ（１秒間あたりの角度）
    static constexpr float ROTATE_SPEED_DEG = 100.0f;

    // 移動の速さ（１秒間あたりの移動量）
    static constexpr float MOVE_SPEED = 2.0f;
    
    //ジャンプの加速度
    static constexpr float JUMP_SPEED = 0.1f;

private:


    // ゲームコンテキスト
    const GameContext& m_gameContext;
    
    // ビュー行列
    const DirectX::SimpleMath::Matrix& m_view;

    // プロジェクション行列
    const DirectX::SimpleMath::Matrix& m_projection;

    // モデルへのポインタ
    DirectX::Model* m_pModel = nullptr;

    //位置
    DirectX::SimpleMath::Vector3 m_position;

    //速度
    DirectX::SimpleMath::Vector3 m_velocity;

    //移動の速さ
    float m_speed;

    //ジャンプ力の高さ
    float m_jump;

    // それぞれ向いている角度（ラジアン）
    DirectX::SimpleMath::Vector3 m_facingAngleRad;

    // 向きを作成
    SimpleMath::Vector3 m_direction = SimpleMath::Vector3::Zero;


    //設置しているかどうか判定
    bool m_isGround;

};

