////////////////
//void open_claw(void)
////pick_up_a_ponit_1_2_3()
///////////////////
//pick_up_a_ponit_1_top()
//{
//	//夹取A点顶部色块。初始爪子位子为 	Tim1_Init(1400,1532,2500,1000);//直立值	Tim2_Init(1500,1560,270,250);
//	//下抓后位置为 xxx，1553,1237,947,2100
//	//下爪注意事项，先下4号舵机，然后才是2号舵机，在3号舵机，最后5号舵机
//	
//	//开始下爪，4号舵机从初始位置（1000），改到956	
//	sprintf(buf1,"放下，4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	for(led0pwmval=servo_4_orign_position;led0pwmval>=a_point_top_down[4-1];led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms_xm(10);
//    }
//	
//	TIM_SetCompare4(TIM1,a_point_top_down[4-1]);
//	
//	delay_ms_xm(100);
//	//delay_ms(1000);
//	
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	//4号就位，2号舵机从初始位置（1532），改到1632
//	k_num=0x00;
//	for(led0pwmval=servo_2_orign_position;led0pwmval<=a_point_top_down[2-1];led0pwmval=led0pwmval+3)
//    {
//		
//		
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		servo_num_1_pwmval=servo_1_orign_position-k_num*3;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		
//		delay_ms_xm(10);
//    }
//	
//	TIM_SetCompare2(TIM1,a_point_top_down[2-1]);
//	TIM_SetCompare1(TIM1,a_point_top_down[1-1]);
//	
//	
//	
//	
//	delay_ms(50);
//	//delay_ms(1000);
//	
//	sprintf(buf1,"3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	//2号就位，3号舵机从初始位置（2500），改到1290
//	for(led0pwmval=servo_3_orign_position;led0pwmval>=a_point_top_down[3-1];led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms_xm(20);
//    }
//	
//	TIM_SetCompare3(TIM1,a_point_top_down[3-1]);
//	
//	delay_ms(100);
//		
//	sprintf(buf1,"5号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	//3号就位，5号舵机从初始位置（1500），改到2100
//	
//	close_claw();
//	
//	
////	sprintf(buf1,"[s10]程序写到这里，下面还没写，所以停车");
////	speak_len1=strlen(( const char *)buf1);
////	speak_context((u8*)buf1,speak_len1);	
////	car_stop();
////	while(1);
//	
//	
//	
//	
//	////////////////////////
//	//开始抬起a顶物块
//	////////////////////////////////
//	
//	
//	
//	//然后抬起机械臂
//	sprintf(buf1,"抬起，3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);		
//	//3号舵机从初始位置（1237），改到2500
//	for(led0pwmval=a_point_top_down[3-1];led0pwmval<=a_point_top_up[3-1];led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms_xm(20);
//    }
//	
//	TIM_SetCompare3(TIM1,a_point_top_up[3-1]);
//	delay_ms(200);

//	
//	
//	sprintf(buf1,"4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	//4号舵机从初始位置（947），改到1000
//	
//	for(led0pwmval=a_point_top_down[4-1];led0pwmval>=a_point_top_up[4-1];led0pwmval=led0pwmval-5)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms_xm(10);
//    }
//	
//	TIM_SetCompare4(TIM1,a_point_top_up[4-1]);
//	
//	delay_ms(200);
//	
//	
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	
//	
//	
//	//2号舵机从初始位置（1632），改到servo_2_orign_position
//	//1号舵机从初始位置（1343），改到servo_1_orign_position
//	k_num=0x00;
//	for(led0pwmval=a_point_top_down[2-1];led0pwmval>=a_point_top_up[2-1];led0pwmval=led0pwmval-3)
//    {
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		servo_num_1_pwmval=a_point_top_down[1-1]+k_num*3;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		
//		
//		delay_ms_xm(20);
//    }
//	
//	
//	TIM_SetCompare2(TIM1,a_point_top_up[2-1]);
//	TIM_SetCompare1(TIM1,a_point_top_up[1-1]);
//	
//	delay_ms(200);
//	
//	open_claw();
//	delay_ms_xm(300);
//	close_claw();
//	
//	
//	
//}

