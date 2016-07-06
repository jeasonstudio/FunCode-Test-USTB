/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#define	BLOCK_COUNT	4		// N * N 的矩阵方块,一个N的大小
////////////////////////////////////////////////////////////////////////////////
//
//
int  IsGameWin();
int			g_iGameState		=	0;		// 游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游戏；2 -- 游戏进行中
//
void		GameInit();
void		GameRun( float fDeltaTime );
void		GameEnd();

// 	按方块大小，在编辑器里摆放的第一块方块的起始坐标
const float	g_fBlockStartX		=	-40.625f;
const float	g_fBlockStartY		=	-28.125f;
// 	屏幕高度75 / 4块 = 18.75每块的大小.编辑器里预先摆放好的方块宽和高  //	必须与此值一致
const float		g_fBlockSize		=	18.75f;

int	iLoopX		= 0, iLoopY		= 0, iLoop = 0;
int	iOneIndex	= 0, iRandIndex = 0;

int	iDataCount	=	BLOCK_COUNT * BLOCK_COUNT - 1;
int	iRandData[BLOCK_COUNT * BLOCK_COUNT - 1] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

int XYToOneIndex( const int iIndexX, const int iIndexY )
{
	return ((iIndexX * BLOCK_COUNT) + iIndexY);
}
int OneIndexToX( const int iIndex )
{
    return (iIndex / BLOCK_COUNT);
}
// 一维数组索引转换到二维数组索引Y，注意这2个数组大小必须一致
int OneIndexToY( const int iIndex )
{
    return (iIndex % BLOCK_COUNT);
}

// 	游戏状态，0 -- 游戏结束等待开始状态；1 -- 按下空格键开始，初始化游//	戏；2 -- 游戏进行中

//	二维数组，存储N*N的矩阵方块信息
int	g_iBlockState[BLOCK_COUNT][BLOCK_COUNT];
//	一维数组，存储上面二维数组中的方块精灵的名字。TO DO 思考一下数	    //	组大小一样的二维数组索引与一维数组索引如何相互转换？
char	g_szBlockName[BLOCK_COUNT * BLOCK_COUNT][64];


