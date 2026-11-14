#include "CommandAllocator.h"

#include <cassert>

bool CommandAllocator::create(const Device& device, const D3D12_COMMAND_LIST_TYPE type) noexcept {
	_type = type;

	const auto hr = device.get()->CreateCommandAllocator(_type, IID_PPV_ARGS(&_commandAllocator));
	if (FAILED(hr)) {
		assert(false && "コマンドアロケータの作成に失敗しました。");

		return false;
	}

	return true;
}

void CommandAllocator::reset() noexcept {
	if (!_commandAllocator) {
		assert(false && "コマンドアロケータが未作成です。");
	}

	_commandAllocator->Reset();
}

ComPtr<ID3D12CommandAllocator> CommandAllocator::get() const noexcept {
	if (!_commandAllocator) {
		assert(false && "コマンドアロケータが未作成です。");

		return nullptr;
	}

	return _commandAllocator;
}

D3D12_COMMAND_LIST_TYPE CommandAllocator::getType() const noexcept {
	if (!_commandAllocator) {
		assert(false && "コマンドリストのタイプが未設定です。");
	}

	return _type;
}