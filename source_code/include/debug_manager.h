
#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H


#define	APP_EMERG	"<0>"	
#define	APP_ALERT	"<1>"	
#define	APP_CRIT	"<2>"	
#define	APP_ERR	    "<3>"	
#define	APP_WARNING	"<4>"	
#define	APP_NOTICE	"<5>"	
#define	APP_INFO	"<6>"	
#define	APP_DEBUG	"<7>"	


#define DEFAULT_DBGLEVEL  4



typedef struct DebugOpr {
	
	char *name;
	int isCanUse;
	
	int (*DebugInit)(void);   
	int (*DebugExit)(void);   
	int (*DebugPrint)(char *strData);  
	
	struct DebugOpr *ptNext;
}T_DebugOpr, *PT_DebugOpr;


int RegisterDebugOpr(PT_DebugOpr ptDebugOpr);


void ShowDebugOpr(void);


PT_DebugOpr GetDebugOpr(char *pcName);


int SetDbgLevel(char *strBuf);


int SetDbgChanel(char *strBuf);


int DebugInit(void);


int DebugPrint(const char *pcFormat, ...);


int InitDebugChanel(void);


int StdoutInit(void);


int NetPrintInit(void);

#endif 
  




