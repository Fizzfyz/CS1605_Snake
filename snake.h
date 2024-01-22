#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "field.h"

enum class Direction
{
    Up = 0,
    Down = 1,
    Left = 2,
    Right = 3,
    //fizz change 1 for task5
    Left_Up = 4,
    Left_Down = 5,
    Right_Up = 6,
    Right_Down = 7
    //fizz change 1 for task5 end
};

class SnakeBody
{
public:
    SnakeBody();
    SnakeBody(int x, int y);
    int getX() const;
    int getY() const;
    bool operator == (const SnakeBody& snakeBody);
private:
    int mX;
    int mY;
};

// Snake class should have no depency on the GUI library
class Snake
{
public:
    //Snake();
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength);
    Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, field new_map);
    // Set random seed
    void setRandomSeed();
    // Initialize snake
    void initializeSnake();
    // Check if the snake is on the coordinate
    // bool isSnakeOn(int x, int y);
    // Checking API for generating random food
    bool isPartOfSnake(int x, int y);
    void senseFood(SnakeBody food);
    void senseLife(SnakeBody life);//fizz change 14 for task4 and end
    void senseEnergyOrbit(std::vector<SnakeBody> energyOrbit);
    // Check if hit wall
    bool hitWall();
    bool touchFood();
    bool touchLife();//fizz change 14 for task4 and end
    bool hitSelf();

    //nayuan change 1
    bool hitfire();
    bool hitwind_X();
    bool hitwind_Y();
    field getfield();
    void changefield(std::string Map_csv);

    //end;
    bool checkCollision();
    bool changeDirection(Direction newDirection);
    std::vector<SnakeBody>& getSnake();
    int getLength();
    SnakeBody createNewHead();
    bool moveFoward();
    bool touchOrbitForward(int presentPositionOfOrbit);
    bool touchOrbitBackward(int presentPositionOfOrbit);

private:
    const int mGameBoardWidth;
    const int mGameBoardHeight;
    // Snake information
    const int mInitialSnakeLength;

    Direction mDirection;
    SnakeBody mFood;
    SnakeBody mLifePosition;//fizz change 15 for task4 and end
    std::vector<SnakeBody> mSnake;
    std::vector<SnakeBody> menergyOrbit;

    //nanyuan change 2
    field snake_field = creatfield(mGameBoardWidth, mGameBoardHeight, "field_free_map.csv");//end


};

#endif
