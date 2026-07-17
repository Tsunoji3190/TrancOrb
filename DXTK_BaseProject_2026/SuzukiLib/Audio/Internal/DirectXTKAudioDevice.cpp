// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#include "pch.h"
#include "DirectXTKAudioDevice.h"
#include <stdexcept>

namespace SuzukiLib::Audio
{

    DirectXTKAudioDevice::DirectXTKAudioDevice()
    {
        DirectX::AUDIO_ENGINE_FLAGS flags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
        flags |= DirectX::AudioEngine_Debug;
#endif
        m_engine = std::make_unique<DirectX::AudioEngine>(flags);
    }

    void DirectXTKAudioDevice::Shutdown()
    {
        if (!m_engine)
            return;

        m_engine->Suspend();
        m_engine.reset();
    }

    void DirectXTKAudioDevice::Update()
    {
        if (!m_engine->Update())
        {
            // AudioEngine::Update が false の場合でも、デバイス一時喪失なら DirectXTK が自動復旧を試みる。
            // 復旧不能な Critical Error だけデバッグ出力へ記録し、次フレームで Update を継続する。
            if (m_engine->IsCriticalError())
                OutputDebugStringA("[Audio] Critical error: audio engine failed and cannot recover.\n");
        }
    }

    DirectX::AudioEngine& DirectXTKAudioDevice::GetNativeEngine()
    {
        if (!m_engine)
            throw std::logic_error("Audio device is not initialized.");
        return *m_engine;
    }

    const DirectX::AudioEngine& DirectXTKAudioDevice::GetNativeEngine() const
    {
        if (!m_engine)
            throw std::logic_error("Audio device is not initialized.");
        return *m_engine;
    }
}
