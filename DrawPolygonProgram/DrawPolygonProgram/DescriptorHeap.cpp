#include "DescriptorHeap.h"

#include <cassert>

bool DescriptorHeap::create(const Device& device, D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible) noexcept {
	// ヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Type			= type;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Flags			= shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	_type = type;

	// ディスクリプタヒープの作成
	HRESULT hr = device.get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&_heap));
	if (FAILED(hr)) {
		assert(false && "ディスクリプタヒープの生成に失敗しました。");

		return false;
	}

	return true;
}

ComPtr<ID3D12DescriptorHeap> DescriptorHeap::get() const noexcept {
	if (!_heap) {
		assert(false && "ディスクリプタヒープが未生成です。");

		return nullptr;
	}

	return _heap;
}

D3D12_DESCRIPTOR_HEAP_TYPE DescriptorHeap::getType() const noexcept {
	if (!_heap) {
		assert(false && "ディスクリプタヒープが未生成です。");
	}

	return _type;
}