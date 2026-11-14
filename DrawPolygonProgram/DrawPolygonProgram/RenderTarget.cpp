#include "RenderTarget.h"

#include <cassert>

bool RenderTarget::createBackBuffer(const Device& device, const SwapChain& swapChain, const DescriptorHeap& heap) noexcept {
	const auto& desc = swapChain.getDesc();

	_renderTargets.resize(desc.BufferCount);

	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません。");

	// バックバッファの作成
	for (uint8_t i = 0; i < desc.BufferCount; ++i) {
		const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました。");

			return false;
		}

		// レンダーターゲットビューを作成してディスクリプタヒープのハンドルと関連付ける
		device.get()->CreateRenderTargetView(_renderTargets[i].Get(), nullptr, handle);

		// 次のハンドルへ
		handle.ptr += device.get()->GetDescriptorHandleIncrementSize(heapType);
	}

	return true;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::getDescriptorHandle(const Device& device, const DescriptorHeap& heap, UINT index) const noexcept {
	if (index >= _renderTargets.size() || !_renderTargets[index]) {
		assert(false && "不正なレンダーターゲットです。");
	}

	auto handle = heap.get()->GetCPUDescriptorHandleForHeapStart();

	auto heapType = heap.getType();
	assert(heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV && "ディスクリプタヒープのタイプが RTV ではありません。");

	// インデックスに応じてハンドルを移動
	handle.ptr += index * device.get()->GetDescriptorHandleIncrementSize(heapType);

	return handle;
}

ComPtr<ID3D12Resource> RenderTarget::get(uint32_t index) const noexcept {
	if (index >= _renderTargets.size() || !_renderTargets[index]) {
		assert(false && "不正なレンダーターゲットです。");

		return nullptr;
	}

	return _renderTargets[index];
}