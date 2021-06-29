#include "App.h"
#include "Box.h"
#include <memory>
#include <sstream>
#include <random>
App::App():wnd(WIDTH, HEIGHT, L"tekst okienny") 
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0, 3.1415 * 2);
    std::uniform_real_distribution<float> ddist(0, 3.1415 * 2);
    std::uniform_real_distribution<float> odist(0, 3.1415 * 0.3);
    std::uniform_real_distribution<float> rdist(6, 20);
    for (auto i = 0; i < 80; i++)
    {
        boxes.push_back(std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
    }
    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1, (3.0 / 4.0), 0.5, 40));
}

int App::Start()
{
    MSG msg;
    int bul;
    wnd.Gfx().Start();
    while (true)
    {
        if (const auto ecode = Window::ProcessMessages())
        {
            return *ecode;
        }
        GenFrame();
       /*
        while (!wnd.mouse.IsEmpty())
        {
            const auto e = wnd.mouse.Read();
            if (e.GetType() == Mouse::Event::Type::Move)
            {
                std::wstringstream oss;
                oss << L"Mouse: {" << e.GetPosX() << L"," << e.GetPosY() << L"}";
                std::wstring ws1 = oss.str();
                LPWSTR ws2 = const_cast<LPWSTR>(ws1.c_str());
                wnd.SetTitle(ws2);
            }
        }
       */

    }
    if (bul == -1)
    {
        WND_LAST_EXCEPT();
    }
    return (int)msg.wParam;
}
App::~App() {}

void App::GenFrame()
{
    wnd.Gfx().Stop();
    auto dt = wnd.Gfx().Elapsed();
    wnd.Gfx().Start();
    wnd.Gfx().ClearBuffer(0.07, 0, 0.12f);
    for (auto& b : boxes)
    {
        b->Update(dt);
        b->Draw(wnd.Gfx());
    }
    wnd.Gfx().EndFrame();
}