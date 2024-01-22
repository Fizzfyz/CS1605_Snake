#include <string>
#include <iostream>
#include <cmath>

// For terminal delay
#include <chrono>
#include <thread>

#include <fstream>
#include <algorithm>
#include <windows.h>
#include "game.h"
#include "field.h"


Game::Game()
{
    // Separate the screen to three windows
    this->mWindows.resize(3);
    initscr();

    // If there wasn't any key pressed don't wait for keypress
    nodelay(stdscr, true);
    // Turn on keypad control
    keypad(stdscr, true);
    // No echo for the key pressed
    noecho();
    // No cursor show
    curs_set(0);
    // Get screen and board parameters
    getmaxyx(stdscr, this->mScreenHeight, this->mScreenWidth);
    this->mGameBoardWidth = this->mScreenWidth - this->mInstructionWidth;
    this->mGameBoardHeight = this->mScreenHeight - this->mInformationHeight;

    this->createInformationBoard();
    this->createGameBoard();
    this->createInstructionBoard();

    // Initialize the leader board to be all zeros
    this->mLeaderBoard.assign(this->mNumLeaders, 0);

    //nanyuan change 11
    this->map_t = creatfield(this->mGameBoardWidth, this->mGameBoardHeight, "field_free_map.csv");//end
}

Game::~Game()
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        delwin(this->mWindows[i]);
    }
    endwin();
}

void Game::createInformationBoard()
{
    int startY = 0;
    int startX = 0;
    this->mWindows[0] = newwin(this->mInformationHeight, this->mScreenWidth, startY, startX);
}

void Game::renderInformationBoard() const
{
    mvwprintw(this->mWindows[0], 1, 1, "Welcome to The Snake Game!");
    mvwprintw(this->mWindows[0], 2, 1, "Author: Lei Mao");
    mvwprintw(this->mWindows[0], 3, 1, "Website: https://github.com/leimao/");
    mvwprintw(this->mWindows[0], 4, 1, "Implemented using C++ and libncurses library.");
    wrefresh(this->mWindows[0]);
}

void Game::createGameBoard()
{
    int startY = this->mInformationHeight;
    int startX = 0;
    this->mWindows[1] = newwin(this->mScreenHeight - this->mInformationHeight, this->mScreenWidth - this->mInstructionWidth, startY, startX);
}

void Game::renderGameBoard() const
{
    wrefresh(this->mWindows[1]);
}

void Game::createInstructionBoard()
{
    int startY = this->mInformationHeight;
    int startX = this->mScreenWidth - this->mInstructionWidth;
    this->mWindows[2] = newwin(this->mScreenHeight - this->mInformationHeight, this->mInstructionWidth, startY, startX);
}

void Game::renderInstructionBoard() const
{
    mvwprintw(this->mWindows[2], 1, 1, "Manual");

    mvwprintw(this->mWindows[2], 2, 1, "Life: ");//fizz change 11 for task 4
    mvwprintw(this->mWindows[2], 3, 1, "Up: W");
    mvwprintw(this->mWindows[2], 4, 1, "Down: S");
    mvwprintw(this->mWindows[2], 5, 1, "Left: A");
    mvwprintw(this->mWindows[2], 6, 1, "Right: D");

    mvwprintw(this->mWindows[2], 8, 1, "Difficulty");
    mvwprintw(this->mWindows[2], 11, 1, "Points");

    wrefresh(this->mWindows[2]);
}


void Game::renderLeaderBoard() const
{
    // If there is not too much space, skip rendering the leader board
    if (this->mScreenHeight - this->mInformationHeight - 14 - 2 < 3 * 2)
    {
        return;
    }
    mvwprintw(this->mWindows[2], 14, 1, "Leader Board");
    std::string pointString;
    std::string rank;
    for (int i = 0; i < std::min(this->mNumLeaders, this->mScreenHeight - this->mInformationHeight - 14 - 2); i ++)
    {
        pointString = std::to_string(this->mLeaderBoard[i]);
        rank = "#" + std::to_string(i + 1) + ":";
        mvwprintw(this->mWindows[2], 14 + (i + 1), 1, rank.c_str());
        mvwprintw(this->mWindows[2], 14 + (i + 1), 5, pointString.c_str());
    }
    wrefresh(this->mWindows[2]);
}

bool Game::renderRestartMenu() const
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Restart", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Final Score:");
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 0 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 1 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void Game::renderPoints() const
{
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(this->mWindows[2], 12, 1, pointString.c_str());
    wrefresh(this->mWindows[2]);
}

