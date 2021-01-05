#include <time.h>
#include <stdlib.h>
#include "breaker.h"
#include "LinkList.h"

PLIST list_block = ListCreate(NULL);
PLIST list_ball = ListCreate(NULL);
PLIST list_tool = ListCreate(NULL);
Breaker breaker;

DWORD dwSorce = 0;//总得分
double timer = 270;
//Object类中函数的定义
int Object::get_x()
{
	return x;
}
int Object::get_y()
{
	return y;
}
void Object::set_x(int a)
{
	x = a;
}
void Object::set_y(int b)
{
	y = b;
}
void Object::set_length(int c)
{
	length = c;
}
void Object::set_height(int d)
{
	height = d;
}
Object::Object(int a,int b,int c,int d)
	:x(a),y(b),length(c),height(d){}
int Object::get_length()
{
	return length;
}
//定义Block类的函数
Block::Block(int a,int b,int c,int d,BLOCK_TYPE e)
	:Object(a,b,c,d),type(e){}
int Block::get_sorce()
{
	if (type == BLOCK_TYPE_MAGIC)
		return MAGIC_BLOCK_S;
	else if (type == BLOCK_TYPE_IRON)
		return IRON_BLOCK_S;
	else if (type == BLOCK_TYPE_DIR)
		return DIR_BLOCK_S;
	else if (type == BLOCK_TYPE_NONE)
		return BLOCK_S;
	else if (type == BLOCK_TYPE_TOOL)
		return TOOL_BLOCK_S;
}
void Block::set_type(BLOCK_TYPE a)
{
	type = a;
}
BLOCK_TYPE Block::get_block_type()
{
	return type;
}

//定义Ball中的函数
Ball::Ball(int a,int b,int c,int d,BALL_TYPE e,int f,int g)
	:Object(a,b,c,d),type(e),galaxy_x(f),galaxy_y(g){}
void Ball::magic_gen(int rand_1)
{
	if (rand_1 >= 1 && rand_1 <= 10){
		set_ball_type(BALL_TYPE_ICE);
		timer += 15;
		setga_x(getga_x()-1);
		setga_y(getga_y()-1);
	}
	else if (rand_1 <=20){
		set_ball_type(BALL_TYPE_TIMER);
		timer += 60;
	}
	else if (rand_1 <= 40){
		set_ball_type(BALL_TYPE_FLAME);
	}
	else if (rand_1 <= 50){
		set_ball_type(BALL_TYPE_LIG);
		breaker.setPower(breaker.getPowerNum(),1);
	}
	else if (rand_1 <= 70){
		set_ball_type(BALL_TYPE_ELEC);
	}
	else if (rand_1  <= 80)
	{
		set_ball_type(BALL_TYPE_NONE);
		breaker.Re_incarnation();
	}
	else{
		//可以与原属性相同
		magic_gen(rand()%70 + 1);
	}
}

void Ball::Moving()
{
	IsHited();//判断球是否被撞，若碰撞则改变方向
	x += galaxy_x;//横坐标变换后的值
	y += galaxy_y;//纵坐标变换后的值
}

