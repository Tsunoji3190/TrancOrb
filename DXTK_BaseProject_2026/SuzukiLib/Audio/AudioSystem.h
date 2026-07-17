// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include "AudioChannel.h"
#include "AudioHandle.h"
#include "AudioPlayDesc.h"

namespace SuzukiLib::Audio
{
    /// @brief 基本的なオーディオ再生機能を提供するクラス。
    /// @details ファサードとして、サウンドのロード、再生、停止、一時停止、再開、音量制御などの機能を提供する。
    /// 各機能の実装は、`DirectXTKAudioDevice`、`DirectXTKAudioResourceManager`、`DirectXTKAudioPlayer`
    /// などの内部クラスに委譲される。
    /// RAII によるリソース管理を行い、オーディオデバイスの初期化と終了処理を安全に行う。
    /// pimpl パターンを使用して、実装詳細の隠蔽と利用側の依存ヘッダを最小限に保っている。
    /// 
    /// # 主な機能
    /// - サウンドのロード
    /// - サウンドの再生・停止・一時停止・再開
    /// - ワンショット再生（制御不要な再生）
    /// - マスターボリューム制御
    /// - チャンネル別ボリューム制御
    /// - インスタンス別ボリューム制御
    ///
    /// # 使用例
    /// ```cpp
    /// AudioSystem audio;
    ///
    /// 毎フレーム状態を更新する
    /// audio.Update();
    ///
    /// サウンドをロード
    /// audio.LoadSound("bgm_title", "Resources/Audio/Bgm/Title.wav");
    /// audio.LoadSound("se_jump", "Resources/Audio/Se/Jump.wav");
    ///
    /// サウンドを再生してハンドルを取得
    /// AudioHandle handle = audio.Play("bgm_title");
    ///
    /// ボリュームを調整
    /// audio.SetVolume(handle, 0.8f);
    ///
    /// 一時停止
    /// audio.Pause(handle);
    ///
    /// 再開
    /// audio.Resume(handle);
    ///
    /// 停止
    /// audio.Stop(handle);
    ///
    /// ワンショット再生（SE等に便利）
    /// audio.PlayOneShot("se_jump");
    /// ```
    class AudioSystem final
    {
    public:
        AudioSystem();
        ~AudioSystem();
        AudioSystem(const AudioSystem&) = delete;
        AudioSystem& operator=(const AudioSystem&) = delete;
        AudioSystem(AudioSystem&&) noexcept = default;
        AudioSystem& operator=(AudioSystem&&) noexcept = default;

        /// @brief AudioEngine の状態を更新する。通常、メインループ内で毎フレーム呼び出す必要がある。
        void Update();

        /// @brief サウンドをロードする。
        void LoadSound(const std::string& key, const std::filesystem::path& path);

        /// @brief サウンドを再生し、後から停止・音量変更できるハンドルを返す。
        [[nodiscard]] AudioHandle Play(const std::string& key, const AudioPlayDesc& desc = {});

        /// @brief サウンドを一度だけ再生する。
        ///
        /// ハンドルは返さないため、再生後の停止・一時停止はできない。
        void PlayOneShot(const std::string& key, const AudioPlayDesc& desc = {});

        /// @brief 指定ハンドルの再生を停止する。
        void Stop(AudioHandle handle);

        /// @brief 指定ハンドルの再生を一時停止する。
        void Pause(AudioHandle handle);

        /// @brief 指定ハンドルの一時停止を解除する。
        void Resume(AudioHandle handle);

        /// @brief 全チャンネルへ適用するマスター音量を設定する。
        void SetMasterVolume(float volume);

        /// @brief 現在のマスター音量を返す。
        [[nodiscard]] float GetMasterVolume() const;

        /// @brief 指定チャンネルの音量を設定する。
        void SetChannelVolume(AudioChannel channel, float volume);

        /// @brief 指定チャンネルの音量を返す。
        [[nodiscard]] float GetChannelVolume(AudioChannel channel) const;

        /// @brief 指定ハンドルのインスタンス音量を設定する。
        void SetVolume(AudioHandle handle, float volume);

        /// @brief 指定ハンドルのインスタンス音量を返す。
        [[nodiscard]] float GetVolume(AudioHandle handle) const;

    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
    };
}