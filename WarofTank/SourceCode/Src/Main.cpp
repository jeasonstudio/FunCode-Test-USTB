//-----------------------------------------------------------------------------
// 坦克大战
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "stdio.h"
#include "List.h"


// 全局变量、常量声明与初始化



Weapon*		g_pMyTank		=	NULL;
Weapon*		g_pMyTank1		=	NULL;	// 我方坦克
bool		g_bStart		=	false;  // 控制一局游戏开始与结束
int			g_iScore		=	0;		// 一局游戏得分
float		g_fGameTime		=	0.f;    // 一局游戏的剩余时间
int			g_iWallCount	=	0;		// 记录墙的数量
int			g_iMap[11][13];				// 地图数组
int			g_iNum			=	0;		// 记录我方子弹、我方坦克、敌方坦克的数量
float		g_fCreateTime	=	0.f;	// 每批次生成坦克的剩余时间

const float	FIRE_TIME		=  2.f;		// 敌方坦克发射炮弹的时间间隔
const float CREATE_TANK_TIME = 5.f;		// 每批次生成坦克的时间间隔
const float	GAME_TIME		=	180.f;	// 一局游戏的时间
const float	TANK_SPEED		 = 10.f;     // 敌方坦克速度
const float BULLET_SPEED	 = 20.f;		// 子弹速度
const float ROTATE_TIME		 = 2.f;     // 敌方坦克调转方向的时间

const float	WORLD_LEFT	     = -26.f;	// 游戏场景边界左值
const float	WORLD_TOP	     = -22.f;	// 游戏场景边界左值
const float	WORLD_RIGHT	     = 26.f;	// 游戏场景边界左值
const float	WORLD_BOTTOM     = 22.f;	// 游戏场景边界左值


// 函数声明

void	MoveMyTank(int, bool);
void	LoadMap();
void	OnFire(char*, int);
void	CreateEnemyTanks();
void	CreateEnemy2Tanks();
void	MoveEnemyTank(const char*);
bool	IsDead(const char*);

// 碰撞函数

void	OnMyTankColOther(const char*, const char*);
void	OnBulletColOther(const char*, const char*);
void	OnWallColOther(const char*, const char*);
void	OnTankColOther(const char*, const char*);
void	OnGoalColOther();

///////////////////////////////////////////////////////////////////////////////////////////
//
// 主函数入口
//
//////////////////////////////////////////////////////////////////////////////////////////

