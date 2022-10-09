#include <iostream>
#include <fstream>
#include "tree.h"

int main() {

    cell a({    {'1', '2', '3'},
                        {'4', '5', '6'},
                        {'7', '8', ' '}}),

            c({ {' ', '2', '3'},
                        {'1', '5', '6'},
                        {'4', '7', '8'}});

    tree::turnBasedBFS(a, c);
    return 0;
}
