#include "game.h"
#include <Windows.h>
//#include <mmsystem.h>
#include <dsound.h>

int main(int argc, char** argv)
{
    //mciSendString(TEXT("open dejavu.wma alias mysong"), NULL, 0,NULL);
    //mciSendString(TEXT("play mysong repeat"), NULL, 0, NULL);
    Game game;
    game.startGame();
}
