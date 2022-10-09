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

//    cell a({    {'7', '4', '2'},
//                        {'3', '5', '8'},
//                        {'6', ' ', '1'}}),
//
//            b({ {'1', '2', '3'},
//                        {'4', '5', '6'},
//                        {'7', ' ', '8'}}),
//
//            c({ {'1', '2', '3'},
//                        {'4', ' ', '5'},
//                        {'6', '7', '8'}});



// STRING tests
//    std::cout << a;
//
//    STRING str;
//
//    str << a << a;
//    std::cout << str;
//
//    str.newRow().switchRow(1) << a;
//    std::cout << str;

// tree::node tests
//    std::shared_ptr<tree::node> head = std::make_shared<tree::node>(a);
//    tree::node child(head, tree::UP);
//
//    std::cout << child.parent->state << '\n' << child.state;

// bfs test
// std::cout << tree::BFS(a, b)
    tree::turnBasedBFS(a, c);

    //    std::cout << (a==c) << (a==a);
    return 0;
}
