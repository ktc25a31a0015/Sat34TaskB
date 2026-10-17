#include <d3d12.h>
#include <dxgi1_4.h>

#include <wrl/client.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using Microsoft::WRL::ComPtr;

class DirectXLibrary {
public:
	ComPtr<IDXGIFactory4> CreateDXGIFactory();
	ComPtr<IDXGIAdapter> GetHardwareAdapter(IDXGIFactory4* factory);
	ComPtr<ID3D12Device> CreateD3D12Device(IDXGIAdapter1* adapter);
	ComPtr<ID3D12CommandQueue> CreateCommandQueue(ID3D12Device* device);
	ComPtr<IDXGISwapChain3> CreateSwapChain(IDXGIFactory4* factory, ID3D12CommandQueue* commandQueue, HWND hwnd);

	void EnableDebugLayer();
};