/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               GOLDrawCallback.C
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-12-2
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

/* Includes ------------------------------------------------------------------*/
#include <includes.h>
#include <menu.h>

/* Private variables ---------------------------------------------------------*/
extern AUDIO_Playback_status_enum AUDIO_Playback_status ;

/* Private function prototypes -----------------------------------------------*/
void DisplayMP3(void);


WORD GOLDrawCallback(void)
{
   switch(ScreenState)
   {
     case DISPLAY_MP3:        
          DisplayMP3();
          return 1; 
     default:
          return 1;    
   }				
}

void DisplayMP3(void)
{
  BUTTON   *Btn;
  LISTBOX  *pLb;
  SLIDER   *pSld;
  PICTURE  *pPicture;
  char     asci;
  static int play_state = -1;
  static LISTITEM *LastpItem = NULL;

  pLb = (LISTBOX*)GOLFindObject(ID_LISTBOX1);				  

  switch(ScreenState)
  {
    case DISPLAY_MP3:
		  
		 if (LbGetSel(pLb,NULL) == NULL)	 /* �б��û��ѡ�� */
		 {
		    SetColor( BLACK );					
			OutTextXY(150 - ( (float)(strlen("��ѡ����ϲ���ĸ���")/2) * 8 ),30,"��ѡ����ϲ���ĸ���");
		 }		 
		 else
		 {	
		    Btn = (BUTTON*)GOLFindObject(ID_BUTTON2); 		

			if ( LastpItem != LbGetSel(pLb,NULL) )		          /*��������� */
			{
			   LastpItem = LbGetSel(pLb,NULL);

			   pPicture =(PICTURE*)GOLFindObject(ID_PICTURE6);
			   PictDraw(pPicture);

		       if(AUDIO_Playback_status == IS_PLAYING)	          /* ������ */
		       {
			      SetColor( BLACK );	    
			      OutTextXY(120,60,"������");	
		       }
		       else if( ( AUDIO_Playback_status == NO_SOUND )  )  /* ��ͣ���� */ 
		       {
			      SetColor( BLACK );
			      OutTextXY(120,60,"��ֹͣ");			  		    
		       }  
			}

			/* �������״̬�仯 */
			if(AUDIO_Playback_status != play_state)
		    {  		
			   pPicture =(PICTURE*)GOLFindObject(ID_PICTURE6);
			   PictDraw(pPicture);

		       if(AUDIO_Playback_status == IS_PLAYING)	          /* ������ */
		       {
			      SetColor( BLACK );	    
			      OutTextXY(120,60,"������");			  
		       }
		       else if( ( AUDIO_Playback_status == NO_SOUND )  )  /* ��ͣ���� */ 
		       {
			      SetColor( BLACK );
			      OutTextXY(120,60,"��ֹͣ");			  		    
		       }  
		       play_state =  AUDIO_Playback_status; 
		    }

		    /* ͬ�����Ž��� */
		    pSld = (SLIDER*)GOLFindObject(ID_SLIDER2);		 
		    SldSetPos(pSld , (uint8_t)( ( (float)MP3_Data_Index / ( mp3FileObject.fsize - mp3_info.data_start) ) * 200 ) );
		    SetState(pSld,SLD_DRAW_THUMB); 

		    if( ( AUDIO_Playback_status == NO_SOUND ) && ( outOfData == 1 ) && ( Btn->pBitmap == (void*)&play_off ))	 /* �����Ѿ����� */
		    {		   	
		   	   BtnSetBitmap(Btn,(void*)&play_on);
			   BtnDraw((BUTTON*)Btn);
		    }

		    if( mp3_info.position != (uint32_t)( ( (float)MP3_Data_Index / ( mp3FileObject.fsize - mp3_info.data_start) ) * mp3_info.duration ))
		    {
			   /* ��ʾ��������ʱ�� */
		   	   mp3_info.position = (uint32_t)( ( (float)MP3_Data_Index / ( mp3FileObject.fsize - mp3_info.data_start) ) * mp3_info.duration );

               pPicture =(PICTURE*)GOLFindObject(ID_PICTURE5);
			   PictDraw(pPicture);
			   
			   SetColor( BLACK );
			   
			   asci = ( mp3_info.position/60 ) /10 + '0';			   
			   OutTextXY(70 , 60 , &asci );	

			   asci = ( mp3_info.position/60 ) %10 + '0';
			   OutTextXY(78 , 60 , &asci );	

			   OutTextXY(86 , 60 , ":");	

			   asci = ( mp3_info.position%60 ) /10 + '0';
			   OutTextXY(94 , 60 , &asci );	

			   asci = ( mp3_info.position%60 ) %10 + '0';
			   OutTextXY(102 , 60 , &asci );

			   /* ��ʾ�������ʡ������� */
		       if ( mp3_info.position == 1 )		 
		       {	
			       pPicture =(PICTURE*)GOLFindObject(ID_PICTURE7);
			       PictDraw(pPicture);

			   	   SetColor( BLACK );
				   
				   mp3_info.bit_rate = VS1003_GetBitrate() * 1000;
		
				   asci = ( (mp3_info.bit_rate/1000)/100 + '0' );
				   OutTextXY(178 , 60 , &asci );	

				   asci = ( ( (mp3_info.bit_rate/1000)%100 ) /10 + '0' );
				   OutTextXY(186 , 60 , &asci );

				   asci = ( ( (mp3_info.bit_rate/1000)%100 ) %10 + '0' );
				   OutTextXY(194 , 60 , &asci);

		           OutTextXY(202 , 60 , "Kbps");	

			       pPicture =(PICTURE*)GOLFindObject(ID_PICTURE4);
			       PictDraw(pPicture);
	
	               SetColor( BLACK );
			       OutTextXY(150 - ( (float)(strlen(mp3_info.title)/2) * 8 ),30,mp3_info.title); 		  							
		        }
		    }
		 } 

		 break;
	default: 
         break; 	
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
