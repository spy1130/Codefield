# include "STC15F2K60S2.H"


//�����������ַ�������������ܶ��롱��ת������
//��ڲ�����seg_string��ת�����ַ�����seg_buf��8���ֽڣ���ת����Ķ���浽BUF��
//����ֵ����
//�������ܣ���seg_string�ĸ���λת��Ϊ����ܶ��룬�洢��seg_buf�����С�
void Seg_Tran(unsigned char *seg_string, unsigned char *seg_buf);


//�������������������顱�ĵ�Nλ��ʾ��
//��ڲ�����seg_buf��8���ֽڣ���ת����Ķ���; pos����ʾ��seg_buf��posλ��
//����ֵ����
//�������ܣ���seg_buf�ĵ�posλ��ʾ������ܵĵ�posλ
void Seg_Disp(unsigned char *seg_buf, unsigned char pos);
