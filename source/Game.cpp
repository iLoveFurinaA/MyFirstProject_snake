#include<easyx/easyx.h>
#include<easyx/graphics.h>
#include <cstdlib> 
#include <ctime>
#include <conio.h> 
#include<stdio.h>
#include<cstdio>
#include"Snake.hpp"


int MoveDiretion = RIGHT;
int score = 0;
int speedLevel = 1;
int dx = GRID_SIZE, dy = 0;// 初始设置向右移动
int highScore=0;
IMAGE img;
IMAGE menu;
IMAGE GameOver;
bool gameOver = false;
bool shouldReturnToMainMenu=false;
Food food;
//-------------函数实现------------------------------
//初始化窗口
void init_window(){
	initgraph(Window_width,Window_height);
	BeginBatchDraw();// 启用双缓冲模式
	setbkcolor(RGB(248,248,255));
	cleardevice();
}
//区域分割线
void darw_Area_dividing_line(){
	setlinecolor(RGB(132,112,255));
	line(Window_width*0.75,0,Window_height,Window_height);
	line(Window_height,Window_height*0.5,Window_width,Window_height*0.5);
}

//绘制方格线
void Draw_grid_line(){
	setlinecolor(RGB(220,220,220));
	// 绘制横竖的方格线
	for(int i = 0; i <= Window_height; i += Window_height / 20) {
		for(int j = 0; j <= Window_width * 0.75; j += Window_width * 0.75 / 20) {
			rectangle(j, i, j + Window_width * 0.75 / 20, i + Window_height / 20);
		}
	}
}

