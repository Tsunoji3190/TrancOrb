// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <memory>
#include <string>
#include <vector>
#include <Audio.h>
#include "SuzukiLib/Audio/AudioChannel.h"
#include "SuzukiLib/Audio/AudioHandle.h"
#include "SuzukiLib/Audio/AudioPlayDesc.h"
#include "SuzukiLib/Audio/Internal/AudioVolumeMixer.h"
#include "SuzukiLib/Audio/Internal/DirectXTKAudioResourceManager.h"

namespace SuzukiLib::Audio
{
    /// @brief DirectXTK SoundEffect を使ってサウンド再生と音量制御を行う。
    class DirectXTKAudioPlayer final
    {
    public:
        /// @brief サウンドリソース提供元を参照してプレイヤーを初期化し、内部再生スロットを確保する。
        /// @param resources DirectXTKAudioResourceManager への参照。プレイヤーの生存期間中有効である必要がある。
        explicit DirectXTKAudioPlayer(DirectXTKAudioResourceManager& resources);

        /// @brief 再生中のインスタンスを停止し、内部スロットを解放する。
        /// @details 呼び出し後の Play などは再初期化手段がないため使用しないこと。
        void Shutdown() noexcept;

        /// @brief 再生が終了したインスタンスを回収する。
        /// @details フレームごとに呼び出すことを想定し、内部スロットの active 状態を変更する。
        void Update();

        /// @brief 指定 key のサウンドを再生し、制御用ハンドルを返す。
        /// @param key 再生するサウンドのキー。DirectXTKAudioResourceManager でロード済みである必要がある。
        /// @param desc 再生時の音量、ピッチ、パン、ループ設定を指定する構造体。
        /// @return 再生中のインスタンスを参照する AudioHandle。スロットが空いていない場合は無効ハンドルを返す。
        /// @throws std::runtime_error 指定 key がロード済みでない場合
        [[nodiscard]] AudioHandle Play(const std::string& key, const AudioPlayDesc& desc);

        /// @brief 指定 key のサウンドを一度だけ再生する。
        /// @param key 再生するサウンドのキー。DirectXTKAudioResourceManager でロード済みである必要がある。
        /// @param desc 再生時の音量、ピッチ、パン、ループ設定を指定する構造体。
        /// @throws std::runtime_error 指定 key がロード済みでない場合
        void PlayOneShot(const std::string& key, const AudioPlayDesc& desc);

        /// @brief 指定ハンドルの再生を停止してスロットを解放する。無効または期限切れハンドルは無視する。
        /// @param handle 停止する再生インスタンスのハンドル
        void Stop(AudioHandle handle);

        /// @brief 指定ハンドルの再生を一時停止する。無効または期限切れハンドルは無視する。
        /// @param handle 一時停止する再生インスタンスのハンドル
        void Pause(AudioHandle handle);

        /// @brief 指定ハンドルの再生を再開する。無効または期限切れハンドルは無視する。
        /// @param handle 再開する再生インスタンスのハンドル
        void Resume(AudioHandle handle);

        /// @brief マスター音量を設定し、再生中インスタンスの DirectXTK 音量にも反映する。volume は 0.0 から 1.0
        /// にクランプされる。
        /// @param volume 設定するマスター音量（0.0 ～ 1.0）
        void SetMasterVolume(float volume);

        /// @brief 現在のマスター音量を返す。
        /// @return 現在のマスター音量（0.0 ～ 1.0）
        [[nodiscard]] float GetMasterVolume() const;

        /// @brief チャンネル音量を設定し、該当チャンネルで再生中の DirectXTK インスタンスへ反映する。volume は 0.0
        /// から 1.0 にクランプされる。
        /// @param channel 設定するチャンネル
        /// @param volume 設定するチャンネル音量（0.0 ～ 1.0）
        /// @throws std::out_of_range 指定チャンネルが未登録の場合
        void SetChannelVolume(AudioChannel channel, float volume);

        /// @brief 指定チャンネルの音量を返す。
        /// @param channel 取得するチャンネル
        /// @return 指定チャンネルの音量（0.0 ～ 1.0）
        /// @throws std::out_of_range 指定チャンネルが未登録の場合
        [[nodiscard]] float GetChannelVolume(AudioChannel channel) const;

        /// @brief 指定ハンドルのインスタンス音量を設定し、DirectXTK インスタンスへ反映する。
        /// @brief volume は 0.0 から 1.0 に丸められ、無効または期限切れハンドルは無視する。
        /// @param handle 設定する再生インスタンスのハンドル
        /// @param volume 設定するインスタンス音量（0.0 ～ 1.0）
        void SetVolume(AudioHandle handle, float volume);

        /// @brief 指定ハンドルのインスタンス音量を返す。無効または期限切れハンドルは 0.0 を返す。
        /// @param handle 取得する再生インスタンスのハンドル
        /// @return 指定ハンドルのインスタンス音量（0.0 ～ 1.0）
        [[nodiscard]] float GetVolume(AudioHandle handle) const;

    private:
        /// @brief 再生中インスタンスの情報を保持する内部スロット構造体。
        struct InstanceSlot
        {
            uint32_t generation{};
            std::unique_ptr<DirectX::SoundEffectInstance> instance{};
            std::string soundKey{};
            AudioChannel channel{};
            float instanceVolume{1.0f};
            bool active{};
        };

        InstanceSlot* TryGetSlot(AudioHandle handle) noexcept;
        const InstanceSlot* TryGetSlot(AudioHandle handle) const noexcept;
        void ApplyEffectiveVolume(InstanceSlot& slot) noexcept;

        DirectXTKAudioResourceManager& m_resources;
        std::vector<InstanceSlot> m_instanceSlots{};
        AudioVolumeMixer m_volumeMixer{};
    };
}
