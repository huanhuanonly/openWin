#include <iostream>
#include <fstream>

#include "../include/Win.h"
#include "../include/Cur.h"
#include "../include/pg_Linear.h"

extern int test_move();
extern int test_move2();

int main()
{
    setlocale(LC_ALL, "chs");

    // return test_move();
    return test_move2();
}