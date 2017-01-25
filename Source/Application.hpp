#pragma once

namespace Fancy
{
    namespace Graphics
    {
        class Win32Window;
    }

    class Application
    {
    public:
        friend Application& App();

        Graphics::Win32Window* SetMainWindow(Graphics::Win32Window& window);
        void Run();

    private:
        Application();
        Graphics::Win32Window* mainWindow_;
    };
}