int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{

	// 初始化游戏引擎
	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;


	// ---------------- 游戏初始化代码 -----------

	dSetWindowTitle("TankWar");  // 设置游戏窗口标题

	// 创建玩家坦克

	g_pMyTank = (Weapon*)malloc(sizeof(Weapon));
	strcpy(g_pMyTank->szName, "player");
	g_pMyTank->iHp = 1;
	dSetSpriteWorldLimit(g_pMyTank->szName, WORLD_LIMIT_NULL, WORLD_LEFT, WORLD_TOP, WORLD_RIGHT, WORLD_BOTTOM); // 设置我方坦克世界边界
	dSetSpriteCollisionSend(g_pMyTank->szName, true);
    g_pMyTank1 = (Weapon*)malloc(sizeof(Weapon));
	strcpy(g_pMyTank1->szName, "player1");
	g_pMyTank1->iHp = 1;
	dSetSpriteWorldLimit(g_pMyTank1->szName, WORLD_LIMIT_NULL, WORLD_LEFT, WORLD_TOP, WORLD_RIGHT, WORLD_BOTTOM); // 设置我方坦克世界边界
	dSetSpriteCollisionSend(g_pMyTank1->szName, true);

	float fPosX0, fPosY0;  // 初始界面时，坦克的坐标

	fPosX0 = dGetSpritePositionX(g_pMyTank->szName);
	fPosY0 = dGetSpritePositionY(g_pMyTank->szName);

    float fPosX1, fPosY1;  // 初始界面时，坦克的坐标

	fPosX1 = dGetSpritePositionX(g_pMyTank1->szName);
	fPosY1 = dGetSpritePositionY(g_pMyTank1->szName);

	bool	bLoad	= true;		// 是否载入地图


	// -------------------------------------------


	// 游戏主循环

	while( dEngineMainLoop() ) // 运行游戏引擎，刷新游戏屏幕。当引擎关闭时，返回false
	{
		// ---------- 游戏主循环代码 -------------

		// 获取两次调用之间的时间差，传递给游戏逻辑处理
		float	fTimeDelta	=	dGetTimeDelta();


  		if(g_bStart)
		{
			if(bLoad)  // 一局游戏只需要载入一次地图即可
			{
				// 载入地图
				LoadMap();
				bLoad = false;
            }

			g_fGameTime -= fTimeDelta;
			if(g_fGameTime > 0.f) // 一局游戏进行中
			{
				dSetTextValue("time", (int)g_fGameTime);
				dSetTextValue("score", g_iScore);

				// 时间到，生成新一批敌方坦克
				g_fCreateTime -= fTimeDelta;
				if(g_fCreateTime < 0.f)
				{
					//tNum++;
					g_fCreateTime = CREATE_TANK_TIME;
					CreateEnemyTanks();
                    CreateEnemy2Tanks();
				}


				// 链表节点指针
				Weapon* pNode	= g_pHead;
				int num = GetListSize();  // 当前链表的节点数量
				// 遍历当前链表，寻找敌方坦克节点
				for(int i=0; i<num; i++)
				{
					if(pNode->iType == ENEMYTANK)  // 敌方坦克
					{
						// 到敌方坦克调转方向时间
						pNode->fRotateTime -= fTimeDelta;
						if(pNode->fRotateTime < 0.f)
						{
							pNode->fRotateTime = ROTATE_TIME;
							MoveEnemyTank(pNode->szName);
						}
						pNode->fFireTime -= fTimeDelta;
						if(pNode->fFireTime < 0.f)
						{
							pNode->fFireTime = FIRE_TIME;
							OnFire(pNode->szName, ENEMYBULLET);
						}

					}
					pNode = pNode->pNext;
}

			}
			else // 一局游戏结束
			{
				g_bStart = false;
			}
		}else{
			dSetSpriteVisible("gameover", true);
			dSetSpritePosition("gameover",0,0);
			dSetSpriteLinearVelocity(g_pMyTank->szName, 0.f, 0.f);
			dSetSpritePosition(g_pMyTank->szName, fPosX0, fPosY0);
			dSetSpriteRotation(g_pMyTank->szName, 0);
			dSetSpriteLinearVelocity(g_pMyTank1->szName, 0.f, 0.f);
			dSetSpritePosition(g_pMyTank1->szName, fPosX1, fPosY1);
			dSetSpriteRotation(g_pMyTank1->szName, 0);
			dSetTextValue("time", 0);

			// 删除地图
			char szName[128];
			for(int i=0; i<g_iWallCount; i++)
			{
				sprintf(szName, "wall%d", i);
				dDeleteSprite(szName);
			}
			g_iWallCount = 0;
			bLoad = true;
			// 删除多余精灵
			DeleteList();

			// 设置指挥部不可见
			dSetSpriteVisible("goal", false);
		}


		// ----------------------------------------

	};

	// 关闭游戏引擎
	dShutdownGameEngine();
	return 0;
}



//==========================================================================
//
// 引擎捕捉鼠标移动消息后，将调用到本函数
// fMouseX：鼠标所在位置的X坐标
// fMouseY：鼠标所在位置的Y坐标
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// 在此添加鼠标滑动的响应代码

}
//==========================================================================
//
// 引擎捕捉鼠标点击消息后，将调用到本函数
// iMouseType：0 - 鼠标左键；1 - 鼠标中键； 2 - 鼠标右键
// fMouseX：鼠标所在位置的X坐标
// fMouseY：鼠标所在位置的Y坐标
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 在此添加鼠标点击的响应代码

}

