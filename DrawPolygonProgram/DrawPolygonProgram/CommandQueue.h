#pragma once

#include "Device.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class CommandQueue final {
public:
	CommandQueue() = default;
	~CommandQueue() = default;

private:
	ComPtr<ID3D12CommandQueue> _commandQueue{};

public:
	/**
	 * @brief コマンドキューの作成
	 * @param device デバイスクラスのインスタンス
	 * @return 生成の可否
	 */
	bool create(const Device& device) noexcept;

	ComPtr<ID3D12CommandQueue> get() const noexcept;
};