#include <iostream>
#include <array>
#include <ctime>
#include "game.h"

checker::checker(int withPosition, bool withIsBlack){
    position = withPosition;
    isBlack = withIsBlack;
    isQueen = false;
}

void board::addChecker(checker* newChecker) {
    checkers[newChecker->position] = newChecker;
}

//ставит фишки на доску

board::board(){
    for (int i = 0; i <= 2; ++i) {
        for (int j = (i%2 == 0 ? 0: 1); j <= 7; j+=2) {
            auto _ = new checker(i*10 + j, false);
            addChecker(_);
        }
    }

    for (int i = 5; i <= 7; ++i) {
        for (int j = (i%2 == 0 ? 0: 1); j <= 7; j+=2) {
            auto _ = new checker(i*10 + j, true);
            addChecker(_);
        }
    }
}

bool board::checkPositionEmpty(int position) {
    if(checkPositionOnMap(position)){
        if(checkers.find(position) == checkers.end()){
            return true;
        }
    }
    return false;
}

bool board::checkPositionOnMap(int position) {
    return position / 10 <= 7 && position / 10 >= 0 && position % 10 <= 7 && position % 10 >= 0;
}

void board::print() {
    array<string, 9> numbers = {
            "0 ",
            "1 ",
            "2 ",
            "3 ",
            "4 ",
            "5 ",
            "6 ",
            "7 "
    };
    cout << "NW";
    for (int i = 0; i <= 7; ++i) {
        cout << numbers[i];
    }
    cout << "NE" << endl;

    for (int i = 0; i <= 7; ++i) {
        cout << "  " << numbers[i];
        for (int j = 0; j <= 7; ++j) {
            if(checkers.find(i*10 + j) != checkers.end()){
                if(checkers[i*10 + j]->isQueen){
                    if(checkers[i*10 + j]->isBlack){
                        cout << "B";
                    }else{
                        cout << "W";
                    }
                }else{
                    if(checkers[i*10 + j]->isBlack){
                        cout << "b";
                    }else{
                        cout << "w";
                    }
                }
            }else{
                if(i%2 == 1){
                    j%2 == 0 ? cout << "  ": cout << ".";
                }else{
                    j%2 == 0 ? cout << ".": cout << "  ";
                }
            }
        }
        cout << numbers[i];
        cout << endl;
    }

    cout << "SW";
    for (int i = 0; i <= 7; ++i) {
        cout << numbers[i];
    }
    cout << "SE" << endl;
}

void board::moveChecker(checker *checkerToMove, int to) {
    checkerToMove->isBlack ? cout << "Black moved from " : cout << "White moved from ";
    cout << checkerToMove->position << " to " << to << endl;

    checkers.erase(checkerToMove->position);
    checkerToMove->position = to;
    if((checkerToMove->isBlack && to / 10 == 0) || (!checkerToMove->isBlack && to / 10 == 7)){
        checkerToMove->isQueen=true;
    }
    addChecker(checkerToMove);

    array<int, 4> directions = {11, -11, 9, -9};

    for (auto& it: checkers) {
        checker *curChecker = it.second;
        curChecker->eat_positions.clear();
//        cout << "redoing eat positions for checker at " << curChecker->position << endl;
        if(curChecker->isQueen){

            int positionIterator;

            for (int i = 0; i < 4; ++i) {
                int direction = directions[i];
                positionIterator = curChecker->position + direction;
                while (checkPositionOnMap(positionIterator)){
                    if(checkers.find(positionIterator) != checkers.end()){
                        if(checkers[positionIterator]->isBlack != curChecker->isBlack){
                            if(checkPositionEmpty(positionIterator + direction)){
                                curChecker->eat_positions.insert(positionIterator);
                            }
                        }
                        break;
                    }

                    positionIterator+=direction;
                }
            }
        }else{
            for (int i = 0; i < 4; ++i) {
                int direction = directions[i];

                if(checkers.find(curChecker->position + direction) != checkers.end()){
                    if(curChecker->isBlack != checkers[curChecker->position + direction]->isBlack){
                        if(checkPositionEmpty(curChecker->position + direction + direction)){
                            curChecker->eat_positions.insert(curChecker->position + direction);
                        }
                    }
                }
            }
        }
        checkers[curChecker->position] = curChecker;
    }
//    cout << "moved" << endl;
}

