// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: Copyright (c) 2026 トライデントコンピュータ専門学校
#pragma once
#include <memory>
#include <Audio.h>

namespace SuzukiLib::Audio
{
    /// @brief DirectXTK AudioEngine の生成、更新、破棄を管理するデバイスラッパー。
    ///
    /// # 概要
    /// DirectX::AudioEngine のライフサイクルを管理し、初期化・終了処理を提供する。
    class DirectXTKAudioDevice final
    {
    public:
        /// @brief DirectX::AudioEngine を生成し、初期化する。
        /// @details DirectX::AudioEngine を内部に生成し、オーディオデバイスとの接続を確立する。
        /// @throws std::exception DirectXTK 側の初期化に失敗した場合（オーディオデバイスが利用できない等）
        DirectXTKAudioDevice();

        /// @brief AudioEngine をサスペンドして破棄する。
        /// @details 音声エンジンを安全に停止し、オーディオリソースを解放する。
        /// 未初期化状態で呼び出しても安全に動作する。
        /// この関数呼び出し後、GetNativeEngine は Initialize が再度呼ばれるまで失敗する。
        void Shutdown();

        /// @brief AudioEngine の状態を更新する。通常、メインループ内で毎フレーム呼び出す必要がある。
        /// @details DirectX::AudioEngine::Update を呼び出し、内部状態を更新する。
        /// @pre Initialize が正常に完了していること
        void Update();

        /// @brief 管理中の AudioEngine への参照を返す。
        /// @return DirectX::AudioEngine& 管理中のオーディオエンジンへの参照
        /// @throws std::logic_error エンジンが未初期化の場合
        [[nodiscard]] DirectX::AudioEngine& GetNativeEngine();

        /// @brief 管理中の AudioEngine への読み取り専用参照を返す。
        /// @return const DirectX::AudioEngine& 管理中のオーディオエンジンへの const 参照
        /// @throws std::logic_error エンジンが未初期化の場合
        [[nodiscard]] const DirectX::AudioEngine& GetNativeEngine() const;

    private:
        /// @brief DirectX::AudioEngine の実体。
        std::unique_ptr<DirectX::AudioEngine> m_engine{};
    };
}
