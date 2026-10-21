//
// 
// -----ムービーの移動を行うカメラ-----
// 
//
#pragma once
#include "CameraBase.h"
#include <SimpleMath.h>

namespace Itsuki
{
    class MovieCamera : public CameraBase
    {
        //コンストラクタ
        MovieCamera(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target)
            : CameraBase(eye,target)
        {
        }

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
    };

}
