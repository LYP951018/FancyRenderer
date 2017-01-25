#include "Application.hpp"
//#include "Win32Window.hpp"
#include <utility>
#include <Windows.h>

namespace Fancy
{
    Application::Application()
        : mainWindow_{}
    {
    }

    Graphics::Win32Window* Application::SetMainWindow(Graphics::Win32Window& window)
    {
        return std::exchange(mainWindow_, &window);
    }

    void Application::Run()
    {
        ::BOOL bRet;
        ::MSG message;
        while ((bRet = GetMessage(&message, nullptr, 0, 0)) != 0)
        {
            if (bRet == -1)
            {
                //TODO: throw
                break;
            }
            else
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }
    }


    Application& App()
    {
        static Application app;
        return app;
    }

}