void MoveSpriteToBlock( const char *szName, const int iIndexX, const int iIndexY )
{
    float	fPosX	=	g_fBlockStartX + iIndexX * g_fBlockSize;
    float	fPosY	=	g_fBlockStartY + iIndexY * g_fBlockSize;
    dSetSpritePosition( szName, fPosX, fPosY );
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
    for( iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++ )
        {
            for( iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++ )
            {
                iOneIndex	=	XYToOneIndex( iLoopX, iLoopY );

                // 数组的最后一个
                if( BLOCK_COUNT - 1 == iLoopX && BLOCK_COUNT - 1 == iLoopY )
                {
                    g_iBlockState[iLoopY][iLoopX] = 0;
                    g_szBlockName[iOneIndex][0] = '\0';
                }
                else
                {
                    // 在当前剩余未使用到的数值里随机一个出来,赋值给二维数组
                    iRandIndex	=	dRandomRange( 0, iDataCount - 1 );
                    g_iBlockState[iLoopY][iLoopX] = iRandData[iRandIndex];
                    strcpy( g_szBlockName[iOneIndex], dMakeSpriteName( "PictureBlock", 	g_iBlockState[iLoopY][iLoopX] ) );
                    // 将该精灵移动到对应的位置
                    MoveSpriteToBlock( g_szBlockName[iOneIndex], iLoopX, iLoopY );
                }
                for( iLoop = iRandIndex; iLoop < iDataCount - 1; iLoop++ )
                {
                    iRandData[iLoop] = iRandData[iLoop + 1];
                }
                // 剩余有效值总数减一
                iDataCount--;
            }
        }

        for( iLoop = iRandIndex; iLoop < iDataCount - 1; iLoop++ )
        {
            iRandData[iLoop] = iRandData[iLoop + 1];
        }
        // 剩余有效值总数减一
        iDataCount--;
}
//==============================================================================
//
// 每局游戏进行中
void GameRun( float fDeltaTime )
{
    g_iGameState = IsGameWin();
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
    if( 2 != g_iGameState )
        return;
    int	iClickIndex	=	-1;
    int	iLoop	=	0;
    for( iLoop = 0; iLoop < BLOCK_COUNT * BLOCK_COUNT; iLoop++ )
    {
        if( '\0' == g_szBlockName[iLoop][0] )
            continue;
        // 使用API dIsPointInSprite 判断指定坐标是否位于某个名字的精灵内部
        if( dIsPointInSprite( g_szBlockName[iLoop], fMouseX, fMouseY ) )
        {
            iClickIndex	=	iLoop;
            break;
        }
    }
    // 判断鼠标是否点中方块
    if( -1 == iClickIndex )
        return;
    // 将该一维数组的Index转换成二维数组的X,Y
    int	iIndexX	=	OneIndexToX( iClickIndex );
    int	iIndexY	=	OneIndexToY( iClickIndex );
    // TODO 在二维数组里查找鼠标点击的方块上下左右4个方向上是否有空位：
    // 注意边界判断，否则数组访问会越界。比如判断左边时，需要判断是否已经是	//最左边的索引(iIndexX == 0)
    // 如果有空位(值为0)，则将该空位的索引赋值给下面这2个变量
    int	iEmptyIndexX = -1, iEmptyIndexY = -1;

    // X 左方向(4个方向均需要判断是否是位于边缘，iIndexX > 0 即起此作用)
    if( iIndexX > 0 )
    {
        if( 0 == g_iBlockState[iIndexY][iIndexX - 1] )
        {
            iEmptyIndexX = iIndexX - 1;
            iEmptyIndexY = iIndexY;
        }
    }
    // X 右方向
    if( -1 == iEmptyIndexX && iIndexX < BLOCK_COUNT - 1 )
    {
        if( 0 == g_iBlockState[iIndexY][iIndexX + 1] )
        {
        iEmptyIndexX = iIndexX + 1;
        iEmptyIndexY = iIndexY;
        }
    }
    // Y 上方向
    if( -1 == iEmptyIndexY && iIndexY > 0 )
    {
        if( 0 == g_iBlockState[iIndexY - 1][iIndexX] )
        {
            iEmptyIndexX = iIndexX;
            iEmptyIndexY = iIndexY - 1;
        }
    }
    // Y 下方向
    if( -1 == iEmptyIndexY && iIndexY < BLOCK_COUNT - 1 )
    {
        if( 0 == g_iBlockState[iIndexY + 1][iIndexX] )
        {
            iEmptyIndexX = iIndexX;
            iEmptyIndexY = iIndexY + 1;
        }
    }
    // 判断是否找到空位
    if( -1 == iEmptyIndexX || -1 == iEmptyIndexY )
    return;
    // 有空位，在二维数组里，将该索引对应的值进行交换
    g_iBlockState[iEmptyIndexY][iEmptyIndexX] = g_iBlockState[iIndexY][iIndexX];
    g_iBlockState[iIndexY][iIndexX]	=	0;
    // 对应的名字也进行交换
    int	iOneIndex	=	XYToOneIndex( iEmptyIndexX, iEmptyIndexY );
    strcpy( g_szBlockName[iOneIndex], g_szBlockName[iClickIndex] );
    g_szBlockName[iClickIndex][0]	=	'\0';
    // 将该精灵移动到对应的位置
    MoveSpriteToBlock( g_szBlockName[iOneIndex], iEmptyIndexX, iEmptyIndexY );

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
		g_iGameState = 1;
        dSetSpriteVisible("GameBegin" , false);
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

}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void OnSpriteColWorldLimit( const char *szName, const int iColSide )
{

}

int  IsGameWin()
{
    int	iLoopX	= 0, iLoopY = 0;
	int	iResult	= 1;
	for( iLoopY = 0; iLoopY < BLOCK_COUNT; iLoopY++ )
	{
		for( iLoopX = 0; iLoopX < BLOCK_COUNT; iLoopX++ )
		{
			// 数组的最后一个
			if( BLOCK_COUNT - 1 == iLoopX && BLOCK_COUNT - 1 == iLoopY )
				break;
			// 其中一个值不等于，那么就没有胜利
			if( g_iBlockState[iLoopY][iLoopX] != iResult )
				return 2;
			iResult++;
		}
	}
	return 0;
}
