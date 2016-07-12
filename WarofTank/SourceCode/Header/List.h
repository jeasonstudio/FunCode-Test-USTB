/////////////////////////////////////////////////////////////////////////////////
//
//
// 链表管理
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LIST_H_
#define _LIST_H_
//
#include <Windows.h>
#include "CommonAPI.h"
/////////////////////////////////////////////////////////////////////////////////
//
//
struct Weapon{
	char	szName[128];		// 精灵名称
	float	fPosX, fPosY;		// 精灵坐标
	float	fSpeedX, fSpeedY;	// X和Y方向上速度
	int		iHp;				// 生命值
	int		iDir;				// 朝向：0 - 上方；1 - 右方；2 - 下方；3 - 左方
	int		iType;				// 类型： 0 - 我方坦克；1 - 敌方坦克；2 - 我方子弹； 3 - 敌方子弹
	float	fFireTime;			// 敌方坦克的开火时间
	float	fRotateTime;		// 敌方坦克距离下一次调转方向的剩余时间
	Weapon* pNext;				// 指向先一个节点的指针
};

enum  Role
{
	MYTANK		=	0,		// 我方坦克
	ENEMYTANK	=	1,		// 敌方坦克
	MYBULLET	=	2,		// 我方子弹
	ENEMYBULLET	=	3		// 敌方子弹
};

enum  Direction
{
	UP		=	0,		// 上方
    RIGHT	=	1,		// 右方
	DOWN	=	2,		// 下方
	LEFT	=	3		// 左方
};

extern	Weapon *g_pHead;

// 链表管理函数
extern int	GetListSize();								// 返回当前链表节点数量

extern Weapon	*FindNode(const char *szName );					// 根据名字获取Sprite
//
extern void	AddToList( Weapon *pSprite );				// 添加一个Sprite到链表里
//
extern void			DeleteNode( const char *szName );		// 根据名字删除Sprite.
//
extern void			DeleteList(  );							// 删除所有Sprite.

#endif // _LIST_H_