void Game::renderDifficulty() const
{
    std::string difficultyString = std::to_string(this->mDifficulty);
    mvwprintw(this->mWindows[2], 9, 1, difficultyString.c_str());
    wrefresh(this->mWindows[2]);
}

//fizz change 7 for task4
void Game::initializeLifeandPointsandSnake()
{
    this->mPoints = 0;
    this->mLifePosition = SnakeBody(-1, -1);
    this->mLife = 3;
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mInitialSnakeLength, this->map_t));
}
//fizz change 7 for task4 end


void Game::initializeGame()
{
    this->mPtrSnake.reset(new Snake(this->mGameBoardWidth, this->mGameBoardHeight, this->mPtrSnake->getLength(), this->map_t));
    this->createRamdonFood();

    this->mPtrSnake->senseFood(this->mFood);
    this->mDifficulty = 0;
    //this->mPoints = 0;//fizz change 6 for task4 and end
    //this->mDelay = this->mBaseDelay;
    //this->mLife = 3;//fizz change 2 for task4 and end
    this->initializecolor();
}

void Game::createRamdonFood()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {
            //nanyuan change 12
            if(this->mPtrSnake->isPartOfSnake(j, i) || this->map_t.get(j, i) != Terrain::Earth)
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mFood = availableGrids[random_idx];
}

void Game::renderFood() const
{
    mvwaddch(this->mWindows[1], this->mFood.getY(), this->mFood.getX(), this->mFoodSymbol | COLOR_PAIR(6));
    wrefresh(this->mWindows[1]);
}

void Game::renderSnake() const
{
    int snakeLength = this->mPtrSnake->getLength();
    std::vector<SnakeBody>& snake = this->mPtrSnake->getSnake();
    //mvwaddch(this->mWindows[1], snake[0].getY(), snake[0].getX(), '~'); this line can change the snake's head
    for (int i = 0; i < snakeLength; i ++)
    {
        mvwaddch(this->mWindows[1], snake[i].getY(), snake[i].getX(), this->mSnakeSymbol | COLOR_PAIR(3));
    }
    wrefresh(this->mWindows[1]);
}

//nanyuan change 7
bool Game::controlSnake()
{
    int key;
    key = getch();
    if (key == 'x')
    {
        while (1)
        {
            key = getch();
            if (key == ' ')
                break;
        }
    }
    if (this->hitwater()){
        Beep(750,300);
        key = getch();
        if (key == 'B' || key == 'b'){
            return false;
        }
        else
            return true;
    }
    switch(key)
    {//fizz change 3 for task5
        case 'W':
        case 'w':
        case KEY_UP:
        {
            this->mPtrSnake->changeDirection(Direction::Up);
            return false;
        }
        case 'S':
        case 's':
        case KEY_DOWN:
        {
            this->mPtrSnake->changeDirection(Direction::Down);
            return false;
        }
        case 'A':
        case 'a':
        case KEY_LEFT:
        {
            this->mPtrSnake->changeDirection(Direction::Left);
            return false;
        }
        case 'D':
        case 'd':
        case KEY_RIGHT:
        {
            this->mPtrSnake->changeDirection(Direction::Right);
            return false;
        }
        case 'Q':
        case 'q':
        {
            this->mPtrSnake->changeDirection(Direction::Left_Up);
            return false;
        }
        case 'E':
        case 'e':
        {
            this->mPtrSnake->changeDirection(Direction::Right_Up);
            return false;
        }
        case 'Z':
        case 'z':
        {
            this->mPtrSnake->changeDirection(Direction::Left_Down);
            return false;
        }
        case 'C':
        case 'c':
        {
            this->mPtrSnake->changeDirection(Direction::Right_Down);
            return false;
        }
        default:
        {
            return false;
        }
    }//fizz change 3 for task 5 end
}

