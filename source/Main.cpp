#include<easyx.h>
#include<graphics.h>
#include <cstdlib> 
#include <ctime>
#include <conio.h> 
#include<stdio.h>
#include<cstdio>
#include<mmsystem.h>
#pragma	comment(lib,"winmm.lib")
#include"Snake.hpp"

//-------------主循环--------------------------------
int main() {
	loadimage(&img, "./img/OIP-C.jpg", GRID_SIZE, GRID_SIZE);
	loadimage(&menu,"./img/mainscen.png",Window_width,Window_height);
	loadimage(&GameOver,"./img/GAME OVER.jpg",Window_width,Window_height);
	highScore = loadHighScore();
	printf("历史最高分：%d\n", highScore);
	showMainMenu();
	init_window();
	darw_Area_dividing_line();
	Print_prompt_message();
	Draw_grid_line();
	SnakeNode* snake = initSnake();
	darwSnake(snake);
	EndBatchDraw();
	generateFood(snake);
	while(1){
		// 处理键盘输入并更新移动方向
		handleKeyboardInput(dx, dy);
		// 移动蛇并删除尾部
		moveSnakeAndDeleteTail(snake, dx, dy);
		if(isCollisionWithSelf(snake)){
			// 游戏结束处理
			gameOver=true;
			printf("Game Over!\n");
			if (score > highScore) {
				highScore = score;
				printf("恭喜！新高分： %d\n", highScore);
				saveHighScore(highScore); // 保存新高分
			}
			showGameOverScreen();
			waitForRestart();
			resetGame(snake);
			redrawGameUI();
			//Sleep(1000);
		}
		// 绘制蛇
		gameDraw(snake);
		// 控制游戏速度
		Sleep(getDelayTimeBySpeedLevel());
	}
	deleteSnake(snake);
	system("pause");
	closegraph();
	return 0;
}



