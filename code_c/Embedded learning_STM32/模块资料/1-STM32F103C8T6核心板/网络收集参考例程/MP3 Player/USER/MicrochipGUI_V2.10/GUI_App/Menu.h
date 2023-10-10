/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               menu.h
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-12-6
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/

#ifndef _MENU_H
#define _MENU_H


/* Private define ------------------------------------------------------------*/
#define     ID_BUTTON1     1    /* ��һ��         */
#define     ID_BUTTON2     2    /* ���Ÿ���       */
#define     ID_BUTTON3     3    /* ֹͣ����		  */
#define     ID_BUTTON4     4	/* ��һ��         */ 
#define     ID_BUTTON5 	   5	/* �������ƿ���	  */
#define     ID_BUTTON6	   6	/* �б���ͷ��	  */
#define     ID_BUTTON7 	   7	/* �б���ͷ��	  */

#define     ID_SLIDER1     10   /* ����������     */
#define     ID_SLIDER2	   11	/* ���Ž�����     */
#define     ID_SLIDER3	   12	/* �����б����� */

#define     ID_LISTBOX1	   13	/* �б��         */

#define     ID_PICTURE1    14	/* ����������      */
#define     ID_PICTURE2    15	/* ��������        */
#define     ID_PICTURE3    16	/* ��������		   */
#define     ID_PICTURE4    17	/* ����������֮һ  */
#define     ID_PICTURE5    18	/* ����������֮��  */
#define     ID_PICTURE6    19	/* ����������֮��  */
#define     ID_PICTURE7    20	/* ����������֮��  */

/* Private typedef -----------------------------------------------------------*/
typedef enum {
   DISPLAY_MP3 =0,
   START_MP3     ,
} SCREEN_STATES;

/* Private variables ---------------------------------------------------------*/
extern SCREEN_STATES  ScreenState;

extern FONT_EXTERNAL  externalfont;
extern volatile const BITMAP_FLASH previous_on;
extern volatile const BITMAP_FLASH previous_off;

extern volatile const BITMAP_FLASH play_on;
extern volatile const BITMAP_FLASH play_off;

extern volatile const BITMAP_FLASH stop_on;
extern volatile const BITMAP_FLASH stop_off;

extern volatile const BITMAP_FLASH next_on;
extern volatile const BITMAP_FLASH next_off ;

extern volatile const BITMAP_FLASH press_up;
extern volatile const BITMAP_FLASH press_down ;

extern volatile const BITMAP_FLASH rpress_up ;
extern volatile const BITMAP_FLASH rpress_dowm ;

extern volatile const BITMAP_FLASH musicon ;
extern volatile const BITMAP_FLASH musicoff ;

extern volatile const BITMAP_FLASH sound_add;
extern volatile const BITMAP_FLASH sound_delete;

extern volatile const BITMAP_FLASH UNMUTE_HOVER;
extern volatile const BITMAP_FLASH MUTE_HOVER;

extern volatile const BITMAP_FLASH Smart_MP3; 
extern volatile const BITMAP_FLASH Smart_MP3_Part1;
extern volatile const BITMAP_FLASH Smart_MP3_Part2;
extern volatile const BITMAP_FLASH Smart_MP3_Part3;
extern volatile const BITMAP_FLASH Smart_MP3_Part4;


/* Private function prototypes -----------------------------------------------*/
void StartMenu(void);


#endif

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
