# include "STC15F2K60S2.H"


//函数名：“字符串”到“数码管段码”的转换函数
//入口参数：seg_string待转换的字符串；seg_buf（8个字节）将转换完的段码存到BUF里
//返回值：无
//函数功能：将seg_string的各个位转换为数码管段码，存储到seg_buf数组中。
void Seg_Tran(unsigned char *seg_string, unsigned char *seg_buf);


//函数名：将“段码数组”的第N位显示。
//入口参数：seg_buf（8个字节）是转换完的段码; pos待显示的seg_buf第pos位。
//返回值：无
//函数功能：将seg_buf的第pos位显示在数码管的第pos位
void Seg_Disp(unsigned char *seg_buf, unsigned char pos);