void Ball::dir_change(int b,DIRECTION dir)
{
	//球是火球
	if (type == BALL_TYPE_FLAME )
	{
		if (get_block_at(b)->get_block_type() == BLOCK_TYPE_MAGIC)
			this->magic_gen(rand()%100+1);
		else if(get_block_at(b)->get_block_type() == BLOCK_TYPE_TOOL)
			CreateTool(get_block_at(b)->get_x(),get_block_at(b)->get_y(),
				TOOL_LENGTH,TOOL_HEIGHT);

		if ((get_block_at(b)->get_block_type() == BLOCK_TYPE_DIR) || (dir == ANGLE1 && galaxy_x > 0 && galaxy_y > 0) || (dir == ANGLE2 && galaxy_x < 0 && galaxy_y > 0) ||
				(dir == ANGLE3 && galaxy_x > 0 && galaxy_y < 0) || (dir == ANGLE4 && galaxy_x < 0 && galaxy_y < 0))
		{
			galaxy_x = -galaxy_x;
			galaxy_y = -galaxy_y;
		}
		else if ((dir == LEFT && galaxy_x > 0) || (dir == RIGHT && galaxy_x <0))
			galaxy_x  = -galaxy_x;
		else if ((dir == TOP && galaxy_y > 0) || (dir == BOTTOM && galaxy_y < 0))
			galaxy_y = - galaxy_y;

		get_block_at(b)->set_type(BLOCK_TYPE_NONE);
		dwSorce += get_block_at(b)->get_sorce();
	}
	//球是电球
	else if (type == BALL_TYPE_ELEC) { 
		//砖是铁砖
		if (get_block_at(b)->get_block_type() == BLOCK_TYPE_IRON)
		{
			if ( (dir == ANGLE1 && galaxy_x > 0 && galaxy_y > 0) || (dir == ANGLE2 && galaxy_x < 0 && galaxy_y > 0) ||
				(dir == ANGLE3 && galaxy_x > 0 && galaxy_y < 0) || (dir == ANGLE4 && galaxy_x < 0 && galaxy_y < 0))
			{
				galaxy_x = -galaxy_x;
				galaxy_y = -galaxy_y;
			}
			else if ((dir == LEFT && galaxy_x > 0) || (dir == RIGHT && galaxy_x < 0))
				galaxy_x  = -galaxy_x;
			else if ((dir == TOP && galaxy_y > 0) || (dir == BOTTOM && galaxy_y < 0))
				galaxy_y = - galaxy_y;
		}
		//砖不是铁砖
		else
		{
			if (get_block_at(b)->get_block_type() == BLOCK_TYPE_MAGIC)
				this->magic_gen(rand()%100+1);
			else if(get_block_at(b)->get_block_type() == BLOCK_TYPE_TOOL)
				CreateTool(get_block_at(b)->get_x(),get_block_at(b)->get_y(),
					TOOL_LENGTH,TOOL_HEIGHT);

			get_block_at(b)->set_type(BLOCK_TYPE_NONE);
			dwSorce += get_block_at(b)->get_sorce();
		}
	}
	//球不是火，电球
	else {
		//砖是铁砖
		if (get_block_at(b)->get_block_type() == BLOCK_TYPE_IRON)
		{
			if ( (dir == ANGLE1 && galaxy_x > 0 && galaxy_y > 0) || (dir == ANGLE2 && galaxy_x < 0 && galaxy_y > 0) ||
				(dir == ANGLE3 && galaxy_x > 0 && galaxy_y < 0) || (dir == ANGLE4 && galaxy_x < 0 && galaxy_y < 0))
			{
				galaxy_x = -galaxy_x;
				galaxy_y = -galaxy_y;
			}

			else if ((dir == LEFT && galaxy_x > 0) || (dir == RIGHT && galaxy_x < 0))
				galaxy_x  = - galaxy_x;
			else if ((dir == TOP && galaxy_y > 0) || (dir == BOTTOM && galaxy_y < 0))
				galaxy_y = - galaxy_y;
		}
		//砖不是铁砖
		else
		{
			if (get_block_at(b)->get_block_type() == BLOCK_TYPE_MAGIC)
				this->magic_gen(rand()%100+1);
			else if(get_block_at(b)->get_block_type() == BLOCK_TYPE_TOOL)
				CreateTool(get_block_at(b)->get_x(),get_block_at(b)->get_y(),
					TOOL_LENGTH,TOOL_HEIGHT);

			if ((get_block_at(b)->get_block_type() == BLOCK_TYPE_DIR) || (dir == ANGLE1 && galaxy_x > 0 && galaxy_y > 0) || (dir == ANGLE2 && galaxy_x < 0 && galaxy_y > 0) ||
				(dir == ANGLE3 && galaxy_x > 0 && galaxy_y < 0) || (dir == ANGLE4 && galaxy_x < 0 && galaxy_y < 0))
			{
				galaxy_x = -galaxy_x;
				galaxy_y = -galaxy_y;
			}
			else if ((dir == LEFT && galaxy_x > 0) || (dir == RIGHT && galaxy_x <0))
				galaxy_x  = -galaxy_x;
			else if ((dir == TOP && galaxy_y > 0) || (dir == BOTTOM && galaxy_y < 0))
				galaxy_y = - galaxy_y;

			get_block_at(b)->set_type(BLOCK_TYPE_NONE);
			dwSorce += get_block_at(b)->get_sorce();
		}	
	}
}

