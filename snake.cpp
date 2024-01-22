#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "snake.h"
#include "field.h"

SnakeBody::SnakeBody()
{
}


SnakeBody::SnakeBody(int x, int y): mX(x), mY(y)
{
}

int SnakeBody::getX() const
{
    return mX;
}

int SnakeBody::getY() const
{
    return mY;
}

bool SnakeBody::operator == (const SnakeBody& snakeBody)
{
    return (this->getX() == snakeBody.getX() && this->getY() == snakeBody.getY());
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength): mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength)
{
    this->initializeSnake();
    this->setRandomSeed();
}

Snake::Snake(int gameBoardWidth, int gameBoardHeight, int initialSnakeLength, field new_map)
    : mGameBoardWidth(gameBoardWidth), mGameBoardHeight(gameBoardHeight), mInitialSnakeLength(initialSnakeLength), snake_field(new_map)
{
    this->initializeSnake();
    this->setRandomSeed();
}

void Snake::setRandomSeed()
{
    // use current time as seed for random generator
    std::srand(std::time(nullptr));
}

void Snake::initializeSnake()
{
    // Instead of using a random initialization algorithm
    // We always put the snake at the center of the game mWindows
    int centerX = this->mGameBoardWidth / 2;
    int centerY = this->mGameBoardHeight / 2;

    for (int i = 0; i < this->mInitialSnakeLength; i ++)
    {
        this->mSnake.push_back(SnakeBody(centerX, centerY + i));
    }
    this->mDirection = Direction::Up;
}

bool Snake::isPartOfSnake(int x, int y)
{
    SnakeBody temp = SnakeBody(x, y);
    for (int i = 0; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == temp)
        {
            return true;
        }
    }
    return false;
}

/*
 * Assumption:
 * Only the head would hit wall.
 */
bool Snake::hitWall()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (headX <= 0 || headX >= this->mGameBoardWidth - 1)
    {
        return true;
    }
    if (headY <= 0 || headY >= this->mGameBoardHeight - 1)
    {
        return true;
    }
    return false;
}

/*
 * The snake head is overlapping with its body
 */
bool Snake::hitSelf()
{
    SnakeBody& head = this->mSnake[0];
    // Exclude the snake head
    for (int i = 1; i < this->mSnake.size(); i ++)
    {
        if (this->mSnake[i] == head)
        {
            return true;
        }
    }
    return false;
}

//nayuan change 3
bool Snake::hitfire()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();

    if (this->snake_field.get(headX, headY) == Terrain::Fire){
        return true;
    }
    else{
        return false;
    }
}

bool Snake::hitwind_X()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();

    if (this->snake_field.get(headX, headY) == Terrain::Wind_X){
        return true;
    }
    else{
        return false;
    }
}

bool Snake::hitwind_Y()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();

    if (this->snake_field.get(headX, headY) == Terrain::Wind_Y){
        return true;
    }
    else{
        return false;
    }
}

field Snake::getfield()
{
    return this->snake_field;

}

void Snake::changefield(std::string Map_csv)
{
    this->snake_field = creatfield(this->mGameBoardWidth, this->mGameBoardHeight, Map_csv);
}
//end;