//打印提示信息
void Print_prompt_message(){
	settextcolor(RGB(106,90,205));
	settextstyle(30,0,"幼圆");
	setbkmode(TRANSPARENT);
	outtextxy(Window_width*0.75+50,50,"分   数");
	outtextxy(Window_width*0.75+50,50+Window_height*0.5,"速   度");
}
//创建蛇的节点
SnakeNode* createSnakeNode(int x,int y){
	SnakeNode* newNode = new SnakeNode;
	newNode->x=x;
	newNode->y=y;
	newNode->next=nullptr;
	return newNode;
}
//初始化蛇
SnakeNode* initSnake(){
	srand(static_cast<unsigned int>(time(nullptr)));
	SnakeNode* head = createSnakeNode(rand() % (Max_Snake_x/GRID_SIZE)*GRID_SIZE,rand()%(Max_Snake_y/GRID_SIZE)*GRID_SIZE);
	SnakeNode* current =head;
	
	for(int i=1;i<SNAKE_LENGTH;++i){
		SnakeNode* newNode = createSnakeNode(current->x-GRID_SIZE,current->y);
		current->next =newNode;
		current=newNode;
	}
	return head;
}
//绘制蛇
void darwSnake(SnakeNode* snake){
	SnakeNode* current = snake;
	while(current!=nullptr){
		setfillcolor(RGB(255,165,0));
		solidrectangle(current->x,current->y,current->x + GRID_SIZE, current->y + GRID_SIZE);
		setlinecolor(RGB(112,128,144));
		rectangle(current->x,current->y,current->x + GRID_SIZE, current->y + GRID_SIZE);
		current=current->next;
	}
}
// 移动蛇并删除尾部
void moveSnakeAndDeleteTail(SnakeNode*& snake, int dx, int dy) {
	// 创建新的头部节点前，先检查是否穿过边界
	int newX = snake->x + dx;
	int newY = snake->y + dy;
	// 检查并处理水平方向上的边界穿越
	if (newX < Min_Snake_x) {
		newX = Max_Snake_x+GRID_SIZE;
	} else if (newX >= Max_Snake_x+GRID_SIZE*2) {
		newX = Min_Snake_x;
	}
	// 检查并处理垂直方向上的边界穿越
	if (newY < Min_Snake_y) {
		newY = Max_Snake_y;
	} else if (newY >= Max_Snake_y+GRID_SIZE) {
		newY = Min_Snake_y;
	}
	// 创建新的头部节点
	SnakeNode* newHead = createSnakeNode(newX, newY);
	// 检查是否吃到食物
	if (isCollisionWithFood(newHead)) {
		int scoreIncrement = calculateScoreIncrease(speedLevel);
		score += scoreIncrement; // 根据速度增加分数
		generateFood(snake); // 生成新食物
		newHead->next = snake;
		snake = newHead;
		// 不删除尾部，蛇变长
	}
	else{
		newHead->next = snake;
		// 更新蛇头
		snake = newHead;
		// 遍历到蛇的尾部
		SnakeNode* current = snake;
		while (current->next->next != nullptr) { // 停在倒数第二个节点，因为最后一个节点是尾部
			current = current->next;
		}
		// 删除尾部节点
		SnakeNode* tailToDelete = current->next;
		clearSnakeBody(tailToDelete);
		current->next = nullptr;
		delete tailToDelete;
	}
}
//释放蛇的内存
void deleteSnake(SnakeNode* snake) {
	SnakeNode* current = snake;
	while (current != nullptr) {
		SnakeNode* temp = current;
		current = current->next;
		delete temp;
	}
}
//填充尾部色块
void clearSnakeBody(SnakeNode* snake) {
	SnakeNode* current = snake;
	setfillcolor(RGB(248,248,255));
	solidrectangle(current->x, current->y, current->x + GRID_SIZE, current->y + GRID_SIZE);
	setlinecolor(RGB(220,220,220));
	rectangle(current->x, current->y, current->x + GRID_SIZE, current->y + GRID_SIZE);
}
// 键盘输入处理函数
void handleKeyboardInput(int &dx, int &dy) {
	if (_kbhit()) {
		char key = _getch();
		switch (key) {
		case 'a':
		case 'A':
			case 75:	
			if(MoveDiretion!=RIGHT){
				dx = -GRID_SIZE;
				dy = 0;
				MoveDiretion=LEFT;
			}
			break;
		case 'd':
		case 'D':
		case 77:
			if(MoveDiretion!=LEFT){
				dx = GRID_SIZE;
				dy = 0;
				MoveDiretion=RIGHT;
			}
			break;
		case 'w':
		case 'W':
			case 72:	
			if(MoveDiretion!=DOWN){
				dx = 0;
				dy = -GRID_SIZE;
				MoveDiretion=UP;
			}	
			break;
		case 's':
		case 'S':
			case 80:	
			if(MoveDiretion!=UP){
				dx = 0;
				dy = GRID_SIZE;
				MoveDiretion=DOWN;
			}
			break;
		case ' ':
			while(1){
				if(_getch()==' ')
					break;
			}
			break;
		case 'C':
		case 'c':
			if(speedLevel<=4){
				speedLevel++;
			}
			break;
		case 'V':
			case 'v':	
			if(speedLevel>=2){
				speedLevel--;
			}
			break;
		case 27: 
			shouldReturnToMainMenu = true;
			break;
			
		default:
			// 不做处理或添加其他逻辑
			break;
		}
	}
	
}
void generateFood(SnakeNode* snake) {
	while (true) {
		food.x = rand() % (Max_Snake_x / GRID_SIZE) * GRID_SIZE; // 随机生成食物x坐标
		food.y = rand() % (Max_Snake_y / GRID_SIZE) * GRID_SIZE; // 随机生成食物y坐标
		
		// 检查新生成的食物位置是否与蛇身重叠
		bool isOverlap = false;
		SnakeNode* current = snake;
		while (current != nullptr) {
			if (current->x == food.x && current->y == food.y) {
				isOverlap = true;
				break;
			}
			current = current->next;
		}
		
		// 如果没有重叠，则跳出循环
		if (!isOverlap) {
			break;
		}
	}
}
bool isCollisionWithFood(SnakeNode* snake) {
	return snake->x == food.x && snake->y == food.y;
}
void drawFood() {
	setfillcolor(RGB(255, 20 ,147)); // 设置食物颜色为绿色
	//solidrectangle(food.x, food.y, food.x + GRID_SIZE, food.y + GRID_SIZE);
	putimage(food.x, food.y,&img);
}
bool isCollisionWithSelf(SnakeNode* snake) {
	SnakeNode* current = snake->next; // 从第二个节点开始检查，避免与自己头部比较
	while (current != nullptr) {
		if (snake->x == current->x && snake->y == current->y) {
			return true; // 发生碰撞
		}
		current = current->next;
	}
	return false; // 未发生碰撞
}
void drawScore() {
	char scoreText[50], speedText[10];
	sprintf(scoreText, "  %d", score);
	sprintf(speedText, "  %d", speedLevel);
	settextcolor(RGB(106,90,205));
	settextstyle(40,0,"幼圆");
	setfillcolor(RGB(248,248,255));
	setbkmode(TRANSPARENT);
	solidrectangle(Window_width*0.75+50,100,Window_width*0.75+200,200);
	outtextxy(Window_width*0.75+50,150,scoreText);
	solidrectangle(Window_width*0.75+50,100+Window_height*0.5,Window_width*0.75+200,200+Window_height*0.5);
	outtextxy(Window_width*0.75+50,150+Window_height*0.5,speedText);
	
}
int getDelayTimeBySpeedLevel() {
	return 200 - (speedLevel - 1) * 20;
}
void redrawGameUI(){
	BeginBatchDraw();
	setbkcolor(RGB(248,248,255));
	cleardevice();
	darw_Area_dividing_line(); // 绘制区域分割线
	Print_prompt_message();   // 打印提示信息
	Draw_grid_line(); // 绘制方格线
	EndBatchDraw();
}
void gameDraw(SnakeNode* snake){
	BeginBatchDraw();
	darwSnake(snake);
	drawFood();
	drawScore();
	EndBatchDraw();
}
void showGameOverScreen() {
	putimage(0,0,&GameOver);
	EndBatchDraw();
}
void waitForRestart() {
	while (!_kbhit() || _getch() != ' ') {} // 等待空格键按下
	// 理论上，这里应该调用resetGame后直接进入下一个循环迭代，
	// 但由于我们已将重绘逻辑放入resetGame中，因此只需等待用户输入即可。
}
void resetGame(SnakeNode*& snake) {
	deleteSnake(snake); // 释放蛇的内存
	snake = initSnake(); // 重新初始化蛇
	score = 0; // 重置分数
	speedLevel=1;
	MoveDiretion = RIGHT;// 重置初始方向
	dx=GRID_SIZE,dy=0;
	generateFood(snake); // 重新生成食物
	highScore = loadHighScore();
	printf("历史最高分：%d\n", highScore);
	gameOver = false; // 重置游戏结束标志
	
}
int calculateScoreIncrease(int speedLevel) {
	return 0 + speedLevel;
}
void showMainMenu() {
	initgraph(Window_width,Window_height);
	setbkcolor(RGB(248,248,255));
	cleardevice();
	putimage(0,0,&menu);
	while (true) {
		if (_kbhit()) {
			char keyPressed = _getch();
			if (keyPressed == 13) { // Enter键
				break; // 用户按下Enter，跳出循环开始游戏
			} else if (keyPressed == 27) { // ESC键
				printf("游戏退出。\n");
				closegraph(); // 关闭图形窗口
				exit(0); // 退出程序
			}
			else if(keyPressed==82||keyPressed==114){
				darwHighScore();
			}
		}
	}
}
int loadHighScore() {
	FILE* highScoreFile = fopen("highscore.txt", "r");
	if (highScoreFile == NULL) {
		printf("无法打开高分文件，可能是首次运行。\n");
		return 0;
	}
	
	int highScore = 0;
	fscanf(highScoreFile, "%d", &highScore);
	fclose(highScoreFile);
	return highScore;
}
void saveHighScore(int newScore) {
	FILE* highScoreFile = fopen("highscore.txt", "w");
	if (highScoreFile == NULL) {
		printf("无法打开文件以保存高分。\n");
		return;
	}
	fprintf(highScoreFile, "%d", newScore);
	fclose(highScoreFile);
	printf("新高分已保存！\n");
}
void darwHighScore(){
	//initgraph(Window_width,Window_height);
	setbkcolor(RGB(248,248,255));
	cleardevice();
	setlinecolor(RGB(72,61,139));
	line(100,200,700,200);
	line(100,500,700,500);
	line(100,200,100,500);
	line(700,200,700,500);
	char scoreText[50];
	sprintf(scoreText, "Highest Score: %d", highScore);
	settextstyle(40,0,"幼圆");
	settextcolor(RGB(255,106,106));
	outtextxy(200,100,"历史最高分");
	outtextxy(150,300,scoreText);
	while (true) {
		if (_kbhit()) {
			char keyPressed = _getch();
			if (keyPressed == 27) {// esc键
				showMainMenu();
				break; 
			}
		}
	}
}

