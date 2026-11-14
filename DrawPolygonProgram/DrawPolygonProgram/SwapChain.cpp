#include "SwapChain.h"

#include <cassert>

bool SwapChain::create(const DXGI& dxgi, const Window& window, const CommandQueue& commandQueue) noexcept {
	const auto [w, h] = window.size();

	// スワップチェインの設定
	_swapChainDesc = {};
	_swapChainDesc.BufferCount		= 2;								// バックバッファの数（ダブルバッファ）
	_swapChainDesc.Width			= w;								// バックバッファの横幅
	_swapChainDesc.Height			= h;								// バックバッファの縦幅
	_swapChainDesc.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;		// バックバッファのフォーマット
	_swapChainDesc.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT;	// レンダーターゲットとしての使用
	_swapChainDesc.SwapEffect		= DXGI_SWAP_EFFECT_DISCARD;			// 舞フレーム画面更新、描画後バッファを破棄
	_swapChainDesc.SampleDesc.Count = 1;								// マルチサンプリングなし

	// 一時的なスワップチェインの作成
	// スワップチェインのアップグレードが必要
	ComPtr<IDXGISwapChain1> tempSwapChain{};
	{
		const auto hr = dxgi.factory()->CreateSwapChainForHwnd(commandQueue.get().Get(), window.handle(),
			&_swapChainDesc, nullptr, nullptr, &tempSwapChain);

		if (FAILED(hr)) {
			assert(false && "スワップチェインの作成に失敗。");

			return false;
		}
	}

	// スワップチェインをアップグレード
	{
		const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&_swapChain));
		if (FAILED(hr)) {
			assert(false && "スワップチェインのアップグレードに失敗。");

			return false;
		}
	}

	return true;
}

ComPtr<IDXGISwapChain3> SwapChain::get() const noexcept {
	if (!_swapChain) {
		assert(false && "スワップチェインが未作成です。");

		return nullptr;
	}

	return _swapChain;
}

const DXGI_SWAP_CHAIN_DESC1& SwapChain::getDesc() const noexcept {
	if (!_swapChain) {
		assert(false && "スワップチェインが未作成です。");
	}

	return _swapChainDesc;
}