////////////////
////pick_up_a_ponit_1_2_3()
///////////////////
//pick_up_a_ponit_2_middle()
//{
//	//夹取A点顶部色块。初始爪子位子为 	Tim1_Init(1400,1532,2500,1000);//直立值	Tim2_Init(1500,1560,270,250);
//	//下抓后位置为 xxx，1553,1237,947,2100
//	//下爪注意事项，先下4号舵机，然后才是2号舵机，在3号舵机，最后5号舵机
//	//开始下爪，4号舵机从初始位置（1000），改到947
//	
//	sprintf(buf1,"放下，4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	for(led0pwmval=1000;led0pwmval>=700;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare4(TIM1,700);
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	//4号就位，2号舵机从初始位置（1575），改到1790   +215
//	//         2号舵机从初始位置（1400），改到1185   -215
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	k_num=0x00;
//	for(led0pwmval=servo_2_orign_position;led0pwmval<=1790;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		
//		
//		delay_ms(20);
//    }
//	
//	TIM_SetCompare2(TIM1,1790);
//	TIM_SetCompare1(TIM1,1185);
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	//2号就位，3号舵机从初始位置（2500），改到1164
//	sprintf(buf1,"3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	for(led0pwmval=2500;led0pwmval>=1210;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms(30);
//    }
//	
//	TIM_SetCompare3(TIM1,1210);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	
//	//3号就位，5号舵机从初始位置（1500），改到2100
//	sprintf(buf1,"5号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	for(led0pwmval=1500;led0pwmval<=2100;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare1(TIM2,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare1(TIM2,2100);
//	
//	
//	
//	//然后抬起机械臂
//	
//	//3号舵机从初始位置（1237），改到2500
//	sprintf(buf1,"抬起，3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);		
//	//3号舵机从初始位置（1237），改到2500
//	
//	for(led0pwmval=1210;led0pwmval<=2500;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms(30);
//    }
//	
//	TIM_SetCompare3(TIM1,2500);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	
//	//4号舵机从初始位置（947），改到1000
//	
//	sprintf(buf1,"4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	
//	for(led0pwmval=700;led0pwmval<=1000;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare4(TIM1,1000);
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	
//		
//	//2号舵机从初始位置（1790），改到1575
//	//1号舵机从初始位置（1185），改到1400
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	k_num=0x00;
//	for(led0pwmval=1790;led0pwmval>=servo_2_orign_position;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		
//		servo_num_1_pwmval=1185+k_num*10;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare2(TIM1,servo_2_orign_position);
//	TIM_SetCompare1(TIM1,servo_1_orign_position);
//	delay_ms(1000);
//	delay_ms(1000);


//}


////////////////
////pick_up_a_ponit_1_2_3()
///////////////////
//pick_up_a_ponit_3_bottom()
//{
//	//夹取A点顶部色块。初始爪子位子为 	Tim1_Init(1400,1532,2500,1000);//直立值	Tim2_Init(1500,1560,270,250);
//	//下抓后位置为 xxx，1553,1237,947,2100
//	//下爪注意事项，先下4号舵机，然后才是2号舵机，在3号舵机，最后5号舵机
//	//开始下爪，4号舵机从初始位置（1000），改到947
//	sprintf(buf1,"放下，4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	for(led0pwmval=1000;led0pwmval>=766;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare4(TIM1,766);
//	
//	delay_ms(1000);
//	delay_ms(1000);
//		
//	
//	//4号就位，3号舵机从初始位置（2500），改到1237
//	sprintf(buf1,"3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	for(led0pwmval=2500;led0pwmval>=1426;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms(30);
//    }
//	
//	TIM_SetCompare3(TIM1,1426);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	//3号就位，2号舵机从初始位置（1575），改到2053    +478
//	//        1号舵机从初始位置（1400）， 改到922    -478
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);

//	k_num=0x00;
//	for(led0pwmval=servo_2_orign_position;led0pwmval<=2053;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		
//		servo_num_1_pwmval=servo_1_orign_position-k_num*10;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		delay_ms(20);
//    }
//	
//	TIM_SetCompare2(TIM1,2053);
//	TIM_SetCompare1(TIM1,922);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//		
//	//2号就位，5号舵机从初始位置（1500），改到2100
//	sprintf(buf1,"5号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	
//	for(led0pwmval=1500;led0pwmval<=2100;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare1(TIM2,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare1(TIM2,2100);
//	
//	
//	
//	//然后抬起机械臂

//		
//	//3号舵机从初始位置（1237），改到2500
//	sprintf(buf1,"抬起，3号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);		
//	for(led0pwmval=1426;led0pwmval<=2500;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare3(TIM1,led0pwmval);
//		delay_ms(30);
//    }
//	
//	TIM_SetCompare3(TIM1,2500);
//	delay_ms(1000);
//	delay_ms(1000);
//	
//	
//	
//	//4号舵机从初始位置（947），改到1000
//	sprintf(buf1,"4号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	
//	for(led0pwmval=766;led0pwmval<=1000;led0pwmval=led0pwmval+10)
//    {
//		TIM_SetCompare4(TIM1,led0pwmval);
//		delay_ms(10);
//    }
//	
//	TIM_SetCompare4(TIM1,1000);
//	
//	delay_ms(1000);
//	delay_ms(1000);
//	

//	//2号舵机从初始位置（2053），改到1575
//	//1号舵机从初始位置（922），改到1400
//	sprintf(buf1,"2号夹");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	
//	k_num=0x00;
//	for(led0pwmval=2053;led0pwmval>=servo_2_orign_position;led0pwmval=led0pwmval-10)
//    {
//		TIM_SetCompare2(TIM1,led0pwmval);
//		
//		
//		servo_num_1_pwmval=922+k_num*10;
//		
//		k_num++;
//		TIM_SetCompare1(TIM1,servo_num_1_pwmval);
//		delay_ms(30);
//    }
//	
//	TIM_SetCompare2(TIM1,servo_2_orign_position);
//	
//	TIM_SetCompare1(TIM1,servo_1_orign_position);
//	
//	delay_ms(1000);
//	delay_ms(1000);

//	
//}






////////////////
////put_down_a_ponit_1()
///////////////////////////