//碰撞检测
void Ball::IsHited()
	{
	//第一种情况，碰到左右两侧的墙壁
	if ((x <= 0 && galaxy_x < 0)|| (x >= MAX_X-BALL_LENGTH && galaxy_x > 0))
		galaxy_x = -galaxy_x;

	//第二种情况，碰到上侧墙壁。若为下侧，y越界直接判断死亡
	if (y <= 0 /*|| y > MAX_Y-10*/)
		galaxy_y = -galaxy_y;

	//遍历每一块砖判断是否碰撞
	for (int i = 0; i < BLOCK_NUM;i ++)
	{
		if (get_block_at(i)->get_block_type() != BLOCK_TYPE_NONE && x >= get_block_at(i)->get_x() - BALL_LENGTH && x <= get_block_at(i)->get_x() + BLOCK_LENGTH
			&& y <= get_block_at(i)->get_y() + BLOCK_HEIGHT && y >= get_block_at(i)->get_y() - BALL_LENGTH)
		{
			//判断左上方
			if (x + BALL_LENGTH/2 < get_block_at(i)->get_x() && y + BALL_LENGTH/2 < get_block_at(i)->get_y())
			{
				if (sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2, get_block_at(i)->get_x(),get_block_at(i)->get_y()) > BALL_LENGTH*BALL_LENGTH/4 )
					continue;
			}
			//判断右上方
			else if (x+BALL_LENGTH/2 > get_block_at(i)->get_x() + BLOCK_LENGTH && y + BALL_LENGTH/2 <  get_block_at(i)->get_y())
			{
				if (sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x() + BLOCK_LENGTH,get_block_at(i)->get_y()) > BALL_LENGTH*BALL_LENGTH/4)
					continue;
			}
			//判断左下方
			else if (x + BALL_LENGTH/2 < get_block_at(i)->get_x() && y + BALL_LENGTH/2 > get_block_at(i)->get_y() + BLOCK_HEIGHT)
			{
				if(sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x(),get_block_at(i)->get_y()+BLOCK_HEIGHT) > BALL_LENGTH*BALL_LENGTH/4)
					continue;
			}
			//判断右下方
			else if (x + BALL_LENGTH/2 > get_block_at(i)->get_x() + BLOCK_LENGTH && y + BALL_LENGTH/2  > get_block_at(i)->get_y() + BLOCK_HEIGHT)
			{
				if (sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x()+BLOCK_LENGTH,get_block_at(i)->get_y()+BLOCK_HEIGHT) > BALL_LENGTH*BALL_LENGTH/4)
					continue;
			}
			else
				dir_change(i,get_dir(i));
		}
	}
	//第四种情况，碰到板
	if ( galaxy_y > 0 && y >= MAX_Y-BREAKER_HEIGHT-BALL_LENGTH && y <= MAX_Y-BREAKER_HEIGHT/2 &&
		x >= breaker.get_x()-breaker.get_length()/2 && x  <= breaker.get_x()+breaker.get_length()-BALL_LENGTH/2)
	{
		galaxy_y = -galaxy_y;
	}
}
DIRECTION Ball::get_dir(int i)
{
	//左上角
	if (x + BALL_LENGTH/2 <= get_block_at(i)->get_x() && y + BALL_LENGTH/2 <= get_block_at(i)->get_y()&&sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2, get_block_at(i)->get_x(),get_block_at(i)->get_y()) <= BALL_LENGTH*BALL_LENGTH/4)
		return ANGLE1;
	//右上角
	else if ((x+BALL_LENGTH/2 >= get_block_at(i)->get_x() + BLOCK_LENGTH) && (y + BALL_LENGTH/2 <=  get_block_at(i)->get_y()) && (sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x() + BLOCK_LENGTH,get_block_at(i)->get_y()) <= BALL_LENGTH*BALL_LENGTH/4))
		return ANGLE2;
	//左下角
	else if	((x + BALL_LENGTH/2 <= get_block_at(i)->get_x()) && (y + BALL_LENGTH/2 >= get_block_at(i)->get_y() + BLOCK_HEIGHT) &&(sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x(),get_block_at(i)->get_y()+BLOCK_HEIGHT) <= BALL_LENGTH*BALL_LENGTH/4))
		return ANGLE3;
	//右下角
	else if ((x + BALL_LENGTH/2 >= get_block_at(i)->get_x() + BLOCK_LENGTH) && (y + BALL_LENGTH/2  >= get_block_at(i)->get_y() + BLOCK_HEIGHT) && (sqrtlength(x + BALL_LENGTH/2,y + BALL_LENGTH/2,get_block_at(i)->get_x()+BLOCK_LENGTH,get_block_at(i)->get_y()+BLOCK_HEIGHT)<= BALL_LENGTH*BALL_LENGTH/4))
		return ANGLE4;
	else if (x + BALL_LENGTH/2 >= get_block_at(i)->get_x() && x + BALL_LENGTH/2 <= get_block_at(i)->get_x()+BLOCK_LENGTH 
		&& y + BALL_LENGTH/2 <= get_block_at(i)->get_y())
		return TOP;
	else if (x + BALL_LENGTH/2 >= get_block_at(i)->get_x() && x + BALL_LENGTH/2 <= get_block_at(i)->get_x()+BLOCK_LENGTH 
		&& y + BALL_LENGTH/2 >= get_block_at(i)->get_y() + BLOCK_HEIGHT )
		return BOTTOM;
	else if (x + BALL_LENGTH/2 <= get_block_at(i)->get_x() && y + BALL_LENGTH/2 <= get_block_at(i)->get_y() + BLOCK_HEIGHT&&
		y + BALL_LENGTH/2 >= get_block_at(i)->get_y())
		return LEFT;
	else 
		return RIGHT;
}

