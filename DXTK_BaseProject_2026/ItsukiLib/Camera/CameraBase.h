#pragma once

#include"SimpleMath.h"

namespace Itsuki
{
    class CameraBase
    {
    public:

        //コンストラクタ
        CameraBase(const DirectX::SimpleMath::Vector3& eye, const DirectX::SimpleMath::Vector3& target)
            : m_eye(eye),
            m_target(target)
        {
            UpdateViewMatrix();
        }

        //デストラクタ
        virtual ~CameraBase() = default;

        // 視点を取得する関数
        DirectX::SimpleMath::Vector3 GetEyePosition() const
        {
            return m_eye;
        }

        // 注視点を取得する関数
        DirectX::SimpleMath::Vector3 GetTargetPosition() const
        {
            return m_target;
        }

        // ビュー行列を取得する関数
        DirectX::SimpleMath::Matrix GetViewMatrix() const
        {
            return m_view;
        }

    protected:

        // 視点の設定関数
        void SetEyePosition(DirectX::SimpleMath::Vector3 eye)
        {
            m_eye = eye;

            //設定したらすかさず更新
            UpdateViewMatrix();
        }

        // 注視点の設定関数
        void SetTargetPosition(DirectX::SimpleMath::Vector3 target)
        {
            m_target = target;

            // 設定したらすかさず更新
            UpdateViewMatrix();
        }

            
        // カメラの上方向を取得し、回転や移動を正常に行わせる関数
        virtual DirectX::SimpleMath::Vector3 GetUp() const
        {
            return DirectX::SimpleMath::Vector3::Up;
        }


    private:

        // 横回転
        float m_yAngle, m_yTmp;

        // 縦回転
        float m_xAngle, m_xTmp;

		// 視点
        DirectX::SimpleMath::Vector3 m_eye;

        // 注視点
        DirectX::SimpleMath::Vector3 m_target;

        // ビュー行列
        DirectX::SimpleMath::Matrix m_view;


        // ビュー行列を更新する関数（今見ている方向を常に更新）
        void UpdateViewMatrix()
        {
            m_view = DirectX::SimpleMath::Matrix::CreateLookAt(m_eye, m_target, GetUp());
        }

    };

    class Camera2DBase
    {
    public:
        // コンストラクタ
        Camera2DBase(const DirectX::SimpleMath::Vector2 pos) : m_position{pos}
        {
            UpdateTransMatrix();
        }

        // デストラクタ
        virtual ~Camera2DBase() = default;

        // 位置を取得する関数
        DirectX::SimpleMath::Vector2 GetPosition() const
        {
            return m_position;
        }

        // 平行移動行列を取得する関数
        DirectX::SimpleMath::Matrix GetTransMatrix() const
        {
            return m_transMat;
        }

    protected:
        // 視点の設定関数
        void SetPosition(DirectX::SimpleMath::Vector2 pos)
        {
            m_position = pos;

            // 設定したらすかさず更新
            void UpdateTransMatrix();
        }

    private:
        // 横回転
        float m_yAngle, m_yTmp;

        // 縦回転
        float m_xAngle, m_xTmp;

        // 位置
        DirectX::SimpleMath::Vector2 m_position;

        // 平行移動マトリクス
        DirectX::SimpleMath::Matrix m_transMat;

        // 平行移動行列を更新する関数（今見ている位置を常に更新）
        void UpdateTransMatrix()
        {
            m_transMat = DirectX::SimpleMath::Matrix::CreateTranslation(m_position.x, m_position.y, 0.0);
        }

    };


}