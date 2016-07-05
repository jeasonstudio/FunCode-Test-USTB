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
int	g_iGameState = 0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void GameInit();
void GameRun( float fDeltaTime );
void GameEnd();

const char	*szGoldName	=	NULL;

char g_szCurGetGold[64];	// 当前抓取到的金子名字
float g_fEmptyHookSpeed; // 空的钩子的移动速度

int finalScore = 0;

// 抓取金子状态：0 -- 等待按键开始抓金子；1 -- 钩子伸出去；2 -- 钩子往回// 伸，未抓到东西；3 -- 钩子往回伸，带着金子
int g_iGetGoldState	;
// 钩子当前转动的朝向
float g_fHookRotation;
// 钩子当前是往左摆动还是往右摆动( 1 右-左， 0 左-右 )
int g_iHookRotToLeft;
// 钩子初始位置。在刚开始的时候初始化一次，用于钩子复位
float g_fHookStartPosX;
float g_fHookStartPosY;

// 本局生成的金子数量，每抓取一个金子，数量递减一个，为0时本局结束
int	g_iGoldCount;
	//以下四个变量为金子能出现在地图上的上下左右边界最大值和最小值
int	g_iGoldBornMinX	;
int	g_iGoldBornMaxX	;
int	g_iGoldBornMinY	;
int	g_iGoldBornMaxY	;


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
    g_iGoldBornMinX	=	0;
    g_iGoldBornMaxX	=	0;
    g_iGoldBornMinY	=	0;
    g_iGoldBornMaxY	=	0;
    g_iGoldCount = 20;

    g_fEmptyHookSpeed = 15.f;

    // 以下变量只需要初始化一次
	static int	iInitedHookPos	=	0;
	if( 0 == iInitedHookPos )
	{
	    //钩子可以发出碰撞
        dSetSpriteCollisionSend("GoldHook",true);
        //金子可以接收碰撞
        dSetSpriteCollisionReceive("GoldTemplate",true);

		iInitedHookPos		=	1;
		// 钩子初始位置值初始化
		g_fHookStartPosX	=	dGetSpritePositionX( "GoldHook" );
		g_fHookStartPosY	=	dGetSpritePositionY( "GoldHook" );

		// 金子可以出现的边界范围初始化
		g_iGoldBornMinX		=	dGetScreenLeft() + 5;
		g_iGoldBornMaxX		=	dGetScreenRight() - 5;
		g_iGoldBornMinY		=	dGetScreenTop() + 20;
		g_iGoldBornMaxY		=	dGetScreenBottom() - 5;
	}

	int	iLoop	=	0;
    char	*szName	=	NULL;
    int	iSize =	4, iPosX = 0, iPosY = 0;
    for( iLoop = 0; iLoop < g_iGoldCount; iLoop++ )
    {
        if( iLoop < 10 )
        {
            iSize	=	4;

        }
        else if( iLoop >= 10 && iLoop < 16 )
        {
            iSize	=	6;

        }
        else
        {
            iSize	=	8;

        }
        szName	=	dMakeSpriteName( "GoldBlock", iLoop );

        dCloneSprite( "GoldTemplate", szName );
        dSetSpriteWidth( szName, (float)iSize );
        dSetSpriteHeight( szName, (float)iSize );
        // 随机一个位置
        iPosX	=	dRandomRange( g_iGoldBornMinX, g_iGoldBornMaxX );
        iPosY	=	dRandomRange( g_iGoldBornMinY, g_iGoldBornMaxY );
        dSetSpriteMass(szName,5*iSize);
        dSetSpritePosition( szName, (float)iPosX, (float)iPosY );
    }


    // 播放挖金者的动作(恢复初始守候动作)
    dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation2", 0 );
    // 隐藏 游戏开始的提示
    dSetSpriteVisible( "GameBegin", 0 );
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
    if( 0 == g_iGetGoldState )
	{
    // 摇摆速度，单位 度/秒
		const  float	fRotateSpeed	=	45.f;
    // 本次旋转的度数
		float fThisRotate = fRotateSpeed * fDeltaTime;
		if( g_iHookRotToLeft )
		{
			g_fHookRotation	+= fThisRotate;
			if( g_fHookRotation >= 180.f )
			{
				g_fHookRotation		=	180.f;
				g_iHookRotToLeft	=	0;
			}
		}
		else
		{
			g_fHookRotation	-= fThisRotate;
			if( g_fHookRotation <= 0.f )
			{
				g_fHookRotation		=	0.f;
				g_iHookRotToLeft	=	1;
			}
		}
		dSetSpriteRotation( "GoldHook", g_fHookRotation );
	}

	// 如果当前为钩子往回归位，则判断是否已经运动到终点。到了终点，则又开始摇// 摆、等待
    else if( 2 == g_iGetGoldState || 3 == g_iGetGoldState )
    {
		// 判断是否移动到终点(判断的依据是XY方向的移动速度为0，即金子是否		// 已经停止移动)
		float	fSpeedX	=	dGetSpriteLinearVelocityX( "GoldHook" );
		float	fSpeedY	=	dGetSpriteLinearVelocityY( "GoldHook" );
		// 当前速度不为0，还在运动中
		// 浮点数是否为0，不能直接判断 == != 0
		if( fSpeedX > 0.00001f || fSpeedX < -0.00001f || fSpeedY > 0.00001f || fSpeedY < -0.00001f )
        {

            return;
            // 速度为0，钩子回到初始点，开始下一轮的抓取
            // 当前抓取到金子，将抓取到的金子释放并删除之。然后将金子数量减一
        }

		if( 3 == g_iGetGoldState )
		{
		    finalScore += dGetSpriteMass(szGoldName);
            dSetTextValue("score",finalScore);
            //在这里添加分数
			g_iGoldCount--;
			dSpriteDismount( g_szCurGetGold );
			dDeleteSprite( g_szCurGetGold );
		}
		// 恢复等待状态与播放等待动作
		g_iGetGoldState	=	0;
		dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation2", 0 );
	}


}
//==============================================================================
//
// 本局游戏结束
void GameEnd()
{
    dSetSpriteVisible( "GameBegin", 1 );
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
    if( KEY_SPACE == iKey && 0 == g_iGameState )
    {
        g_iGameState	=	1;
    }
    // 当前处于游戏进行中，按下向下的方向键，钩子伸出抓取金子
	else if( KEY_DOWN == iKey && 2 == g_iGameState && 0 == g_iGetGoldState )
    {
		// 设置抓取状态为：钩子往外伸
		g_iGetGoldState	=	1;
		// 以当前朝向给钩子一个向前的速度
		dSetSpriteLinearVelocityPolar("GoldHook",g_fEmptyHookSpeed, g_fHookRotation );
		// 播放挖金者的动作(一个胳膊往下压的动作)
		dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation1", 0 );
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
    // 首先判断钩子是否伸出，已经游戏是否真在进行，否则推出函数：
    if( 2 != g_iGameState || 1 != g_iGetGoldState )
        return;
    //判断是否是钩子与金子进行碰撞(即这两个名字字符串里必须一个是金子名，一	//个是钩子名)
    if( stricmp( "GoldHook", szSrcName ) != 0 && stricmp( "GoldHook", szTarName ) != 0 )
        return;;
    // 找到哪个是金子的名字
    //const char	*szGoldName	=	NULL;
    if( strstr( szSrcName, "GoldBlock" ) )
        szGoldName = szSrcName;
    else if( strstr( szTarName, "GoldBlock" ) )
        szGoldName = szTarName;
    else
        return;

    // 将金子挂接到钩子的2号挂接点上
    dSpriteMountToSpriteLinkPoint( szGoldName, "GoldHook", 2 );
    // 设置抓取状态为：钩子往回收，抓取到东西。记录金子名字
    g_iGetGoldState	=	3;
    strcpy( g_szCurGetGold, szGoldName );
    // 根据金子大小，计算钩子的往回收的速度，越大的越慢。
    // 算法：之前设置的金子大小为4,6,8，用10减去该大小再除以10，得到3个小// 数：0.6,0.4,0.2.该小数乘以空钩子的速度即得到挂接不同金子后的实际速度
    float	fWidth	=	dGetSpriteWidth( szGoldName );
    float	fSpeed	=	((10.f - fWidth) / 10.f) * g_fEmptyHookSpeed;
    // 钩子往初始位置移动
    dSpriteMoveTo( "GoldHook", g_fHookStartPosX, g_fHookStartPosY, fSpeed, 1 );
    // 播放挖金者的动作(胳膊来回动的动作)
    dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation3", 0 );

}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
    // 判断是否是钩子碰到边界，往回收
    if( stricmp( szName, "GoldHook" ) == 0 )
    {
        // 设置抓取状态为：钩子往回收，未抓取到东西
        g_iGetGoldState	=	2;
        // 钩子往初始位置移动
        dSpriteMoveTo( "GoldHook", g_fHookStartPosX, g_fHookStartPosY, 				g_fEmptyHookSpeed, 1 );
        // 播放挖金者的动作(胳膊来回动的动作)
        dAnimateSpritePlayAnimation( "GoldMan", "GolderManAnimation3", 0 );
     }
}
