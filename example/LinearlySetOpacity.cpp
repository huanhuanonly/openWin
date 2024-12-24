#include <utility>

#include "../include/Win.h"
#include "../include/Cur.h"
#include "../include/pg_Linear.h"

int main()
{
    pg::Linear<int, int, 1> linear(1.00f, 2);

#if 1
    for (Win last, cur; ; last = std::exchange(cur, Win::fromForeground()))
#else
    for (Win last, cur; ; last = std::exchange(cur, Win::fromPoint(Cur::pos())))
#endif
    {
        if (cur != last)
        {
            cur.setOpacity(255, &linear);
            
            last = cur;
        }

        Win::wait(200);
    }

    return 0;
}