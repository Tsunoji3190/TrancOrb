#include "pch.h"
#include "NormalCamera.h"

Itsuki::NormalCamera::NormalCamera(
    const DirectX::SimpleMath::Vector3& eye,
    const DirectX::SimpleMath::Vector3& target)
    : CameraBase(eye,target)
{
}

void Itsuki::NormalCamera::Update(float elapsedTime)
{


    // 視点の計算
    SimpleMath::Vector3 eye = GetEyePosition();

    SetEyePosition(eye);

    // 注視点の計算
    SimpleMath::Vector3 target = GetTargetPosition();

    SetTargetPosition(target);
}

// カメラを設定する関数
void Itsuki::NormalCamera::SetCamera(const DirectX::SimpleMath::Vector3& eye,
                                          const DirectX::SimpleMath::Vector3& target)
{
    // 視点を設定
    SetEyePosition(eye);

    // 注視点を設定
    SetTargetPosition(target);
}