void Ball::setga_x(int a)
{
	galaxy_x = a;
}
int Ball::getga_x()
{
	return galaxy_x;
}
void Ball::setga_y(int a)
{
	galaxy_y = a;
}
int Ball::getga_y()
{
	return galaxy_y;
}

//通过纵坐标判断球是否死亡
bool Ball::IsDead()
{
	if (get_y() >= MAX_Y)
		return true;
	else
		return false;
}
void Ball::set_ball_type(BALL_TYPE a)
{
	type = a;
}
BALL_TYPE Ball::get_ball_type()
{
	return type;
}

//定义breaker函数
Breaker::Breaker(int a,int b,int c,int d,int e,int f,int g)
	:Object(a,b,c,d),LigNum(e),intensify(f),speed(g){}
void Breaker::setspeed(int a)
{
	speed = a;
}
int Breaker::getspeed()
{
	return speed;
}
void Breaker::setPower(int a,int b)
{
	LigNum = a;
	intensify = b;
}
int Breaker::getPowerNum()
{
	return LigNum;
}
int  Breaker::getPowerId()
{
	return intensify;
}
void Breaker::LMoving()
{
	if (breaker.get_x ()<= BREAKER_STEP )
		breaker.set_x(0);
	else
		breaker.set_x(breaker.get_x() - BREAKER_STEP);
}
void Breaker::RMoving()
{
	if (breaker.get_x ()+ BREAKER_STEP+ breaker.get_length()>=MAX_X)
		breaker.set_x(MAX_X-breaker.get_length());
	else
		breaker.set_x(breaker.get_x() + BREAKER_STEP);
}
void Breaker::Re_incarnation()
{
	set_length(BREAKER_LENGTH);
	setPower();
	setspeed(0);
}
//定义tool函数
Tool::Tool(int a,int b,int c,int d,TOOL_TYPE e,int f)
	:Object(a,b,c,d),type(e),dropSpeed(f){}
