#ifndef GAME_H
#define GAME_H

#include "curses.h"
#include <string>
#include <vector>
#include <memory>

#include "snake.h"
#include "field.h"

class Game
{
public:
    Game();
    ~Game();

    void createInformationBoard();
    void renderInformationBoard() const;

    void createGameBoard();
    void renderGameBoard() const;

    void createInstructionBoard();
    void renderInstructionBoard() const;

    void loadLeadBoard();
    void updateLeadBoard();
    bool readLeaderBoard();
    bool updateLeaderBoard();
    bool writeLeaderBoard();
    void renderLeaderBoard() const;

    void renderBoards() const;

    void initializeGame();
    void runGame();
    void renderPoints() const;
    void renderDifficulty() const;

    void createRamdonFood();
    void renderFood() const;
    void renderSnake() const;
    bool controlSnake();

    //nanyuan change 4
    bool hitwater();
    void rederTerrain() const;
    void initializecolor();
    //end

    void startGame();
    bool renderRestartMenu() const;
    void adjustDelay();

    void changegamefield(std::string Map_csv);

    bool renderContinueMenu();//fizz change 8 for task4 and end
    void initializeLifeandPointsandSnake();//fizz change 4 for task4 and end
    void renderLife();//fizz change 10 for task4 and end
    void createRandomLife();//fizz change 13 for task4 and end
    void renderRandomLife();//fizz change 13 for task4 and end
    bool noExistingLife();//fizz change 13 for task4 and end
    void eraseLifePosition();//fizz change 13 for task4 and end
    void createRandomEnergyOrbit();//fizz change 3 for task3 and end
    bool noExistingEnergyOrbit();
    void renderEnergyOrbit();
    bool renderStartMenu();
    bool renderTaskUpMenu();
    bool renderModeMenu();
    bool renderFinishMenu();

private:
    // We need to have two windows
    // One is for game introduction
    // One is for game mWindows
    int mScreenWidth;
    int mScreenHeight;
    int mGameBoardWidth;
    int mGameBoardHeight;
    const int mInformationHeight = 6;
    const int mInstructionWidth = 18;
    std::vector<WINDOW *> mWindows;
    // Snake information
    const int mInitialSnakeLength = 2;
    const char mSnakeSymbol = '@';
    std::unique_ptr<Snake> mPtrSnake;
    // Food information
    SnakeBody mFood;
    //fizz change 14 for task4
    SnakeBody mLifePosition;
    const char mLifeSymbol = '$';
    //fizz change 14 for task4 end
    const char mFoodSymbol = '#';

    //nanyuan change 5
    const char mFireSymbol = '|';
    const char mWaterSymbol = '~';
    const char mWindYSymbol = 'Y';
    const char mWindXSymbol = 'X';
    //end

    int mPoints = 0;
    int mDifficulty = 0;
    int mBaseDelay = 100;
    int mDelay;
    int mLife;//fizz change 1 for task4 and end
    const std::string mRecordBoardFilePath = "record.dat";
    std::vector<int> mLeaderBoard;
    const int mNumLeaders = 3;
    //fizz change 2 for task3
    int counter = 0;
    //fizz change 1 for task3
    std::vector<SnakeBody> energyOrbit = {};
    const char energyOrbitSymbol = '+';
    bool energyOrbitShouldBePrinted = false;//fizz change 4 for task3
    SnakeBody orbitPosition = SnakeBody(-1, -1);
    int presentPositionOfOrbit = -1;
    int energyOrbitType = 0;
    bool break_because_of_taskup = false;
    bool break_because_of_finish = false;
    bool chooseToContinue = false;
    int task = 1;
    bool gameModeOn;

    //nanyuan change 6
    field map_t;//end;
};

#endif
