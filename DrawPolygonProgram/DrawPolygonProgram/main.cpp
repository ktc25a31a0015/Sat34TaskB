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

	void loop() noexcept {
		while (_windowInsntance.messageLoop()) {
			const auto backBufferIndex = _swapChainInstance.get()->GetCurrentBackBufferIndex();

			// 以前のフレームの GPU 処理が完了しているか確認して待機
			if (_frameFenceValue[backBufferIndex] != 0) {
				_fenceInstance.wait(_frameFenceValue[backBufferIndex]);
			}

			// リセット
			_commandAllocatorInstance[backBufferIndex].reset();
			_commandListInstance.reset(_commandAllocatorInstance[backBufferIndex]);

			// リソースバリアでレンダーターゲットを Present から RenderTarget へ変更
			auto pToRT = resourceBarrier(_renderTargetInstance.get(backBufferIndex).Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
			_commandListInstance.get()->ResourceBarrier(1, &pToRT);

			// レンダーターゲットの設定
			D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { _renderTargetInstance.getDescriptorHandle(_deviceInstance, _descriptorHeapInstance, backBufferIndex) };
			_commandListInstance.get()->OMSetRenderTargets(1, handles, false, nullptr);

			// レンダーターゲットのクリア
			const float clearColor[] = { 1.f, 0.f, 0.f, 1.f }; // 赤色でクリア
			_commandListInstance.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);

			// リソースバリアでレンダーターゲットを RenderTarget から Present へ変更
			auto rtToP = resourceBarrier(_renderTargetInstance.get(backBufferIndex).Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			_commandListInstance.get()->ResourceBarrier(1, &rtToP);

			// コマンドリストをクローズ
			_commandListInstance.get()->Close();

			// コマンドキューにコマンドリストを送信
			ID3D12CommandList* ppCommandLists[] = { _commandListInstance.get().Get() };
			_commandQueueInstance.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			// プレゼント
			_swapChainInstance.get()->Present(1, 0);

			// フェンスにフェンス値を設定
			_commandQueueInstance.get()->Signal(_fenceInstance.get().Get(), _nextFenceValue);
			_frameFenceValue[backBufferIndex] = _nextFenceValue;
			_nextFenceValue++;
		}
	}

	D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to) noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resource;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Application app;

	if (!app.initialize(hInstance)) {
		assert(false && "アプリケーションの初期化に失敗しました。");
	}

	app.loop();

	return 0;
}