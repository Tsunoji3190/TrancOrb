// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <unordered_map>
#include "SuzukiLib/Audio/AudioChannel.h"

namespace SuzukiLib::Audio
{
    /// @brief マスター音量、チャンネル音量、インスタンス音量を合成する音量ミキサー。
    /// 
    /// # 概要
    /// `AudioVolumeMixer` は、オーディオシステムにおける階層的な音量制御を提供する。
    /// マスター音量、チャンネル別音量（BGM、SE など）、個別インスタンス音量の3層構造で音量を管理する。
    /// 
    /// # 音量計算
    /// 最終的な実効音量は以下の式で計算される：
    /// ```
    /// 実効音量 = マスター音量 * チャンネル音量 * インスタンス音量
    /// ```
    /// 
    /// # 使用例
    /// ```cpp
    /// AudioVolumeMixer mixer;
    /// mixer.SetMasterVolume(0.8f); // マスター音量を80%に設定
    /// mixer.SetChannelVolume(AudioChannel::Bgm, 0.5f); // BGM音量を50%に設定
    /// float volume = mixer.CalculateEffectiveVolume(AudioChannel::Bgm, 0.7f); // 実効音量 = 0.8 * 0.5 * 0.7 = 0.28
    /// ```
    /// 
    /// # 音量範囲
    /// すべての音量値は自動的に 0.0 ～ 1.0 の範囲にクランプされる。
    class AudioVolumeMixer final
    {
    public:
        /// @brief BGM / SE のチャンネル音量を 1.0、マスター音量を 1.0 として初期化する。
        /// @details デフォルトでは、すべてのチャンネルが最大音量（1.0）で初期化される。
        AudioVolumeMixer()
        {
            m_channelVolumeMap[AudioChannel::Bgm] = 1.0f;
            m_channelVolumeMap[AudioChannel::Se] = 1.0f;
        }

        /// @brief すべてのチャンネルに共通して適用されるマスター音量を設定する。volume は 0.0 から 1.0 にクランプされる。
        /// @param volume 設定する音量値
        void SetMasterVolume(float volume);

        /// @brief 現在のマスター音量を返す。
        /// @return 現在のマスター音量（0.0 ～ 1.0）
        [[nodiscard]] float GetMasterVolume() const;

        /// @brief 特定のチャンネルに適用される音量を設定する。volume は 0.0 から 1.0 にクランプされる。
        /// @param channel 音量を設定するチャンネル
        /// @param volume 設定する音量値
        void SetChannelVolume(AudioChannel channel, float volume);

        /// @brief 指定チャンネルの音量を返す。
        /// @param channel 音量を取得するチャンネル
        /// @return 指定されたチャンネルの音量（0.0 ～ 1.0）
        /// @throws std::out_of_range 指定されたチャンネルが未登録の場合
        [[nodiscard]] float GetChannelVolume(AudioChannel channel) const;

        /// @brief 指定チャンネル音量と instanceVolume から最終的な再生音量を計算する。
        /// @param channel 計算対象のチャンネル
        /// @param instanceVolume 個別インスタンスの音量（0.0 ～ 1.0 を期待）
        /// @return 実効音量（マスター音量 × チャンネル音量 × インスタンス音量）
        /// @throws std::out_of_range 指定されたチャンネルが未登録の場合
        [[nodiscard]] float CalculateEffectiveVolume(AudioChannel channel, float instanceVolume) const;

    private:
        /// @brief 音量を 0.0 ～ 1.0 の範囲にクランプする。
        /// @details 0.0 未満の値は 0.0 に、1.0 超過の値は 1.0 にクランプされる。
        /// @param volume クランプする音量値
        /// @return クランプされた音量値（0.0 ～ 1.0）
        [[nodiscard]] static float ClampVolume(float volume);

        /// @brief マスター音量（0.0 ～ 1.0）
        float m_masterVolume = 1.0f;
        
        /// @brief チャンネル別の音量マップ（キー: AudioChannel、値: 音量 0.0 ～ 1.0）
        std::unordered_map<AudioChannel, float> m_channelVolumeMap{};
    };
}
