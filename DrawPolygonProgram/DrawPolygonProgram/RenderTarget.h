#pragma once

#include "Device.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"

#include <vector>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class RenderTarget final {
public:
	RenderTarget() = default;
	~RenderTarget() = default;

private:
	std::vector<ComPtr<ID3D12Resource>> _renderTargets;

public:
	/**
	 * @brief バックバッファを作成する
	 * @param device デバイスクラスのインスタンス
	 * @param swapChain スワップチェインのComPtr
	 * @param heap ディスクリプタヒープのインスタンス
	 * @return 生成の可否
	 */
	bool createBackBuffer(const Device& device, const SwapChain& swapChain, const DescriptorHeap& heap) noexcept;

	/**
	 * @brief ビュー（ディスクリプタハンドル）を取得する
	 * @param device デバイスクラスのインスタンス
	 * @param heap ディスクリプタヒープのインスタンス
	 * @param index インデックス
	 * @return ディスクリプタハンドル
	 */
	D3D12_CPU_DESCRIPTOR_HANDLE getDescriptorHandle(const Device& device, const DescriptorHeap& heap, UINT index) const noexcept;

	ComPtr<ID3D12Resource> get(uint32_t index) const noexcept;
};