//nanyuan change 8
bool Game::hitwater()
{
    SnakeBody head = this->mPtrSnake->getSnake()[0];
    int headX = head.getX();
    int headY = head.getY();
    if (this->map_t.get(headX, headY) == Terrain::Water){
        return true;
    }
    else{
        return false;
    }
}
void Game::rederTerrain() const
{

    for (int i = 1; i < this->mGameBoardHeight - 1; i ++){
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++){
            if(this->map_t.get(j, i) == Terrain::Fire){
                mvwaddch(this->mWindows[1], i, j, this->mFireSymbol | COLOR_PAIR(1));
            }
            else if (this->map_t.get(j, i) == Terrain::Water){
                mvwaddch(this->mWindows[1], i, j, this->mWaterSymbol | COLOR_PAIR(4));
            }
            else if (this->map_t.get(j, i) == Terrain::Wind_X){
                mvwaddch(this->mWindows[1], i, j, this->mWindXSymbol | COLOR_PAIR(7));
            }
            else if (this->map_t.get(j, i) == Terrain::Wind_Y){
                mvwaddch(this->mWindows[1], i, j, this->mWindYSymbol | COLOR_PAIR(7));
            }
        }
    }
    wrefresh(this->mWindows[1]);
}
void Game::initializecolor()
{
    if(has_colors() == FALSE)
    {
        endwin();
        printf("You terminal does not support color\n");
        exit(1);
    }
    start_color();  /*Æô¶¯color »úÖÆ*/
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_CYAN);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);

}
//end

void Game::renderBoards() const
{
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        werase(this->mWindows[i]);
    }
    this->renderInformationBoard();
    this->renderGameBoard();
    this->renderInstructionBoard();
    for (int i = 0; i < this->mWindows.size(); i ++)
    {
        box(this->mWindows[i], 0, 0);
        wrefresh(this->mWindows[i]);
    }
    this->renderLeaderBoard();
}


void Game::adjustDelay()
{
    this->mDifficulty = this->mPoints / 5;

    this->mDelay = this->mBaseDelay * pow(0.995, this->mPoints);

}

void Game::changegamefield(std::string Map_csv)
{
    this->map_t = creatfield(this->mGameBoardWidth, this->mGameBoardHeight, Map_csv);
    this->mPtrSnake->changefield(Map_csv);
}

void Game::runGame()
{
    //bool moveSuccess;
    //int key;
    //fizz: I don't think the two variables are necessary

    //test this->changegamefield("field_map2.csv");
    while (true)
    {
        //nanyuan change 9
        if(this->controlSnake())
        {
            this->mLife -= 1;
            this->eraseLifePosition();
            this->energyOrbit = {};
            orbitPosition = SnakeBody(-1, -1);
            this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
            break;
        }
        werase(this->mWindows[1]);
        box(this->mWindows[1], 0, 0);

        if (energyOrbitShouldBePrinted == true)
            this->renderEnergyOrbit();
        //fizz change 3 for task3 end
        this->renderRandomLife();
        this->renderFood();
        this->rederTerrain();



        bool eatFood = this->mPtrSnake->moveFoward();
        bool collision = this->mPtrSnake->checkCollision();
        if (collision == true)
        {
            this->mLife -= 1;//fizz change 3 for task4
            this->eraseLifePosition();
            this->energyOrbit = {};
            this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
            orbitPosition = SnakeBody(-1, -1);
            break;
        }
        this->renderSnake();
        if (eatFood == true)
        {
            this->mPoints += 1;
            this->createRamdonFood();
            this->mPtrSnake->senseFood(this->mFood);
            //this->adjustDelay();
            Beep(750,300);
        }
        this->adjustDelay();
        //fizz change 12 for task4
        /*
        bool eatLife = true;
        if (eatLife == true)
        {
            this->mLife += 1;
            this->eraseLifePosition();
            eatLife = false;
        }*/

        bool eatLife = false;
        if (this->mPtrSnake->touchLife())
        {
            this->mLife += 1;
            this->renderLife();
            this->eraseLifePosition();
            eatLife = true;
            Beep(1500,300);
        }





        if (counter % 50 == 1 && this->noExistingLife() && eatLife == false )
        {
            this->createRandomLife();
            this->mPtrSnake->senseLife(this->mLifePosition);
        }

        //fizz change 3 for task3
        if (counter % 50 == 1 && this->noExistingEnergyOrbit())
        {
            this->createRandomEnergyOrbit();
            this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);

            presentPositionOfOrbit += 1;
            orbitPosition = this->energyOrbit[presentPositionOfOrbit];
            energyOrbitShouldBePrinted = true;
        }

        if (this->mPtrSnake->touchOrbitForward(presentPositionOfOrbit))
        {
            presentPositionOfOrbit += 1;
            Beep(2500,300);

            if (presentPositionOfOrbit < this->energyOrbit.size())
                orbitPosition = this->energyOrbit[presentPositionOfOrbit];
            else
            {
                mPoints += 10;
                presentPositionOfOrbit = -1;
                energyOrbitShouldBePrinted = false;
                orbitPosition = SnakeBody(-1, -1);
                this->energyOrbit = {};
                this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
            }
        }
        else if (this->mPtrSnake->touchOrbitBackward(presentPositionOfOrbit))
        {
            presentPositionOfOrbit += 1;
            Beep(2500,300);

            if (presentPositionOfOrbit < this->energyOrbit.size())
                orbitPosition = this->energyOrbit[this->energyOrbit.size() - presentPositionOfOrbit - 1];
            else
            {
                mPoints += 10;
                presentPositionOfOrbit = -1;
                energyOrbitShouldBePrinted = false;
                orbitPosition = SnakeBody(-1, -1);
                this->energyOrbit = {};
                this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
            }
        }
        else
        {
            if (!this->noExistingEnergyOrbit())
            {
                presentPositionOfOrbit = 0;
                orbitPosition = energyOrbit[presentPositionOfOrbit];
            }
        }



        //fizz change 12 for task4 end

        this->renderDifficulty();
        this->renderPoints();

        if ( (gameModeOn == true) && ((this->mPoints >= 10 && task == 1) || (this->mPoints >= 20 && task == 2) || (this->mPoints >= 30 && task == 3)))
        {
            task += 1;
            break_because_of_taskup = true;
            this->eraseLifePosition();
            this->energyOrbit = {};
            orbitPosition = SnakeBody(-1,-1);
            this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
            break;
        }

        if (task == 4 && this->mPoints >= 44 && chooseToContinue == false)
        {
            break_because_of_finish = this->renderFinishMenu();

            if (break_because_of_finish == true)
            {
                this->eraseLifePosition();
                this->energyOrbit = {};
                orbitPosition = SnakeBody(-1,-1);
                this->mPtrSnake->senseEnergyOrbit(this->energyOrbit);
                break;
            }
            else
                chooseToContinue = true;

        }

        std::this_thread::sleep_for(std::chrono::milliseconds(this->mDelay));

        refresh();
        counter++;
    }
}

