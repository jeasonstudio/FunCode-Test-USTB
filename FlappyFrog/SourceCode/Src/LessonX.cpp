/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
////////////////////////////////////////////////////////////////////////////////
//
//
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//

bool        isGameContinue      =   false;

int         distanceByZhuzi     =   40;
int         speedOfZhuzi        =   -20;
int         score               =   0;


void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

void        dOnFrogColWorldLimit( const int iColSide );
void        dOnZhuziColWorldLimit( const int iColSide );

void        dCreateZhuzi();
void        dFrogFloat();
void        dMyPlaySound();
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
			if( isGameContinue )
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

//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void GameInit()
{
    distanceByZhuzi     =   40;
    speedOfZhuzi        =   -20;
    score               =   0;

    isGameContinue = true;

    dSetSpritePosition("frog" , -10 , 0 );

    dSetSpritePosition("GameOver" , 0 , 40 );

    dSetSpriteFlipY("frog" , 0 );

    if(dIsSpriteVisible("Title") == 1 || dIsSpriteVisible("title2") == 1)
    {
        dSetSpriteVisible("Title" , 0 );
        dSetSpriteVisible("title2" , 0 );
    }
    dSetSpriteMass("frog" , 10 );

    dSetSpriteWorldLimitMode("frog", WORLD_LIMIT_CLAMP);

    dCreateZhuzi();

    dSetSpriteVisible("result1" , 0 );

    dSetSpritePosition("score1" , -24 , -16 );
    dSetSpritePosition("score2" , -24 , -16 );



}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
    dSetSpriteConstantForceY("frog",900);   //设置y方向敞亮推理
    dSetSpriteAngularVelocity("frog", 70 );     //旋转速度
    dSetTextValue("score",score);

    dMyPlaySound();

}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
    dSetSpriteConstantForceY("frog", 30);   //设置y方向敞亮推理
    dSetSpriteAngularVelocity("frog", 0 );  //设置精灵角度旋转速度
    dSetSpriteRotation("frog", 0);  //设置精灵的旋转角度
    dSetSpriteFlipY( "frog" , 1);   //Y方向反转
    dSetSpriteLinearVelocityY("frog", 0);     //y方向速度
    dSetSpritePosition("GameOver" , 0 , -13 );
    dSetSpriteVisible("result1" , 1 );      //显示结果

    dSetTextValue("score1",score);
    dSetTextValue("score2",score);

    dSetSpritePosition("score1" , 7 , -1 );
    dSetSpritePosition("score2" , 7 , 3 );



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
    if(iKey == KEY_SPACE && g_iGameState == 0 )
    {
        g_iGameState = 1;

    }
    if(g_iGameState == 2)
    {
        if(iKey == KEY_SPACE)
        {
            dSetSpriteLinearVelocityY("frog", -40);
            dSetSpriteRotation("frog", -20);
            dPlaySound("flap.ogg", 0 , 1 );
        }
    }
}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void OnKeyUp( const int iKey )
{

}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
    if( strcmp(szSrcName, "frog")==0 && strstr(szTarName, "zhuzi") != NULL )
    {
        dPlaySound("hurt5.ogg", 0 , 1 );
        g_iGameState =	0;
        isGameContinue = false;
        GameEnd();
    }

}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    if(strcmp(szName, "frog")==0 )
    {
        dOnFrogColWorldLimit(iColSide);
    }
    if(strstr(szName, "zhuzi") != NULL)
    {
        dOnZhuziColWorldLimit(iColSide);
    }
}

void dOnFrogColWorldLimit( const int iColSide )
{
    if(iColSide == 3)
    {
        g_iGameState =	0;
        isGameContinue = false;
        GameEnd();
    }
}

void dOnZhuziColWorldLimit( const int iColSide )
{
    if(iColSide == 0 && g_iGameState == 2 )
    {
        score++;
        dCreateZhuzi();
    }
}

void dCreateZhuzi()
{

    int heightZhuzi1 = dRandomRange( 17 , 42 );
    int heightZhuzi2 = heightZhuzi1-(35+distanceByZhuzi);

    dSetSpritePosition("zhuzi1", 24 , heightZhuzi1);
    dSetSpritePosition("zhuzi2", 24 , heightZhuzi2);

    dSetSpriteLinearVelocityX("zhuzi1", speedOfZhuzi);
    dSetSpriteLinearVelocityX("zhuzi2", speedOfZhuzi);

    if(distanceByZhuzi >= 30)
    {
        distanceByZhuzi--;
        if(speedOfZhuzi >= -25)
        {
            speedOfZhuzi--;
        }
    }
}

void dFrogFloat()
{
    dSetSpriteConstantForceY("frog", 2 );
    if(dGetSpriteLinearVelocityY("frog") == 5 )
    {
        dSetSpriteConstantForceY("frog", -2 );
    }
}

void dMyPlaySound()
{
    if(score % 3 == 0)
    {
        if(score % 6 == 0)
        {
            dPlaySound("score10.ogg" , 0 , 0.7 );
        }
        else
            dPlaySound("score8.ogg" , 0 , 0.5 );
    }
    else if(score % 5 == 0)
    {
        dPlaySound("hurt8.ogg" , 0 , 0.7 );
    }
}
