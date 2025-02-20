#include <openWin.h>

using namespace win;

int main()
{
    ds::LinkedList<int> list;

    list.append(1);
    list.append(2);
    list.insert(list.begin(), 3);

    for (const auto& i : list)
    {
        std::cout << i << ' ';
    }

    std::cout.put('\n');

    std::cout << std::boolalpha << (list.clone() == ds::LinkedList<int>{3, 1, 2});

    return 0;
}