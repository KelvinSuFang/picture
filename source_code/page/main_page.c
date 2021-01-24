
#include <config.h>
#include <render.h>
#include <stdlib.h>

static T_Layout g_atMainPageIconsLayout[] = {
	{0, 0, 0, 0, "browse_mode.bmp"},
	{0, 0, 0, 0, "continue_mod.bmp"},
	{0, 0, 0, 0, "setting.bmp"},
	{0, 0, 0, 0, NULL},
};

static T_PageLayout g_tMainPageLayout = {
	.iMaxTotalBytes = 0,
	.atLayout       = g_atMainPageIconsLayout,
};



static void  CalcMainPageLayout(PT_PageLayout ptPageLayout)
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
	app_printf("the icons location is : iTopLeftY = %d, iBotRightY = %d, iTopLeftX =%d, iBotRightX = %d \n",
		atLayout[0].iTopLeftY,atLayout[0].iBotRightY,atLayout[0].iTopLeftX,atLayout[0].iBotRightX);
	
	iTmpTotalBytes = (atLayout[0].iBotRightX - atLayout[0].iTopLeftX + 1) * (atLayout[0].iBotRightY - atLayout[0].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}


	
	atLayout[1].iTopLeftY  = atLayout[0].iBotRightY + iHeight / 2 + 1;
	atLayout[1].iBotRightY = atLayout[1].iTopLeftY + iHeight - 1;
	atLayout[1].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[1].iBotRightX = atLayout[1].iTopLeftX + iWidth * 2 - 1;
	app_printf("the icons location is : iTopLeftY = %d, iBotRightY = %d, iTopLeftX =%d, iBotRightX = %d \n",
		atLayout[1].iTopLeftY,atLayout[1].iBotRightY,atLayout[1].iTopLeftX,atLayout[1].iBotRightX);

	
	iTmpTotalBytes = (atLayout[1].iBotRightX - atLayout[1].iTopLeftX + 1) * (atLayout[1].iBotRightY - atLayout[1].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

	
	atLayout[2].iTopLeftY  = atLayout[1].iBotRightY + iHeight / 2 + 1;
	atLayout[2].iBotRightY = atLayout[2].iTopLeftY + iHeight - 1;
	atLayout[2].iTopLeftX  = (iXres - iWidth * 2) / 2;
	atLayout[2].iBotRightX = atLayout[2].iTopLeftX + iWidth * 2 - 1;
	app_printf("the icons location is : iTopLeftY = %d, iBotRightY = %d, iTopLeftX =%d, iBotRightX = %d \n",
		atLayout[2].iTopLeftY,atLayout[2].iBotRightY,atLayout[2].iTopLeftX,atLayout[2].iBotRightX);

	iTmpTotalBytes = (atLayout[2].iBotRightX - atLayout[2].iTopLeftX + 1) * (atLayout[2].iBotRightY - atLayout[2].iTopLeftY + 1) * iBpp / 8;
	if (ptPageLayout->iMaxTotalBytes < iTmpTotalBytes)
	{
		ptPageLayout->iMaxTotalBytes = iTmpTotalBytes;
	}

}



static int MainPageGetInputEvent(PT_PageLayout ptPageLayout, PT_InputEvent ptInputEvent)
{
	return GenericGetInputEvent(ptPageLayout, ptInputEvent);
}



static void ShowMainPage(PT_PageLayout ptPageLayout)
{
	PT_VideoMem ptVideoMem;
	int iError;

	PT_Layout atLayout = ptPageLayout->atLayout;

	app_printf("the func is : %s \n",__func__);
	
	
	ptVideoMem = GetVideoMem(ID("main"), 1);
	if (ptVideoMem == NULL)
	{
		DBG_PRINTF("can't get video mem for main page!\n");
		return;
	}

	

	
	if (atLayout[0].iTopLeftX == 0)
	{
		CalcMainPageLayout(ptPageLayout);
	}

	
	iError = GeneratePage(ptPageLayout, ptVideoMem);	

	
	FlushVideoMemToDev(ptVideoMem);

	
	PutVideoMem(ptVideoMem);
}



static void MainPageRun(PT_PageParams ptParentPageParams)
{
	int iIndex;
	T_InputEvent tInputEvent;
	int bPressed = 0;
	int iIndexPressed = -1;
    T_PageParams tPageParams;

	app_printf("the func is : %s \n",__func__);

    tPageParams.iPageID = ID("main");
	
	
	ShowMainPage(&g_tMainPageLayout);

	

	
	while (1)
	{
		iIndex = MainPageGetInputEvent(&g_tMainPageLayout, &tInputEvent);
		if (tInputEvent.iPressure == 0)
		{
			
			if (bPressed)
			{
				app_printf("the icon has been pressed, and now ,it is loosen \n");
				
				
				ReleaseButton(&g_atMainPageIconsLayout[iIndexPressed]);
				bPressed = 0;

				if (iIndexPressed == iIndex) 
				{
					switch (iIndexPressed)
					{
						case IS_BROWSE_BUTTON: 
						{
							app_printf("it is IS_BROWSE_BUTTON \n");
                            
                            tPageParams.strCurPictureFile[0] = '\0';
							Page("manual")->Run(&tPageParams); 

							
							ShowMainPage(&g_tMainPageLayout);

							break;
						}
						case IS_AUTO_BUTTON: 
						{
							Page("auto")->Run(&tPageParams); 

							
							ShowMainPage(&g_tMainPageLayout);

							break;
						}
						case IS_SELECT_BUTTON: 
						{
							Page("setting")->Run(&tPageParams); 

							
							ShowMainPage(&g_tMainPageLayout);

							break;
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
				app_printf("the icon has been still pressed !\n");
				if (!bPressed)
				{
					
					bPressed = 1;
					iIndexPressed = iIndex;
					PressButton(&g_atMainPageIconsLayout[iIndexPressed]);
				}
			}
		}		
	}
}

static T_PageAction g_tMainPageAction = {
	.name          = "main",
	.Run           = MainPageRun,
	.GetInputEvent = MainPageGetInputEvent,
	
};



int MainPageInit(void)
{
	return RegisterPageAction(&g_tMainPageAction);
}





