
#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdio.h>
#include <debug_manager.h>

#define FB_DEVICE_NAME "/dev/fb0"
#define DEFAULT_DIR "/"

#define COLOR_BACKGROUND   0xE7DBB5  
#define COLOR_FOREGROUND   0x514438  


#define DBG_PRINTF DebugPrint

#define app_printf DebugPrint



#define ICON_PATH  "/etc/digitpic/icons"


#define IS_BROWSE_BUTTON	0	
#define IS_AUTO_BUTTON		1
#define IS_SELECT_BUTTON	2

#define  RETURN_BUTTON		0
#define  SHRINK_BUTTON		1
#define  ZOOM_IN_BUTTON		2
#define  LAST_BUTTON		3
#define  NEXT_BUTTON		4
#define  AUTO_BUTTON		5




#endif 




