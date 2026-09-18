//--------------------------------------------------------------------------------------
// File: Title.h
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

	class Title
	{
	public:

		//	変数
	private:


		unsigned int m_titleIndex;
		DX::DeviceResources* m_pDR;

		std::vector<std::unique_ptr<Effect3D::UserInterface>> m_userInterface;

		std::unique_ptr<Effect3D::UserInterface> m_baseWindow;

		int m_windowWidth, m_windowHeight;

		DirectX::Keyboard::KeyboardStateTracker m_tracker;

		float m_cos;

		float m_timer;
		//	関数
	public:
		Title();
		~Title();


		void Initialize(DX::DeviceResources* pDR, int width, int height);
		void Update(float elapsedtime);
		void Render();


		void Add(const wchar_t* path
			, DirectX::SimpleMath::Vector2 position
			, DirectX::SimpleMath::Vector2 scale
			, Effect3D::ANCHOR anchor);


		int GetTitleState()
		{
			return m_titleIndex;
		}

		void SetTitleState(int titleIndex)
		{
			m_titleIndex = titleIndex;
		}

		//	弾の状態
		enum STATE
		{
			NONE = -1,
			GAUGE,
			MAGAZINE,
		};

	};
}