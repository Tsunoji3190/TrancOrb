#pragma once
#include <pch.h>

namespace Itsuki
{

	class FadeBase
    {
    public:


        ~FadeBase()=default;

        //初期化
        virtual void Initialize() = 0;

        //更新
        virtual void Update() = 0;

        //描画
        virtual void Render() = 0;

        void SetPosition(DirectX::SimpleMath::Vector2 position)
        {
            m_position = position;
        }

        DirectX::SimpleMath::Vector2 GetPosition()
        {
            return m_position;
        }

        //動いているか止まっているかを設定
        void SetMove(bool move)
        {
            IsMove = move;
        }

    protected:
        //フェードの位置
        DirectX::SimpleMath::Vector2 m_position;

        //フェードのテクスチャ
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

        bool IsMove;

    };

}