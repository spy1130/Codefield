////////////////
//void open_claw(void)
////pick_up_a_ponit_1_2_3()
///////////////////
//pick_up_a_ponit_1_top()
//{
//	//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
//	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
//	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
//	
//	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�956	
//	sprintf(buf1,"���£�4�ż�");
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
//	sprintf(buf1,"2�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1532�����ĵ�1632
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
//	sprintf(buf1,"3�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	
//	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1290
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
//	sprintf(buf1,"5�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);	
//	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
//	
//	close_claw();
//	
//	
////	sprintf(buf1,"[s10]����д��������滹ûд������ͣ��");
////	speak_len1=strlen(( const char *)buf1);
////	speak_context((u8*)buf1,speak_len1);	
////	car_stop();
////	while(1);
//	
//	
//	
//	
//	////////////////////////
//	//��ʼ̧��a�����
//	////////////////////////////////
//	
//	
//	
//	//Ȼ��̧���е��
//	sprintf(buf1,"̧��3�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);		
//	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
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
//	sprintf(buf1,"4�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
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
//	sprintf(buf1,"2�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);
//	
//	
//	
//	//2�Ŷ���ӳ�ʼλ�ã�1632�����ĵ�servo_2_orign_position
//	//1�Ŷ���ӳ�ʼλ�ã�1343�����ĵ�servo_1_orign_position
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
//	//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
//	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
//	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
//	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�947
//	
//	sprintf(buf1,"���£�4�ż�");
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
//	//4�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1575�����ĵ�1790   +215
//	//         2�Ŷ���ӳ�ʼλ�ã�1400�����ĵ�1185   -215
//	sprintf(buf1,"2�ż�");
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
//	//2�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1164
//	sprintf(buf1,"3�ż�");
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
//	//3�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
//	sprintf(buf1,"5�ż�");
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
//	//Ȼ��̧���е��
//	
//	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
//	sprintf(buf1,"̧��3�ż�");
//	speak_len1=strlen(( const char *)buf1);
//	speak_context((u8*)buf1,speak_len1);		
//	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
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
//	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
//	
//	sprintf(buf1,"4�ż�");
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
//	//2�Ŷ���ӳ�ʼλ�ã�1790�����ĵ�1575
//	//1�Ŷ���ӳ�ʼλ�ã�1185�����ĵ�1400
//	sprintf(buf1,"2�ż�");
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
//	//��ȡA�㶥��ɫ�顣��ʼצ��λ��Ϊ 	Tim1_Init(1400,1532,2500,1000);//ֱ��ֵ	Tim2_Init(1500,1560,270,250);
//	//��ץ��λ��Ϊ xxx��1553,1237,947,2100
//	//��צע���������4�Ŷ����Ȼ�����2�Ŷ������3�Ŷ�������5�Ŷ��
//	//��ʼ��צ��4�Ŷ���ӳ�ʼλ�ã�1000�����ĵ�947
//	sprintf(buf1,"���£�4�ż�");
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
//	//4�ž�λ��3�Ŷ���ӳ�ʼλ�ã�2500�����ĵ�1237
//	sprintf(buf1,"3�ż�");
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
//	//3�ž�λ��2�Ŷ���ӳ�ʼλ�ã�1575�����ĵ�2053    +478
//	//        1�Ŷ���ӳ�ʼλ�ã�1400���� �ĵ�922    -478
//	sprintf(buf1,"2�ż�");
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
//	//2�ž�λ��5�Ŷ���ӳ�ʼλ�ã�1500�����ĵ�2100
//	sprintf(buf1,"5�ż�");
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
//	//Ȼ��̧���е��

//		
//	//3�Ŷ���ӳ�ʼλ�ã�1237�����ĵ�2500
//	sprintf(buf1,"̧��3�ż�");
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
//	//4�Ŷ���ӳ�ʼλ�ã�947�����ĵ�1000
//	sprintf(buf1,"4�ż�");
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

//	//2�Ŷ���ӳ�ʼλ�ã�2053�����ĵ�1575
//	//1�Ŷ���ӳ�ʼλ�ã�922�����ĵ�1400
//	sprintf(buf1,"2�ż�");
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
