// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#include "pch.h"
#include "AudioVolumeMixer.h"
#include <algorithm>
#include <stdexcept>

namespace SuzukiLib::Audio
{
    void AudioVolumeMixer::SetMasterVolume(float volume)
    {
        m_masterVolume = ClampVolume(volume);
    }

    float AudioVolumeMixer::GetMasterVolume() const
    {
        return m_masterVolume;
    }

    void AudioVolumeMixer::SetChannelVolume(AudioChannel channel, float volume)
    {
        m_channelVolumeMap[channel] = ClampVolume(volume);
    }

    float AudioVolumeMixer::GetChannelVolume(AudioChannel channel) const
    {
        return m_channelVolumeMap.at(channel);
    }

    float AudioVolumeMixer::CalculateEffectiveVolume(AudioChannel channel, float instanceVolume) const
    {
        return m_masterVolume * m_channelVolumeMap.at(channel) * instanceVolume;
    }

    float AudioVolumeMixer::ClampVolume(float volume)
    {
        return std::clamp(volume, 0.0f, 1.0f);
    }
}
