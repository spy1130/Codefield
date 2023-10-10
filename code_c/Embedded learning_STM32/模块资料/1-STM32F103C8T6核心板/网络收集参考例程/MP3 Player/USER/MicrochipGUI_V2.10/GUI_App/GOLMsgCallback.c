/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               GOLMsgCallback.c
** Descriptions:            None
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-30
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
/* ����MP3���� */
extern OS_EVENT *mp3Mbox;		
/* OS�����ź��� */	
extern OS_EVENT *StopMP3Decode;

extern OS_EVENT *MP3Continue; 

extern OS_EVENT *MP3Pause;

/* MP3����״̬ */
extern AUDIO_Playback_status_enum AUDIO_Playback_status ;

extern uint8_t vs1003ram[5];


/* Private function prototypes -----------------------------------------------*/
WORD MsgMP3(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg);


WORD GOLMsgCallback(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{
   switch(ScreenState)
   {
     case DISPLAY_MP3:        
          MsgMP3(objMsg, pObj, pMsg);		/* �������� */
          return 1; 
     default:
          return 1;    
   }
}


WORD MsgMP3(WORD objMsg, OBJ_HEADER* pObj, GOL_MSG* pMsg)
{ 
  SLIDER   *pSld;
  LISTBOX  *pLb;
  BUTTON   *Btn;
  LISTITEM *pItem;
  PICTURE  *pPicture;
  char     asci;
  char     TxedMessage[40];
  pLb =(LISTBOX*)GOLFindObject(ID_LISTBOX1);

   switch(GetObjID(pObj))
   {
     case ID_LISTBOX1:   /* �б�� */	  
          if(pMsg->uiEvent == EVENT_PRESS && objMsg == LB_MSG_TOUCHSCREEN )
		  {
		    static LISTITEM *LastpItem = NULL;
            LbMsgDefault(objMsg, (LISTBOX*)pObj, pMsg);
	        pItem = LbGetSel(pLb,NULL); 

		    if(pItem != NULL)
  		    {  	
	           if( ( LastpItem != NULL ) && (LastpItem != pItem) )
	           {
			      LbSetBitmap(LastpItem, (void*)&musicoff);
               }
			   			 			  						   
			   pSld = (SLIDER*)GOLFindObject(ID_SLIDER3); 
               SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
               SetState(pSld, SLD_DRAW_THUMB);
		       SldDraw(pSld);

			   Btn = (BUTTON*)GOLFindObject(ID_BUTTON2);
			   if( ( Btn->pBitmap == (void*)&play_on && outOfData != (uint32_t)-1 ) && outOfData != 1 )   /* ����Ѿ���ͣ���� */
			   {
			       OSSemPost(MP3Continue);	
				   OSTimeDlyHMSM(0, 0, 0, 100);	               /* 100ms */
				   AUDIO_Playback_status = IS_PLAYING;
			   }
			   BtnSetBitmap(Btn,(void*)&play_off);
			   BtnDraw((BUTTON*)Btn);	

		       LastpItem = pItem;
		       LbSetBitmap(pItem, (void*)&musicon);
			   SetState(pLb, LB_DRAW_ITEMS);
		       LbDraw(pLb);

			   memset(TxedMessage,0,sizeof(TxedMessage));
			   strcpy(TxedMessage,MUSIC_DIRECTORY);
			   strcat(TxedMessage,"/");
			   strcat(TxedMessage,pItem->pText);
			   printf("-- select music %s \r\n",TxedMessage);
			   if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			   {
			     if( (strstr(TxedMessage,"MP3") !=NULL) )
			     {
				    TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				 }
			     else if( (strstr(TxedMessage,"mp3") !=NULL) )
			     {
				    TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				 } 

			     if(AUDIO_Playback_status == IS_PLAYING)
			     {		      
			        OSSemPost(StopMP3Decode);	           /* ֹͣ����     */	 
				    AUDIO_Playback_Stop();
			     }
			     OSTimeDlyHMSM(0, 0, 1, 0);	               /* 1 second     */
			     OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* ��ʼ�������� */	 
			   }	 
		     }
		   }
		  return 1;
	  
      case ID_SLIDER2:   /* ���Ž�����     */	
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
	       {  
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER2);
		     SldMsgDefault(objMsg,(SLIDER*)pObj, pMsg);	
			 /* ��תREADBUF_SIZE��������*/
			 MP3_Data_Index = ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) - \
			                  ( (uint32_t)( ( (float)SldGetPos(pSld)/200 ) * ( mp3FileObject.fsize - mp3_info.data_start) ) % READBUF_SIZE ) );

		     res = f_lseek(&mp3FileObject, mp3_info.data_start + MP3_Data_Index );		
		     SetState(pSld,SLD_DRAW_THUMB); 
		   } 
		   return 1;
		  	      	  
	  case ID_SLIDER3:   /* �б�򻬶��� */
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
		   {
	         pSld = (SLIDER*)GOLFindObject(ID_SLIDER3); 
	         /*  Process message by default */
             SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
             /* Set new list box position	*/
             if(LbGetFocusedItem(pLb) != LbGetCount(pLb) - SldGetPos(pSld) -1 )
		     {
               LbSetFocusedItem(pLb,LbGetCount(pLb) - SldGetPos(pSld) - 1 );
               SetState(pLb, LB_DRAW_ITEMS);
             }
		   }
           return 1;		

      case ID_BUTTON1: 	                 /* ��һ��         */
	       if(objMsg == BTN_MSG_PRESSED)
		   {		      
			  /* �����б�� */
			  pItem = LbGetSel(pLb,NULL);
			  if(pItem != NULL)
		      {			
			  	 Btn = (BUTTON*)GOLFindObject(ID_BUTTON2); 
				 if( Btn->pBitmap == (void*)&play_on )   /* ����Ѿ���ͣ���� */
			     {
			       OSSemPost(MP3Continue);	
				   OSTimeDlyHMSM(0, 0, 0, 100);	               /* 100ms */
			     }
		         BtnSetBitmap(Btn,(void*)&play_off);
				 BtnDraw((BUTTON*)Btn);	
				 				 	         
				 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);

		         if( (*pItem).pPrevItem == NULL )	   /* ����Ѿ��ǵ�һ����Ԫ */
		         {
				    pItem = LbGetSel(pLb,NULL);	
				 }
				 else
				 {	
				    LbSetBitmap(pItem, (void*)&musicoff);
				    pItem = (*pItem).pPrevItem;					
				 }				 
				 		  
	             LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);	 
				 LbSetBitmap(pLb->pFocusItem, (void*)&musicon);
				 LbChangeSel(pLb,pLb->pFocusItem);
    	         SetState(pLb, LB_DRAW_ITEMS);

			     if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)-1)
				 {
					SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
            		SetState(pSld, SLD_DRAW_THUMB);
				 }
				 pItem = LbGetSel(pLb,NULL);			
			     memset(TxedMessage,0,sizeof(TxedMessage));
			     strcpy(TxedMessage,MUSIC_DIRECTORY);
			     strcat(TxedMessage,"/");
			     strcat(TxedMessage,pItem->pText);
			     printf("-- select music %s \r\n",TxedMessage);
			     if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			     {
			       if( (strstr(TxedMessage,"MP3") !=NULL) )
			       {
				      TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
			       else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			       {
				      TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0; 
				   }

			       if (outOfData != 1)
				   {
				     OSSemPost(StopMP3Decode);	             /* ֹͣ����     */	 
				     AUDIO_Playback_Stop();
				   }

				   BtnSetBitmap(pObj,(void*)&previous_off);
			       BtnDraw((BUTTON*)pObj); 

			       OSTimeDlyHMSM(0, 0, 1, 0);	             /* 1 second   */

                   BtnSetBitmap(pObj,(void*)&previous_on);
			       
				   OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* ��ʼ�������� */
			     }
			  }
		   }
		   return 1;

      case ID_BUTTON2: 	                 /* ���Ÿ���       */
	       if(objMsg == BTN_MSG_PRESSED)
		   {
		      if( ((BUTTON *)pObj)->pBitmap == (void*)&play_on )	 /* ����Ѿ���ͣ���� */
			  {	 
			     if( ( AUDIO_Playback_status == NO_SOUND ) && !( outOfData == 1 ) )
				 {
			        BtnSetBitmap(pObj,(void*)&play_off);
				    BtnDraw((BUTTON*)pObj); 
					OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */
			
					OSSemPost(MP3Continue);	
                    AUDIO_Playback_status = IS_PLAYING;       
				 }
				 else if( ( AUDIO_Playback_status == NO_SOUND ) && ( outOfData == 1 ) )	 /* �����Ѿ����� */
				 {
			        BtnSetBitmap(pObj,(void*)&play_off);
				    BtnDraw((BUTTON*)pObj); 

					pItem = LbGetSel(pLb,NULL); 
				    memset(TxedMessage,0,sizeof(TxedMessage));
			        strcpy(TxedMessage,MUSIC_DIRECTORY);
			        strcat(TxedMessage,"/");
			        strcat(TxedMessage,pItem->pText);
			        printf("-- select music %s \r\n",TxedMessage);
			        if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			        {
			           if( (strstr(TxedMessage,"MP3") !=NULL) )
			           {   
					       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
					   }
			           else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			           {
					       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0; 
					   }
			           OSTimeDlyHMSM(0, 0, 1, 0);	               /* 1 second     */
			           OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* ��ʼ�������� */	 
			        }
				 }
			  }
		      else											/* ���ڲ����� */
		      {	
			     BtnSetBitmap(pObj,(void*)&play_on);
				 BtnDraw((BUTTON*)pObj); 
				 OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */
				 		 
	             OSSemPost(MP3Pause);	
                 AUDIO_Playback_status = NO_SOUND;       
			    
				 /* ��ʾ�������ʡ������� */
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
		   return 1;

      case ID_BUTTON3: 	                 /* ֹͣ����	   */
	       if(objMsg == BTN_MSG_PRESSED)
		   {
		      pItem = LbGetSel(pLb,NULL); 
			  if(pItem != NULL)
			  {
			    Btn = (BUTTON*)GOLFindObject(ID_BUTTON2);
				if( Btn->pBitmap == (void*)&play_on )   /* ����Ѿ���ͣ���� */
			    {
			       OSSemPost(MP3Continue);	
				   OSTimeDlyHMSM(0, 0, 0, 100);	               /* 100ms */
			    }
			    BtnSetBitmap(Btn,(void*)&play_on);
			    BtnDraw((BUTTON*)Btn);	

				BtnSetBitmap(pObj,(void*)&stop_off);
			    BtnDraw((BUTTON*)pObj); 

			    OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

                BtnSetBitmap(pObj,(void*)&stop_on);

				outOfData = 1;
			    AUDIO_Playback_Stop();
				OSSemPost(StopMP3Decode);	             /* ֹͣ����     */
			  }
		   }	       
		   return 1;

      case ID_BUTTON4: 	                 /* ��һ��         */
	       if(objMsg == BTN_MSG_PRESSED)
		   {		      
			  /* �����б�� */
			  pItem = LbGetSel(pLb,NULL);
			  if(pItem != NULL)
		      {
		         Btn = (BUTTON*)GOLFindObject(ID_BUTTON2); 
				 if( Btn->pBitmap == (void*)&play_on )   /* ����Ѿ���ͣ���� */
			     {
			       OSSemPost(MP3Continue);	
				   OSTimeDlyHMSM(0, 0, 0, 100);	               /* 100ms */
			     }
		         BtnSetBitmap(Btn,(void*)&play_off);
				 BtnDraw((BUTTON*)Btn);	

				 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);			

		         if( (*pItem).pNextItem == NULL )	   /* ����Ѿ������һ����Ԫ */
		         {
				    pItem = LbGetSel(pLb,NULL);	
				 }
				 else
				 {	
				    LbSetBitmap(pItem, (void*)&musicoff);
				    pItem = (*pItem).pNextItem;					
				 }			 
				  
                 LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);	
				 LbSetBitmap(pLb->pFocusItem, (void*)&musicon);
				 LbChangeSel(pLb,pLb->pFocusItem);		 
                 SetState(pLb, LB_DRAW_ITEMS);

			     if(LbGetFocusedItem(pLb) != LbGetCount(pLb)-SldGetPos(pSld)-1)
				 {
					SldSetPos(pSld,LbGetCount(pLb)-LbGetFocusedItem(pLb)-1);
            		SetState(pSld, SLD_DRAW_THUMB);
				 }

				 pItem = LbGetSel(pLb,NULL);	
			     memset(TxedMessage,0,sizeof(TxedMessage));
			     strcpy(TxedMessage,MUSIC_DIRECTORY);
			     strcat(TxedMessage,"/");
			     strcat(TxedMessage,pItem->pText);
			     printf("-- select music %s \r\n",TxedMessage);
			     if( (strstr(TxedMessage,"MP3") !=NULL) || (strstr(TxedMessage,"mp3") !=NULL) )	  
			     {
			       if( (strstr(TxedMessage,"MP3") !=NULL) )
			       {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   }
			       else	if( (strstr(TxedMessage,"mp3") !=NULL) )
			       {
				       TxedMessage[ strstr(TxedMessage,"MP3") - TxedMessage + 3 ] = 0;
				   } 

			       if (outOfData != 1)
				   {
				     OSSemPost(StopMP3Decode);	             /* ֹͣ����     */	 
				     AUDIO_Playback_Stop();
				   }

				   BtnSetBitmap(pObj,(void*)&next_off);
			       BtnDraw((BUTTON*)pObj); 

			       OSTimeDlyHMSM(0, 0, 1, 0);	             /* 1 second   */

                   BtnSetBitmap(pObj,(void*)&next_on);

			       OSMboxPost(mp3Mbox,(void*)TxedMessage );  /* ��ʼ�������� */	 
			     }
			  }
		   }	       
		   return 1;

      case ID_BUTTON5: 	                 /* �������ƿ��� */
		   if(objMsg == BTN_MSG_PRESSED)
		   {			 
		     if( ((BUTTON *)pObj)->pBitmap == (void*)&UNMUTE_HOVER)
		   	 {	
		        BtnSetBitmap(pObj,(void*)&MUTE_HOVER);
		        vs1003ram[4] = 0;
				VS1003_SetVol();
			 }
			 else
			 {
			    BtnSetBitmap(pObj,(void*)&UNMUTE_HOVER);
	  	        
				pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 	 
			    vs1003ram[4] = 250 * ( (float)SldGetPos(pSld) / 80 );  /* �������� */
				VS1003_SetVol();
			 }
			 OSTimeDlyHMSM(0, 0, 0, 50);  /* delay 50MS */
		   }
		   return 1;
      case ID_SLIDER1:	  /* ���������� */
	       if(objMsg == SLD_MSG_INC || objMsg == SLD_MSG_DEC )
		   {
	  	     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
		     /*  Process message by default */
		     SldMsgDefault(objMsg, (SLIDER*)pObj, pMsg);
			 
			 vs1003ram[4] = 250 * ( (float)SldGetPos(pSld) / 80 );  /* �������� */
			 VS1003_SetVol();
		   }	
	  	   return 1;

      case ID_PICTURE2:	  /* �������� */
		   if(objMsg == PICT_MSG_SELECTED)
		   {
		     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
			 SldSetPos(pSld, SldGetPos(pSld) + 1 );
			 SetState(pSld, SLD_DRAW_THUMB);

			 vs1003ram[4] = 250 * ( (float)SldGetPos(pSld) / 80 );  /* �������� */
			 VS1003_SetVol();
		   }
	  	   return 1;

      case ID_PICTURE3:	  /* �������� */
		   if(objMsg == PICT_MSG_SELECTED)
		   {
		     pSld = (SLIDER*)GOLFindObject(ID_SLIDER1); 
			 SldSetPos(pSld, SldGetPos(pSld) - 1 );
			 SetState(pSld, SLD_DRAW_THUMB);
			 			 
			 vs1003ram[4] = 250 * ( (float)SldGetPos(pSld) / 80 );  /* �������� */
			 VS1003_SetVol();
		   }
	  	   return 1;	
				
	  case ID_BUTTON6:				     /* �б���ͷ�� */
	       if(objMsg == BTN_MSG_PRESSED)	
		   {
			 pSld =(SLIDER*)GOLFindObject(ID_SLIDER3); 

		     BtnSetBitmap(pObj,(void*)&press_down);
		     BtnDraw((BUTTON*)pObj);

		     OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

	         LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)-1);
    	     SetState(pLb, LB_DRAW_ITEMS);

             SldSetPos(pSld,SldGetPos(pSld)+1);
             SetState(pSld, SLD_DRAW_THUMB);

		     BtnSetBitmap(pObj,(void*)&press_up);
		   }
		   return 1;

	  case  ID_BUTTON7:                  /* �б���ͷ�� */
		    if(objMsg == BTN_MSG_PRESSED)
			{
			  pSld =(SLIDER*)GOLFindObject(ID_SLIDER3);	
		  	
		      BtnSetBitmap(pObj,(void*)&rpress_dowm);
		      BtnDraw((BUTTON*)pObj);		 

		      OSTimeDlyHMSM(0, 0, 0, 200);  /* delay 200MS */

              LbSetFocusedItem(pLb,LbGetFocusedItem(pLb)+1);
              SetState(pLb, LB_DRAW_ITEMS);

              SldSetPos(pSld,SldGetPos(pSld)-1);
              SetState(pSld, SLD_DRAW_THUMB);

		      BtnSetBitmap(pObj,(void*)&rpress_up);
		    }
		    return 1;
				         			   	    	
	  default: 
           return 1; 	
   }	
} 


WORD ExternalMemoryCallback(EXTDATA* memory, LONG offset, WORD nCount, void* buffer)
{		  
   return 1;
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