//fizz change 13 for task4

void Game::eraseLifePosition()
{
    this->mLifePosition = SnakeBody(-1, -1);
}

bool Game::noExistingLife()
{
    return (this->mLifePosition == SnakeBody(-1, -1));
}


void Game::createRandomLife()
{
    std::vector<SnakeBody> availableGrids;
    for (int i = 1; i < this->mGameBoardHeight - 1; i ++)
    {
        for (int j = 1; j < this->mGameBoardWidth - 1; j ++)
        {   //nanyuan change 13
            if(this->mPtrSnake->isPartOfSnake(j, i) || SnakeBody(j, i) ==  this->mFood || this->map_t.get(j, i) != Terrain::Earth)//end
            {
                continue;
            }
            else
            {
                availableGrids.push_back(SnakeBody(j, i));
            }
        }
    }

    // Randomly select a grid that is not occupied by the snake
    int random_idx = std::rand() % availableGrids.size();
    this->mLifePosition = availableGrids[random_idx];
}

void Game::renderRandomLife()
{
    mvwaddch(this->mWindows[1], this->mLifePosition.getY(), this->mLifePosition.getX(), this->mLifeSymbol | COLOR_PAIR(2));
    wrefresh(this->mWindows[1]);
}
//fizz change 13 for task4 end


void Game::startGame()
{
    refresh();
    bool choice;
    bool startChoice;
    bool modeChoice;
    startChoice = this->renderStartMenu();
    if (startChoice == true)
    {
        modeChoice = this->renderModeMenu();
        if (modeChoice == true)
            gameModeOn = true;
        else
            gameModeOn = false;
        while (true)
        {
            task = 1;
            break_because_of_taskup = false;
            this->energyOrbit = {};
            this->eraseLifePosition();
            orbitPosition = SnakeBody(-1, -1);
            this->readLeaderBoard();
            this->renderBoards();
            //fizz change 5 for task4
            this->initializeLifeandPointsandSnake();
            //fizz change 11 for task4
            this->mDelay = this->mBaseDelay;

            while(this->mLife > 0)
            {
                if (this->task == 1 && gameModeOn)
                    changegamefield("field_map1.csv");
                if (this->task == 2)
                    changegamefield("field_map2.csv");
                if (this->task == 3)
                    changegamefield("field_map3.csv");
                if (this->task == 4)
                    changegamefield("field_map4.csv");
                this->initializeGame();
                this->renderLife();
                this->runGame();
                bool choiceContinue = true;
                if (break_because_of_taskup == true)
                {
                    choiceContinue = this->renderTaskUpMenu();
                    break_because_of_taskup = false;
                }

                else if (break_because_of_finish == true)
                {
                    break_because_of_finish = false;
                    break;
                }

                else
                {
                    if (this->mLife > 0)
                        choiceContinue = this->renderContinueMenu();
                }

                if (choiceContinue == false)
                    break;
            }
            this->updateLeaderBoard();
            this->writeLeaderBoard();
            choice = this->renderRestartMenu();
            if (choice == false)
            {
                break;
            }
        }
    }
    else
        return;


}
//fizz change 11 for task4
void Game::renderLife()
{
    std::string lifeString = std::to_string(this->mLife);
    mvwprintw(this->mWindows[2], 2, 7, lifeString.c_str());
    wrefresh(this->mWindows[2]);
}
//fizz change 11 for task4 end