bool board::tryMoveCheck(int from, int to) {
    if(checkers.find(from) == checkers.end()){
        cout << "Wait, no checker at " << from << endl;
        return false;
    }
    checker* checker = checkers[from];
    if(to/10 > 7 || to%10 > 7 || to/10 < 0 || to%10 < 0){
        cout << "you are trying to go out of map" << endl;
        return false;
    }
    if(checkers[from]->isBlack != isBlackTurn){
        cout << "its not your turn" << endl;
        return false;
    }

    //check all eat_positions

    vector<int> myCheckers;

    for (auto& it: checkers) {
        if(it.second->isBlack == isBlackTurn){
            myCheckers.push_back(it.second->position);
        }
    }

    bool toIsInEatPositions = false;
    bool eatPositionsAreEmpty = true;

    for (int & myChecker : myCheckers) {
        if(!checkers[myChecker]->eat_positions.empty()){
            eatPositionsAreEmpty = false;
            if(checker->eat_positions.find(to) != checker->eat_positions.end()){
                toIsInEatPositions = true;
                break;
            }
        }
    }

    if(!eatPositionsAreEmpty){
//        cout << "not empty" << endl;
        if(!toIsInEatPositions){
            cout << "you must eat" << endl;
            return false;
        }
    }

    if(checker->isQueen){
        cout << "trying to move queen from " << from << " to " << to << endl;
        if(abs(from/10 - to/10) == abs(from%10 - to%10)){
            int pathLength = abs(from/10 - to/10);
            unordered_set<int> obstacleCoordinates;
            int step = (to - from)/pathLength;

            for (int i = from + step; i != to+step; i+=step) {  //итерируемся по пути и считаем преграды
                if(checkers.find(i) != checkers.end()){
                    if(checkers[i]->isBlack == checker->isBlack){
                        return false;
                    }
                    obstacleCoordinates.insert(i);
                }
            }

            if(obstacleCoordinates.size() > 1){
                return false;
            }else{
                if(checkers.find(to) != checkers.end()){
                    if(checkPositionEmpty(to + step)){
                        removeChecker(checkers[to]);
                        moveChecker(checker, to+step);
                        return true;
                    }else{
                        return false;
                    }
                }
                moveChecker(checker, to);
                return true;
            }
        }else{
            return false;
        }
    }else {
        if(abs(from - to) == 22 || abs(from - to) == 18){
            to = to - (to-from/2);
        }
        if (abs(from - to) == 11 || abs(from - to) == 9) {
            if (checkers.find(to) != checkers.end()) {
                if (checkers[to]->isBlack == checker->isBlack) {
                    cout << "friendly fire" << endl;
                    return false;
                } else {
                    if(!checkPositionEmpty(from + (to - from) * 2)){
                        cout << "you cant eat that" << endl;
                        return false;
                    }else{
                        cout << "you ate" << endl;
                        removeChecker(checkers[to]);

                        moveChecker(checker, to + (to-from));
                        return true;
                    }
                }
            } else {  //идет на пустую клетку
                if (checker->isBlack) {
                    if(from > to){
                        moveChecker(checker, to);
                        return true;
                    }
                } else {
                    if(to > from){
                        moveChecker(checker, to);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void board::removeChecker(checker *checker) {
    if(checker->isBlack){
        numberOfBlackCheckers--;
        if (numberOfBlackCheckers == 0){
            win();
            cout << "Game has ended, white won!" << endl;
        }
    }else{
        numberOfWhiteCheckers--;
        if (numberOfWhiteCheckers == 0){
            win();
            cout << "Game has ended, black won!" << endl;
        }
    }
    checkers.erase(checker->position);
}

void board::win() {
    gameHasEnded = true;
    print();
}

//проверка введеного значения на число (интернет)
inline bool isInteger(const std::string & s)
{
    if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

void play() {
    board currBoard = board();

    bool moveHappen = false;
    int oldNumberOfCheckers;
    bool lastMoveAte = false;

    while (!currBoard.gameHasEnded){
        cout << "Please make your move" << endl;
        currBoard.print();

        string input;
        cin >> input;

        if(isInteger(input)){
            string input2;
            cin >> input2;

            int inputInt = stoi(input);
            checker *lastMovedChecker = currBoard.checkers[inputInt];

            int input2Int;



            if(isInteger(input2) || input2 == "SW" || input2 == "SE" || input2 == "NE" || input2 == "NW"){
                if(input2 == "SW"){
                    input2Int = inputInt + 9;
                }else if (input2 == "SE"){
                    input2Int = inputInt + 11;
                }else if (input2 == "NE"){
                    input2Int = inputInt - 9;
                }else if (input2 == "NW"){
                    input2Int = inputInt - 11;
                }else{
                    input2Int = stoi(input2);
                }

                bool moveSuccessful = false;

                oldNumberOfCheckers = currBoard.numberOfBlackCheckers + currBoard.numberOfWhiteCheckers;

                if(moveHappen){
                    vector<int> myCheckers;

                    for (auto& it: currBoard.checkers) {
                        if(it.second->isBlack == currBoard.isBlackTurn){
                            myCheckers.push_back(it.second->position);
                        }
                    }

                    bool eatPositionsAreEmpty = true;

                    for (int & myChecker : myCheckers) {
                        if(!currBoard.checkers[myChecker]->eat_positions.empty()){
                            eatPositionsAreEmpty = false;
                        }
                    }
                    if(lastMoveAte && !eatPositionsAreEmpty){
                        moveSuccessful = currBoard.tryMoveCheck(inputInt, input2Int);
                    }else{
                        cout << "Sorry, you are out of moves. Type 'end' to finish turn." << endl;
                    }
                }else{
                    moveSuccessful = currBoard.tryMoveCheck(inputInt, input2Int);
                }

                if(moveSuccessful){
                    moveHappen = true;
                    lastMoveAte = oldNumberOfCheckers != currBoard.numberOfBlackCheckers + currBoard.numberOfWhiteCheckers;
                    vector<int> myCheckers;

                    for (auto& it: currBoard.checkers) {
                        if(it.second->isBlack == currBoard.isBlackTurn){
                            myCheckers.push_back(it.second->position);
                        }
                    }

                    bool eatPositionsAreEmpty = lastMovedChecker->eat_positions.empty();

                    if(eatPositionsAreEmpty || !lastMoveAte){
                        cout << "color changed" << endl;
                        currBoard.isBlackTurn = !currBoard.isBlackTurn;
                        moveHappen = false;
                    }
                }else{
                    cout << "No way you can do this!" << endl;
                }
            }
        }else {cout << "That was strange. Retype it" << endl;}
    }
    cout << "game ended" << endl;
}

void whiteStrategyMakeMove(board* board){
    vector<int> myCheckers;

    srand ( time(NULL) );  //для работы рандомайза (интернет)

    for (auto& pair: board->checkers) {
        if(!pair.second->isBlack){
            myCheckers.push_back(pair.second->position);
        }
    }

    bool moveSuccessful = false;
    while (!moveSuccessful){
        int randIndex = rand() % 4;
        array<int, 4> directions = {11, -11, 9, -9};
        int randDirection = directions[randIndex];
        randIndex = rand() % myCheckers.size();
        int randCheck = myCheckers[randIndex];
        if(board->checkers.find(randCheck + randDirection * 2) != board->checkers.end() &&
           !board->checkers[randCheck + randDirection * 2]->isBlack &&
           (randCheck + randDirection * 2)/10 >= 0 && (randCheck + randDirection * 2)/10 <= 7 &&
           (randCheck + randDirection * 2)%10 >= 0 && (randCheck + randDirection * 2)%10 <= 7
                ){
            moveSuccessful = board->tryMoveCheck(randCheck, randCheck + randDirection * 2);
        }else{
            moveSuccessful = board->tryMoveCheck(randCheck, randCheck + randDirection);
        }
    }
}

void blackStrategyMakeMove(board* board){
    vector<int> myCheckers;

    srand ( time(NULL) );

    for (auto& it: board->checkers) {
        if(it.second->isBlack){
            myCheckers.push_back(it.second->position);
        }
    }

    array<int, 4> directions = {11, -11, 9, -9};

    bool targetFound = false;
    for (int & myChecker : myCheckers) {
        checker* checker = board->checkers[myChecker];
        if(!targetFound) {
            for (int j = 0; j < 4; ++j) {
                int direction = directions[j];

                if (board->checkers.find(checker->position + direction) != board->checkers.end() &&
                    board->checkers.find(checker->position + direction * 2) == board->checkers.end() &&
                    !board->checkers[checker->position + direction]->isBlack
                        ) {
                    targetFound = board->tryMoveCheck(checker->position, checker->position + direction * 2);
                    if (targetFound) {
                        break;
                    }
                }
            }
        }
    }

    if(!targetFound){
        bool moveSuccessful = false;
        while (!moveSuccessful){
            int randIndex = rand() % 4;
            array<int, 4> directions = {11, -11, 9, -9};
            int randDirection = directions[randIndex];
            randIndex = rand() % myCheckers.size();
            int randCheck = myCheckers[randIndex];
            if(board->checkers.find(randCheck + randDirection * 2) != board->checkers.end() &&
               board->checkers[randCheck + randDirection * 2]->isBlack &&
               (randCheck + randDirection * 2)/10 >= 0 && (randCheck + randDirection * 2)/10 <= 7 &&
               (randCheck + randDirection * 2)%10 >= 0 && (randCheck + randDirection * 2)%10 <= 7
                    ){
                moveSuccessful = board->tryMoveCheck(randCheck, randCheck + randDirection * 2);
            }else{
                moveSuccessful = board->tryMoveCheck(randCheck, randCheck + randDirection);
            }
        }
    }
}

void autoplay() {
    board currBoard = board();

    while (!currBoard.gameHasEnded){
        currBoard.print();
        if(currBoard.isBlackTurn){
            blackStrategyMakeMove(&currBoard);
        }else{
            whiteStrategyMakeMove(&currBoard);
        }
        currBoard.isBlackTurn = !currBoard.isBlackTurn;
    }
}