void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 在此添加鼠标抬起的响应代码

}

//==========================================================================
//
// 引擎捕捉键盘按下消息后，将调用到本函数
// iKey：按下的某个键盘
// iAltPress iShiftPress iCtrlPress 分别为判断Shift，Alt，Ctrl当前是否也处于按下状态。比如可以判断Ctrl+E组合键
void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{
	// 在此添加键盘按下的响应代码
	if(g_bStart)  // 一局游戏进行中
	{
		if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
		{

			MoveMyTank(iKey, true);
		}

		// 按下J键，我方坦克发射子弹
		if(iKey == KEY_J)
			OnFire(g_pMyTank->szName, MYBULLET);
		if(iKey == KEY_UP|| iKey == KEY_LEFT || iKey == KEY_DOWN || iKey == KEY_RIGHT )
		{

			MoveMyTank(iKey, true);
		}
		// 按下J键，我方坦克发射子弹

		if(iKey == KEY_ENTER)
			OnFire(g_pMyTank1->szName, MYBULLET);

	}
	if(iKey == KEY_SPACE && g_bStart == false) // 游戏未开始，按下空格键
	{
		g_bStart = true;
		g_fGameTime = GAME_TIME;
		dSetSpriteVisible("kaishi", false);
		dSetSpriteVisible("splash", false);
		dSetTextValue("time", (int)g_fGameTime);
		// 将我方坦克放在第11行第5列
		dSetSpritePosition(g_pMyTank->szName, -24 + 4*4, -20 + 4*10);
        dSetSpritePosition(g_pMyTank1->szName, -24 + 4*8, -20 + 4*10);
		g_fCreateTime = CREATE_TANK_TIME;

		g_iScore = 0;
		dSetTextValue("score", g_iScore);
	}
}
//==========================================================================
//
// 引擎捕捉键盘弹起消息后，将调用到本函数
// iKey：弹起的某个键盘
void dOnKeyUp( const int iKey )
{
	// 在此添加游键盘弹起的响应代码
	if(g_bStart)
	{
		if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
		{
			MoveMyTank(iKey, false);
		}
        if(iKey == KEY_UP|| iKey == KEY_LEFT || iKey == KEY_DOWN || iKey == KEY_RIGHT )
		{

			MoveMyTank(iKey, false);
		}
	}
}

//===========================================================================
//
// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
// szSrcName：精灵与精灵碰撞中的一方
// szTarName：精灵与精灵碰撞中的另一方
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// 在此添加精灵与精灵碰撞的响应代码
	// 发送碰撞一方
	if(strcmp(szSrcName, g_pMyTank->szName) == 0)  // 我方坦克
		OnMyTankColOther(szSrcName, szTarName);
    if(strcmp(szSrcName, g_pMyTank1->szName) == 0)  // 我方坦克
		OnMyTankColOther(szSrcName, szTarName);
	if(strstr(szSrcName, "bullet") != NULL)           // 子弹
		OnBulletColOther(szSrcName, szTarName);

	if(strstr(szSrcName, "tank") != NULL)			  // 敌方坦克
		OnTankColOther(szSrcName, szTarName);


	// 接受碰撞一方
	if(strstr(szTarName, "wall") != NULL)
		OnWallColOther(szTarName, szSrcName);

	if(strstr(szTarName, "bullet") != NULL)           // 子弹
		OnBulletColOther(szTarName, szSrcName);

	if(strstr(szTarName, "tank") != NULL)			  // 敌方坦克
		OnTankColOther(szTarName, szSrcName);

	if(strcmp(szTarName, "goal") == 0)				// 玩家指挥部
		OnGoalColOther();

	Weapon* p0 = FindNode(szSrcName);
	Weapon* p1 = FindNode(szTarName);
	if(p0 != NULL && IsDead(szSrcName))
	{
		dDeleteSprite(szSrcName);
		DeleteNode(szSrcName);
	}
	if(p1 != NULL && IsDead(szTarName))
	{
		dDeleteSprite(szTarName);
		DeleteNode(szTarName);
	}
}