//fizz change 9 for task4
bool Game::renderContinueMenu()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Score:");
    mvwprintw(menu, 3, 1, "Life:");
    std::string lifeString = std::to_string(this->mLife);
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    mvwprintw(menu, 4, 1, lifeString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 2 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset + 2, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset + 2, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
//fizz change 9 for task4 end

// https://en.cppreference.com/w/cpp/io/basic_fstream
bool Game::readLeaderBoard()
{
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.in);
    if (!fhand.is_open())
    {
        return false;
    }
    int temp;
    int i = 0;
    while ((!fhand.eof()) && (i < mNumLeaders))
    {
        fhand.read(reinterpret_cast<char*>(&temp), sizeof(temp));
        this->mLeaderBoard[i] = temp;
        i ++;
    }
    fhand.close();
    return true;
}

bool Game::updateLeaderBoard()
{
    bool updated = false;
    int newScore = this->mPoints;
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        if (this->mLeaderBoard[i] >= this->mPoints)
        {
            continue;
        }
        int oldScore = this->mLeaderBoard[i];
        this->mLeaderBoard[i] = newScore;
        newScore = oldScore;
        updated = true;
    }
    return updated;
}

bool Game::writeLeaderBoard()
{
    // trunc: clear the data file
    std::fstream fhand(this->mRecordBoardFilePath, fhand.binary | fhand.trunc | fhand.out);
    if (!fhand.is_open())
    {
        return false;
    }
    for (int i = 0; i < this->mNumLeaders; i ++)
    {
        fhand.write(reinterpret_cast<char*>(&this->mLeaderBoard[i]), sizeof(this->mLeaderBoard[i]));;
    }
    fhand.close();
    return true;
}

//fizz change 5 for task3
void Game::createRandomEnergyOrbit()
{
    std::srand((unsigned)time(0));

    int random_y = (std::rand() % this->mGameBoardHeight) /2 + 6 ;
    int random_x = (std::rand() %this->mGameBoardWidth) / 2 + 25 ;
    this->energyOrbit.push_back(SnakeBody(random_x, random_y));
    int random[2] = {0, 1};

    if (energyOrbitType == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            int random_y_new = random_y + random[std::rand() % 2];
            int random_x_new = random_x + random[std::rand() % 2];
            if (!(random_y_new == random_y && random_x_new == random_x) && !(random_y_new >= (this->mGameBoardHeight - 2) || random_x_new >= (this->mGameBoardWidth - 2)) && this->map_t.get(random_x_new, random_y_new) == Terrain::Earth)
            {
                this->energyOrbit.push_back(SnakeBody(random_x_new, random_y_new));
                random_x = random_x_new;
                random_y = random_y_new;
            }

        }
        energyOrbitType = 1;
        return;
    }

    if (energyOrbitType == 1)
    {
        for (int i = 0; i < 10; i++)
        {
            int flag = std::rand() % 2;
            if (flag == 0)
                random_y += 1;
            else
                random_x += 1;
            if (this->map_t.get(random_x, random_y) == Terrain::Earth && !(random_y >= (this->mGameBoardHeight - 2) || random_x >= (this->mGameBoardWidth - 2)))
                this->energyOrbit.push_back(SnakeBody(random_x, random_y));
            else
            {
                if (flag == 0)
                    random_y -= 1;
                else
                    random_x -= 1;
            }

        }
        energyOrbitType = 0;
        return;
    }

}

bool Game::noExistingEnergyOrbit()
{
    if (this->energyOrbit.size() == 0)
        return true;
    else
        return false;
}

