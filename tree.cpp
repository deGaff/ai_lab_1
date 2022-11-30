//
// Created by d3Gaff on 20.09.2022.
//

#include "tree.h"
#include <queue>
#include <stack>
#include <unordered_set>
#include <fstream>
#include <conio.h>

char pause() {
    std::string dump;
    char c = getch();
    system("cls");
    return c;
}

cell::cell() {
    numbers.resize(3, std::vector<char>(3));
    number_coord.resize(9);
}

std::ostream &operator<<(std::ostream &stream, const STRING &c) {
    for (const auto &str: c.strings) {
        stream << str << std::endl;
    }
    return stream;
}

STRING& STRING::addSpacing(const std::string& mes = mes::spacing) {
    size_t i = 0;
    for(;i < 2; ++i) strings[i] += mes::spacing;
    strings[i++] += mes;
    for(;i < 5; ++i) strings[i] += mes::spacing;

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

std::istream& operator>>(std::istream& stream, cell& c) {
    for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
            auto& ref = c.getNumber({i, j});
            stream >> ref;

            if(ref == '0') {
                c.empty = {i, j};
                c.getNumbCoord(0) = {i, j};
                ref = ' ';
            }
            else {
                c.getNumbCoord(ref - '0') = {i, j};
            }
        }
    }
    return stream;
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
    child.getNumbCoord(0) = {emptyCoord.first+r_shift, emptyCoord.second+c_shift};
    child.getNumbCoord(child.getNumber({emptyCoord.first+r_shift,
                                        emptyCoord.second+c_shift}) - '0') = emptyCoord;
    child.getNumber(emptyCoord) = child.getNumber({emptyCoord.first+r_shift, emptyCoord.second+c_shift});
    emptyCoord.first += r_shift;
    emptyCoord.second += c_shift;
    child.getNumber(emptyCoord) = ' ';

    return child;
}

