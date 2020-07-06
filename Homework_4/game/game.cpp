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

void board::print() {
    array<string, 9> numbers = {
            "0️⃣",
            "1️⃣",
            "2️⃣",
            "3️⃣",
            "4️⃣",
            "5️⃣",
            "6️⃣",
            "7️⃣"
    };
    cout << "NW🟦";
    for (int i = 0; i <= 7; ++i) {
        cout << numbers[i];
    }
    cout << "🟦NE" << endl;

    for (int i = 0; i <= 7; ++i) {
        cout << "  " << numbers[i];
        for (int j = 0; j <= 7; ++j) {
            if(checkers.find(i*10 + j) != checkers.end()){
                if(checkers[i*10 + j]->isQueen){
                    if(checkers[i*10 + j]->isBlack){
                        cout << "👸";
                    }else{
                        cout << "🤴";
                    }
                }else{
                    if(checkers[i*10 + j]->isBlack){
                        cout << "👩";
                    }else{
                        cout << "👨";
                    }
                }
            }else{
                if(i%2 == 1){
                    j%2 == 0 ? cout << "⬜": cout << "⬛";
                }else{
                    j%2 == 0 ? cout << "⬛": cout << "⬜";
                }
            }
        }
        cout << numbers[i];
        cout << endl;
    }

    cout << "SW🟦";
    for (int i = 0; i <= 7; ++i) {
        cout << numbers[i];
    }
    cout << "🟦SE" << endl;
}

void board::moveChecker(checker *checker, int to) {
    checker->isBlack ? cout << "Women moved from " : cout << "Men moved from ";
    cout << checker->position << " to " << to << endl;
    checkers.erase(checker->position);
    checker->position = to;
    if((checker->isBlack && to/10 == 0) || (!checker->isBlack && to/10 == 7)){
        checker->isQueen=true;
    }
    addChecker(checker);
}

bool board::tryMoveCheck(int from, int to) {
    if(checkers.find(from) == checkers.end()){
        cout << "Wait, no checker at " << from << endl;
        return false;
    }
    checker* checker = checkers[from];
    if(to/10 > 7 || to%10 > 7 || to/10 < 0 || to%10 < 0){
        return false;
    }
    if(checkers[from]->isBlack != isBlackTurn){
        return false;
    }
    if(checker->isQueen){
       if(abs(from/10 - to/10) == abs(from%10 - to%10)){
           int pathLength = abs(from/10 - to/10);
           unordered_set<int> obstacleCoordinates;
           int step = (to - from)/pathLength;

           if(checkers.find(to) != checkers.end()){
               return false;
           }

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
               for (const auto& elem: obstacleCoordinates) {
                  removeChecker(checkers[elem]);
               }
               moveChecker(checker, to);
               return true;
           }
       }else{
           return false;
       }
    }else {
        if (abs(from - to) == 11 || abs(from - to) == 9) {
            if (checkers.find(to) != checkers.end()) {
                if (checkers[to]->isBlack == checker->isBlack) {
                    return false;
                } else {
                    if (checkers.find(from + (to - from) * 2) != checkers.end()) {
                        return false;
                    } else {
                        if(to + (to-from)/10 > 7 || to + (to-from)/10 < 0 || to + (to-from)%10 > 7 || to + (to-from)%10 < 0){
                            return false;
                        }
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
        } else if(abs(from - to) == 22 || abs(from - to) == 18){
            if (checkers.find(from + (to - from) / 2) != checkers.end()) {
                if(checkers[from + (to - from) / 2]->isBlack == checker->isBlack){
                    return false;
                }
                if(checkers.find(from + (to - from)) != checkers.end()){
                    return false;
                }
                removeChecker(checkers[from + (to - from) / 2]);
                moveChecker(checker, to);
                return true;
            }else{
                return false;
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
            cout << "Game has ended, men won!" << endl;
        }
    }else{
        numberOfWhiteCheckers--;
        if (numberOfWhiteCheckers == 0){
            win();
            cout << "Game has ended, women won!" << endl;
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

        if(isInteger(input) || input == "end"){
            if (input == "end"){
                if(moveHappen){
                    currBoard.isBlackTurn = !currBoard.isBlackTurn;
                    moveHappen = false;
                }else{
                    cout << "Wait, you need to make a move first";
                }
            }else{
                string input2;
                cin >> input2;

                int inputInt = stoi(input);
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
                        if(
                                lastMoveAte &&
                                (currBoard.checkers.find(input2Int) != currBoard.checkers.end() ||
                                currBoard.checkers.find(input2Int + (input2Int - inputInt)) != currBoard.checkers.end())
                                ){
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
                    }else{
                        cout << "No way you can do this!" << endl;
                    }
                }
            }
        }else {cout << "That was strange. Retype it" << endl;}
    }
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
