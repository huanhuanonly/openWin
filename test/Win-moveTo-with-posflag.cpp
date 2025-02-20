#include <openWin.h>

using namespace win;

int main()
{
    Win win = Win::currentForegroundWindow();

    std::cout << win << std::endl;

    pg::Linear<Point> linear(0.25F);

    win.moveTo(Win::TopLeftCorner, 0, linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::BottomRightCorner, 0, linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::Center, 0, linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::BottomLeftCorner, 0, linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::TopRightCorner, 0, linear);
    std::cout << win.rect() << std::endl;

    return 0;
}