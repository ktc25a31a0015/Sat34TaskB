#include "Window.h"

namespace {
	/**
	 * @brief ウィンドウプロシージャ
	 * @param hwnd		ウィンドウハンドル
	 * @param msg		メッセージ
	 * @param wParam	メッセージパラメータ
	 * @param lParam	メッセージパラメータ
	 * @return 処理結果
	 */
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_DESTROY: // ウィンドウが閉じられたとき
			PostQuitMessage(0);

			return 0;
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

HRESULT Window::create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {
	// ウィンドウの定義
	WNDCLASS wc{};
	wc.lpfnWndProc	 = WindowProc;
	wc.hInstance	 = instance;
	wc.lpszClassName = name.data();
	wc.hCursor		 = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClass(&wc);

	_handle = CreateWindow(wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, nullptr);

	if (!_handle) return E_FAIL;

	ShowWindow(_handle, SW_SHOW);
	UpdateWindow(_handle);

	_width = width;
	_height = height;

	return S_OK;
}

bool Window::messageLoop() const noexcept {
	MSG msg{};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}