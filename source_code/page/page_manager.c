#include <config.h>
#include <page_manager.h>
#include <render.h>
#include <string.h>
#include <stdlib.h>


extern void GetSelectedDir(char *strSeletedDir);
extern void GetIntervalTime(int *piIntervalSecond);

static PT_PageAction g_ptPageActionHead;



int RegisterPageAction(PT_PageAction ptPageAction)
{
	PT_PageAction ptTmp;

	if (!g_ptPageActionHead)
	{
		g_ptPageActionHead   = ptPageAction;
		ptPageAction->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptPageActionHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptPageAction;
		ptPageAction->ptNext = NULL;
		app_printf("the fun is %s, and the disp opr is : %s\n",__func__,ptTmp->name);
	}

	return 0;
}



void ShowPages(void)
{
	int i = 0;
	PT_PageAction ptTmp = g_ptPageActionHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}


PT_PageAction Page(char *pcName)
{
	PT_PageAction ptTmp = g_ptPageActionHead;
	
	while (ptTmp)
	{
		if (strcmp(ptTmp->name, pcName) == 0)
		{
			return ptTmp;
		}
		ptTmp = ptTmp->ptNext;
	}
	return NULL;
}



int ID(char *strName)
{
	return (int)(strName[0]) + (int)(strName[1]) + (int)(strName[2]) + (int)(strName[3]);
}



int GeneratePage(PT_PageLayout ptPageLayout, PT_VideoMem ptVideoMem)
{
	T_PixelDatas tOriginIconPixelDatas;
	T_PixelDatas tIconPixelDatas;
	int iError;
	PT_Layout atLayout = ptPageLayout->atLayout;
		
	
	if (ptVideoMem->ePicState != PS_GENERATED)
	{
		ClearVideoMem(ptVideoMem, COLOR_BACKGROUND);

		tIconPixelDatas.iBpp          = ptPageLayout->iBpp;
        
		tIconPixelDatas.aucPixelDatas = malloc(ptPageLayout->iMaxTotalBytes);
		if (tIconPixelDatas.aucPixelDatas == NULL)
		{
			return -1;
		}

		while (atLayout->strIconName)
		{
            
			iError = GetPixelDatasFromIcon(atLayout->strIconName, &tOriginIconPixelDatas);
			if (iError)
			{
				DBG_PRINTF("GetPixelDatasFromIcon %s error!\n", atLayout->strIconName);
                free(tIconPixelDatas.aucPixelDatas);
				return -1;
			}
 			tIconPixelDatas.iHeight = atLayout->iBotRightY - atLayout->iTopLeftY + 1;
			tIconPixelDatas.iWidth  = atLayout->iBotRightX - atLayout->iTopLeftX+ 1;
			tIconPixelDatas.iLineBytes  = tIconPixelDatas.iWidth * tIconPixelDatas.iBpp / 8;
			tIconPixelDatas.iTotalBytes = tIconPixelDatas.iLineBytes * tIconPixelDatas.iHeight;

            
 			PicZoom(&tOriginIconPixelDatas, &tIconPixelDatas);

            
 			PicMerge(atLayout->iTopLeftX, atLayout->iTopLeftY, &tIconPixelDatas, &ptVideoMem->tPixelDatas);

            
 			FreePixelDatasForIcon(&tOriginIconPixelDatas);
 			atLayout++;
		}

        
		free(tIconPixelDatas.aucPixelDatas);
		ptVideoMem->ePicState = PS_GENERATED;
	}

	return 0;
}


int TimeMSBetween(struct timeval tTimeStart, struct timeval tTimeEnd)
{
	int iMS;
	iMS = (tTimeEnd.tv_sec - tTimeStart.tv_sec) * 1000 + (tTimeEnd.tv_usec - tTimeStart.tv_usec) / 1000;
	return iMS;
}


int GenericGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	T_InputEvent tInputEvent;
	int iRet;
	int i = 0;
	PT_Layout atLayout = ptPageLayout->atLayout;

	app_printf(" the func is : %s \n",__func__);
	
	
	iRet = GetInputEvent(&tInputEvent);
	if (iRet)
	{
		return -1;
	}

	if (tInputEvent.iType != INPUT_TYPE_TOUCHSCREEN)
	{
		return -1;
	}

	*ptInputEvent = tInputEvent;
	
	
	
	while (atLayout[i].strIconName)
	{
		app_printf("the click icon name is : %s\n",atLayout[i].strIconName);
		if ((tInputEvent.iX >= atLayout[i].iTopLeftX) && (tInputEvent.iX <= atLayout[i].iBotRightX) && \
			 (tInputEvent.iY >= atLayout[i].iTopLeftY) && (tInputEvent.iY <= atLayout[i].iBotRightY))
		{
			
			return i;
		}
		else
		{
			i++;
		}			
	}

	
	return -1;
}



void GetPageCfg(PT_PageCfg ptPageCfg)
{

	app_printf("the func is : %s\n",__func__);
    GetSelectedDir(ptPageCfg->strSeletedDir);
    GetIntervalTime(&ptPageCfg->iIntervalSecond);
}


int PagesInit(void)
{
	int iError;

	iError  = MainPageInit();
	iError |= SettingPageInit();
	iError |= IntervalPageInit();
	iError |= BrowsePageInit();
    iError |= AutoPageInit();
    iError |= ManualPageInit();	
	
	return iError;
}






