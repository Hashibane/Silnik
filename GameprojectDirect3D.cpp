// GameprojectDirect3D.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "GameprojectDirect3D.h"
#include "Window.h"
#include <sstream>
#include "App.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    try
    {
        return App{}.Start();
    }




    //exception handling
    catch (const Exception& e)
    {
        MessageBox(NULL, reinterpret_cast<LPCWSTR>(e.what()), reinterpret_cast<LPCWSTR>(e.GetType()), MB_OK | MB_ICONEXCLAMATION );
    }
    catch (const std::exception& e)
    {
        MessageBox(NULL, reinterpret_cast<LPCWSTR>(e.what()), L"Standard exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(NULL, L"Nie wiem co sie stalo", L"Unknown exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return 2;
}