cell::cell(CELL  numbers) : numbers(std::move(numbers)), empty({0, 0}), number_coord(9) {

    bool found_empty = false;
    for(size_t i = 0; i < 3; ++i) {
        for(size_t j = 0; j < 3; ++j) {
            if(this->numbers[i][j] == ' ') found_empty = true;

            if(this->numbers[i][j] == ' ') number_coord[0] = {i, j};
            else number_coord[this->numbers[i][j] - '0'] = {i, j};

            if(!found_empty) ++empty.second;
        }
        if(!found_empty) {
            empty.second = 0;
            ++empty.first;
        }
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

void tree::turnBasedRoutine(STRING& str, std::ostream& out, bool isTurnBased) {
    if(isTurnBased) {
        out << str;
        pause();
    }
}

void tree::BFS(const cell &original_state, const cell &target_state, bool isTurnBased, std::ostream& out) {

    auto start = sc::system_clock::now();

    STRING str;
    std::unordered_set<cell, cell_hash> states;
    size_t iter = 0, count = 0;

    std::queue<std::shared_ptr<tree::node>> q;
    q.push(std::make_shared<tree::node>(original_state));
    states.insert(original_state);

    while(!q.empty()) {
        ++iter;
        str.clear();

        auto current = q.front();

        str << current->state;
        turnBasedRoutine(str, out, isTurnBased);

        if (current->state == target_state) {
            str.addSpacing(mes::goal_state);
            out << str;
            out << mes::depthspc << current->depth << std::endl;


            out <<  mes::nodesspc << count << std::endl <<
                    mes::iterspc << iter << std::endl;
            auto end = sc::system_clock::now();
            auto duration = sc::duration_cast<sc::milliseconds>(end - start);
            if(profiling) {
                out << mes::time_spent << duration.count() << " ms" << std::endl;
            }

            return;
        }

        str.addSpacing(mes::children);

        for (auto &performed_action: tree::actions) {
            if (performed_action.isPossible(current->state)) {

                auto ptr = std::make_shared<tree::node>(current, performed_action);
                str << ptr->state;

                if(!(states.insert(ptr->state).second)) {
                    str.addSpacing(mes::visited);
                }
                else {
                    ++count;
                    q.push(ptr);
                }

                turnBasedRoutine(str, out, isTurnBased);
            }
        }
        if(!isTurnBased) out << str;
        q.pop();
    }

    str.addSpacing(mes::no_goal_state);
    out << str;

    out <<  mes::nodesspc << count << std::endl <<
        mes::iterspc << iter << std::endl;
    auto end = sc::system_clock::now();
    auto duration = sc::duration_cast<sc::milliseconds>(end - start);
    if(profiling) {
        out << mes::time_spent << duration.count() << " ms" << std::endl;
    }
}

void tree::limitedDFS(const cell &original_state, const cell &target_state,
                bool isTurnBased, size_t limited, std::ostream& out) {

    auto start = sc::system_clock::now();

    STRING str;
    std::unordered_set<cell, cell_hash> states;
    size_t iter = 0, count = 0;

    std::stack<std::shared_ptr<tree::node>> s;
    s.push(std::make_shared<tree::node>(original_state));

    while(!s.empty()) {
        ++iter;
        str.clear();

        auto current = s.top();

        str << current->state;
        turnBasedRoutine(str, out, isTurnBased);
        s.pop();

        if (current->state == target_state) {
            str.addSpacing(mes::goal_state);
            out << str;
            out << mes::depthspc << current->depth << std::endl;

            out <<  mes::nodesspc << count << std::endl <<
                mes::iterspc << iter << std::endl;
            auto end = sc::system_clock::now();
            auto duration = sc::duration_cast<sc::milliseconds>(end - start);
            if(profiling) {
                out << mes::time_spent << duration.count() << " ms" << std::endl;
            }

            return;
        }


        if(!(states.insert(current->state).second)) {
            str.addSpacing(mes::visited);
            turnBasedRoutine(str, out, isTurnBased);
            out << str;
            continue;
        }
        if(current->depth == limited) {
            str.addSpacing(mes::depth_limit);
            turnBasedRoutine(str, out, isTurnBased);
            out << str;
            continue;
        }

        str.addSpacing(mes::children);

        for (auto &performed_action: tree::actions) {
            if (performed_action.isPossible(current->state)) {

                auto ptr = std::make_shared<tree::node>(current, performed_action);
                str << ptr->state;
                s.push(ptr);

                ++count;

                turnBasedRoutine(str, out, isTurnBased);
            }
        }
        if(!isTurnBased) out << str;
    }
    str.addSpacing(mes::no_goal_state);
    out << str;

    out <<  mes::nodesspc << count << std::endl <<
        mes::iterspc << iter << std::endl;
    auto end = sc::system_clock::now();
    auto duration = sc::duration_cast<sc::milliseconds>(end - start);
    if(profiling) {
        out << mes::time_spent << duration.count() << " ms" << std::endl;
    }
}

void menu::print() {
    std::cout <<
                "\t8PUZZLE SOLVER\n" <<
                "1. Enter original state\n" <<
                "2. Enter target state\n";

    if(original.first && target.first)
        std::cout <<
                "________________________________________________________\n" <<
                "3. Solve with BFS: Turn Based\n" <<
                "4. Solve with BFS: into \"output_bfs.txt\" file\n" <<
                "5. Solve with BFS: at once into console\n" <<
                "________________________________________________________\n" <<
                "6. Solve with limited DFS: Turn Based\n" <<
                "7. Solve with limited DFS: into \"output_dfs.txt\" file\n" <<
                "8. Solve with limited DFS: at once into console\n" <<
                "________________________________________________________\n" <<
                "9. Solve with A* (first heuristic func): Turn Based\n" <<
                "q. Solve with A* (first heuristic func): into \"output_astar1.txt\" file\n" <<
                "w. Solve with A* (first heuristic func): at once into console\n" <<
                "________________________________________________________\n" <<
                "e. Solve with A* (second heuristic func): Turn Based\n" <<
                "r. Solve with A* (second heuristic func): into \"output_astar2.txt\" file\n" <<
                "t. Solve with A* (second heuristic func): at once into console\n" <<
                "________________________________________________________\n" <<
                "y. Turn " << ((profiling)?("off"):("on")) << " time profiling\n" <<
                "________________________________________________________\n";
    std::cout << "0. Exit\n";
}

void menu::use() {
    unsigned limit = 0;
    std::ofstream o_bfs("output_bfs.txt"),
            o_dfs("output_dfs.txt"),
            o_star1("output_astar1.txt"),
            o_star2("output_astar2.txt");

    while (true) {
        print();
        if(original.first && target.first) {
            switch(pause()) {
                case '1':
                    std::cout << "Enter original state (0 is empty space)\n\n";
                    std::cin >> original.second;
                    original.first = true;
                    std::cout << "\nEntered state:\n" << original.second;
                    break;
                case '2':
                    std::cout << "Enter target state (0 is empty space)\n\n";
                    std::cin >> target.second;
                    target.first = true;
                    std::cout << "\nEntered state:\n" << target.second;
                    break;
                case '3':
                    tree::BFS(original.second, target.second, true, std::cout);
                    break;
                case '4':
                    tree::BFS(original.second, target.second, false, o_bfs);
                    std::cout << "Puzzle was solved. Results are in \"output_bfs.txt\" file\n";
                    break;
                case '5':
                    tree::BFS(original.second, target.second, false, std::cout);
                    break;
                case '6':
                    std::cout << "Enter limit\n";
                    std::cin >> limit;
                    tree::limitedDFS(original.second, target.second, true, limit, std::cout);
                    break;
                case '7':
                    std::cout << "Enter limit\n";
                    std::cin >> limit;
                    tree::limitedDFS(original.second, target.second, false, limit, o_dfs);
                    std::cout << "Puzzle was solved. Results are in \"output_dfs.txt\" file\n";
                    break;
                case '8':
                    std::cout << "Enter limit\n";
                    std::cin >> limit;
                    tree::limitedDFS(original.second, target.second, false, limit, std::cout);
                    break;
                case '9':
                    tree::AStar<comparator_h1>(original.second, target.second, true, std::cout);
                    break;
                case 'q':
                    tree::AStar<comparator_h1>(original.second, target.second, false, o_star1);
                    std::cout << "Puzzle was solved. Results are in \"output_astar1.txt\" file\n";
                    break;
                case 'w':
                    tree::AStar<comparator_h1>(original.second, target.second, false, std::cout);
                    break;
                case 'e':
                    tree::AStar<comparator_h2>(original.second, target.second, true, std::cout);
                    break;
                case 'r':
                    tree::AStar<comparator_h2>(original.second, target.second, false, o_star2);
                    std::cout << "Puzzle was solved. Results are in \"output_astar2.txt\" file\n";
                    break;
                case 't':
                    tree::AStar<comparator_h2>(original.second, target.second, false, std::cout);
                    break;
                case 'y':
                    profiling = (profiling+1)%2;
                    std::cout << "Profiling was turned " << ((profiling)?("on"):("off"));
                    break;
                case '0':
                    o_bfs.close();
                    o_dfs.close();
                    o_star1.close();
                    o_star2.close();
                    exit(0);
                default:
                    std::cout << "Wrong symbol\n";
            }
        }
        else {
            switch(pause()) {
                case '1':
                    std::cout << "Enter original state (0 is empty space)\n\n";
                    std::cin >> original.second;
                    original.first = true;
                    std::cout << "\nEntered state:\n" << original.second;
                    break;
                case '2':
                    std::cout << "Enter target state (0 is empty space)\n\n";
                    std::cin >> target.second;
                    target.first = true;
                    std::cout << "\nEntered state:\n" << target.second;
                    break;
                case '0':
                    exit(0);
                    o_bfs.close();
                    o_dfs.close();
                default:
                    std::cout << "Wrong symbol\n";
            }
        }
        pause();
    }

}

std::pair<size_t, size_t>& cell::getNumbCoord(size_t n) {
    return number_coord[n];
}

size_t cell::getManhattanDist(const cell& oth) const {
    size_t count = 0;

    for (size_t i = 0; i < 9; ++i) {
        count += abs((int)number_coord[i].first - (int)oth.number_coord[i].first) +
                abs((int)number_coord[i].second - (int)oth.number_coord[i].second);
    }
    return count;
}


size_t h1(const cell &cur) {
    size_t count = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if(cur.getNumber({i, j}) != goal.getNumber({i, j})) ++count;
        }
    }
    return count;
}

size_t h2(const cell& cur) {
    return cur.getManhattanDist(goal);
}

bool comparator_h1::operator()(std::shared_ptr<tree::node> lhs,
                               std::shared_ptr<tree::node> rhs) const {
    return lhs->depth + h1(lhs->state) > rhs->depth + h1(rhs->state);
}

bool comparator_h2::operator()(std::shared_ptr<tree::node> lhs,
                               std::shared_ptr<tree::node> rhs) const {
    return lhs->depth + h2(lhs->state) > rhs->depth + h2(rhs->state);
}