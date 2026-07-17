// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#include "pch.h"
#include "DirectXTKAudioPlayer.h"
#include <algorithm>
#include <stdexcept>

namespace SuzukiLib::Audio
{
    DirectXTKAudioPlayer::DirectXTKAudioPlayer(DirectXTKAudioResourceManager& resources) : m_resources(resources)
    {
        // SoundEffectInstance の同時再生数を固定スロットで管理し、AudioHandle から O(1) で参照できるようにする。
        m_instanceSlots.resize(32);
    }

    void DirectXTKAudioPlayer::Shutdown() noexcept
    {
        for (auto& slot : m_instanceSlots)
        {
            if (slot.instance)
            {
                slot.instance->Stop(true);
                slot.instance.reset();
            }
        }

        m_instanceSlots.clear();
    }

    void DirectXTKAudioPlayer::Update()
    {
        for (auto& slot : m_instanceSlots)
        {
            // DirectXTK 側で自然停止したインスタンスを回収し、次の Play でスロットを再利用できるようにする。
            if (slot.active && slot.instance)
            {
                if (slot.instance->GetState() == DirectX::SoundState::STOPPED)
                {
                    slot.instance.reset();
                    slot.active = false;
                }
            }
        }
    }

    AudioHandle DirectXTKAudioPlayer::Play(const std::string& key, const AudioPlayDesc& desc)
    {
        auto soundEffect = m_resources.FindSoundEffect(key);
        if (!soundEffect)
            throw std::runtime_error("Sound key not found: " + key);

        // ハンドルは index + generationで構成し、
        // スロット再利用後に古いハンドルが誤って新しい音を操作しないようにする。
        if (auto freeSlot =
                std::ranges::find_if(m_instanceSlots, [](const InstanceSlot& slot) { return !slot.active; });
            freeSlot != m_instanceSlots.end())
        {
            auto instance = soundEffect->CreateInstance();

            if (!instance)
                throw std::runtime_error("Failed to create sound effect instance: " + key);

            const auto effectiveVolume =
                m_volumeMixer.CalculateEffectiveVolume(desc.channel, std::clamp(desc.volume, 0.0f, 1.0f));
            instance->SetVolume(effectiveVolume);
            instance->SetPitch(desc.pitch);
            instance->SetPan(desc.pan);
            instance->Play(desc.loop);
            freeSlot->instance = std::move(instance);
            freeSlot->active = true;
            if (++freeSlot->generation == 0)
                freeSlot->generation = 1;
            freeSlot->soundKey = key;
            freeSlot->channel = desc.channel;
            freeSlot->instanceVolume = std::clamp(desc.volume, 0.0f, 1.0f);

            return AudioHandle{.index = static_cast<uint32_t>(std::distance(m_instanceSlots.begin(), freeSlot)),
                               .generation = freeSlot->generation};
        }

        return {};
    }

    void DirectXTKAudioPlayer::PlayOneShot(const std::string& key, const AudioPlayDesc& desc)
    {
        auto soundEffect = m_resources.FindSoundEffect(key);
        if (!soundEffect)
            throw std::runtime_error("Sound key not found: " + key);

        const auto effectiveVolume =
            m_volumeMixer.CalculateEffectiveVolume(desc.channel, std::clamp(desc.volume, 0.0f, 1.0f));
        soundEffect->Play(effectiveVolume, desc.pitch, desc.pan);
    }

    void DirectXTKAudioPlayer::Stop(AudioHandle handle)
    {
        if (auto slot = TryGetSlot(handle))
        {
            if (slot->instance)
                slot->instance->Stop(true);
            slot->instance.reset();
            slot->active = false;
        }
    }

    void DirectXTKAudioPlayer::Pause(AudioHandle handle)
    {
        if (auto slot = TryGetSlot(handle))
        {
            if (slot->instance)
                slot->instance->Pause();
        }
    }

    void DirectXTKAudioPlayer::Resume(AudioHandle handle)
    {
        if (auto slot = TryGetSlot(handle))
        {
            if (slot->instance)
                slot->instance->Resume();
        }
    }

    void DirectXTKAudioPlayer::SetMasterVolume(float volume)
    {
        m_volumeMixer.SetMasterVolume(volume);

        // ミキサーは数値を保持するだけなので、再生中の DirectXTK インスタンスへ実効音量を再適用する。
        for (auto& slot : m_instanceSlots)
            ApplyEffectiveVolume(slot);
    }

    float DirectXTKAudioPlayer::GetMasterVolume() const
    {
        return m_volumeMixer.GetMasterVolume();
    }

    void DirectXTKAudioPlayer::SetChannelVolume(AudioChannel channel, float volume)
    {
        m_volumeMixer.SetChannelVolume(channel, volume);

        // 対象チャンネルだけ更新し、他チャンネルのインスタンスには不要な SetVolume を発行しない。
        for (auto& slot : m_instanceSlots)
        {
            if (slot.channel == channel)
                ApplyEffectiveVolume(slot);
        }
    }

    float DirectXTKAudioPlayer::GetChannelVolume(AudioChannel channel) const
    {
        return m_volumeMixer.GetChannelVolume(channel);
    }

    void DirectXTKAudioPlayer::SetVolume(AudioHandle handle, float volume)
    {
        if (auto slot = TryGetSlot(handle))
        {
            if (slot->instance)
            {
                slot->instanceVolume = std::clamp(volume, 0.0f, 1.0f);

                const auto effectiveVolume =
                    m_volumeMixer.CalculateEffectiveVolume(slot->channel, slot->instanceVolume);
                slot->instance->SetVolume(effectiveVolume);
            }
        }
    }

    float DirectXTKAudioPlayer::GetVolume(AudioHandle handle) const
    {
        if (auto slot = TryGetSlot(handle))
            return slot->instanceVolume;

        return 0.0f;
    }

    const DirectXTKAudioPlayer::InstanceSlot* DirectXTKAudioPlayer::TryGetSlot(AudioHandle handle) const noexcept
    {
        // generation を照合して、停止後に同じ index が再利用された場合の stale handle を弾く。
        if (!handle.IsValid())
            return nullptr;

        if (handle.index >= m_instanceSlots.size())
            return nullptr;

        const auto& slot = m_instanceSlots[handle.index];
        if (!slot.active)
            return nullptr;

        if (slot.generation != handle.generation)
            return nullptr;

        return &slot;
    }

    DirectXTKAudioPlayer::InstanceSlot* DirectXTKAudioPlayer::TryGetSlot(AudioHandle handle) noexcept
    {
        return const_cast<InstanceSlot*>(std::as_const(*this).TryGetSlot(handle));
    }

    void DirectXTKAudioPlayer::ApplyEffectiveVolume(InstanceSlot& slot) noexcept
    {
        if (!slot.active || !slot.instance)
            return;
        slot.instance->SetVolume(m_volumeMixer.CalculateEffectiveVolume(slot.channel, slot.instanceVolume));
    }
}
