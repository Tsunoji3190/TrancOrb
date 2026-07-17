//
// 
// 一人称のカメラ
// 
//
#pragma once

#include"CameraBase.h"

namespace Itsuki
{
    class NormalCamera : public CameraBase
    {
    public:
        NormalCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

        // 更新
        void Update(float elapsedTime);

        // カメラを設定する関数
        void SetCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target);

        // ターゲットを設定する関数
        void SetTarget(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target, float time);

        // 移動完了までのおおよその時間を設定する関数
        void SetMoveTime(float time);

        // 移動終了か調べる関数
        bool IsArrived() const;

    private:



        // ----- 視点 ----- //
        // 目標の位置
        DirectX::SimpleMath::Vector3 m_eyeTarget;
        // 現在の速度
        DirectX::SimpleMath::Vector3 m_eyeVelocity;

        // ----- 注視点 ----- //
        // 目標の位置
        DirectX::SimpleMath::Vector3 m_targetTarget;
        // 現在の速度
        DirectX::SimpleMath::Vector3 m_targetVelocity;

    };

}
