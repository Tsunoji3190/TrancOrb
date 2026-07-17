// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#include "pch.h"
#include "AudioSystem.h"
#include "Internal/DirectXTKAudioDevice.h"
#include "Internal/DirectXTKAudioPlayer.h"
#include "Internal/DirectXTKAudioResourceManager.h"

namespace SuzukiLib::Audio
{
    class AudioSystem::Impl
    {
    public:
        DirectXTKAudioDevice m_device;
        DirectXTKAudioResourceManager m_resources;
        DirectXTKAudioPlayer m_player;

        Impl() : m_resources(m_device.GetNativeEngine()), m_player(m_resources)
        {
        }

        void Shutdown()
        {
            // DirectX::Audio では、SoundEffect より先に AudioEngine を破棄する必要がある。
            m_player.Shutdown();
            m_device.Shutdown();
            m_resources.ReleaseResources();
        }
    };

    AudioSystem::AudioSystem() : m_impl(std::make_unique<Impl>())
    {
    }

    AudioSystem::~AudioSystem()
    {
        m_impl->Shutdown();
    }

    void AudioSystem::Update()
    {
        m_impl->m_device.Update();
        m_impl->m_player.Update();
    }

    void AudioSystem::LoadSound(const std::string& key, const std::filesystem::path& path)
    {
        m_impl->m_resources.LoadSound(key, path);
    }

    AudioHandle AudioSystem::Play(const std::string& key, const AudioPlayDesc& desc)
    {
        return m_impl->m_player.Play(key, desc);
    }

    void AudioSystem::PlayOneShot(const std::string& key, const AudioPlayDesc& desc)
    {
        m_impl->m_player.PlayOneShot(key, desc);
    }

    void AudioSystem::Stop(AudioHandle handle)
    {
        m_impl->m_player.Stop(handle);
    }

    void AudioSystem::Pause(AudioHandle handle)
    {
        m_impl->m_player.Pause(handle);
    }

    void AudioSystem::Resume(AudioHandle handle)
    {
        m_impl->m_player.Resume(handle);
    }

    void AudioSystem::SetMasterVolume(float volume)
    {
        m_impl->m_player.SetMasterVolume(volume);
    }

    float AudioSystem::GetMasterVolume() const
    {
        return m_impl->m_player.GetMasterVolume();
    }

    void AudioSystem::SetChannelVolume(AudioChannel channel, float volume)
    {
        m_impl->m_player.SetChannelVolume(channel, volume);
    }

    float AudioSystem::GetChannelVolume(AudioChannel channel) const
    {
        return m_impl->m_player.GetChannelVolume(channel);
    }

    void AudioSystem::SetVolume(AudioHandle handle, float volume)
    {
        m_impl->m_player.SetVolume(handle, volume);
    }

    float AudioSystem::GetVolume(AudioHandle handle) const
    {
        return m_impl->m_player.GetVolume(handle);
    }
}