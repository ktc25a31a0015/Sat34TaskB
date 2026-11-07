#include <Windows.h>
#include <d3d12.h>

#include "DirectX12.h";

void EnableDebugLayer() {
#if defined(_DEBUG)
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {

		// デバッグレイヤーを有効化
		debugController->EnableDebugLayer();

		// より詳細な検証を有効化
		ID3D12Debug1* debugController1;
		if (SUCCEEDED(debugController->QueryInterface(IID_PPV_ARGS(&debugController1)))) {
			debugController1->SetEnableGPUBasedValidation(TRUE);
		}
	}
#endif
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);

		return 0;

	case WM_PAINT:

		return 0;

	case WM_KEYDOWN:

		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

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