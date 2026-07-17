// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#include "pch.h"
#include "DirectXTKAudioResourceManager.h"

namespace SuzukiLib::Audio
{
    void DirectXTKAudioResourceManager::LoadSound(const std::string& key, const std::filesystem::path& path)
    {
        // ロード済み key は再読み込みせず、同じリソースを参照し続ける。
        if (m_soundEffects.contains(key))
            return;

        try
        {
            auto soundEffect = std::make_unique<DirectX::SoundEffect>(&m_engine, path.c_str());
            m_soundEffects.emplace(key, std::move(soundEffect));
        }
        catch (const std::exception& e)
        {
            // DirectXTK の例外だけではアセット特定が難しいため、key と path を付け直して上位へ投げる。
            throw std::runtime_error("Failed to load sound.\n"
                                     "key: " +
                                     key +
                                     "\n"
                                     "path: " +
                                     path.string() +
                                     "\n"
                                     "reason: " +
                                     e.what());
        }
    }

    DirectX::SoundEffect* DirectXTKAudioResourceManager::FindSoundEffect(const std::string& key) const
    {
        if (const auto it = m_soundEffects.find(key); it != m_soundEffects.end())
            return it->second.get();
        return nullptr;
    }

    void DirectXTKAudioResourceManager::ReleaseResources()
    {
        m_soundEffects.clear();
    }
}
