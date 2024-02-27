#ifndef __RUN_H
#define __RUN_H

#include "delay.h"
#include "cfg.h"
#include "stdbool.h"
#include "ult.h"

#define leftservo    GPIO_Pin_6
#define rightservo   GPIO_Pin_7

/*���ļ�Ϊ�����ֵ�������к��������������ֵ����ж���*/
//ע�������ֵ���������һ��������ͬ������������ת����ת�ĽǶ��ǲ�һ����
//��ֱǰ�����ˣ������ֵĸߵ�ƽʱ��Ҳ������ͬ



void PulseOut(uint8_t pin,int speed);


void tiaoling(void);	//����

void fastforward_xm(void);

void turn_left_90_with_qti2_3_1(void);
void turn_left_90_with_qti2_3_2(void);
void walk_alone_MiddleQti(void);

void turn_right_circle(void);
void turn_left_circle(void);


void turn_right_circle_light(void);
void turn_left_circle_light(void);
void turn_right_180(void)	;


void fastforward(void);//����ǰ��

void slowforward(void);//����ǰ��


void left_turn(void);//��ת


void right_turn(void); //��ת


void GoBack(void);//������


void stop(void);//ֹͣ

void car_stop(void);

void turn_left_90(void);
	
void turn_right_90(void);
void turn_right_90_1(void)	;
void turn_right_90_qti2_3(void);

void SpinLeft(void);// ����ת


void SpinRight(void);// ����ת

void TurnLeftAnyPulse(int pulses);//����תĳ������

void TurnRightAnyPulse(int pulses); //����תĳ������


void Crossblk(int pulses);//����ǰ����������


bool IsTailQtiBlack(void);//�����������



bool IsMLeftQtiBlack(void);

bool IsMRightQtiBlack(void);


bool IsELeftQtiBlack(void);

/*
 * //���������QTI�Ƿ��ں�����
 */
bool IsERightQtiBlack(void);

/*
 * ǰ���м�QTIѭ��
 */
bool MoveMiddleQti(void);

bool MoveMiddleQti_qti7_stop(void);

/*
 * ǰ���м�QTIѭ��
 */
bool MoveSlowQti(void);


bool MoveSlowQti1(void);

/*
 * ǰ���м�QTIѭ��
 */
bool MoveQti(void);

bool MoveQti1(void);



/*
 * ���Ӹ���QTIѭ��
 */
bool MoveEdgeQti(void);


void Turn180(void);//��ͷ



void OCtoOA(void);  //�ж�ת��ʱ���Լ�ת���Ƿ�λ

void OCtoOE(void);


void OCtoOD(void);

void ODtoOE(void);

void OCtoOB(void);

void OCtoOB1(void);

void OBtoOA(void);

void OEtoOD(void);

void OEtoOA(void);

void ODtoOC(void);


void FirstOtoMarkArea(void);//��һ�ε�Ŀ������ ��û�п���Ŀ������

void AOtoStartLine(void);


void BOtoStartLine(void);


void COtoStartLine(void);


void DOtoStartLine(void);




void EOtoStartLine(void);


void car_action_judge(void);

bool car_walk_to_middle(void);

void car_left_turn_big(void);

void car_right_turn_big(void);

void car_left_turn(void);

void car_right_turn(void);

void car_show_gy25(void);

void left_turn(void);//��ת

void right_turn(void);

void car_position_test(void);

void car_excute_decision_1(void);

void car_excute_decision(void);

void car_excute_decision_back_up(void);

void car_special_run_backup_car(void);

void car_special_run_forward_car(void);

void car_judge_decision(void);
void car_judge_decision_back_up(void);

void car_position_test_tail(void);

void car_position_test_head(void);

void car_detec_position_decision_action(void);

void turn_left_90_with_axis_recover_6050_check(float des);
void turn_left_90_with_6050_check(float des);
void turn_left_180_with_6050_check(float des);
void turn_right_90_with_6050_check(float des);

void turn_right_135(void);

void car_from_star_to_2(void);

void car_from_2_to_ae(void);

void car_point_a_pull_up_down(void);
	
void car_point_b_pull_up_down(void);
	
void car_point_c_pull_up_down(void);

void car_pick_up_a_ponit_1_top(void);

void car_pick_up_a_ponit_2_middle(void);

void car_pick_up_a_ponit_3_bottle(void);

void car_yellow_pull_down_block(void);
void car_yellow_not_pull_down_block(void);

void car_position_test_head_all_white(void);

void car_position_test_head_all_white(void);

void car_position_test_all_white(void);

void  car_pull_down_top(void);

void  car_pull_down_bottle(void);

void  car_pull_down_middle(void);

void car_axis_recover(void);

void car_axis_recover_d_point(void);
void turn_left_45(void);

void turn_right_45(void);

void mpu_6050_check(float des);

void car_c_ponit_yellow(void);

void car_c_point_white(void);

void car_c_point_red(void);

void car_c_point_black(void);
void car_c_point_blue(void);

void car_a_point_yellow(void);

void car_a_point_white(void);
void car_a_point_red(void);

void car_a_point_black(void);
void car_a_point_blue(void);

void car_e_point_yellow(void);

void car_e_point_white(void);

void car_e_point_red(void);

void car_e_point_black(void);

void car_e_point_blue(void);

void car_o_point_yellow(void);

void car_o_point_white(void);

void car_o_point_red(void);

void car_o_point_black(void);

void car_o_point_blue(void);

void mpu_6050_white(void);

void  car_o_point_to_temp_a(void);
void  car_o_point_to_temp_b(void);
void  car_o_point_to_temp_c(void);

void  car_o_point_to_temp_bo(void);
void  car_o_point_to_temp_co(void);

void car_o_point_to_zero_2(void);

void turn_left_180(void)	;

void turn_left_10(void)	;
void turn_right_10(void);

#endif


