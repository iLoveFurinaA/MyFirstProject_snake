#ifndef __NAME_H__
#define __NAME_H__
#undef UNICODE
#include<easyx.h>
#include<graphics.h>

//-------------宏定义--------------------------------
#define Window_width 800
#define Window_height 600
const int GRID_SIZE = Window_width*0.75/20;
const int Min_Snake_x = 0;
const int Min_Snake_y = 0;
const int Max_Snake_x= Window_width - Window_width*0.75/20 - 200;
const int Max_Snake_y=Window_height - Window_width*0.75/20;
const int SNAKE_LENGTH = 4;
//------------结构定义--------------------------------
struct SnakeNode{
	int x;
	int y;
	SnakeNode* next;
};
struct Food{
	int x;
	int y;
};
enum DIR{
	UP,
	DOWN,
	LEFT,
	RIGHT,
};
extern int MoveDiretion;
extern int score;
extern int speedLevel;
extern int dx, dy ;// 初始设置向右移动
extern int highScore;
extern IMAGE img;
extern IMAGE menu;
extern IMAGE GameOver;
extern bool gameOver;
extern Food food;
//-------------函数声明-------------------------------
void init_window();
void darw_Area_dividing_line();
void Print_prompt_message();
void Draw_grid_line();
void darwSnake(SnakeNode* snake);
void moveSnakeAndDeleteTail(SnakeNode*& snake, int dx, int dy);
void deleteSnake(SnakeNode* snake);
void clearSnakeBody(SnakeNode* snake);
void handleKeyboardInput(int &dx, int &dy);
void generateFood(SnakeNode* snake); 
void drawFood();
void drawScore();
void redrawGameUI();
void gameDraw(SnakeNode* snake);
void showGameOverScreen();
void waitForRestart();
void resetGame(SnakeNode*& snake);
void showMainMenu();
void saveHighScore(int newScore);
void darwHighScore();
bool isCollisionWithFood(SnakeNode* snake);
bool isCollisionWithSelf(SnakeNode* snake);
int getDelayTimeBySpeedLevel();
int calculateScoreIncrease(int speedLevel);
int loadHighScore();
SnakeNode* createSnakeNode(int x,int y);
SnakeNode* initSnake();


#endif // __NAME_H__
