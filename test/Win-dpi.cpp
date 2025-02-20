#include <openWin.h>

using namespace win;

int main()
{
    Win foreground = Win::currentForegroundWindow();

    std::cout << foreground << "\n\n";

    std::cout << foreground.dpi() << '\n';
    std::cout << Win::systemDpi() << '\n';

    return 0;
}