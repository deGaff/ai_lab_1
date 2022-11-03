#include <iostream>
#include <fstream>
#include "tree.h"
#include <stack>

int main() {

    cell a({    {'1', '2', '3'},
                        {'4', '5', '6'},
                        {'7', '8', ' '}}),

            c({ {' ', '2', '3'},
                        {'1', '5', '6'},
                        {'4', '7', '8'}});

//    tree::turnBasedBFS(a, c);

//    tree::BFS(a, c, std::cout);
//    pause();
//
//    std::ofstream fstr("t.txt");
//    tree::limitedDFS(a, c, 4, fstr);
//
//    tree::turnBasedLimitedDFS(a, c, 4);

//    std::cout << a.getManhattanDist(c);
//    pause();

    menu m;

    m.use();


//    AStar<comparator_h1>(a, c, std::cout);
//    AStar<comparator_h1>(a, c, fstr);
//    pause();

    return 0;
}
