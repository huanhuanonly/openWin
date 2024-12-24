#include <iostream>
#include <fstream>

#include "../include/Win.h"
#include "../include/Cur.h"
#include "../include/pg/Linear.h"

int test_move()
{
    Win win = Win::fromForeground();

    std::cout << win << '\n' << std::endl;

    pg::Linear<Win::Point, int, 2> linear(1.00F, 0);

    while (true)
    {
        int x, y;
        std::cin >> x >> y;

        std::cout << "From " << win.pos() << '\n';

        win.moveTo(x, y, &linear);

        std::cout << "To " << win.pos() << '\n';

        std::cout << std::endl;
    }

    return 0;
}

int test_move2()
{
    Win win = Win::fromForeground();

    std::cout << win << std::endl;

    pg::Linear<Win::Point, int, 2> linear(0.25F);

    win.moveTo(Win::TopLeftCorner, 0, &linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::BottomRightCorner, 0, &linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::Center, 0, &linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::BottomLeftCorner, 0, &linear);
    std::cout << win.rect() << std::endl;

    win.moveTo(Win::TopRightCorner, 0, &linear);
    std::cout << win.rect() << std::endl;

    return 0;
}