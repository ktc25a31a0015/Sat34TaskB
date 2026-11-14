#pragma once

#include "Device.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DescriptorHeap final {
public:
	DescriptorHeap() = default;
	~DescriptorHeap() = default;

private:
	ComPtr<ID3D12DescriptorHeap> _heap {};
	D3D12_DESCRIPTOR_HEAP_TYPE _type{};

public:
	/**
	 * @brief ディスクリプタヒープを作成する
	 * @param device デバイスクラスのインスタンス
	 * @param type ディスクリプタヒープのタイプ
	 * @param numDescriptors ディスクリプタの数
	 * @param shaderVisible シェーダーからのアクセス可能性
	 * @return 生成の可否
	 */
	bool create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;

	ComPtr<ID3D12DescriptorHeap> get() const noexcept;
	D3D12_DESCRIPTOR_HEAP_TYPE getType() const noexcept;
};