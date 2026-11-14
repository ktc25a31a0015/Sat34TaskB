#pragma once

#include "Device.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class Fence final {
public:
	Fence() = default;
	~Fence() = default;

private:
	ComPtr<ID3D12Fence> _fence{};
	HANDLE _waitGpuEvent{};

public:
	bool create(const Device& device) noexcept;

	void wait(UINT64 fenceValue) const noexcept;

	ComPtr<ID3D12Fence> get() const noexcept;
};