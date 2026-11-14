#include "Device.h"

#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")

bool Device::create(const DXGI& dxgi) noexcept {
	const auto hr = D3D12CreateDevice(dxgi.displayAdapter().Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&_device));
	if (FAILED(hr)) {
		assert(false && "デバイス作成に失敗");

		return false;
	}

	return true;
}

ComPtr<ID3D12Device> Device::get() const noexcept {
	if (!_device) {
		assert(false && "デバイスが未作成です。");

		return nullptr;
	}

	return _device;
}