#pragma once    //применяется для защиты от «двойного подключения» заголовочных файлов
using namespace std;

#include <memory>
#include <vector>

#include <unordered_map>
#include <unordered_set>

class checker{
public:
    int position;
    bool isBlack;
    bool isQueen;

    checker(int withPosition, bool withIsBlack);
};

class board{
private:
    void moveChecker(checker* checker, int to);
public:
    int numberOfWhiteCheckers = 12;
    int numberOfBlackCheckers = 12;
    bool isBlackTurn = false;
    bool gameHasEnded = false;

    unordered_map<int, checker*> checkers;

    board();
    void print();
    void addChecker(checker* checker);
    bool tryMoveCheck(int from, int to);
    void removeChecker(checker* checker);
    void win();
};

