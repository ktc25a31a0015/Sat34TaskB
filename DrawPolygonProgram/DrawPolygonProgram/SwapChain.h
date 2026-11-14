#pragma once

#include "DXGI.h"
#include "CommandQueue.h"
#include "Window.h"

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class SwapChain final {
public:
	SwapChain() = default;
	~SwapChain() = default;

private:
	ComPtr<IDXGISwapChain3> _swapChain{};
	DXGI_SWAP_CHAIN_DESC1 _swapChainDesc{};

public:
	/**
	 * @brief スワップチェインの生成
	 * @param dxgi dxgiクラスのインスタンス
	 * @param window ウィンドウクラスのインスタンス
	 * @param commandQueue コマンドキュークラスのインスタンス
	 * @return 
	 */
	bool create(const DXGI& dxgi, const Window& window, const CommandQueue& commandQueue) noexcept;

	ComPtr<IDXGISwapChain3> get() const noexcept;
	const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;
};