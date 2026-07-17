// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <Audio.h>

namespace SuzukiLib::Audio
{
    /// @brief DirectXTK SoundEffect を key で管理するオーディオリソースマネージャー。
    /// 
    /// # 概要
    /// `DirectXTKAudioResourceManager` は、DirectX Tool Kit の SoundEffect リソースを文字列キーで管理し、
    /// ロードおよび取得機能を提供する。
    class DirectXTKAudioResourceManager final
    {
    public:
        /// @brief AudioEngine を参照してマネージャーを初期化する。
        /// @param engine DirectXTK の AudioEngine への参照。この参照はマネージャーの生存期間中有効である必要がある。
        explicit DirectXTKAudioResourceManager(DirectX::AudioEngine& engine) : m_engine(engine)
        {
        }

        /// @brief オーディオファイルを読み込む。
        /// @param key オーディオリソースを識別するための一意の文字列キー
        /// @param path 読み込むオーディオファイルのファイルシステムパス
        /// @throws std::runtime_error 読み込みに失敗した場合、key と path を含むエラーメッセージと共に送出される。
        /// @note key が既に存在する場合は何もしない。
        /// @note 成功時、オーディオリソースは内部で unique_ptr として保持され、マネージャーの破棄時に自動的に解放される。
        void LoadSound(const std::string& key, const std::filesystem::path& path);

        /// @brief key に対応する SoundEffect を返す。
        /// @param key 検索するオーディオリソースのキー
        /// @return 見つかった場合は SoundEffect へのポインタ、見つからない場合は nullptr
        [[nodiscard]] DirectX::SoundEffect* FindSoundEffect(const std::string& key) const;

        /// @brief 管理中のすべての SoundEffect を解放する。
        void ReleaseResources();

    private:
        /// @brief SoundEffect の作成に使用する AudioEngine への参照。
        DirectX::AudioEngine& m_engine;
        
        /// @brief key と SoundEffect の対応を管理するマップ。
        std::unordered_map<std::string, std::unique_ptr<DirectX::SoundEffect>> m_soundEffects{};
    };
}