//===========================================================================
//
// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// szName : 与世界边界碰撞的精灵的名称
// iColSide : 0 左边，1 右边，2 上边，3 下边
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	// 在此添加精灵与世界边界碰撞的响应代码
	if(strcmp(szName, g_pMyTank->szName)==0)  // 玩家坦克
	{
		dSetSpriteLinearVelocity(g_pMyTank->szName, 0.f, 0.f);
	}
    if(strcmp(szName, g_pMyTank1->szName)==0)  // 玩家坦克
	{
		dSetSpriteLinearVelocity(g_pMyTank1->szName, 0.f, 0.f);
	}
	if(strstr(szName, "bullet") != NULL)   // 子弹
	{
		// 删除精灵
		dDeleteSprite(szName);
		// 删除对应链表节点
		DeleteNode(szName);
	}
	if(strstr(szName, "tank") != NULL)   // 敌方坦克
	{
		Weapon* enemyTank = FindNode(szName);
		float fSpeedX = 0.f, fSpeedY = 0.f;
		int	  iDir = 0;

		// 调头
		switch(iColSide)
		{
		case 0:  // 碰到左边，调头朝右
			fSpeedX = TANK_SPEED;
			iDir = RIGHT;
			break;
		case 1:  // 碰到右边，调头朝左
			fSpeedX = -TANK_SPEED;
			iDir =	LEFT;
			break;
		case 2:  // 碰到上边，调头朝下
			fSpeedY = TANK_SPEED;
			iDir = DOWN;
			break;
		case 3:  // 碰到下边，调头朝上
			fSpeedY = -TANK_SPEED;
			iDir = UP;
			break;
		}
		dSetSpriteLinearVelocity(szName, fSpeedX, fSpeedY);
		dSetSpriteRotation(szName, iDir*90);

	}
}

// 我方坦克运动
void  MoveMyTank(int iKey, bool bPress)
{
	if(bPress)  // 按下键盘
	{
		switch (iKey)
		{
		case KEY_W:
			g_pMyTank->iDir = UP;
			g_pMyTank->fSpeedX = 0.f;
			g_pMyTank->fSpeedY = -TANK_SPEED;
			break;
		case KEY_D:
			g_pMyTank->iDir = RIGHT;
			g_pMyTank->fSpeedX = TANK_SPEED;
			g_pMyTank->fSpeedY = 0.f;
			break;
		case KEY_S:
			g_pMyTank->iDir = DOWN;
			g_pMyTank->fSpeedX = 0.f;
			g_pMyTank->fSpeedY = TANK_SPEED;
			break;
		case KEY_A:
			g_pMyTank->iDir = LEFT;
			g_pMyTank->fSpeedX = -TANK_SPEED;
			g_pMyTank->fSpeedY = 0.f;
			break;
		}
        switch (iKey)
		{
		case KEY_UP:
			g_pMyTank1->iDir = UP;
			g_pMyTank1->fSpeedX = 0.f;
			g_pMyTank1->fSpeedY = -TANK_SPEED;
			break;
		case KEY_RIGHT:
			g_pMyTank1->iDir = RIGHT;
			g_pMyTank1->fSpeedX = TANK_SPEED;
			g_pMyTank1->fSpeedY = 0.f;
			break;
		case KEY_DOWN:
			g_pMyTank1->iDir = DOWN;
			g_pMyTank1->fSpeedX = 0.f;
			g_pMyTank1->fSpeedY = TANK_SPEED;
			break;
		case KEY_LEFT:
			g_pMyTank1->iDir = LEFT;
			g_pMyTank1->fSpeedX = -TANK_SPEED;
			g_pMyTank1->fSpeedY = 0.f;
			break;
		}
	}
	else    // 松开键盘
	{
		switch (iKey)
		{
		case KEY_W:
			g_pMyTank->fSpeedY = 0.f;
			break;
		case KEY_D:
			g_pMyTank->fSpeedX = 0.f;
			break;
		case KEY_S:
			g_pMyTank->fSpeedY = 0.f;
			break;
		case KEY_A:
			g_pMyTank->fSpeedX = 0.f;
			break;
		}
        switch (iKey)
		{
		case KEY_UP:
			g_pMyTank1->fSpeedY = 0.f;
			break;
		case KEY_RIGHT:
			g_pMyTank1->fSpeedX = 0.f;
			break;
		case KEY_DOWN:
			g_pMyTank1->fSpeedY = 0.f;
			break;
		case KEY_LEFT:
			g_pMyTank1->fSpeedX = 0.f;
			break;
		}
	}

	dSetSpriteRotation(g_pMyTank->szName, g_pMyTank->iDir * 90);
	dSetSpriteLinearVelocity(g_pMyTank->szName, g_pMyTank->fSpeedX, g_pMyTank->fSpeedY);
    dSetSpriteRotation(g_pMyTank1->szName, g_pMyTank1->iDir * 90);
	dSetSpriteLinearVelocity(g_pMyTank1->szName, g_pMyTank1->fSpeedX, g_pMyTank1->fSpeedY);
}

