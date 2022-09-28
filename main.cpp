#include <iostream>
#include "tree.h"

int main() {

    cell a({{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', ' '}});
    std::cout << a;

    STRING str;

    str << a << a;
    std::cout << str;
    std::cout << a;

    str.newRow().switchRow(1) << a;
    std::cout << str;
    return 0;
}
