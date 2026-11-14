#include "Window.h"

#include "Device.h"
#include "DXGI.h"
#include "CommandAllocator.h"
#include "CommandList.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "RenderTarget.h"
#include "Fence.h"

#include <cassert>

class Application final {
public:
	Application() = default;
	~Application() = default;

private:
	Window _windowInsntance{};
	DXGI _dxgiInstance{};
	Device _deviceInstance{};
	CommandQueue _commandQueueInstance{};
	SwapChain _swapChainInstance{};
	DescriptorHeap _descriptorHeapInstance{};
	RenderTarget _renderTargetInstance{};
	CommandAllocator _commandAllocatorInstance[2]{};
	CommandList _commandListInstance{};

	Fence _fenceInstance{};
	UINT64 _frameFenceValue[2]{};
	UINT64 _nextFenceValue = 1;

public:
	bool initialize(HINSTANCE instance) noexcept {
		if (S_OK != _windowInsntance.create(instance, 1280, 720, "MyApp")) {
			assert(false && "ウィンドウの作成に失敗しました。");

			return false;
		}

		if (!_dxgiInstance.setDisplayAdapter()) {
			assert(false && "DXGIのアダプタ設定に失敗しました。");

			return false;
		}

		if (!_deviceInstance.create(_dxgiInstance)) {
			assert(false && "デバイスの作成に失敗しました。");

			return false;
		}

		if (!_commandQueueInstance.create(_deviceInstance)) {
			assert(false && "コマンドキューの作成に失敗しました。");

			return false;
		}

		if (!_swapChainInstance.create(_dxgiInstance, _windowInsntance, _commandQueueInstance)) {
			assert(false && "スワップチェインの作成に失敗しました。");

			return false;
		}

		if (!_descriptorHeapInstance.create(_deviceInstance, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, _swapChainInstance.getDesc().BufferCount)) {
			assert(false && "ディスクリプタヒープの作成に失敗しました。");

			return false;
		}

		if (!_renderTargetInstance.createBackBuffer(_deviceInstance, _swapChainInstance, _descriptorHeapInstance)) {
			assert(false, "レンダーターゲットの作成に失敗しました。");

			return false;
		}

		if (!_commandAllocatorInstance[0].create(_deviceInstance, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
			assert(false && "コマンドアロケータ０の作成に失敗しました。");

			return false;
		}

		if (!_commandAllocatorInstance[1].create(_deviceInstance, D3D12_COMMAND_LIST_TYPE_DIRECT)) {
			assert(false && "コマンドアロケータ１の作成に失敗しました。");

			return false;
		}

		if (!_commandListInstance.create(_deviceInstance, _commandAllocatorInstance[0])) {
			assert(false && "コマンドリストの作成に失敗しました。");

			return false;
		}

		if (!_fenceInstance.create(_deviceInstance)) {
			assert(false && "フェンスの作成に失敗しました。");

			return false;
		}

		return true;
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = "GameWindow";
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
		"GameWindow", "Draw Polygon Program", // ウィンドウクラス名、ウィンドウタイトル
		WS_OVERLAPPEDWINDOW, // ウィンドウスタイル
		CW_USEDEFAULT, CW_USEDEFAULT, // 位置（自動）
		800, 600, // サイズ（幅、高さ）
		NULL, NULL, hInstance, // 親ウィンドウ、メニュー、インスタンス
		NULL // 追加データ
	);
	ShowWindow(hwnd, nCmdShow);
	
	EnableDebugLayer();
	DirectX12 dx12(hwnd);

	MSG msg{};
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}