TOOL_TYPE Tool::get_tool_type()
{
	return type;
}
void Tool::tMoving()
{
	for (int i = 0;i < ListSize(list_tool);i++)
		get_tool_at(i)->y += get_tool_at(i)->dropSpeed;
}		
bool Tool::IsCatched()
{
	if (x <= BREAKER_LENGTH + breaker.get_x() + TOOL_LENGTH/2 && x >= breaker.get_x() - TOOL_LENGTH/2 &&
		y <= MAX_Y - TOOL_HEIGHT/2 && y >= MAX_Y - BREAKER_HEIGHT - TOOL_HEIGHT)
		return true;
	else
		return false;
}

bool Tool::IsDisappear()
{
	if (IsCatched())
		return true;
	else if (y >= MAX_Y)
		return true;
	else
		return false;
}

//全局函数
BLOCK_TYPE get_block_type()
{
	//初始化随机数
	int rand_0 = rand()%100+1;
	if (rand_0 >= 1 && rand_0 <= 8)
		return BLOCK_TYPE_MAGIC;
	else if (rand_0 <= 18)
		return BLOCK_TYPE_TOOL;
	else if (rand_0 <= 28)
		return BLOCK_TYPE_IRON;
	else if (rand_0 <= 42)
		return BLOCK_TYPE_DIR;
	else
		return BLOCK_TYPE_BLOCK;
}

TOOL_TYPE get_tool_type()
{
	int rand_3 = rand()%100 + 1;
	if (rand_3 >= 1 && rand_3 <= 10)
		return TOOL_TYPE_LIG;
	else if (rand_3 <= 30)
		return TOOL_TYPE_FISSION;
	else if (rand_3 <= 50)
		return TOOL_TYPE_SPEEDLL;
	else if (rand_3 <= 75)
		return TOOL_TYPE_SPEEDER;
	else
		return TOOL_TYPE_LENGTH;
}
void CreateBlock(int a,int b,int c,int d)
{
	ListPushBack(list_block,new Block(a,b,c,d,get_block_type()));
}

void CreateTool(int a,int b,int c,int d)
{
	ListPushBack(list_tool,new Tool(a,b,c,d,get_tool_type()));
}

void CreateBall(int a,int b,int c,int d)
{
	ListPushBack(list_ball,new Ball(a,b,c,d,BALL_TYPE_NONE));
}

void block_destory()
{
	ListClearAndFree(list_block);
	list_block = 0;
}

void tool_destroy()
{
	for (int i = ListSize(list_tool)-1;i >= 0;i--)
	{
		if (get_tool_at(i)->IsDisappear())
			ListDeleteAt(list_tool,i);
	}
}
GAMEEND GameOver(PLIST list_ball)
{
	//通过判断IsDead，判断小球是否全灭
	int i,flag1 = 0,flag2 = 0;
	for (i = ListSize(list_ball)-1; i >= 0;i--)
	{
		if (get_ball_at(i)->IsDead())
			ListDeleteAt(list_ball,i);
	}
	for (i = ListSize(list_ball)-1 ; i >= 0; i--)
	{
		if (get_ball_at(i)->get_ball_type() == BALL_TYPE_LIG)
		{
			breaker.setPower(breaker.getPowerNum(),1);
			flag1 = 1;
		}
	}
	if (flag1 == 0)
		breaker.setPower(breaker.getPowerNum(),0);
	else
		flag1 = 0;
	//通过判断、链表是否为空判断游戏是否结束
	if (ListSize(list_ball) <= 0)
		return YOULOST;
	else {
		for (int j = 0; j < BLOCK_NUM;j++ )
		{
			if (get_block_at(j)->get_block_type() != BLOCK_TYPE_NONE)
			{
				flag2 = 1;
				break;
			}
		}
		if (flag2 == 1)
		{
			flag2 = 0;
			return NO;
		}
		if (flag2 == 0)
			return YOUWIN;
	}
}

