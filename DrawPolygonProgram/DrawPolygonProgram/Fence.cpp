#include "Fence.h"

#include <cassert>

bool Fence::create(const Device& device) noexcept {
	// フェンスの作成
	HRESULT hr = device.get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗しました。");

		return false;
	}

	// GPU 同期用のイベントハンドルを作成
	_waitGpuEvent = CreateEvent(nullptr, false, false, "WAIT_GPU");
	if (!_waitGpuEvent) {
		assert(false, "GPU 同期用のイベントハンドルの作成に失敗しました。");

		return false;
	}

	return true;
}

void Fence::wait(UINT64 fenceValue) const noexcept {
	if (!_fence) {
		assert(false && "フェンスが未作成です。");

		return;
	}

	// フェンスの値が指定された値に達するまで待機
	if (_fence->GetCompletedValue() < fenceValue) {
		// GPU がフェンス値に達するまで待機
		_fence->SetEventOnCompletion(fenceValue, _waitGpuEvent);
		WaitForSingleObject(_waitGpuEvent, INFINITE);
	}
}

ComPtr<ID3D12Fence> Fence::get() const noexcept {
	if (!_fence) {
		assert(false && "フェンスが未作成です。");

		return nullptr;
	}

	return _fence;
}