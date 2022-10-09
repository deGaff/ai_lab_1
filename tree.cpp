//
// Created by d3Gaff on 20.09.2022.
//

#include "tree.h"
#include <queue>
#include <unordered_set>
#include <fstream>
#include <conio.h>

static std::string spacing =     "             ";
static std::string children =    "  CHILDREN:  ";
static std::string visited =     " VISITED NODE";
static std::string no_goal_state = " GOAL STATE NOT FOUND ";
static std::string goal_state = "<-- GOAL STATE";

//todo make not turn based & file dump one
//todo make STRING namespace (and rename perhaps)

void pause() {
    std::string dump;
    getch();
    system("cls");
}

std::ostream &operator<<(std::ostream &stream, const STRING &c) {
    for (const auto &str: c.strings) {
        stream << str << std::endl;
    }
    return stream;
}

STRING& STRING::addSpacing(const std::string& mes = spacing) {
    size_t i = 0;
    for(;i < 2; ++i) strings[i] += spacing;
    strings[i++] += mes;
    for(;i < 5; ++i) strings[i] += spacing;

    return *this;
}

std::ostream &operator<<(std::ostream &stream, const cell &c) {
    for (const auto &line: c.numbers) {
        for (const auto &number: line) {
            stream << number << ' ';
        }
        stream << '\n';
    }
    return stream;
}

STRING &operator<<(STRING &strings, const cell &c) {
    size_t i = 0;

    strings.line(i) += "_________   ";
    for (const auto &row: c.numbers) {
        strings.line(++i) += "| ";
        for (auto number: row) {
            strings.line(i) += number;
            strings.line(i) += ' ';
        }
        strings.line(i) += "|   ";
    }
    strings.line(++i) += "_________   ";

    return strings;
}

void STRING::clear() {
    for (auto& line : strings) {
        line = "";
    }
}


bool tree::action::isPossible(const cell &c) const {
    auto emptyCoord = c.getEmpty();
    size_t new_first = emptyCoord.first + r_shift,
            new_second = emptyCoord.second + c_shift;
    return (new_first >= 0 && new_first <= 2) && (new_second >= 0 && new_second <= 2);
}

bool cell::operator==(const cell& oth) const {
    for(size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            if(numbers[i][j] != oth.numbers[i][j]) return false;
        }
    }
    return true;
}

cell tree::action::operator()(const cell& c) const {
    cell child(c);

    auto& emptyCoord = child.getEmpty();
    child.getNumber(emptyCoord) = child.getNumber({emptyCoord.first+r_shift, emptyCoord.second+c_shift});
    emptyCoord.first += r_shift;
    emptyCoord.second += c_shift;
    child.getNumber(emptyCoord) = ' ';

    return child;
}

cell::cell(CELL  numbers) : numbers(std::move(numbers)), empty({0, 0}) {
    for (const auto& row : this->numbers) {
        for (auto number : row) {
            if(number == ' ') return;
            ++empty.second;
        }
        empty.second = 0;
        ++empty.first;
    }
}

size_t cell_hash::operator()(const cell &c) const {
    size_t result = 0;
    for (size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            result += primes[i][j] * c.getNumber({i, j});
        }
    }
    return result;
}

void tree::turnBasedBFS(const cell &original_state, const cell &target_state) {
//    std::ofstream fout("output.txt");
//    STRING str(fout);

    STRING str;
    std::unordered_set<cell, cell_hash> states;

    std::queue<std::shared_ptr<tree::node>> q;
    q.push(std::make_shared<tree::node>(original_state));

    while(!q.empty()) {
        str.clear();

        auto current = q.front();

        str << current->state;
        str.addSpacing(children);
        std::cout << str;
        pause();

        if (!(states.insert(current->state).second)) {
            q.pop();
            str.addSpacing(visited);
            std::cout << str;
            pause();
            continue;
        }
        for (auto &performed_action: tree::actions) {
            if (performed_action.isPossible(current->state)) {
                //todo is it needed to first push the node that was already visited?
                q.push(std::make_shared<tree::node>(current, performed_action));

                str << q.back()->state;
                std::cout << str;
                pause();

                if (q.back()->state == target_state) {
                    str.addSpacing(goal_state);
                    std::cout << str;
                    pause();
                    return;
                }
            }
        }
        q.pop();
    }
    str.addSpacing(no_goal_state);
    std::cout << str;
    pause();
}