
#include <input_manager.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>



static int StdinDevInit(void)
{
    struct termios tTTYState;
 
    
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    
    tTTYState.c_lflag &= ~ICANON;
    
    tTTYState.c_cc[VMIN] = 1;   

    
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);

	return 0;
}


static int StdinDevExit(void)
{

    struct termios tTTYState;
 
    
    tcgetattr(STDIN_FILENO, &tTTYState);
 
    
    tTTYState.c_lflag |= ICANON;
	
    
    tcsetattr(STDIN_FILENO, TCSANOW, &tTTYState);	
	return 0;
}


static int StdinGetInputEvent(PT_InputEvent ptInputEvent)
{
	

	

 	char c;
	
		
	ptInputEvent->iType = INPUT_TYPE_STDIN;
	
	c = fgetc(stdin);  
	gettimeofday(&ptInputEvent->tTime, NULL);

#if 0	
	if (c == 'u')
	{
		ptInputEvent->iVal = INPUT_VALUE_UP;
	}
	else if (c == 'n')
	{
		ptInputEvent->iVal = INPUT_VALUE_DOWN;
	}
	else if (c == 'q')
	{
		ptInputEvent->iVal = INPUT_VALUE_EXIT;
	}
	else
	{
		ptInputEvent->iVal = INPUT_VALUE_UNKNOWN;
	}		
#endif	
	return 0;
 }

static T_InputOpr g_tStdinOpr = {
	.name          = "stdin",
	.DeviceInit    = StdinDevInit,
	.DeviceExit    = StdinDevExit,
	.GetInputEvent = StdinGetInputEvent,
};



int StdinInit(void)
{
	return RegisterInputOpr(&g_tStdinOpr);
}