void Game::renderEnergyOrbit()
{
    for (int i = 0; i < this->energyOrbit.size(); i++)
    {
        mvwaddch(this->mWindows[1], this->energyOrbit[i].getY(), this->energyOrbit[i].getX(), this->energyOrbitSymbol | COLOR_PAIR(5));
    }
    wrefresh(this->mWindows[1]);
}


//fizz change 5 for task3 end
//fizz change 1 for task 7
bool Game::renderStartMenu()
{
    WINDOW * menu;

    int width = this->mScreenWidth * 0.5;
    int height = this->mScreenHeight * 0.5;
    int startX = this->mScreenWidth * 0.25;
    int startY = this->mScreenHeight * 0.25;// + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Start", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, width/2 - 10, "Welcome to Snake Game!",1);
    mvwprintw(menu, 2, width/2 - 12, "(Splendid Game Interface)",1 );
    /*
    mvwprintw(menu, 3, 1, "Life:");
    std::string lifeString = std::to_string(this->mLife);
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    mvwprintw(menu, 4, 1, lifeString.c_str());*/
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 2 + offset, width/2 - 3, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, width/2 - 3, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset + 2, width/2 - 3, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, width/2 - 3, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset + 2, width/2 - 3, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, width/2 - 3, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}
//fizz change 1 for task 7 end



bool Game::renderTaskUpMenu()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5 + 2;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Score:");
    mvwprintw(menu, 3, 1, "Life:");
    mvwprintw(menu, 5, 1, "Welcome to Higher Level! Now you are going to task: ");
    std::string taskString = std::to_string(this->task);
    std::string lifeString = std::to_string(this->mLife);
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    mvwprintw(menu, 4, 1, lifeString.c_str());
    mvwprintw(menu, 6, 1, taskString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 4 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool Game::renderModeMenu()
{
    WINDOW * menu;
    int width = this->mScreenWidth * 0.5;
    int height = this->mScreenHeight * 0.5;
    int startX = this->mScreenWidth * 0.25;
    int startY = this->mScreenHeight * 0.25;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Free Mode", "Level Mode"};

    int index = 0;
    int offset = 4;

    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 2 + offset, width/2 - 5, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, width/2 - 5, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset + 2, width/2 - 5, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, width/2 - 5, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset + 2, width/2 - 5, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 2, width/2 - 5, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return false;
    }
    else
    {
        return true;
    }

}



bool Game::renderFinishMenu()
{
    WINDOW * menu;
    int width = this->mGameBoardWidth * 0.5 + 2;
    int height = this->mGameBoardHeight * 0.5;
    int startX = this->mGameBoardWidth * 0.25;
    int startY = this->mGameBoardHeight * 0.25 + this->mInformationHeight;

    menu = newwin(height, width, startY, startX);
    box(menu, 0, 0);
    std::vector<std::string> menuItems = {"Continue", "Quit"};

    int index = 0;
    int offset = 4;
    mvwprintw(menu, 1, 1, "Your Score:");
    mvwprintw(menu, 3, 1, "Life:");
    mvwprintw(menu, 5, 1, "Congratulations! You win! Want to continue?");
    std::string taskString = std::to_string(this->task);
    std::string lifeString = std::to_string(this->mLife);
    std::string pointString = std::to_string(this->mPoints);
    mvwprintw(menu, 2, 1, pointString.c_str());
    mvwprintw(menu, 4, 1, lifeString.c_str());
    mvwprintw(menu, 6, 1, taskString.c_str());
    wattron(menu, A_STANDOUT);
    mvwprintw(menu, 3 + offset, 1, menuItems[0].c_str());
    wattroff(menu, A_STANDOUT);
    mvwprintw(menu, 4 + offset, 1, menuItems[1].c_str());

    wrefresh(menu);

    int key;
    while (true)
    {
        key = getch();
        switch(key)
        {
            case 'W':
            case 'w':
            case KEY_UP:
            {
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                index --;
                index = (index < 0) ? menuItems.size() - 1 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
            case 'S':
            case 's':
            case KEY_DOWN:
            {
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                index ++;
                index = (index > menuItems.size() - 1) ? 0 : index;
                wattron(menu, A_STANDOUT);
                mvwprintw(menu, index + offset + 3, 1, menuItems[index].c_str());
                wattroff(menu, A_STANDOUT);
                break;
            }
        }
        wrefresh(menu);
        if (key == ' ' || key == 10)
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delwin(menu);

    if (index == 0)
    {
        return false;
    }
    else
    {
        return true;
    }

}