// 敌方坦克运动

void	MoveEnemyTank(const char* szName)

{
	int iDir = dGetSpriteRotation(szName) / 90;
	float	fSpeedX = 0, fSpeedY = 0;

	switch(iDir)
	{
	case 0:
		fSpeedX = TANK_SPEED;
		fSpeedY = 0;
		break;
	case 1:
		fSpeedX = 0;
		fSpeedY = TANK_SPEED;
		break;
	case 2:
		fSpeedX = -TANK_SPEED;
		fSpeedY = 0;
		break;
	case 3:
		fSpeedX = 0;
		fSpeedY = -TANK_SPEED;
	}

	iDir += 1;
	dSetSpriteLinearVelocity(szName, fSpeedX, fSpeedY);
	dSetSpriteRotation(szName, iDir*90);

}

// 载入地图
void  LoadMap()
{
	// 地图数据 一个11行13列的地图，其中1代表墙
	int iMap[11][13]=
	{
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{3,0,0,3,3,0,0,3,3,0,3,3,3},
		{2,2,2,1,2,2,2,2,2,1,2,2,2},
		{2,2,4,4,4,2,4,2,4,4,4,2,2},
		{4,1,4,1,1,3,3,3,1,1,4,0,0},
		{4,1,4,4,1,1,3,1,4,1,4,1,1},
		{1,1,1,1,0,0,3,1,1,1,1,4,1},
		{1,1,1,1,0,0,3,2,0,0,1,1,1},
		{3,3,1,2,2,2,3,2,2,2,1,3,3},
		{1,1,1,0,0,1,1,1,0,0,1,1,1},
		{2,0,0,0,0,1,0,1,0,0,0,2,0},
	};

	// 赋值给g_iMap
	int i,j;
	for(i=0;i<11;i++)
	{
		for(j=0;j<13;j++)
		{
			g_iMap[i][j] = iMap[i][j];
		}
	}

	// 加载地图
	// iMap[0][0] 位于地图上的-24，-20的位置

	char szName[128];

	float x,y;
	for(i=0;i<11;i++)
	{
		for(j=0;j<13;j++)
		{
			if(g_iMap[i][j]==1)
			{
				sprintf(szName, "wall%d", g_iWallCount++);
				dCloneSprite("wall",szName);
				// 将wall设为接受碰撞
				dSetSpriteCollisionReceive(szName, true);

				x = -24+4*j;
				y = -20+4*i;
				dSetSpritePosition(szName,x,y);
			}
            if(g_iMap[i][j]==2)
			{
				sprintf(szName, "grass%d", g_iWallCount++);
				dCloneSprite("grass",szName);
				// 将wall设为接受碰撞
				dSetSpriteCollisionReceive(szName, false);

				x = -24+4*j;
				y = -20+4*i;
				dSetSpritePosition(szName,x,y);
			}
            if(g_iMap[i][j]==3)
			{
				sprintf(szName, "sea%d", g_iWallCount++);
				dCloneSprite("sea",szName);
				// 将wall设为接受碰撞
				dSetSpriteCollisionReceive(szName, true);

				x = -24+4*j;
				y = -20+4*i;
				dSetSpritePosition(szName,x,y);
			}
           else if(g_iMap[i][j]==4)
			{
				sprintf(szName, "stock%d", g_iWallCount++);
				dCloneSprite("stock",szName);
				// 将wall设为接受碰撞
				dSetSpriteCollisionReceive(szName, true);

				x = -24+4*j;
				y = -20+4*i;
				dSetSpritePosition(szName,x,y);
			}
		}
	}

	// 指挥部设置在第11行第7列
	dSetSpritePosition("goal", -0.f, 20.f);
	dSetSpriteVisible("goal", true);
	dSetSpriteCollisionReceive("goal", true);
}

