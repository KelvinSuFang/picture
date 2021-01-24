#include <config.h>
#include <render.h>
#include <stdlib.h>
#include <string.h>

static T_Layout g_atSettingPageIconsLayout[] = {
	{0, 0, 0, 0, "select_fold.bmp"},
	{0, 0, 0, 0, "interval.bmp"},
	{0, 0, 0, 0, "return.bmp"},
	{0, 0, 0, 0, NULL},
};

static T_PageLayout g_tSettingPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atSettingPageIconsLayout,
};



static void  CalcSettingPageLayout(PT_PageLayout ptPageLayout)
{
	int iStartY;
	int iWidth;
	int iHeight;
	int iXres, iYres, iBpp;
	int iTmpTotalBytes;
	PT_Layout atLayout;

	atLayout = ptPageLayout->atLayout;
	GetDispResolution(&iXres, &iYres, &iBpp);
	ptPageLayout->iBpp = iBpp;

	
	 
	iHeight = iYres * 2 / 10;
	iWidth  = iHeight;
	iStartY = iHeight / 2;
	
	
	atLayout[0].iTopLeftY  = iStartY;
	atLayout[0].iBotRightY = atLayout[0].iTopLeftY + iHeight - 1;
	atLayout[0].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[0].iBotRightX = atLayout[0].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1) * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}


	
	atLayout[1].iTopLeftY  = atLayout[0].iBotRightY + iHeight / 2 + 1;
	atLayout[1].iBotRightY = atLayout[1].iTopLeftY + iHeight - 1;
	atLayout[1].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[1].iBotRightX = atLayout[1].iTopLeftX + iWidth * 2 - 1;

	iTmpTotalBytes = (atLayout[1].iBotRightX - atLayout[1].iTopLeftX + 1) * (atLayout[1].iBotRightY - atLayout[1].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

	
	atLayout[2].iTopLeftY  = atLayout[1].iBotRightY + iHeight / 2 + 1;
	atLayout[2].iBotRightY = atLayout[2].iTopLeftY + iHeight - 1;
	atLayout[2].iTopLeftX  = (iXres - iWidth) / 2;
	atLayout[2].iBotRightX = atLayout[2].iTopLeftX + iWidth - 1;

	iTmpTotalBytes = (atLayout[2].iBotRightX - atLayout[2].iTopLeftX + 1) * (atLayout[2].iBotRightY - atLayout[2].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

}



static void ShowSettingPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;

	app_printf("the func is : %s\n",__func__);
	
	
	ptVideoMem = GetVideoMem(ID("setting"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for setting page!\n");
		return;
	}

	

	
	if (atLayout[0].iTopLeftX == 0)
	{
		CalcSettingPageLayout(ptPageLayout);
	}

	iError = GeneratePage(ptPageLayout, ptVideoMem);	

	
	FlushVideoMemToDev(ptVideoMem);

	
	PutVideoMem(ptVideoMem);
}



static int SettingPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	app_printf("the func is : %s\n",__func__);
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}



static void SettingPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
    T_PageParams tPageParams;

    tPageParams.iPageID = ID("setting");

	app_printf("the func is : %s\n",__func__);
	
	
	ShowSettingPage(&g_tSettingPageLayout);

	

	
	while (1)
	{
		iIndex = SettingPageGetInputEvent(&g_tSettingPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			
			if (bPressed)
			{
				
				ReleaseButton(&g_atSettingPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) 
				{
					switch (iIndexPressed)
					{
						case 0: 
						{
                            
							Page("browse")->Run(&tPageParams);
							ShowSettingPage(&g_tSettingPageLayout);
							break;
						}
						case 1: 
						{
							Page("interval")->Run(&tPageParams);  
							ShowSettingPage(&g_tSettingPageLayout);
							break;
						}
						case 2: 
						{
							return;
						}
						default:
						{
							break;
						}
					}
				}

				iIndexPressed = -1;
			}
		}
		else
		{
			
			if (iIndex != -1)
			{
				if (!bPressed)
				{
					
					bPressed = 1;
					iIndexPressed = iIndex;
					PressButton(&g_atSettingPageIconsLayout[iIndexPressed]);
				}
			}
		}		
	}	
}

static T_PageAction g_tSettingPageAction = {
	.name          = "setting",
	.Run           = SettingPageRun,
	.GetInputEvent = SettingPageGetInputEvent,
	
};



int SettingPageInit(void)
{
	return RegisterPageAction(&g_tSettingPageAction);
}





