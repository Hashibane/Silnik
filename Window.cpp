#include "Window.h"
#include <sstream>
//konstruktor okna
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept:hInst(GetModuleHandle(NULL))
{
    WNDCLASSEX wcx;
    wcx.cbSize = sizeof(WNDCLASSEX);
    wcx.style = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc = HandleMsgSetup;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = GetInstance();
    //do zmienienia pozniej
    wcx.hIcon = NULL;
    wcx.hIconSm = NULL;
    //menu reference
    wcx.lpszMenuName = NULL;

    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = NULL;
    //konwersja char* na LPCWSTR
    wcx.lpszClassName = GetName();
    RegisterClassEx(&wcx);
}

Window::WindowClass::~WindowClass()
{

    UnregisterClass(wndClassName, GetInstance());
}

LPCWSTR Window::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

Window::Window(int width, int height, const wchar_t* name) noexcept
{
    RECT wr;
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE))
    {
        throw WND_LAST_EXCEPT();
    }
    hwnd = CreateWindow(WindowClass::GetName(), name, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, WindowClass::GetInstance(), this);
    if (hwnd == nullptr)
    {
        throw WND_LAST_EXCEPT();
    }
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    pGfx = std::make_unique<Graphics>(hwnd);
}

Window::~Window()
{
    DestroyWindow(hwnd);
}

void Window::SetTitle(const wchar_t* t)
{
    if (!SetWindowText(hwnd, reinterpret_cast<LPCWSTR>(t)))
    {
        throw WND_LAST_EXCEPT();
    }
}


//wazne
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (msg == WM_NCCREATE)
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    switch (msg)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(66);
            return 0;
        }
        //Obsluga klawiatury
        case WM_KILLFOCUS:
        {
            kbd.ClearState();
            break;
        }
        case WM_KEYDOWN:
        {
            if (!(lParam & 0x40000000) or kbd.IsAutorepeatEnabled())
            {
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        }
        case WM_SYSKEYDOWN:
        {
            if (!(lParam & 0x40000000) or kbd.IsAutorepeatEnabled())
            {
                kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
            }
            break;
        }
        case WM_KEYUP:
        {
            kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
            break;
        }

        //Obsluga myszy
        case WM_MOUSEMOVE:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            //w okienku 
            if ((pt.x >= 0) && (pt.x < width) && (pt.y >= 0) && (pt.y < height))
            {
                mouse.OnMouseMove(pt.x, pt.y);
                if (!mouse.IsInWindow())
                {
                    SetCapture(hwnd);
                    mouse.OnMouseEnter();
                }
                else
                {
                    if (wParam & (MK_LBUTTON | MK_RBUTTON))
                    {
                        mouse.OnMouseMove(pt.x, pt.y);
                    }
                    else
                    {
                        ReleaseCapture();
                        mouse.OnMouseLeave();
                    }
                }
            }
            break;
        }
        case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftPressed(pt.x, pt.y);
            break;
        }
        case WM_RBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightPressed(pt.x, pt.y);
            break;
        }
        case WM_LBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnLeftReleased(pt.x, pt.y);
            break;
        }
        case WM_RBUTTONUP:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            mouse.OnRightReleased(pt.x, pt.y);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            const POINTS pt = MAKEPOINTS(lParam);
            if (GET_WHEEL_DELTA_WPARAM(wParam))
            {
                mouse.OnWheelUp(pt.x, pt.y);
            }
            else if (GET_WHEEL_DELTA_WPARAM(wParam))
            {
                mouse.OnWheelDown(pt.x, pt.y);
            }
            break;
        }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//exceptions
//kopia z exception.cpp

Window::HrException::HrException(int line, const char* file, HRESULT hr) noexcept : Exception(line, file),  hr(hr) {}

std::string Window::wException::TranslateErrorCode(HRESULT hr) noexcept
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPWSTR>(&pMsgBuf), 0, NULL);
    if (!nMsgLen)
    {
        return "Niezidentyfikowany blad";
    }
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}
HRESULT Window::HrException::GetErrorCode() const noexcept
{
    return hr;
}
std::optional<int> Window::ProcessMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return msg.wParam;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {};
}

Graphics& Window::Gfx()
{
    if (!pGfx)
    {
        throw WND_NOGFX_EXCEPT();
    }
    return *pGfx;
}

const char* Window::HrException::GetType() const noexcept
{
    return "Hr Window Exception";
}

const char* Window::NoGfxException::GetType() const noexcept
{
    return "No gfx wnd except";
}

const char* Window::HrException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
        << std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
        << "[Description] " << 
        GetErrorDescription() << std::endl
        << GetOriginString();
    std::string wbr = oss.str();
    return wbr.c_str();
}

std::string Window::HrException::GetErrorDescription() const noexcept
{
    WCHAR buf[512];
    DXGetErrorDescription(hr, buf, sizeof(buf) * 2);
    std::wstring ws = std::wstring(buf);
    std::string str(ws.begin(), ws.end());
    return str;
}