if(1<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=3)
        {
            //car_point_a_pull_up_down();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     ԭ����ת90�ȣ�ָ��270�ȷ���,ץȡ����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
            turn_left_90_with_axis_recover_6050_check(270.0);
             car_stop();
           
            
            front_dis= GetDis_front(front_echo,front_trig);
//            if(front_dis>=90)
//            {
//                //ǰ��
//        
//               pwm_num=0;
//                 led0pwmval=1500+50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500-50+15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                
//                tempa=front_dis-88;
//                while(pwm_num<=tempa);//6v ��ѹ 
//                
//                car_stop();
//                car_stop();
//            }
//            else if(front_dis<=86)
//            {
//                //����
//        
//               pwm_num=0;
//                 led0pwmval=1500-50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500+50-15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                 tempa=88-front_dis;
//                while(pwm_num<=tempa);//6v ��ѹ 
//                car_stop();
//            }
            
            if(1==grasp_order[grasp_order_index])
            {
                car_pick_up_a_ponit_1_top();
            }
            
            else if(2==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_2_middle();
            }
            
            else if(3==grasp_order[grasp_order_index])
            {
                
                car_pick_up_a_ponit_3_bottle();
            }
             car_stop();
             //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     �ж���ɫ��Ҳ����ֱ�ӷ�����ɫ�������飬��ֱ�ӷ�����ɫ��������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                if(count_point_a<=1)
                {
//                    car_yellow_pull_down_block();
                    
                    //ֱ�ӷŻ�ɫ��飬ǰ����βȫ��
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]β��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            car_stop();
                            break;
                            
                        }
                    }
                     car_stop();    
                        
                      mpu_6050_check(270);  
 //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_yellow==0)
                    {
                        car_pull_down_bottle();
                        count_stack_yellow=1;
                        
                    }
                     else if(count_stack_yellow==1)
                    {
                        car_pull_down_middle();
                        count_stack_yellow=2;
                        
                    }   
                    
                     else if(count_stack_yellow==2)
                    {
                        car_pull_down_top();
                        count_stack_yellow=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
                    turn_left_180_with_6050_check(90.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                        else if(walk_ae_state==1)
                        {
                            if(first_start_state_1==0)
                            {
                                first_start_state_1=1;

                                pwm_num_distance=0;
                        
                            }
                            car_judge_decision();
                            car_excute_decision();
                            //ֱ��100���ף������غ�
                            if(pwm_num_distance>=55)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]55��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
                    turn_right_90_with_6050_check(0.0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                    
                    
                    
                    
                }
                
   ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//���������ϰ�������    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
                
                
                else
                {
                    //car_yellow_not_pull_down_block();
            
                     










/////////////////////////////////
            //////////////////////////////////////                
            //a��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    
                    if(colour_tempa==other)
                    {
                        colour_tempa=yellow;
                        count_stack_tempa=0x00;
                        
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    mpu_6050_check(0);
                    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempa==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempa=1;
                        
                    }
                     else if(count_stack_tempa==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempa=2;
                        
                    }   
                    
                     else if(count_stack_tempa==2)
                    {
                        car_pull_down_top();
                        count_stack_tempa=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    
                    
                    










             /////////////////////////////////
            //////////////////////////////////////                
            //b��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=yellow;
                        count_stack_tempb=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                    //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                      count_point_a--;
    
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                      /////////////////////////////////
            //////////////////////////////////////                
            //c��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=yellow;
                        count_stack_tempc=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                  //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;
                        

                        
                    }
                 
                }
            } 












            
            else if(white==block_colour_order[grasp_order_index])
            {
                //��ת45��
                turn_right_45();
                
                
      //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��b�㣬ж����β�״�����ָ���㣬ж��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                 walk_ae_state=0;
                while(1)
                {
                    car_position_test_all_white();
                    
                    if(walk_ae_state==0)
                    {
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]ͷ��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                        }
                        
                        first_start_state_1=0x00;
                        
                    }
                    
                    
                    else if(walk_ae_state==1)
                    {
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��100���ף������غ�
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]β��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                        
                    }
   
                }
            
                
                //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_white==0)
                    {
                        car_pull_down_bottle();
                        count_stack_white=1;
                        
                    }
                     else if(count_stack_white==1)
                    {
                        car_pull_down_middle();
                        count_stack_white=2;
                        
                    }   
                    
                     else if(count_stack_white==2)
                    {
                        car_pull_down_top();
                        count_stack_white=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
                    turn_left_180_with_6050_check(45.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_5 && 1==qti_6 && 1==qti_7)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]���");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                        else if(walk_ae_state==1)
                        {
                            if(first_start_state_1==0)
                            {
                                first_start_state_1=1;

                                pwm_num_distance=0;
                        
                            }
                            car_judge_decision();
                            car_excute_decision();
                            //ֱ��100���ף������غ�
                            if(pwm_num_distance>=20)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]20��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
                    turn_right_45();
                    mpu_6050_check(0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;

                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
            }
            else if(red==block_colour_order[grasp_order_index])
            {
                
                if(count_point_c<=1)
                {
//                    car_yellow_pull_down_block();
                    turn_right_90_with_6050_check(180.0);
                    //ֱ�ӷź�ɫ��飬ǰ����βȫ��
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]β��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            car_stop();
                            break;
                            
                        }
                    }
                     car_stop();    
                        
                      mpu_6050_check(180);  
 //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_red==0)
                    {
                        car_pull_down_bottle();
                        count_stack_red=1;
                        
                    }
                     else if(count_stack_red==1)
                    {
                        car_pull_down_middle();
                        count_stack_red=2;
                        
                    }   
                    
                     else if(count_stack_red==2)
                    {
                        car_pull_down_top();
                        count_stack_red=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
                    turn_left_180_with_6050_check(90.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]β��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                        else if(walk_ae_state==1)
                        {
                            
                            car_judge_decision();
                            car_excute_decision();
                         
                            if(1==qti_8&& 1==qti_9 && 1==qti_10 && 1==qti_11)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                   
                    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                    
                    
                    
                }
                
  
                     

                else
                {
                     if(colour_tempa==other)
                    {
                        colour_tempa=red;
                        count_stack_tempa=0x00;
                        
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    mpu_6050_check(0);
                    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempa==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempa=1;
                        
                    }
                     else if(count_stack_tempa==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempa=2;
                        
                    }   
                    
                     else if(count_stack_tempa==2)
                    {
                        car_pull_down_top();
                        count_stack_tempa=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    


             /////////////////////////////////
            //////////////////////////////////////                
            //b��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=red;
                        count_stack_tempb=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                    //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                        
                       count_point_a--;;
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
  
                    
 
                      /////////////////////////////////
            //////////////////////////////////////                
            //c��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=red;
                        count_stack_tempc=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                  //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;

                        
                    }
                 

                
                
          
                }
              
            }
       
            else if(black==block_colour_order[grasp_order_index])
            {
                
                //��ת135��
                turn_right_135();
               
                mpu_6050_check(135);
                car_axis_recover_d_point();
                
                
      //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��d�㣬ж����β�״�����ָ���㣬ж��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                 walk_ae_state=0;
                while(1)
                {
                    car_position_test_all_white();
                    
                    if(walk_ae_state==0)
                    {
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_1 && 0==qti_2 && 0==qti_3 && 0==qti_4)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]ͷ��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                        }
                        
                        first_start_state_1=0x00;
                        
                    }
                    
                    
                    else if(walk_ae_state==1)
                    {
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��100���ף������غ�
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]β��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                        
                    }
   
                }
            
                
                //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_black==0)
                    {
                        car_pull_down_bottle();
                        count_stack_black=1;
                        
                    }
                     else if(count_stack_black==1)
                    {
                        car_pull_down_middle();
                        count_stack_black=2;
                        
                    }   
                    
                     else if(count_stack_black==2)
                    {
                        car_pull_down_top();
                        count_stack_black=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
                    turn_left_180_with_6050_check(315.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת45��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_5 && 1==qti_6 && 1==qti_7)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]���");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                        else if(walk_ae_state==1)
                        {
                            if(first_start_state_1==0)
                            {
                                first_start_state_1=1;

                                pwm_num_distance=0;
                        
                            }
                            car_judge_decision();
                            car_excute_decision();
                            //ֱ��100���ף������غ�
                            if(pwm_num_distance>=14)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]14��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
                    turn_left_45();
                    mpu_6050_check(0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                
                

            }
            else if(blue==block_colour_order[grasp_order_index])
            {
               
                if(count_point_e<=1)
                {
//                    car_yellow_pull_down_block();
                    turn_right_90_with_6050_check(180.0);
                    turn_right_90_with_6050_check(90.0);
                    //ֱ�ӷ���ɫ��飬ǰ����βȫ��
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]β��");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            car_stop();
                            break;
                            
                        }
                    }
                     car_stop();    
                        
                      mpu_6050_check(90);  
 //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_blue==0)
                    {
                        car_pull_down_bottle();
                        count_stack_blue=1;
                        
                    }
                     else if(count_stack_blue==1)
                    {
                        car_pull_down_middle();
                        count_stack_blue=2;
                        
                    }   
                    
                     else if(count_stack_blue==2)
                    {
                        car_pull_down_top();
                        count_stack_blue=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��90�ȷ���  
                    turn_left_180_with_6050_check(270.0);
                    car_stop();
                    //
                    


                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ׼��ǰ��o�㣬��������ͣ��o�㣬Ȼ����ת90��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    ////////////////////////////////////////////////////// 
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                car_stop();
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                        else if(walk_ae_state==1)
                        {
                            if(first_start_state_1==0)
                            {
                                first_start_state_1=1;

                                pwm_num_distance=0;
                        
                            }
                            car_judge_decision();
                            car_excute_decision();
                            //ֱ��100���ף������غ�
                            if(pwm_num_distance>=49)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]49��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ���  
                    turn_left_90_with_6050_check(0.0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ�ԭ�ص�ͷ180
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //ԭ�ص�ͷ180�ȣ�ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;


//////////////////////////////////////////////////////
    
                }
                
  
                     

                else
                {
                     if(colour_tempa==other)
                    {
                        colour_tempa=blue;
                        count_stack_tempa=0x00;
                        
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��2������ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    mpu_6050_check(0);
                    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempa==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempa=1;
                        
                    }
                     else if(count_stack_tempa==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempa=2;
                        
                    }   
                    
                     else if(count_stack_tempa==2)
                    {
                        car_pull_down_top();
                        count_stack_tempa=3;
                        
                    } 
                    car_stop();                    
                        
                        //���������ϣ�ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //��2������׼��������һ����飬  �������������1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    


             /////////////////////////////////
            //////////////////////////////////////                
            //b��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=blue;
                        count_stack_tempb=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                    //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                        
                        count_point_a--;
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
  
                    
 
                      /////////////////////////////////
            //////////////////////////////////////                
            //c��ʱ��ŵ���Դ�� ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=blue;
                        count_stack_tempc=0x00;
                        
                             
                    //    //ԭ����ת90�ȣ�ָ��0�ȷ��� //����д�ķ��㣬���ӵ����˶���ûд��������ʱֻ��ǰ��
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            ֱ��ֱ��ͷ�ڣ�
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //ԭ�ص�ͷ180�ȣ���ָ��180�ȷ���  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              ֱ��10���ף��ܿ�����
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //ֱ��10���ף��ܿ�����
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2������");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               ׼��ǰ��aeֱ�ߣ�ֱ��ֱ��ͷ�ڣ��Ż�ɫ ɫ��
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                    walk_ae_state=0;
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        if(walk_ae_state==0)
                        {
                            car_judge_decision();
                            car_excute_decision();
                            
                            if(1==qti_1 && 1==qti_2 && 1==qti_3 && 1==qti_4)
                            {
                                walk_ae_state=1;
                                
                                sprintf(buf1,"[s10]ͷ��");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //ԭ����ת90�ȣ�ָ��180�ȷ��� 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //��ʼ׼������� //С����λ��׼�������
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                         
                    if(count_stack_tempb==0)
                    {
                        car_pull_down_bottle();
                        count_stack_tempb=1;
                        
                    }
                     else if(count_stack_tempb==1)
                    {
                        car_pull_down_middle();
                        count_stack_tempb=2;
                        
                    }   
                    
                     else if(count_stack_tempb==2)
                    {
                        car_pull_down_top();
                        count_stack_tempb=3;
                        
                    } 
                    car_stop();                    
                        
                        
                  //    //ԭ����ת90�ȣ�ָ��279�ȷ��� 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;  

                        
                    }
                 

                
                
          
                }
              
            
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
            
            }
                
        }
        