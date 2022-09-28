//
// Created by d3Gaff on 20.09.2022.
//

#include "tree.h"

std::ostream &operator<<(std::ostream &stream, const STRING &c) {
    for (const auto &str: c.strings) {
        stream << str << '\n';
    }
    return stream;
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
} //TODO: ? ??? ? ? line  ???????? ????, ?? ???????? ??????