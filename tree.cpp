//
// Created by d3Gaff on 20.09.2022.
//

#include "tree.h"
#include <queue>
#include <unordered_set>
#include <fstream>

static std::string spacing =  "          ";
static std::string children = " CHILDREN ";

std::ostream &operator<<(std::ostream &stream, const STRING &c) {
    for (const auto &str: c.strings) {
        stream << str << std::endl;
    }
    return stream;
}

STRING& STRING::dumpRow() {
    stream << *this;
    for (auto& i : strings) {
        i = "";
    }
    return *this;
}

STRING& STRING::addSpacing() {
    size_t i = 0;
    for(;i < 2; ++i) strings[i] += spacing;
    strings[++i] += children;
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
    size_t i = strings.curRow() * 5;

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

STRING &STRING::newRow() {
    strings.resize(5 * (++length));
    for (size_t i = 0; i < 5; ++i) {
        strings.emplace_back("");
    }
    return *this;
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

STRING tree::BFS(const cell &original_state, const cell &target_state) {
    std::ofstream fout("output.txt");
    STRING str(fout);
    std::unordered_set<cell, cell_hash> states;

    std::queue<std::shared_ptr<tree::node>> q;
    q.push(std::make_shared<tree::node>(original_state));

    str << q.back()->state;//TODO nemnozhko kostil'
    str.dumpRow();

    while(!q.empty()) {
        auto current = q.front();
        str << current->state;
        str.addSpacing();
        if(!(states.insert(current->state).second)) {
            q.pop();
            continue;
        }
        for(auto& performed_action : tree::actions) {
            if(performed_action.isPossible(current->state)){
                q.push(std::make_shared<tree::node>(current, performed_action));
                str << q.back()->state;
                if (q.back()->state == target_state){
                    return str;
                }
            }
        }
        str.dumpRow();
        q.pop();
    }

    return str;
}