// 我方坦克与其他精灵碰撞
void	OnMyTankColOther(const char* szTankName, const char* szOtherName)
{
    if(strstr(szOtherName, "wall") != NULL)
	     dSetSpriteLinearVelocity(g_pMyTank->szName, 0, 0);
    if(strstr(szOtherName, "stock") != NULL)
	     dSetSpriteLinearVelocity(g_pMyTank->szName, 0, 0);
    if(strstr(szOtherName, "sea") != NULL)
		 dSetSpriteLinearVelocity(g_pMyTank->szName, 0, 0);

	if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(g_pMyTank1->szName, 0, 0);
    if(strstr(szOtherName, "stock") != NULL)
		dSetSpriteLinearVelocity(g_pMyTank1->szName, 0, 0);
    if(strstr(szOtherName, "sea") != NULL)
		dSetSpriteLinearVelocity(g_pMyTank1->szName, 0, 0);
	if(strstr(szOtherName, "tank") != NULL)
		g_bStart = false;
	if(strstr(szOtherName, "bullet") != NULL)
		g_bStart = false;
}

// 子弹与其他精灵碰撞
void	OnBulletColOther(const char* szBulletName, const char* szOtherName)
{
	Weapon * pBullet = FindNode(szBulletName);  // 根据名称找出对应的子弹节点

	if(pBullet != NULL)
	{
		pBullet->iHp = 0;
	}

}

// 墙与其他精灵碰撞
void	OnWallColOther(const char* szWallName, const char* szOtherName)
{
	// 如果碰上的是子弹，墙被删除
	if(strstr(szOtherName, "bullet") != NULL)
		dDeleteSprite(szWallName);

}

