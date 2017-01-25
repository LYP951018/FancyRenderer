#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <utility>
#include <memory>

namespace Fancy::Graphics
{
    class Canvas;

    using WindowHandle = void*;

    template<typename... Args>
    class Event
    {
    public:
        using HandlerType = std::function<void(Args...)>;

        template<typename Func>
        void AddHandler(Func&& func)
        {
            handlers_.emplace_back(std::forward<Func>(func));
        }

    private:
        std::vector<HandlerType> handlers_;
    };

    class Win32Window
    {
    public:
        Win32Window(std::uint32_t width, std::uint32_t height);
        Win32Window();

        Win32Window(const Win32Window&) = delete;
        Win32Window& operator= (const Win32Window&) = delete;

        WindowHandle GetNativeHandle() const noexcept;
        void Show();

        virtual void OnUpdate(Canvas& canvas);
        virtual void OnResize(std::uint32_t newWidth, std::uint32_t newHeight);

        virtual ~Win32Window();

    private:
        struct Win32Params;

        void OnPaint(void* hdc);
        void OnResizeInternal(std::uint32_t newWidth, std::uint32_t newHeight);
        WindowHandle handle_;
        std::unique_ptr<Canvas> canvas_;

    public:
        //Make it public because I don't want to write friend declarition for WindowProc (which full of Win32 Shit)
        std::int32_t ProcessMessage(std::uint32_t message, Win32Params params);
    };
};