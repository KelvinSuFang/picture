
#include <config.h>
#include <input_manager.h>
#include <string.h>

static PT_InputOpr g_ptInputOprHead;
static T_InputEvent g_tInputEvent;

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;


int RegisterInputOpr(PT_InputOpr ptInputOpr)
{
	PT_InputOpr ptTmp;
	
	if (!g_ptInputOprHead)
	{
		g_ptInputOprHead   = ptInputOpr;
		ptInputOpr->ptNext = NULL;
	}
	else
	{
		ptTmp = g_ptInputOprHead;
		while (ptTmp->ptNext)
		{
			ptTmp = ptTmp->ptNext;
		}
		ptTmp->ptNext	  = ptInputOpr;
		ptInputOpr->ptNext = NULL;
		app_printf("the fun is %s, and the disp opr is : %s\n",__func__,ptTmp->name);
	}

	return 0;
}



void ShowInputOpr(void)
{
	int i = 0;
	PT_InputOpr ptTmp = g_ptInputOprHead;

	while (ptTmp)
	{
		printf("%02d %s\n", i++, ptTmp->name);
		ptTmp = ptTmp->ptNext;
	}
}


static void *InputEventThreadFunction(void *pVoid)
{
	T_InputEvent tInputEvent;
	
	
	int (*GetInputEvent)(PT_InputEvent ptInputEvent);
	GetInputEvent = (int (*)(PT_InputEvent))pVoid;

	app_printf("the thread is : %s\n",__func__);
	while (1)
	{
		
		if(0 == GetInputEvent(&tInputEvent))
		{
			
			
			pthread_mutex_lock(&g_tMutex);
			g_tInputEvent = tInputEvent;

			app_printf("pthread_cond_signal , wake the other thread !\n ");
			
			pthread_cond_signal(&g_tConVar);

			
			pthread_mutex_unlock(&g_tMutex);
		}
	}

	return NULL;
}


int AllInputDevicesInit(void)
{
	PT_InputOpr ptTmp = g_ptInputOprHead;
	int iError = -1;
	
	app_printf("the fun is %s\n",__func__);
	
	while (ptTmp)
	{
		app_printf("the PT_InputOpr ptTmp->name is : %s\n",ptTmp->name);
		
		
		if (0 == ptTmp->DeviceInit())
		{
			
			pthread_create(&ptTmp->tTreadID, NULL, InputEventThreadFunction, ptTmp->GetInputEvent);			
			iError = 0;
		}
		ptTmp = ptTmp->ptNext;
	}
	return iError;
}


int GetInputEvent(PT_InputEvent ptInputEvent)
{	
	
	pthread_mutex_lock(&g_tMutex);
	pthread_cond_wait(&g_tConVar, &g_tMutex);	

	
	*ptInputEvent = g_tInputEvent;
	pthread_mutex_unlock(&g_tMutex);
	return 0;	
}


int InputInit(void)
{
	int iError = 0;
	
	iError |= TouchScreenInit();
	return iError;
}





