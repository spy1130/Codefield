if(1<=grasp_order[grasp_order_index]&&grasp_order[grasp_order_index]<=3)
        {
            //car_point_a_pull_up_down();
            
                         //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///     原地左转90度，指向270度方向,抓取物体
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////
             //  
            turn_left_90_with_axis_recover_6050_check(270.0);
             car_stop();
           
            
            front_dis= GetDis_front(front_echo,front_trig);
//            if(front_dis>=90)
//            {
//                //前进
//        
//               pwm_num=0;
//                 led0pwmval=1500+50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500-50+15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                
//                tempa=front_dis-88;
//                while(pwm_num<=tempa);//6v 电压 
//                
//                car_stop();
//                car_stop();
//            }
//            else if(front_dis<=86)
//            {
//                //后退
//        
//               pwm_num=0;
//                 led0pwmval=1500-50;//1550;//high mean low speed
//                 TIM_SetCompare3(TIM3,led0pwmval);
//                 
//                 led0pwmval=1500+50-15;//1450;//high mean low speed
//                 TIM_SetCompare4(TIM3,led0pwmval);	
//                 tempa=88-front_dis;
//                while(pwm_num<=tempa);//6v 电压 
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
     ///     判断颜色，也可以直接访问颜色序列数组，我直接访问颜色序列数组
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////     
           
            if(yellow==block_colour_order[grasp_order_index])
            {
                if(count_point_a<=1)
                {
//                    car_yellow_pull_down_block();
                    
                    //直接放黄色物块，前进，尾全白
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]尾白");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(90.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转90度
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
                                sprintf(buf1,"[s10]头黑");
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
                            //直行100毫米，轴心重合
                            if(pwm_num_distance>=55)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]55到");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //原地右转90度，指向0度方向  
                    turn_right_90_with_6050_check(0.0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，原地调头180
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                    
                    
                    
                    
                }
                
   ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                
//下面是有障碍物的情况    ////////////////////////////////
/////////////////////////////////
//////////////////////////////////////                  
                
                
                else
                {
                    //car_yellow_not_pull_down_block();
            
                     










/////////////////////////////////
            //////////////////////////////////////                
            //a临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    
                    if(colour_tempa==other)
                    {
                        colour_tempa=yellow;
                        count_stack_tempa=0x00;
                        
                    //    //原地左转90度，指向0度方向 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    
                    
                    










             /////////////////////////////////
            //////////////////////////////////////                
            //b临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=yellow;
                        count_stack_tempb=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                    //    //原地右转90度，指向180度方向 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                      count_point_a--;
    
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                      /////////////////////////////////
            //////////////////////////////////////                
            //c临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=yellow;
                        count_stack_tempc=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //原地右转90度，指向180度方向 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                  //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;
                        

                        
                    }
                 
                }
            } 












            
            else if(white==block_colour_order[grasp_order_index])
            {
                //右转45度
                turn_right_45();
                
                
      //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往b点，卸货，尾白处到达指定点，卸货
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
                            sprintf(buf1,"[s10]头白");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                        }
                        
                        first_start_state_1=0x00;
                        
                    }
                    
                    
                    else if(walk_ae_state==1)
                    {
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行100毫米，轴心重合
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]尾白");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                        
                    }
   
                }
            
                
                //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(45.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转45度
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
                                sprintf(buf1,"[s10]轴黑");
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
                            //直行100毫米，轴心重合
                            if(pwm_num_distance>=20)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]20到");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //原地右转90度，指向0度方向  
                    turn_right_45();
                    mpu_6050_check(0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，原地调头180
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;

                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
            }
            else if(red==block_colour_order[grasp_order_index])
            {
                
                if(count_point_c<=1)
                {
//                    car_yellow_pull_down_block();
                    turn_right_90_with_6050_check(180.0);
                    //直接放红色物块，前进，尾全白
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]尾白");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(90.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转90度
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
                                sprintf(buf1,"[s10]尾黑");
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
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                   
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                    
                    
                    
                }
                
  
                     

                else
                {
                     if(colour_tempa==other)
                    {
                        colour_tempa=red;
                        count_stack_tempa=0x00;
                        
                    //    //原地左转90度，指向0度方向 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    


             /////////////////////////////////
            //////////////////////////////////////                
            //b临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=red;
                        count_stack_tempb=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                    //    //原地右转90度，指向180度方向 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                        
                       count_point_a--;;
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
  
                    
 
                      /////////////////////////////////
            //////////////////////////////////////                
            //c临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=red;
                        count_stack_tempc=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //原地右转90度，指向180度方向 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                  //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;

                        
                    }
                 

                
                
          
                }
              
            }
       
            else if(black==block_colour_order[grasp_order_index])
            {
                
                //右转135度
                turn_right_135();
               
                mpu_6050_check(135);
                car_axis_recover_d_point();
                
                
      //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往d点，卸货，尾白处到达指定点，卸货
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
                            sprintf(buf1,"[s10]头白");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                        }
                        
                        first_start_state_1=0x00;
                        
                    }
                    
                    
                    else if(walk_ae_state==1)
                    {
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行100毫米，轴心重合
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            walk_ae_state=1;
                            car_stop();
                            sprintf(buf1,"[s10]尾白");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                        
                    }
   
                }
            
                
                //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(315.0);
                    car_stop();
                    //
                                           //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转45度
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
                                sprintf(buf1,"[s10]轴黑");
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
                            //直行100毫米，轴心重合
                            if(pwm_num_distance>=14)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]14到");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //原地右转90度，指向0度方向  
                    turn_left_45();
                    mpu_6050_check(0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，原地调头180
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
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
                    //直接放蓝色物块，前进，尾全白
                     while(1)
                    {
                        car_position_test_all_white();
                        
                               
                        car_judge_decision();
                        car_excute_decision();
                        
                        if(0==qti_8 && 0==qti_9 && 0==qti_10 && 0==qti_11)
                        {
                            
                            
                            sprintf(buf1,"[s10]尾白");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向90度方向  
                    turn_left_180_with_6050_check(270.0);
                    car_stop();
                    //
                    


                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              准备前往o点，并且轴心停在o点，然后右转90度
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
                                sprintf(buf1,"[s10]头黑");
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
                            //直行100毫米，轴心重合
                            if(pwm_num_distance>=49)
                            {
                                walk_ae_state=2;
                                sprintf(buf1,"[s10]49到");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                car_stop();
                                break;
                                
                            }
                            
                        }
                        

                        
                    }
                    //    //原地右转90度，指向0度方向  
                    turn_left_90_with_6050_check(0.0);
                    car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，原地调头180
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    //原地调头180度，指向180度方向  
                    turn_left_180_with_6050_check(180.0);    
                    
                     car_stop();
                       //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
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
                        
                    //    //原地左转90度，指向0度方向 
                        turn_left_90_with_6050_check(0.0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往2号区域，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
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
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        //物体放下完毕，原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                    //在2号区域，准备搬运下一个五块，  因此物料索引加1  
                    grasp_order_index++;
                    count_point_a--;
                        

                        
             }
                    


             /////////////////////////////////
            //////////////////////////////////////                
            //b临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////     
                    else if(colour_tempb==other)
                    {
                        colour_tempb=blue;
                        count_stack_tempb=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///             直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                        
                    //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                    //    //原地右转90度，指向180度方向 
                        turn_right_90_with_6050_check(180.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                        
                        count_point_a--;
                        
                        
                    }
                    
                    
                    
                    
                    
                    
                    
  
                    
 
                      /////////////////////////////////
            //////////////////////////////////////                
            //c临时存放点可以存放 ////////////////////////////////
            /////////////////////////////////
            //////////////////////////////////////   
                    else
                    {
                        
                        
                        colour_tempc=blue;
                        count_stack_tempc=0x00;
                        
                             
                    //    //原地右转90度，指向0度方向 //倒车写的方便，可视倒车运动还没写，所以暂时只有前进
                        turn_left_90_with_6050_check(0);
                        
                          car_stop();   
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///            直行直到头黑，
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    }
                    
                    
                      car_stop();       
                        //原地调头180度，，指向180度方向  
                    turn_left_180_with_6050_check(180.0);
                    car_stop();
                    //
                    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              直行10毫米，避开黑区
    /////////////////////////// /////////////////////////// 
    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
                     pwm_num_distance=0;  
                    
                    while(1)
                    {
                        car_position_test_all_white();
                        
                        
                        car_judge_decision();
                        car_excute_decision();
                        //直行10毫米，避开黑区
                        if(pwm_num_distance>=11)
                        {
                            walk_ae_state=2;
                            sprintf(buf1,"[s10]2号区域");
                            speak_len1=strlen(( const char *)buf1);
                            speak_context((u8*)buf1,speak_len1);
                            
                            break;
                            
                        }
                       
                    }
                      car_stop();
                   //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///               准备前往ae直线，直行直到头黑，放黄色 色块
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
                                
                                sprintf(buf1,"[s10]头黑");
                                speak_len1=strlen(( const char *)buf1);
                                speak_context((u8*)buf1,speak_len1);
                                 car_stop();
                                break;
                                
                            }
                            
                            first_start_state_1=0x00;
                            
                        }
                        
                        
                    } 
                        
                    car_stop();   
                    
                 //    //原地右转90度，指向180度方向 
                    turn_right_90_with_6050_check(180.0);

                          car_stop();

//////////////////////////////////////////////////////
    //////////////////////////////////////////////////////                    
    //////////////////////////////////////////////////////
     ///              //开始准备放物块 //小车就位，准备放物块
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
                        
                        
                  //    //原地左转90度，指向279度方向 
                        turn_left_90_with_6050_check(270.0);
                        
                          car_stop();  
                    
                      grasp_order_index++;
                       count_point_a--;  

                        
                    }
                 

                
                
          
                }
              
            
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
                
            
            }
                
        }
        