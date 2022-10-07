//
// Created by d3Gaff on 20.09.2022.
//

#ifndef INC_1_TREE_H
#define INC_1_TREE_H
#include <utility>
#include <vector>
#include <iostream>
#include <string>
#include <memory>
#include <tuple>

#define ONE_TURN_PATH_COST 1

class STRING {
public:
//    STRING() : strings(5) {}
    STRING(std::ostream& stream) : strings(5), stream(stream) {}
    friend std::ostream& operator<<(std::ostream& stream, const STRING& c);
    std::string& line(size_t i) { return strings[i];}
    STRING& newRow();
    STRING& switchRow(size_t n) { this->n = n; return *this;}
    STRING& incrementRow() { newRow(); ++n; return *this;}
    STRING& dumpRow();
    STRING& addSpacing();
    size_t curRow() { return n; }
private:
    std::vector<std::string> strings;
    std::ostream& stream;
    size_t length = 0,
            n = 0;
};

class cell {
    using CELL = std::vector<std::vector<char>>;
public:
    cell(CELL  numbers);

    cell(const cell& c) = default;

    std::pair<size_t, size_t>& getEmpty() { return empty; }
    std::pair<size_t, size_t> getEmpty() const { return empty; }

    bool operator==(const cell& oth) const;

    char& getNumber(std::pair<size_t, size_t> rowColumn) { return numbers[rowColumn.first][rowColumn.second]; }
    char getNumber(std::pair<size_t, size_t> rowColumn) const { return numbers[rowColumn.first][rowColumn.second]; }

    friend std::ostream& operator<<(std::ostream& stream, const cell& c);
    friend STRING& operator<<(STRING& strings, const cell& c);
private:
    CELL numbers;
    std::pair<size_t, size_t> empty;
};

class cell_hash {
public:
    size_t operator()(const cell& c) const;
private:
    std::vector<std::vector<size_t>> primes =               {{5398739783, 4270020037, 8060744617},
                                                            {1410727537, 4671687281, 8285079581},
                                                            {1711141951, 3069034961, 8243200571}};
};

namespace tree {
    class action {
        int r_shift, c_shift;
    public:
        action(int r_shift, int c_shift) : r_shift(r_shift), c_shift(c_shift) {}

        cell operator()(const cell& c) const;
        bool isPossible(const cell& c) const;
    };

    static action LEFT(0, -1), RIGHT(0, 1),
    DOWN(1, 0), UP(-1, 0), NONE(0, 0);

    static std::vector<action> actions = {LEFT, RIGHT, DOWN, UP};

    struct node {
        node(const cell& state) : parent(nullptr), performed_action(NONE), state(state), path_cost(0), depth(0) {}
        node(std::shared_ptr<node> parent, const action& performed_action) : parent(parent), performed_action(performed_action), state(performed_action(parent->state)),
                path_cost(parent->path_cost + ONE_TURN_PATH_COST), depth(parent->depth + 1) {}

        std::shared_ptr<node> parent;
        const action& performed_action;
        cell state;
        unsigned path_cost, depth;
    };

    STRING BFS(const cell& original_state, const cell& target_state);
}


#endif //INC_1_TREE_H