// 敌方坦克与其他精灵碰撞
void	OnTankColOther(const char* szTankName, const char* szOtherName)
{
	// 如果碰上的是墙，敌方坦克停下来
	if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(szTankName, 0.f, 0.f);
    if(strstr(szOtherName, "stock") != NULL)
		dSetSpriteLinearVelocity(szTankName, 0.f, 0.f);
    if(strstr(szOtherName, "sea") != NULL)
		dSetSpriteLinearVelocity(szTankName, 0.f, 0.f);

	// 如果碰到我方坦克，停下来
	if(strcmp(szOtherName, "player") == 0)
		dSetSpriteLinearVelocity(szTankName, 0.f, 0.f);

	// 如果碰上的是玩家子弹，敌方坦克被摧毁，玩家得一分
	if(strstr(szOtherName, "bullet") != NULL)
	{
		Weapon* bullet = FindNode(szOtherName);
		Weapon* enemyTank = FindNode(szTankName);
		if(bullet != NULL && enemyTank != NULL && bullet->iType == MYBULLET)
		{
			enemyTank->iHp = 0;
			g_iScore++;
		}
	}

	// 如果碰到也是敌方坦克，敌方坦克停下来
	if(strstr(szOtherName, "tank") != NULL)
	{
		float fSpeedX0 = dGetSpriteLinearVelocityX(szTankName);
		float fSpeedY0 = dGetSpriteLinearVelocityY(szTankName);
		float fSpeedX1 = dGetSpriteLinearVelocityX(szOtherName);
		float fSpeedY1 = dGetSpriteLinearVelocityY(szOtherName);

		// 如果该坦克原来有速度，调头前行

		if(fSpeedX0 > 0.0001)
		{
			dSetSpriteLinearVelocity(szTankName, -TANK_SPEED, 0);
			dSetSpriteRotation(szTankName, 270);
		}
		else
		{
			dSetSpriteLinearVelocity(szTankName, TANK_SPEED, 0);
			dSetSpriteRotation(szTankName, 90);
		}


		if(fSpeedY0 > 0.0001)
		{
			dSetSpriteLinearVelocity(szTankName, 0, -TANK_SPEED);
			dSetSpriteRotation(szTankName, 0);
		}
		else
		{
			dSetSpriteLinearVelocity(szTankName, 0, TANK_SPEED);
			dSetSpriteRotation(szTankName, 180);
		}

	}
}


// 玩家指挥所与其他精灵碰撞
void	OnGoalColOther()
{
	g_bStart = false;
}


// 判断某个精灵是否死亡

bool IsDead(const char* szName)
{
	Weapon* pSprite = FindNode(szName);

	if(pSprite != NULL && pSprite->iHp == 0)
		return true;

	return false;
}

// 坦克开炮
// szName - 坦克的名称
// iType - 子弹类型

void OnFire(char* szName, int iType)
{
	char szBullet[128];
	float fPosX, fPosY;
	int iDir;
	float fSpeedX,fSpeedY;

	//获得坦克的朝向
	iDir = dGetSpriteRotation(szName)/90;
	sprintf(szBullet, "bullet%d",g_iNum++);
	dCloneSprite("bullet",szBullet);
	dSetSpriteWorldLimit(szBullet,WORLD_LIMIT_NULL,WORLD_LEFT, WORLD_TOP, WORLD_RIGHT, WORLD_BOTTOM);  // 设置子弹世界边界
	// 设置碰撞模式
	dSetSpriteCollisionSend(szBullet,true);
	dSetSpriteCollisionReceive(szBullet,true);

	// 获取坦克坐标值
	fPosX = dGetSpritePositionX(szName);
	fPosY = dGetSpritePositionY(szName);

	// 根据坦克朝向，将子弹设置在坦克前面1个单位，并将子弹速度设置比坦克快一些
	switch(iDir)
	{
	case 0:
		fPosY -= dGetSpriteHeight(szName)/2 + 1;
		fSpeedX = 0;
		fSpeedY =-BULLET_SPEED;
		break;
	case 1:
		fPosX += dGetSpriteWidth(szName)/2 + 1;
		fSpeedX = BULLET_SPEED;
		fSpeedY = 0;
		break;
	case 2:
		fPosY += dGetSpriteHeight(szName)/2 + 1;
		fSpeedX = 0;
		fSpeedY = BULLET_SPEED;
		break;
	case 3:
		fPosX -= dGetSpriteWidth(szName)/2 + 1;
		fSpeedX = -BULLET_SPEED;
		fSpeedY = 0;
		break;
	}
	dSetSpritePosition(szBullet, fPosX, fPosY);
	dSetSpriteRotation(szBullet, iDir*90);
	dSetSpriteLinearVelocity(szBullet, fSpeedX, fSpeedY);

	// 创建子弹的指针变量，并加入到链表中
	Weapon * pBullet = (Weapon*)malloc(sizeof(Weapon));
	pBullet->iDir = iDir;   // 子弹朝向跟坦克朝向一致
	sprintf(pBullet->szName, szBullet);
	pBullet->fPosX = fPosX;
	pBullet->fPosY = fPosY;
	pBullet->fSpeedX = fSpeedX;
	pBullet->fSpeedY = fSpeedY;
	pBullet->iType = iType;
	pBullet->iHp  = 1;

	AddToList(pBullet);

}

