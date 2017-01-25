#include "Win32Window.hpp"
#include "Canvas.hpp"
#include <atomic>
#include <unordered_map>
#include <Windows.h>
//#include <d2d1_1.h>
//#include <wrl/client.h>

namespace Fancy::Graphics
{
    std::atomic<bool> hasWindowClassRegistered{ false };
    
    ::HINSTANCE GetInstanceHandle()
    {
        const static ::HINSTANCE instance = ::GetModuleHandle(nullptr);
        Ensures(instance != nullptr);
        return instance;
    }

    struct Win32Window::Win32Params
    {
        std::uintptr_t wParam, lParam;
    };

    ::HWND GetWin32WindowHandle(const Win32Window& window) noexcept
    {
        return static_cast<::HWND>(window.GetNativeHandle());
    }

    ::LRESULT CALLBACK WindowProc(::HWND windowHandle, ::UINT messageId, ::WPARAM wParam, ::LPARAM lParam)
    {
        Win32Window* window = {};
        if (messageId == WM_NCCREATE)
        {
            const auto pcs = reinterpret_cast<::CREATESTRUCT*>(lParam);
            window = static_cast<Win32Window*>(pcs->lpCreateParams);
            Ensures(window != nullptr);
            ::SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<::LONG_PTR>(window));
        }
        else
        {
            window = reinterpret_cast<Win32Window*>(::GetWindowLongPtr(windowHandle, GWLP_USERDATA));
            if (window != nullptr)
            {
                window->ProcessMessage(static_cast<std::uint32_t>(messageId), { wParam, static_cast<std::uintptr_t>(lParam) });
            }
        }
        return ::DefWindowProc(windowHandle, messageId, wParam, lParam);
    }

    //canvas' resize is deferred to WM_SIZE.
    Win32Window::Win32Window(std::uint32_t width, std::uint32_t height)
    {
        if (!hasWindowClassRegistered.load(std::memory_order::memory_order_seq_cst))
        {
            hasWindowClassRegistered.store(true, std::memory_order_release);
            ::WNDCLASSEX windowClass = {};
            windowClass.cbSize = sizeof(::WNDCLASSEX);
            windowClass.style = CS_HREDRAW | CS_VREDRAW;
            windowClass.lpfnWndProc = WindowProc;
            windowClass.hInstance = GetInstanceHandle();
            windowClass.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
            windowClass.lpszClassName = L"FancyUIWindow";
            windowClass.cbWndExtra = sizeof(std::intptr_t);
            ::RegisterClassEx(&windowClass);
        }
        handle_ = ::CreateWindow(L"FancyUIWindow", L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
            nullptr, nullptr, GetInstanceHandle(), static_cast<void*>(this));
        Ensures(handle_ != nullptr);
    }

    Win32Window::Win32Window()
        : Win32Window{ CW_USEDEFAULT, CW_USEDEFAULT }
    {
    }

    WindowHandle Win32Window::GetNativeHandle() const noexcept
    {
        return handle_;
    }

    void Win32Window::Show()
    {
        ::ShowWindow(GetWin32WindowHandle(*this), SW_SHOW);
        ::UpdateWindow(GetWin32WindowHandle(*this));
    }

    void Win32Window::OnUpdate(Canvas & /*canvas*/)
    {
    }

    void Win32Window::OnResize(std::uint32_t/* newWidth*/, std::uint32_t /*newHeight*/)
    {
    }

    Win32Window::~Win32Window()
    {
    }

    void Win32Window::OnPaint(void* hdc)
    {
        if (!canvas_)
            return;

        //DO NOT throw exception in WndProc.
        try
        {
            this->OnUpdate(*canvas_);
        }
        catch (...)
        {
            //TODO: What to do?
        }
        
        const auto dcHandle = ::GetDC(GetWin32WindowHandle(*this));
        ::BITMAPINFO bmi = {};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = canvas_->GetWidth();
        bmi.bmiHeader.biHeight = -static_cast<std::int32_t>(canvas_->GetHeight()); // top-down image
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = 0;
        const auto pixels = canvas_->GetBitmap();
        const auto result = ::SetDIBitsToDevice(
            static_cast<::HDC>(hdc), 0, 0, canvas_->GetWidth(), canvas_->GetHeight(), 0, 0, 0, canvas_->GetHeight(), pixels.data(), &bmi,
            DIB_RGB_COLORS);
        //TODO: how to represent error?
    }

    void Win32Window::OnResizeInternal(std::uint32_t newWidth, std::uint32_t newHeight)
    {
        if (!canvas_)
            canvas_ = std::make_unique<Canvas>(newWidth, newHeight);
        else
            canvas_->Resize(newWidth, newHeight);
        OnResize(newWidth, newHeight);
    }

    std::int32_t Win32Window::ProcessMessage(std::uint32_t message, Win32Params params)
    {
        switch (message)
        {
        case WM_PAINT:
        {
            ::PAINTSTRUCT ps;
            const auto hdc = BeginPaint(GetWin32WindowHandle(*this), &ps);
            OnPaint(hdc);
            EndPaint(GetWin32WindowHandle(*this), &ps);
        }
        break;
        case WM_SIZE:
        {
            const auto lParam = params.lParam;
            const auto newWidth = static_cast<std::uint32_t>(lParam & 0xFFFF);
            const auto newHeight = static_cast<std::uint32_t>(lParam >> 16);
            OnResizeInternal(newWidth, newHeight);
        }
        break;
        case WM_QUIT:
            ::PostQuitMessage(0);
            break;
        default:
            break;
        }
        return 0;
    }
}

