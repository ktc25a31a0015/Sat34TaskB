#pragma once

#include "DXGI.h"
#include <d3d12.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Device final {
public:
	Device() = default;
	~Device() = default;

private:
	ComPtr<ID3D12Device> _device;

public:
	/**
	 * @brief デバイスを作成する
	 * @param dxgi DXGIクラスのインスタンス
	 * @return 作成できた場合は true
	 */
	bool create(const DXGI& dxgi) noexcept;

	/**
	 * @brief デバイスを取得する
	 * @return dx12 デバイスのComPtr
	 */
	ComPtr<ID3D12Device> get() const noexcept;
};