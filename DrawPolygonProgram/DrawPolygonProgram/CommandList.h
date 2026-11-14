#pragma once

#include "Device.h"
#include "CommandAllocator.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class CommandList final {
public:
	CommandList() = default;
	~CommandList() = default;

private:
	ComPtr<ID3D12GraphicsCommandList> _commandList{};

public:
	/**
	 * @brief コマンドリスト作成
	 * @param device デバイスクラスのインスタンス
	 * @param commandAllocator コマンドアロケータクラスのインスタンス
	 * @return 生成の可否
	 */
	bool create(const Device& device, const CommandAllocator& commandAllocator) noexcept;

	void reset(const CommandAllocator& commandAllocator) noexcept;

	ComPtr<ID3D12GraphicsCommandList> get() const noexcept;
};