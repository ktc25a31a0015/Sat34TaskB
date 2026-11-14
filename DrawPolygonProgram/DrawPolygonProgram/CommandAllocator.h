#pragma once

#include "Device.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class CommandAllocator final {
public:
	CommandAllocator() = default;
	~CommandAllocator() = default;

private:
	ComPtr<ID3D12CommandAllocator> _commandAllocator{};
	D3D12_COMMAND_LIST_TYPE _type{};

public:
	/**
	 * @brief コマンドアロケータ作成
	 * @param device デバイスクラスのインスタンス
	 * @param type   コマンドリストのタイプ
	 */
	bool create(const Device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept;

	void reset() noexcept;

	ComPtr<ID3D12CommandAllocator> get() const noexcept;
	D3D12_COMMAND_LIST_TYPE getType() const noexcept;
};