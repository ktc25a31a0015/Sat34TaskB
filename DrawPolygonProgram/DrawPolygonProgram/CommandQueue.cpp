#include "CommandQueue.h"

#include <cassert>

bool CommandQueue::create(const Device& device) noexcept {
	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type	  = D3D12_COMMAND_LIST_TYPE_DIRECT;  // ダイレクトコマンドキュー
	desc.Priority = D3D12_RESIDENCY_PRIORITY_NORMAL; // 通常優先度
	desc.Flags	  = D3D12_COMMAND_QUEUE_FLAG_NONE;	 // 特別フラグなし
	desc.NodeMask = 0;								 // GPU の使用は１つのみ

	const auto hr = device.get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&_commandQueue));
	if (FAILED(hr)) {
		assert(false && "コマンドキューの作成に失敗。");

		return false;
	}

	return true;
}

ComPtr<ID3D12CommandQueue> CommandQueue::get() const noexcept {
	if (!_commandQueue) {
		assert(false && "コマンドキューが未作成です。");

		return nullptr;
	}

	return _commandQueue;
}