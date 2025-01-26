#include <openWin.h>

using namespace win;

int main()
{
    Win win = Win::fromForeground();

    std::cout << win << '\n' << std::endl;

    pg::Linear<Point> linear(1.00F, 0);

    while (true)
    {
        int x, y;
        std::cin >> x >> y;

        std::cout << "From " << win.pos() << '\n';

        win.moveTo(x, y, linear);

        std::cout << "To " << win.pos() << '\n';

        std::cout << std::endl;
    }

    return 0;
}