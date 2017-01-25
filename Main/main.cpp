#include <Application.hpp>
#include <Win32Window.hpp>
#include <Canvas.hpp>
#include <memory>

using namespace Fancy::Graphics;

class MainWindow : public Win32Window
{
public:
    void OnUpdate(Canvas& canvas) override
    {
        canvas.Clear({ 255, 0, 0, 255 });
        canvas.DrawLine({ 0, 0 }, { 30, 30 }, { 0, 0, 0, 255 });
    }
};

int main()
{
    auto window = std::make_unique<MainWindow>();
    auto& app = Fancy::App();
    window->Show();
    app.SetMainWindow(*window);
    app.Run();
}