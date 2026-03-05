#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include "Grid.h"
class App {
public:
    bool Create(HINSTANCE hInstance);
    int Run();
private:
    HWND hwnd_ = nullptr;
    HINSTANCE hInst_ = nullptr;
    Grid grid_;
    enum Mode { Start, Goal, Obstacle, Erase };
    Mode mode_ = Obstacle;
    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
    void OnPaint();
    void OnClick(int x, int y);
    void OnKeyDown(WPARAM key);
};