bool Snake::touchFood()
{
    SnakeBody newHead = this->createNewHead();
    if (this->mFood == newHead)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::senseFood(SnakeBody food)
{
    this->mFood = food;
}

//fizz change 15 for task4
void Snake::senseLife(SnakeBody life)
{
    this->mLifePosition = life;
}
//fizz change 15 for task4 end


std::vector<SnakeBody>& Snake::getSnake()
{
    return this->mSnake;
}

bool Snake::changeDirection(Direction newDirection)
{
    switch (this->mDirection)
    {
    //fizz change 2 for task5
        case Direction::Up:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right || newDirection == Direction::Left_Up || newDirection == Direction::Right_Up)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Down:
        {
            if (newDirection == Direction::Left || newDirection == Direction::Right || newDirection == Direction::Left_Down || newDirection == Direction::Right_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down || newDirection == Direction::Left_Up || newDirection == Direction::Left_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Down || newDirection == Direction::Right_Up || newDirection == Direction::Right_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left_Up:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Left || newDirection == Direction::Right_Up || newDirection == Direction::Left_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Left_Down:
        {
            if (newDirection == Direction::Left_Up || newDirection == Direction::Left || newDirection == Direction::Right_Down || newDirection == Direction::Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right_Up:
        {
            if (newDirection == Direction::Up || newDirection == Direction::Left_Up || newDirection == Direction::Right || newDirection == Direction::Right_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
        case Direction::Right_Down:
        {
            if (newDirection == Direction::Right_Up || newDirection == Direction::Right || newDirection == Direction::Down || newDirection == Direction::Left_Down)
            {
                this->mDirection = newDirection;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    //fizz change 2 for task5 end
    return false;
}


SnakeBody Snake::createNewHead()
{
    SnakeBody& head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    int headXNext;
    int headYNext;
    //fizz change 4 for task5
    switch (this->mDirection)
    {
        case Direction::Up:
        {
            headXNext = headX;
            headYNext = headY - 1;
            break;
        }
        case Direction::Down:
        {
            headXNext = headX;
            headYNext = headY + 1;
            break;
        }
        case Direction::Left:
        {
            headXNext = headX - 1;
            headYNext = headY;
            break;
        }
        case Direction::Right:
        {
            headXNext = headX + 1;
            headYNext = headY;
            break;
        }
        case Direction::Left_Up:
        {
            headXNext = headX - 1;
            headYNext = headY - 1;
            break;
        }
        case Direction::Left_Down:
        {
            headXNext = headX - 1;
            headYNext = headY + 1;
            break;
        }
        case Direction::Right_Up:
        {
            headXNext = headX + 1;
            headYNext = headY - 1;
            break;
        }
        case Direction::Right_Down:
        {
            headXNext = headX + 1;
            headYNext = headY + 1;
            break;
        }
    }
    //fizz change 4 for task5 end
    SnakeBody newHead = SnakeBody(headXNext, headYNext);

    return newHead;
}

/*
 * If eat food, return true, otherwise return false
 */
bool Snake::moveFoward()
{
    //nanyuan change
    SnakeBody head = this->mSnake[0];
    int headX = head.getX();
    int headY = head.getY();
    if (this->hitwind_X())
    {
        SnakeBody newHead = SnakeBody(this->mGameBoardWidth - 1 - headX, headY);
        this->mSnake.insert(this->mSnake.begin(), newHead);
        this->mSnake.pop_back();
    }
    if (this->hitwind_Y())
    {
        SnakeBody newHead = SnakeBody(headX, this->mGameBoardHeight - 1 - headY);
        this->mSnake.insert(this->mSnake.begin(), newHead);
        this->mSnake.pop_back();
    }
    //end
    if (this->touchFood())
    {
        SnakeBody newHead = this->mFood;
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return true;
    }
    else
    {
        this->mSnake.pop_back();
        SnakeBody newHead = this->createNewHead();
        this->mSnake.insert(this->mSnake.begin(), newHead);
        return false;
    }
}

//nanyuan change 10
bool Snake::checkCollision()
{
    if (this->hitWall() || this->hitSelf() || this->hitfire() )
    {
        return true;
    }
    else
    {
        return false;
    }
}


int Snake::getLength()
{
    return this->mSnake.size();
}

bool Snake::touchLife()
{
    if (this->mLifePosition == this->mSnake[0])
    {
        return true;
    }
    else
    {
        return false;
    }
}

//fizz change 6 for task3
void Snake::senseEnergyOrbit(std::vector<SnakeBody> energyOrbit)
{
    this->menergyOrbit = energyOrbit;
}

bool Snake::touchOrbitForward(int presentPositionOfOrbit)
{
    if (menergyOrbit.size() == 0)
        return false;
    if (mSnake[0] == menergyOrbit[presentPositionOfOrbit])
        return true;
    else
        return false;
}

bool Snake::touchOrbitBackward(int presentPositionOfOrbit)
{
    if (menergyOrbit.size() == 0)
        return false;
    if (mSnake[0] == menergyOrbit[menergyOrbit.size() - presentPositionOfOrbit - 1])
        return true;
    else
        return false;
}

