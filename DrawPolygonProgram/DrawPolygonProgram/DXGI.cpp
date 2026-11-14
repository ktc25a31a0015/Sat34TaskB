#include "DXGI.h"

#include <cassert>

#pragma comment(lib, "dxgi.lib")

bool DXGI::setDisplayAdapter() noexcept {
#if _DEBUG
	ComPtr<ID3D12Debug> debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug)))) {
		debug->EnableDebugLayer();
	}
#endif

	// DXGIファクトリーの作成
	{
		UINT createFactoryFlags = 0;
#if _DEBUG
		createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&_dxgiFactory));
		if (FAILED(hr)) {
			assert(false && "DXGIファクトリーの作成に失敗");

			return false;
		}
	}

	// アダプタの取得
	{
		// アダプタを列挙
		auto				  select = 0;
		ComPtr<IDXGIAdapter1> dxgiAdapter{};

		// 適切なアダプタを選ぶ
		while (_dxgiFactory->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER_DESC1 desc{};
			dxgiAdapter->GetDesc1(&desc);

			select++;

			// ソフトウェアアダプタは除外
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

			// Direct3D12 が動かない場合も除外
			if (FAILED(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) continue;

			_dxgiAdapter = dxgiAdapter;

			break;
		}

		if (!_dxgiAdapter) {
			assert(false && "アダプタの取得に失敗");

			return false;
		}
	}

	return true;
}

ComPtr<IDXGIFactory4> DXGI::factory() const noexcept {
	if (!_dxgiFactory) {
		assert(false && "DXGIファクトリーが未作成です。");

		return nullptr;
	}

	return _dxgiFactory;
}

ComPtr<IDXGIAdapter1> DXGI::displayAdapter() const noexcept {
	if (!_dxgiAdapter) {
		assert(false && "ディスプレイアダプターが未作成です。");

		return nullptr;
	}

	return _dxgiAdapter;
}