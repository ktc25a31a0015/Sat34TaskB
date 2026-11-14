#include "CommandList.h"

#include <cassert>

bool CommandList::create(const Device& device, const CommandAllocator& commandAllocator) noexcept {
	const auto hr = device.get()->CreateCommandList(0, commandAllocator.getType(), commandAllocator.get().Get(), nullptr, IID_PPV_ARGS(&_commandList));
	if (FAILED(hr)) {
		assert(false && "コマンドリストの作成に失敗しました。");

		return false;
	}

	_commandList->Close();

	return true;
}

void CommandList::reset(const CommandAllocator& commandAllocator) noexcept {
	if (!_commandList) {
		assert(false && "コマンドリストが未完成です。");
	}

	_commandList->Reset(commandAllocator.get().Get(), nullptr);
}

ComPtr<ID3D12GraphicsCommandList> CommandList::get() const noexcept {
	if (!_commandList) {
		assert(false && "コマンドリストが未作成です。");

		return nullptr;
	}

	return _commandList;
}