DWORD get_sorce()
{
	return dwSorce;
}

Block* get_block_at(unsigned int n)
{
	return (Block*)ListGetAt(list_block,n);
}

Ball* get_ball_at(unsigned int n)
{
	return (Ball*)ListGetAt(list_ball,n);
}
Tool* get_tool_at ( unsigned int n)
{
	return (Tool*)ListGetAt(list_tool,n);
}
int get_block_num()
{
	return ListSize(list_block);
}
void tool_effect(int i)
{
	if (get_tool_at(i) ->get_tool_type() == TOOL_TYPE_SPEEDER)
		breaker.setspeed(breaker.getspeed()+2);
	else if (get_tool_at(i)->get_tool_type() == TOOL_TYPE_SPEEDLL)
	{
		for (int j = 0; j < ListSize(list_ball) ; j++)
		{
			get_ball_at(j)->setga_x(get_ball_at(j)->getga_x()+1);
			get_ball_at(j)->setga_y(get_ball_at(j)->getga_y()+1);
			if (get_ball_at(j)->getga_x() == 0)
				get_ball_at(j)->setga_x(1);
			if (get_ball_at(j)->getga_y() == 0 )
				get_ball_at(j)->setga_y(1);
		}
	}
	else if (get_tool_at(i)->get_tool_type() == TOOL_TYPE_LIG)
	{
		if (breaker.getPowerNum() <= 5)
			breaker.setPower(breaker.getPowerNum()+3,breaker.getPowerId());
		else
			breaker.setPower(8,breaker.getPowerId());
	}
	else if (get_tool_at(i)->get_tool_type() == TOOL_TYPE_FISSION)
	{
		if (ListSize(list_ball) < 2) {
			CreateBall(get_ball_at(0)->get_x(),get_ball_at(0)->get_y());
			if(get_ball_at(0)->getga_x() > 0)
				get_ball_at(1)->setga_x(-get_ball_at(1)->getga_x());
		}
	}
	else if (get_tool_at(i)->get_tool_type() == TOOL_TYPE_LENGTH)
	{
		if (breaker.get_length() <= 100)
			breaker.set_length(breaker.get_length()+20);
	}
	else if (get_tool_at(i)->get_tool_type() == TOOL_TYPE_SPEEDLL)
	{
		for (int k = 0;k < ListSize(list_ball);k++)
		{
			get_ball_at(k)->setga_x(get_ball_at(k)->getga_x()+1);
			get_ball_at(k)->setga_y(get_ball_at(k)->getga_y()+1);
		}
	}
}

int bullet_gen()
{
	if (breaker.getPowerNum() > 0)
	{
		if (breaker.getPowerId() != 0)
		{
			for (int i = 10; i >= 1;i-- )
			{
				get_block_at(15*(i-1) + (breaker.get_x() + breaker.get_length()/2)/BLOCK_LENGTH )->set_type(BLOCK_TYPE_NONE);
			}
			breaker.setPower(breaker.getPowerNum()-1,breaker.getPowerId());
			return 0;
		}
		else {
			for (int i = 9; i >= 0;i-- )
			{
				if (get_block_at(15*i + (breaker.get_x() + BREAKER_LENGTH/2)/BLOCK_LENGTH) ->get_block_type() != BLOCK_TYPE_NONE)
				{
					get_block_at(15*i + (breaker.get_x() + BREAKER_LENGTH/2)/BLOCK_LENGTH )->set_type(BLOCK_TYPE_NONE);
					break;			
				}
				breaker.setPower(breaker.getPowerNum()-1,breaker.getPowerId());
				return (i+1)*BLOCK_HEIGHT;
			}
		}
	}
}
double sqrtlength(double x1,double y1,double x2,double y2)
{
	return (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
}
