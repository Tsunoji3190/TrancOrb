//--------------------------------------------------------------------------------------
// File: BackGround.h
//
// メニュークラス
//
//-------------------------------------------------------------------------------------

#pragma once

#include "Common/StepTimer.h"
#include "Effect3D/UI/UserInterface.h"
#include "Common/DeviceResources.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include "Keyboard.h"
namespace Effect3D
{

	class BackGround
	{
    public:
        BackGround();
        ~BackGround();

        void Initialize(DX::DeviceResources* pDR, int width, int height);
        void Render();

        void Add(const wchar_t* path, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale,
                 Effect3D::ANCHOR anchor);

		//画像の設定
		void SetTexture(const wchar_t* path)
		{
            for (int i = 0; i < m_userInterface.size(); i++)
            {
                m_userInterface[i]->LoadTexture(path);
            }
		}

	public:

		static constexpr int SIZE = 64;

		//	変数
	private:


		DX::DeviceResources* m_pDR;

		std::vector<std::unique_ptr<Effect3D::UserInterface>> m_userInterface;

		std::unique_ptr<Effect3D::UserInterface> m_baseWindow;

		int m_windowWidth, m_windowHeight;

		DirectX::Keyboard::KeyboardStateTracker m_tracker;

		float m_timer;

	};
}