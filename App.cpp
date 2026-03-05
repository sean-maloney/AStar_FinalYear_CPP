#include "App.h"
#include "AStar.h"
#include <vector>
static const int CELL = 25;
bool App::Create(HINSTANCE hInstance) {
    hInst_ = hInstance;
    WNDCLASS wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst_;
    wc.lpszClassName = L"AStarWindow";
    RegisterClass(&wc);
    hwnd_ = CreateWindowEx(
        0, L"AStarWindow", L"A* Parking Demo",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 900, 650,
        nullptr, nullptr, hInst_, this
    );
    if (!hwnd_) return false;
    ShowWindow(hwnd_, SW_SHOW);
    return true;
}
int App::Run() {
    MSG msg{};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    App* app;
    if (msg == WM_NCCREATE) {
        app = static_cast<App*>(((CREATESTRUCT*)lParam)->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)app);
        return TRUE;
    }
    app = (App*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (!app) return DefWindowProc(hwnd, msg, wParam, lParam);
    switch (msg) {
    case WM_PAINT:
        app->OnPaint();
        return 0;
    case WM_LBUTTONDOWN:
        app->OnClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    case WM_KEYDOWN:
        app->OnKeyDown(wParam);
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
void App::OnPaint() {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd_, &ps);
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HGDIOBJ oldPen = SelectObject(hdc, gridPen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
    HBRUSH borderBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
    for (int r = 0; r < grid_.rows(); r++) {
        for (int c = 0; c < grid_.cols(); c++) {
            RECT rect{ c * CELL, r * CELL, c * CELL + CELL, r * CELL + CELL };
            HBRUSH fill = CreateSolidBrush(grid_.cellColor(r, c));
            FillRect(hdc, &rect, fill);
            DeleteObject(fill);
            FrameRect(hdc, &rect, borderBrush);
        }
    }
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);
    EndPaint(hwnd_, &ps);
}
void App::OnClick(int x, int y) {
    int c = x / CELL;
    int r = y / CELL;
    if (!grid_.inBounds(r, c)) return;
    if (mode_ == Start) grid_.setStart(r, c);
    else if (mode_ == Goal) grid_.toggleGoal(r, c);
    else if (mode_ == Obstacle) grid_.toggleObstacle(r, c);
    else if (mode_ == Erase) grid_.eraseCell(r, c);
}
void App::OnKeyDown(WPARAM key) {
    if (key == 'S') mode_ = Start;
    if (key == 'G') mode_ = Goal;
    if (key == 'O') mode_ = Obstacle;
    if (key == 'E') mode_ = Erase;
    if (key == VK_SPACE) {
        auto res = FindPathToClosestGoal(grid_);
        grid_.clearPath();
        if (res.found) grid_.applyPath(res.path);
    }
}