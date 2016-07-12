/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "List.h"
/////////////////////////////////////////////////////////////////////////////////
//
//
Weapon	*g_pHead	=	NULL;
int		 g_listSize =	0;			// 链表中的节点数
//

/////////////////////////////////////////////////////////////////////////////
// 返回链表中节点数量
int	GetListSize()
{
	return g_listSize;
}


/////////////////////////////////////////////////////////////////////////////
// 根据精灵名称、得分以及时间间隔，创建一个新节点保存到链表中最后一个位置
void AddToList(Weapon *pSprite)
{
	Weapon* pTemp = (Weapon*)malloc(sizeof(Weapon)) ;
	strcpy(pTemp->szName, pSprite->szName);
	pTemp->fPosX = pSprite->fPosX;
	pTemp->fPosY = pSprite->fPosY;
	pTemp->fSpeedX = pSprite->fSpeedX;
	pTemp->fSpeedY = pSprite->fSpeedY;
	pTemp->iDir = pSprite->iDir;
	pTemp->iType = pSprite->iType;
	pTemp->iHp = pTemp->iHp;
	pTemp->fRotateTime = pSprite->fRotateTime;
	pTemp->fFireTime = pSprite->fFireTime;

	pTemp->pNext = NULL;

	Weapon* pNode = g_pHead;

	if(g_pHead == NULL) // 表头为空，说明加入的是一个节点
	{
		g_pHead = pTemp;
	}
	else{
		while(pNode->pNext != NULL)  // 循环读到链表的最后一个节点
		{
			pNode = pNode->pNext;
		}
		pNode->pNext = pTemp;
	}
	g_listSize++;

}
/////////////////////////////////////////////////////////////////////////////////
//
// 根据名字获取Sprite
Weapon* FindNode( const char *szName )
{
	Weapon	*pNode	=	g_pHead;
	while( NULL != pNode )
	{
		if( strcmp( pNode->szName, szName ) == 0 )
			return pNode;

		pNode	=	pNode->pNext;
	}

	return NULL;
}


//==============================================================================
//
// 根据名字删除Sprite
void DeleteNode( const char *szName)
{
	Weapon	*pNode	=	g_pHead;
	Weapon  *pTemp	=	NULL;

	// 如果链表不为空
	if(pNode != NULL)
	{
		if(strcmp(pNode->szName, szName)==0)  // 如果表头就是要找的节点
		{
			g_pHead = g_pHead->pNext;
			free(pNode);
		}
		else
		{
			while(strcmp(pNode->pNext->szName, szName)) // 该节点的下一个节点不是要找的节点
			{
				pNode = pNode->pNext;
			}
			pTemp = pNode->pNext;
			pNode->pNext = pNode->pNext->pNext;
			free(pTemp);
		}
	}
	g_listSize--;
}

//==============================================================================
//
// 删除所有Sprite
void DeleteList( )
{
	Weapon	*pNode				=	NULL;
	Weapon	*pNext	=	g_pHead;
	while( NULL != pNext )
	{
		pNode	=	pNext;
		pNext	=	pNext->pNext;

		// 删除该节点对应的精灵
		dDeleteSprite(pNode->szName);

		// 释放内存
		free( pNode );
	};

	//
	g_listSize = 0;
	g_pHead	=	NULL;
}


