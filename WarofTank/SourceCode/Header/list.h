#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"

bool g_bStart	    =	false;  	// 控制一局游戏开始true与结束false
float g_fGameTime	=	0.f;    	// 一局游戏的剩余时间
int	g_iScore	    =	0;		// 一局游戏得分
const float	GAME_TIME	=	30.f;		// 一局游戏的时间
const float	TANK_SPEED	=	5.f;		// 坦克速度
const  float 	BULLET_SPEED	 = 	8.f;		// 子弹速度
const  float	FIRE_TIME		 = 	2.f;		// 坦克开炮时间间隔
const  float 	CREATE_TANK_TIME = 	5.f;		// 每批次生成坦克的时间间隔
float g_fCreateTank;	// 距离生成下一批敌方坦克的剩余时间


struct Weapon{
	char		szName[128];			// 精灵名称
	float		fPosX,  fPosY;			// 精灵坐标
	float		fSpeedX,  fSpeedY;	// X和Y方向上速度
	float		fFireTime;			// 敌方坦克距下一次开炮的剩余时间
	int		iHp;					// 生命值
	int		iDir;		            // 朝向：0 - 上方；1 - 右方；2 - 下方；3 - 左方
	int		iType;               // 类型： 0 - 我方坦克；1 - 敌方坦克；2 - 我方
// 子弹； 3 - 敌方子弹
	Weapon* pNext;			    // 指向下一个节点的指针
};