void CreateEnemyTanks()
{
	// 每次出现1辆坦克，都出现在第一行，列数随机，但前面不能有障碍
	float	fPosX, fPosY;
	float	fSpeedX, fSpeedY;
	char	szName[128];
	int		iDir;

	Weapon* tank = (Weapon*)malloc(sizeof(Weapon));

	fPosY = -20;  // 统一出现在第一行
	iDir = 2;	  // 朝向统一向下

	// 如果前方有障碍，就重新找位置
	int j;
	do{
		j = dRandomRange(0, 10);
	}while(g_iMap[1][j] == 1);


	fPosX = -24 + j*4;

	sprintf(szName, "tank%d", g_iNum++);
	dCloneSprite("enemy", szName);
	dSetSpritePosition(szName, fPosX, fPosY);
	dSetSpriteRotation(szName, iDir*90);
	dSetSpriteLinearVelocityY(szName, TANK_SPEED);
	// 设置碰撞
	dSetSpriteCollisionSend(szName, true);
	dSetSpriteCollisionReceive(szName, true);
	// 设置世界边界
	dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, WORLD_LEFT, WORLD_TOP, WORLD_RIGHT, WORLD_BOTTOM);

	// 将指针节点加入到链表中
	strcpy(tank->szName,  szName);
	tank->iType = ENEMYTANK;
	tank->iDir = iDir;
	tank->fRotateTime = ROTATE_TIME;
	tank->iHp = 1;
	tank->fFireTime = FIRE_TIME;

	AddToList(tank);

}
void CreateEnemy2Tanks()
{
	// 每次出现1辆坦克，都出现在第一行，列数随机，但前面不能有障碍
	float	fPosX, fPosY;
	float	fSpeedX, fSpeedY;
	char	szName[128];
	int		iDir;

	Weapon* tank = (Weapon*)malloc(sizeof(Weapon));

	fPosY = -20;  // 统一出现在第一行
	iDir = 2;	  // 朝向统一向下

	// 如果前方有障碍，就重新找位置
	int j;
	do{
		j = dRandomRange(0, 10);
	}while(g_iMap[1][j] == 1);


	fPosX = -24 + j*4;

	sprintf(szName, "tank%d", g_iNum++);
	dCloneSprite("enemy", szName);
	dSetSpritePosition(szName, fPosX, fPosY);
	dSetSpriteRotation(szName, iDir*90);
	dSetSpriteLinearVelocityY(szName, TANK_SPEED);
	// 设置碰撞
	dSetSpriteCollisionSend(szName, true);
	dSetSpriteCollisionReceive(szName, true);
	// 设置世界边界
	dSetSpriteWorldLimit(szName, WORLD_LIMIT_NULL, WORLD_LEFT, WORLD_TOP, WORLD_RIGHT, WORLD_BOTTOM);

	// 将指针节点加入到链表中
	strcpy(tank->szName,  szName);
	tank->iType = ENEMYTANK;
	tank->iDir = iDir;
	tank->fRotateTime = ROTATE_TIME;
	tank->iHp = 1;
	tank->fFireTime = FIRE_TIME;

	AddToList(tank);

}
