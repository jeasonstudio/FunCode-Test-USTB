//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#include "CommonAPI.h"
#include "LessonX.h"
#define dtime 0.01
///////////////////////////////////////////////////////////////////////////////////////////
//
// 主函数入口
//
//////////////////////////////////////////////////////////////////////////////////////////
float g_fSpeedLeft=0.f;  	// 左方向速度
float g_fSpeedRight=0.f;  	// 右
float g_fSpeedTop=0.f;  	// 上
float g_fSpeedBottom=0.f;  	// 下

float g_fScreenLeft	= 0.f;    // 屏幕左边界值
float g_fScreenRight = 0.f;    // 右
float g_fScreenTop = 0.f;    // 上
float g_fScreenBottom =	0.f;    // 下

void Swimming()
{
	if((g_fSpeedLeft + g_fSpeedRight) > 0)
		dSetSpriteFlipX("myfish", false);
	else if((g_fSpeedLeft + g_fSpeedRight) < 0)
		dSetSpriteFlipX("myfish", true);

	dSetSpriteLinearVelocity("myfish", g_fSpeedLeft + g_fSpeedRight, g_fSpeedTop + g_fSpeedBottom);
}


int PASCAL WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
	// 初始化游戏引擎

	if( !dInitGameEngine( hInstance, lpCmdLine ) )
		return 0;

	// To do : 在此使用API更改窗口标题
	dSetWindowTitle("seaGameByJeason");

	// 引擎主循环，处理屏幕图像刷新等工作
	while( dEngineMainLoop() )
	{

		// 获取两次调用之间的时间差，传递给游戏逻辑处理
		float	fTimeDelta	=	dGetTimeDelta();


		for(int i=0; i<4; i++)
        {
            char  szName[128];
            sprintf(szName, "fish%d", i);  // 注意，szName传入时不能是空指针
        }


		// 执行游戏主循环
		GameMainLoop( fTimeDelta );
	};

	// 关闭游戏引擎
	dShutdownGameEngine();
	return 0;
}

//==========================================================================
//
// 引擎捕捉鼠标移动消息后，将调用到本函数
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseMove( const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseMove(fMouseX, fMouseY );
}
//==========================================================================
//
// 引擎捕捉鼠标点击消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseClick(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// 引擎捕捉鼠标弹起消息后，将调用到本函数
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
//
void dOnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	// 可以在此添加游戏需要的响应函数
	OnMouseUp(iMouseType, fMouseX, fMouseY);

}
//==========================================================================
//
// 引擎捕捉键盘按下消息后，将调用到本函数
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
//

float nextSpeed(float lastSpeed)
{
    float nextSpeed = lastSpeed + ( dtime * 5 );
    return nextSpeed;

}

void dOnKeyDown( const int iKey, const int iAltPress, const int iShiftPress, const int iCtrlPress )
{

    switch(iKey)
    {
    case KEY_W:
        g_fSpeedTop = -25.f;
        break;
    case KEY_A:
        g_fSpeedLeft = -30.f;
        break;
    case KEY_S:
        g_fSpeedBottom = 25.f;
        break;
    case KEY_D:
        g_fSpeedRight = 30.f;
        break;
    }

    Swimming();

	// 可以在此添加游戏需要的响应函数
	OnKeyDown(iKey, iAltPress, iShiftPress, iCtrlPress);
}
//==========================================================================
//
// 引擎捕捉键盘弹起消息后，将调用到本函数
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
//
void dOnKeyUp( const int iKey )
{

    switch(iKey)
    {
        case KEY_W:
            g_fSpeedTop = 0.f;
            break;
        case KEY_A:
            g_fSpeedLeft = 0.f;
            break;
        case KEY_S:
            g_fSpeedBottom = 0.f;
            break;
        case KEY_D:
            g_fSpeedRight = 0.f;
            break;
    }

    Swimming();

	// 可以在此添加游戏需要的响应函数
	OnKeyUp(iKey);
}

//===========================================================================
//
// 引擎捕捉到精灵与精灵碰撞之后，调用此函数
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵发送及接受碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
//
void dOnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
	// 可以在此添加游戏需要的响应函数
	OnSpriteColSprite(szSrcName, szTarName);
}

//===========================================================================
//
// 引擎捕捉到精灵与世界边界碰撞之后，调用此函数.
// 精灵之间要产生碰撞，必须在编辑器或者代码里设置精灵的世界边界限制
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
//
void dOnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	// 可以在此添加游戏需要的响应函数

    g_fScreenLeft = dGetScreenLeft();
    g_fScreenRight = dGetScreenRight();
    g_fScreenTop = dGetScreenTop();
    g_fScreenBottom = dGetScreenBottom();

	for(int i=0; i<5; i++)
    {
        char  szName[128];
        float fPosY;
        bool bFlip;
        float fSpeedX;

        if(strstr(szName, "fish")!=NULL)
        {
            if(iColSide == 1)  // 右边
            {
                bFlip = true;
                fSpeedX = -dRandomRange(10, 20);	// 随机速度
            }
            else if(iColSide == 0)   // 左边
            {
                bFlip = false;
                fSpeedX = dRandomRange(10, 20);     // 随机速度
            }
            dSetSpriteFlipX(szName, bFlip);
            dSetSpriteLinearVelocity(szName, fSpeedX, 0);
            // 每次调头时，位置是随机的
            fPosY = dRandomRange(g_fScreenTop+10.f, g_fScreenBottom-10.f);
            dSetSpritePositionY(szName, fPosY);

        }
        //dSetSpriteWorldLimit(szName, g_fScreenLeft-20.f, g_fScreenTop, g_fScreenRight+20.f,g_fScreenBottom);
    }

	OnSpriteColWorldLimit(szName, iColSide);
}

