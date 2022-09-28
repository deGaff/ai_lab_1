//
// Created by d3Gaff on 20.09.2022.
//

#ifndef INC_1_TREE_H
#define INC_1_TREE_H
#include <vector>
#include <iostream>
#include <string>

class STRING {
public:

    //TODO add spacing
    STRING() : strings(5) {}
    friend std::ostream& operator<<(std::ostream& stream, const STRING& c);
    std::string& line(size_t i) { return strings[i];}
    STRING& newRow();
    STRING& switchRow(size_t n) { this->n = n; return *this;}
    size_t curRow() { return n; }
private:
    std::vector<std::string> strings;
    size_t length = 0,
            n = 0;
};

class cell {
    using CELL = std::vector<std::vector<char>>;
public:
    cell(CELL&& numbers) : numbers(move(numbers)) {}
    friend std::ostream& operator<<(std::ostream& stream, const cell& c);
    friend STRING& operator<<(STRING& strings, const cell& c);
private:
    const CELL numbers;
};


#endif //INC_1_TREE_H
