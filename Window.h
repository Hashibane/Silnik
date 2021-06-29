#pragma once
#include "NHWindows.h"
#include "EditedExceptions.h"
#include "keyboard.h"
#include "mouse.h"
#include "Graphics.h"
#include "dxerr.h"
#include <optional>
#include <memory>
#define WIDTH 800
#define HEIGHT 600
class Window
{
	public:
		class wException : public Exception
		{
				using Exception::Exception;
			public:
				std::string TranslateErrorCode(HRESULT hr) noexcept;
		};
		class HrException : public Exception
		{
			public:
				//hresult - windows error code
				HrException(int linia, const char* plik, HRESULT hr) noexcept;
				const char* what() const noexcept override;
				const char* GetType() const noexcept override;
				HRESULT	GetErrorCode() const noexcept;
				std::string GetErrorString() const noexcept;
				std::string GetErrorDescription() const noexcept;
			private:
				HRESULT hr;
		};
		class NoGfxException : public Exception
		{
			public:
				using Exception::Exception;
				const char* GetType() const noexcept override;
		};
	private:
		class WindowClass
		{
			public:
				static const wchar_t* GetName() noexcept;
				static HINSTANCE GetInstance() noexcept;
			private:
				WindowClass() noexcept;
				~WindowClass();
				WindowClass(const WindowClass&) = delete;
				WindowClass& operator = (const WindowClass&) = delete;
				static constexpr const LPCWSTR wndClassName = L"Window.h testing";
				static WindowClass wndClass;
				HINSTANCE hInst;
		};
	public:
		Window(int width, int height, const wchar_t* name) noexcept;
		~Window();
		Window(const Window&) = delete;
		WindowClass& operator = (const WindowClass&) = delete;
		void SetTitle(const wchar_t* title);
		static std::optional<int> ProcessMessages();
		Graphics& Gfx();
	private:
		static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	public:
		Keyboard kbd;
		Mouse mouse; 
	private:
		int width, height;
		HWND hwnd;
		std::unique_ptr<Graphics> pGfx;
};

#define WND_EXCEPT( hr ) Window::HrException(__LINE__,__FILE__,hr)
#define WND_LAST_EXCEPT() Window::HrException(__LINE__,__FILE__, GetLastError())
#define WND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__,__FILE__ )