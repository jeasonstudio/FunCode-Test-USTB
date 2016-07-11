#include <Stdio.h>
#include "CommonAPI.h"
#include "LessonX.h"
#include "list.h"



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
		pTemp->pNext = NULL;

		Weapon* pNode = g_pHead;

		if(g_pHead == NULL) // 表头为空，说明加入的是第一个节点
		{
			g_pHead = pTemp;
		}
		else{
			// 循环读到链表的最后一个节点
			while(pNode->pNext != NULL)
			{
				pNode = pNode->pNext;
			}
			pNode->pNext = pTemp;
		}
}
