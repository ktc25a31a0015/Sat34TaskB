#pragma once

#include <d3d12.h>
#include <dxgi1_4.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DXGI final {
public:
	DXGI() = default;
	~DXGI() = default;

private:
	ComPtr<IDXGIFactory4> _dxgiFactory{}; // DXGIを作成するファクトリー
	ComPtr<IDXGIAdapter1> _dxgiAdapter{}; // ディスプレイモード取得用アダプタ

public:
	/**
	 * @brief  ディスプレイアダプタの設定
	 * @return ディスプレイの情報が正しく取得出来たら true
	 */
	bool setDisplayAdapter() noexcept;

	ComPtr<IDXGIFactory4> factory() const noexcept;
	ComPtr<IDXGIAdapter1> displayAdapter() const noexcept;
};