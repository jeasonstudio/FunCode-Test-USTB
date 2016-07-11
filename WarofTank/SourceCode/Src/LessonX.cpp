/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"

////////////////////////////////////////////////////////////////////////////////
//
//

/*
const  float	GAME_TIME		 =	30.f;    // 一局游戏时间
const  float 	CREATE_TANK_TIME = 	5.f;		// 每批次生成坦克的时间间隔
const  float	TANK_SPEED		 = 	5.f;     	// 坦克速度
const  float 	BULLET_SPEED	 = 	8.f;		// 子弹速度
const  float	FIRE_TIME		 = 	2.f;		// 坦克开炮时间间隔
*/
const  float	WORLD_LEFT	     = 	-26.f;	// 游戏场景边界左值
const  float	WORLD_TOP	     = 	-22.f;	// 游戏场景边界shang值
const  float	WORLD_RIGHT	 = 	26.f;		// 游戏场景边界you值
const  float	WORLD_BOTTOM  = 	22.f;		// 游戏场景边界xia值

enum  Direction{
	UP		=	0,		// 上方
    RIGHT	=	1,		// 右方
	DOWN	=	2,		// 下方
	LEFT	    =	3		// 左方
};

enum  Role
{
	MYTANK		=	0,		// 我方坦克
	ENEMYTANK	=	1,		// 敌方坦克
	MYBULLET	=	2,		// 我方子弹
	ENEMYBULLET	  =	3		// 敌方子弹
};

int	g_iNum	=	0;		// 记录全部精灵的数目，用来给精灵命名

Weapon* g_pMyTank	=	NULL;	// 我方坦克

Weapon* g_pHead	=	NULL;
/*
bool g_bStart;			// 标识一局游戏开始还是结束
int	g_iScore;			// 一局游戏得分
float g_fGameTime;	    // 一局游戏的剩余时间
float g_fCreateTank;	// 距离生成下一批敌方坦克的剩余时间
// 游戏地图，0表示此处为空，1表示此处有墙。根据游戏空间大小、墙以及坦克大小，
// 我们把地图分成11行，13列，每格大小刚好放一块墙。
*/

int	iMap[11][13];

int	g_iWallCount	=	0;		// 记录墙的数量
int	g_iMap[11][13];			// 地图数组

void LoadMap()
{
    int i,j,x,y;
    char szName[128];

    int g_iMap[11][13]=
    {
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,1,0,1,0,1,1,1,0,1,0,1,0},
		{0,1,0,1,0,1,0,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,1,1,1,0,0,0,0,0},
		{0,1,0,1,0,0,0,0,0,1,0,1,0},
		{0,1,0,1,0,0,0,0,0,1,0,1,0},
		{0,1,0,1,0,1,1,1,0,1,0,1,0},
		{0,0,0,0,0,1,0,1,0,0,0,0,0},
	};
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
		}
	}
	// 指挥部设置在第11行第7列
	dSetSpriteVisible("goal", true);
	dSetSpritePosition("goal", 0.f, 20);

	dSetSpriteVisible("splash", false);

}

void  MoveMyTank(int iKey, bool bPress)
{
    if(bPress)
    {
        if(iKey == KEY_W)
        {
            dSetSpriteLinearVelocity( "player" , 0 , -TANK_SPEED );
            dSetSpriteRotation( "player" , 0 );
        }
        else if(iKey == KEY_S)
        {
            dSetSpriteLinearVelocity( "player" , 0 , TANK_SPEED );
            dSetSpriteRotation( "player" , 180 );
        }
        else if(iKey == KEY_A)
        {
            dSetSpriteLinearVelocity( "player" , -TANK_SPEED , 0 );
            dSetSpriteRotation( "player" , -90 );
        }
        else if(iKey == KEY_D)
        {
            dSetSpriteLinearVelocity( "player" , TANK_SPEED , 0 );
            dSetSpriteRotation( "player" , 90 );
        }
    }
    else{
        dSetSpriteLinearVelocity( "player" , 0 , 0);
    }

}


int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

bool bLoad = true;

void	OnMyTankColOther(const char* szMyTankName, const char* szOtherName)
{
	// 如果碰到墙上
	if(strstr(szOtherName, "wall") != NULL)
		dSetSpriteLinearVelocity(szMyTankName, 0.f, 0.f);
}

void OnFire(char* szName, int iType)
{
    float du = dGetSpriteRotation(szName);
    float tankX = dGetSpritePositionX(szName);
    float tankY = dGetSpritePositionY(szName);
    dSetSpritePosition( "bullet" , tankX , tankY );
    dSetSpriteVisible("bullet", true);
    dSetSpriteRotation("bullet" , du);
    if(du == 0)
        {
            dSetSpriteLinearVelocity( "bullet" , 0 , -BULLET_SPEED );
        }
    if(du == 90)
        {
            dSetSpriteLinearVelocity( "bullet" , BULLET_SPEED , 0 );
        }
    if(du == 180)
        {
            dSetSpriteLinearVelocity( "bullet" , 0 , BULLET_SPEED );
        }
    if(du == -90)
        {
            dSetSpriteLinearVelocity( "bullet" , -BULLET_SPEED ,0 );
        }

}


//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。


//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态.
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void GameMainLoop( float	fDeltaTime )
{

    if(g_bStart)    //TODO
    {
        if(bLoad)  // 一局游戏只需要载入一次地图即可
        {
            // 载入地图
            LoadMap();
            bLoad = false;
        }

        g_fGameTime -= fDeltaTime;
        if(g_fGameTime > 0.f)  // 一局游戏进行中
        {

        }
        else  // 一局游戏结束
        {
            g_bStart = false;
            char szName[128];
            for(int i=0; i<g_iWallCount; i++)
            {
                sprintf(szName, "wall%d", i);
                dDeleteSprite(szName);
            }
            g_iWallCount = 0;
            bLoad = true;
            // 设置指挥所不可见
            dSetSpriteVisible("goal", false);

        }
    }
    else  // 游戏结束后处理
    {

    }

	switch( g_iGameState )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			g_iGameState	=	2; // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else
			{
				// 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
				g_iGameState	=	0;
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
}

//==============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseMove( const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{

}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{
    if(iKey == KEY_SPACE && g_bStart == false) // 游戏未开始，按下空格键
    {
        g_bStart = true;
        g_fGameTime = GAME_TIME;
        dSetSpriteVisible("kaishi", false);
        dSetTextValue("time", (int)g_fGameTime);
        dSetTextValue("score", g_iScore);
        dSetSpritePosition("player", -8 , 20);
    }
    // 按下键盘
    if(g_bStart)  // 一局游戏进行中
    {
        if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
		{
			MoveMyTank(iKey, true);
		}
    }
    if(iKey == KEY_J && g_bStart == true)
    {
        OnFire( "player" , 2 );
    }

}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{
    // 松开键盘
    if(g_bStart)
    {
        if(iKey == KEY_W || iKey == KEY_A || iKey == KEY_S || iKey == KEY_D )
        {
            MoveMyTank(iKey, false);
        }
    }


}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    OnMyTankColOther( szSrcName , szTarName );
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    if(strcmp(szName, g_pMyTank->szName)==0)  // 玩家坦克
    {
        dSetSpriteLinearVelocity(g_pMyTank->szName, 0.f, 0.f);
    }
}
