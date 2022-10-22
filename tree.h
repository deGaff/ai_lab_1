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
#include <unordered_set>
#include <queue>


#define ONE_TURN_PATH_COST 1

namespace mes {
    static std::string spacing =        "             ";
    static std::string children =       "  CHILDREN:  ";
    static std::string queue =          "    QUEUE:   ";
    static std::string visited =        " VISITED NODE";
    static std::string no_goal_state =  " GOAL STATE NOT FOUND ";
    static std::string depth_limit =    "  DEPTH LIMIT REACHED ";
    static std::string goal_state =     "<-- GOAL STATE";
    static std::string longline =       "_______________________________________________________________________\n";
}

char pause();

class STRING {
public:
    STRING() : strings(5) {}
    friend std::ostream& operator<<(std::ostream& stream, const STRING& c);
    std::string& line(size_t i) { return strings[i];}
    STRING& addSpacing(const std::string& mes);
    void clear();
private:
    std::vector<std::string> strings;
};

class cell {
    using CELL = std::vector<std::vector<char>>;
public:
    cell();
    cell(CELL  numbers);

    cell(const cell& c) = default;

    std::pair<size_t, size_t>& getEmpty() { return empty; }
    std::pair<size_t, size_t> getEmpty() const { return empty; }

    bool operator==(const cell& oth) const;

    char& getNumber(std::pair<size_t, size_t> rowColumn) { return numbers[rowColumn.first][rowColumn.second]; }
    char getNumber(std::pair<size_t, size_t> rowColumn) const { return numbers[rowColumn.first][rowColumn.second]; }

    friend std::ostream& operator<<(std::ostream& stream, const cell& c);
    friend STRING& operator<<(STRING& strings, const cell& c);
    friend std::istream& operator>>(std::istream& stream, cell& c);

    std::pair<size_t, size_t>& getNumbCoord(size_t n);
    size_t getManhattanDist(const cell& oth) const;

private:
    CELL numbers;
    std::pair<size_t, size_t> empty;
    std::vector<std::pair<size_t, size_t>> number_coord;
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

    void turnBasedBFS(const cell& original_state, const cell& target_state);
    void BFS(const cell &original_state, const cell &target_state, std::ostream& out);

    void limitedDFS(const cell& original_state, const cell& target_state, size_t limited, std::ostream& out);
    void turnBasedLimitedDFS(const cell& original_state, const cell& target_state, size_t limited);
}

class menu {
public:
    void print();
    void use();
private:
    std::pair<bool, cell> original = {false, cell()},
                            target  = {false, cell()};
};

static cell goal({  {' ', '2', '3'},
                    {'1', '5', '6'},
                    {'4', '7', '8'}});

size_t h1(const cell &cur);
size_t h2(const cell &cur);

class comparator_h1 {
public:
    bool operator()(std::shared_ptr<tree::node> lhs,
                    std::shared_ptr<tree::node> rhs) const;
};

class comparator_h2 {
public:
    bool operator()(std::shared_ptr<tree::node> lhs,
                    std::shared_ptr<tree::node> rhs) const;
};

template <typename comparator>
void AStar(const cell &original_state, const cell &target_state, std::ostream& out) {
    STRING str;

    std::unordered_set<cell, cell_hash> open_list, closed_list;
    out << mes::longline;

    std::priority_queue<std::shared_ptr<tree::node>, std::vector<std::shared_ptr<tree::node>>, comparator> q;
    q.push(std::make_shared<tree::node>(original_state));
    open_list.insert(q.top()->state);

    while(!q.empty()) {
        str.clear();

        auto current = q.top();
        q.pop();

        str << current->state;
        str.addSpacing(mes::children);

        open_list.erase(current->state);
        closed_list.insert(current->state);

        if (current->state == target_state) {
            str.addSpacing(mes::goal_state);
            out << str;
            out << mes::longline;
            return;
        }

        for (auto &performed_action: tree::actions) {
            if (performed_action.isPossible(current->state)) {
                auto ptr = std::make_shared<tree::node>(current, performed_action);
                if(closed_list.find(ptr->state) == closed_list.end()
                    && open_list.insert(ptr->state).second) {
                    q.push(ptr);
                    str << ptr->state;
                }
            }
        }

        out << str;
    }
    str.addSpacing(mes::no_goal_state);
    out << str;
    out << mes::longline;
}

#endif //INC_1_TREE_H
