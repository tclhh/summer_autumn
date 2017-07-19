#include <Windows.h>
#include "LinkList.h"

#ifndef _BREAKER_H
#define _BREAKER_H

#define BLOCK_LENGTH 40//一行15块砖
#define BLOCK_HEIGHT 20//共10行

#define BREAKER_LENGTH 60
#define BREAKER_HEIGHT 10

#define TOOL_LENGTH 40
#define TOOL_HEIGHT 20

#define BALL_LENGTH 15

#define MAX_X 600 //右边界
#define MAX_Y 600 //下边界

#define MAGIC_BLOCK_S 10
#define BLOCK_S 20
#define TOOL_BLOCK_S 10
#define IRON_BLOCK_S 40
#define DIR_BLOCK_S 0

#define BREAKER_STEP 3
#define BLOCK_NUM 150

typedef enum GAMEEND
{
	YOUWIN,
	YOULOST,
	NO
}GAMEEND;

typedef enum DIRRCTION
{
	LEFT,
	TOP,
	RIGHT,
	BOTTOM,
	ANGLE1,
	ANGLE2,
	ANGLE3,
	ANGLE4
}DIRECTION;

typedef enum BLOCK_TYPE
{
	BLOCK_TYPE_NONE,
	BLOCK_TYPE_MAGIC,
	BLOCK_TYPE_IRON,
	BLOCK_TYPE_DIR,
	BLOCK_TYPE_BLOCK,
	BLOCK_TYPE_TOOL
}BLOCK_TYPE;

typedef enum BALL_TYPE
{
	BALL_TYPE_NONE,
	BALL_TYPE_FLAME,
	BALL_TYPE_ICE,
	BALL_TYPE_ELEC,
	BALL_TYPE_TIMER,
	BALL_TYPE_LIG
}BALL_TYPE;

typedef enum TOOL
{
	TOOL_TYPE_LENGTH,
	TOOL_TYPE_LIG,
	TOOL_TYPE_SPEEDER,
	TOOL_TYPE_SPEEDLL,
	TOOL_TYPE_FISSION
}TOOL_TYPE;

class Object
{
public:
	Object(int,int,int,int);
	/*~Object();*/
	void set_x(int );
	void set_y(int );
	void set_length(int);
	int get_length();
	void set_height(int);
	int get_x();
	int get_y();
protected:
	int x;//最左上横坐标
	int y;//最左上纵坐标
	int length;//长度
	int height;//高度
};

class Tool:public Object
{
public:
	Tool(int x,int y,int length ,int height ,TOOL_TYPE type,int drop = 1);
	TOOL_TYPE get_tool_type();
	void tMoving();
	bool IsCatched();
	bool IsDisappear();
protected:
	int dropSpeed;
	TOOL_TYPE type;
};

class Block:public Object
{
public:
	Block(int x,int y,int length,int height,BLOCK_TYPE type);
	/*~Block();*/
	int get_sorce();//单个砖块分数
	void set_type(BLOCK_TYPE);
	BLOCK_TYPE get_block_type();
protected:
	BLOCK_TYPE type;
};

class Ball:public Object
{
public:
	Ball(int x = MAX_X/2,int y = MAX_Y-BREAKER_HEIGHT-BALL_LENGTH,int length = BALL_LENGTH,int height = BALL_LENGTH,BALL_TYPE type = BALL_TYPE_NONE,int galaxy_x = 3,int galaxy_y = 3);
	void IsHited();
	bool IsDead();
	void Moving();
	void setga_x(int);
	int getga_x();
	void setga_y(int);
	int getga_y();
	void set_ball_type(BALL_TYPE);
	BALL_TYPE get_ball_type();
	void magic_gen(int);
	void dir_change(int,DIRECTION);
	DIRRCTION get_dir(int );
protected:
	BALL_TYPE type;
	int galaxy_x;//单位时间横坐标的移动量
	int galaxy_y;//单位时间纵坐标的移动量
};

class Breaker:public Object
{
public:
	Breaker(int x = MAX_X/2,int y = MAX_Y-BREAKER_HEIGHT,int length = BREAKER_LENGTH,int height = BREAKER_HEIGHT,
		int lignum = 0,int intensify = 0,int speed = BREAKER_STEP);
	/*~Breaker();*/
	void setspeed(int);
	int getspeed();
	void setPower(int =0 ,int = 0);
	int getPowerNum();
	int  getPowerId();
	void LMoving();
	void RMoving();
	void Re_incarnation();
protected:
	int LigNum;//激光数量,上限为8
	int intensify;//若强化则值为1，否则为零
	int speed;
};

unsigned int get_block_num(PLIST);//返回砖块的数量，以便判断游戏是否更新
//unsigned int get_ball_num(PLIST);//返回球的数量，判断游戏是否结束

DWORD get_sorce();//返回游戏的总分数
GAMEEND GameOver(PLIST list_ball);

void CreateBlock(int x,int y,int length,int height);
void CreateTool(int x,int y,int length,int height);
void CreateBall (int x = MAX_X/2,int y = MAX_Y - BALL_LENGTH-BREAKER_HEIGHT,int galaxy_x = 2,int galaxy = 2);

BLOCK_TYPE get_block_type();
TOOL_TYPE get_tool_type();
void tool_destroy();
void block_destory();
Tool* get_tool_at ( unsigned int );
Block* get_block_at(unsigned int);
Ball* get_ball_at(unsigned int);
int get_block_num();
void tool_effect(int);
int bullet_gen();
double sqrtlength(double x1,double y1,double x2,double y2);

#endif
