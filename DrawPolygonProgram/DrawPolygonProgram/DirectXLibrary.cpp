#include "DirectXLibrary.h"

ComPtr<IDXGIFactory4> DirectXLibrary::CreateDXGIFactory() {
	ComPtr<IDXGIFactory4> factory;
	UINT createFactoryFlags = 0;

#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	HRESULT hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory));
	if(FAILED(hr)) {
		OutputDebugString("Failed to create DXGI Factory\n");

		return nullptr;
	}

	return factory;
}

ComPtr<IDXGIAdapter> DirectXLibrary::GetHardwareAdapter(IDXGIFactory4* factory) {
	ComPtr<IDXGIAdapter1> adapter = nullptr;

	// ハードウェアアダプターを順番に検索
	for(UINT adapterIndex = 0; ; ++adapterIndex) {
		adapter = nullptr;

		// アダプターが見つからなければループを終了
		if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &adapter)) break;

		// アダプターの情報を取得
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		// ソフトウェアアダプターはスキップ
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

		// DirectX12 対応テスト
		if(SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr))) {
			return adapter; // 対応するアダプターを返す
		}
	}

	return nullptr;
}

ComPtr<ID3D12Device> DirectXLibrary::CreateD3D12Device(IDXGIAdapter1* adapter) {
	ComPtr<ID3D12Device> device;

	// 指定アダプターでデバイス作成を試行
	HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

	if(FAILED(hr)) {
		// 規定アダプターによる、フォールバック：ソフトウェアアダプターを試行
		hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device));

		if(FAILED(hr)) {
			OutputDebugString("Failed to create D3D12 Device\n");

			return nullptr;
		}

		OutputDebugString("Using softwar adapter (WARP)\n");
	}

	return device;
}

ComPtr<ID3D12CommandQueue> DirectXLibrary::CreateCommandQueue(ID3D12Device* device) {
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; // 直接実行型
	queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; // 標準優先度
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // 特別フラグなし
	queueDesc.NodeMask = 0; // 単一 GPU 使用

	ComPtr<ID3D12CommandQueue> commandQueue;

	HRESULT hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(hr)) {
		OutputDebugString("Failed to create D3D12 Command Queue\n");

		return nullptr;
	}

	commandQueue->SetName(L"Command Queue");

	return commandQueue;
}

ComPtr<IDXGISwapChain3> DirectXLibrary::CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2; // ダブルバッファリング
	swapChainDesc.Width = 800; // 画面幅
	swapChainDesc.Height = 600; // 画面高
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ピクセルフォーマット
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // レンダーターゲット用
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // 高速切り替え
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプリングなし

	ComPtr<IDXGISwapChain1> swapChain1;
	HRESULT hr = factory->CreateSwapChainForHwnd(
		commandQueue, // コマンドキュー
		hwnd, // ターゲットウィンドウ
		&swapChainDesc, // 設定
		nullptr, // フルスクリーン設定
		nullptr, // 出力制限
		&swapChain1 // 作成されたスワップチェーン
	);

	if (FAILED(hr)) {
		OutputDebugString("Failed to create Swap Chain\n");

		return nullptr;
	}

	// より高機能なインターフェースにキャスト
	ComPtr<IDXGISwapChain3> swapChain;
	hr = swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));

	if(FAILED(hr)) {
		OutputDebugString("Failed to cast to SwapChain3\n");

		return nullptr;
	}

	return swapChain;
}

void DirectXLibrary::EnableDebugLayer() {
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {

		// デバッグレイヤーを有効化
		debugController->EnableDebugLayer();

		// より詳細な検証を有効化
		ComPtr<ID3D12Debug1> debugController1;
		if(SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1)))) {
			debugController1->SetEnableGPUBasedValidation(TRUE);
		}
	}
#endif
}