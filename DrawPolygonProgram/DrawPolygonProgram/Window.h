#pragma once

#include <Windows.h>
#include <string>

class Window final {
public:
	Window() = default;
	~Window() = default;

private:
	HWND _handle{};
	int _width{};
	int _height{};

public:
	/**
	 * @brief ウィンドウの生成
	 * @param instance	インスタンスハンドル
	 * @param width		横幅
	 * @param hegiht	縦幅
	 * @param name		ウィンドウ名
	 * @return 生成の可否
	 */
	HRESULT create(HINSTANCE instance, int width, int hegiht, std::string_view name) noexcept;

	bool messageLoop() const noexcept;

	/**
	 * @brief	ウィンドウハンドルを取得する
	 * @return	ウィンドウハンドル
	 */
	HWND handle() const noexcept { return _handle; };

	/**
	 * @brief	ウィンドウのサイズを取得する
	 * @return	ウィンドウのサイズ（横幅、縦幅）
	 */
	std::pair<int, int> size() const noexcept { return { _width, _height }; };
};