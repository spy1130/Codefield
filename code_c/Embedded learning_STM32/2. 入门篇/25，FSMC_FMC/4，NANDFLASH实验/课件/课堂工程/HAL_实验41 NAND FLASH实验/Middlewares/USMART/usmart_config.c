#include "./USMART/usmart.h"
#include "./USMART/usmart_str.h"

/******************************************************************************************/
/* 用户配置区
 * 这下面要包含所用到的函数所申明的头文件(用户自己添加)
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/NAND/ftl.h"
#include "./BSP/NAND/nand.h"
#include "./BSP/NAND/nandtester.h"


/* 函数名列表初始化(用户自己添加)
 * 用户直接在这里输入要执行的函数名及其查找串
 */
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS == 1      /* 如果使能了读写操作 */
    (void *)read_addr, "uint32_t read_addr(uint32_t addr)",
    (void *)write_addr, "void write_addr(uint32_t addr, uint32_t val)",
#endif
    (void *)delay_ms, "void delay_ms(uint16_t nms)",
    (void *)delay_us, "void delay_us(uint32_t nus)",
    
    (void *)nand_modeset, "uint8_t nand_modeset(uint8_t mode)",
    (void *)nand_eraseblock, "uint8_t nand_eraseblock(uint32_t blocknum)",
    (void *)nand_erasechip, "void nand_erasechip(void)",
    (void *)nand_copypage_withoutwrite, "uint8_t nand_copypage_withoutwrite(uint32_t source_pagenum, uint32_t dest_pagenum)",

    (void *)test_copypageandwrite, "uint8_t test_copypageandwrite(uint32_t spnum, uint32_t dpnum, uint16_t colnum, uint16_t writebytes)",
    (void *)test_readpage, "uint8_t test_readpage(uint32_t pagenum, uint16_t colnum, uint16_t readbytes)",
    (void *)test_writepage, "uint8_t test_writepage(uint32_t pagenum, uint16_t colnum, uint16_t writebytes)",
    (void *)test_readspare, "uint8_t test_readspare(uint32_t pagenum, uint16_t colnum, uint16_t readbytes)",
    (void *)test_readallblockinfo, "void test_readallblockinfo(uint32_t sblock)",
    (void *)test_ftlwritesectors, "uint8_t test_ftlwritesectors(uint32_t secx, uint16_t secsize, uint16_t seccnt)",
    (void *)test_ftlreadsectors, "uint8_t test_ftlreadsectors(uint32_t secx, uint16_t secsize, uint16_t seccnt)",

    (void *)ftl_init, "uint8_t ftl_init(void)",
    (void *)ftl_check_badblock, "uint8_t ftl_check_badblock(uint32_t blocknum)",
    (void *)ftl_used_blockmark, "uint8_t ftl_used_blockmark(uint32_t blocknum)",
    (void *)ftl_find_unused_block, "uint32_t ftl_find_unused_block(uint32_t sblock, uint8_t flag)",
    (void *)ftl_find_same_plane_unused_block, "uint32_t ftl_find_same_plane_unused_block(uint32_t blocknum)",
    (void *)ftl_lbn_to_pbn, "uint16_t ftl_lbn_to_pbn(uint32_t blocknum)",
    (void *)ftl_create_lut, "uint8_t ftl_create_lut(void)",
    (void *)ftl_format, "uint8_t ftl_format(void)",
};


/******************************************************************************************/

/* 函数控制管理器初始化
 * 得到各个受控函数的名字
 * 得到函数总数量
 */
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), /* 函数数量 */
    0,      /* 参数数量 */
    0,      /* 函数ID */
    1,      /* 参数显示类型,0,10进制;1,16进制 */
    0,      /* 参数类型.bitx:,0,数字;1,字符串 */
    0,      /* 每个参数的长度暂存表,需要MAX_PARM个0初始化 */
    0,      /* 函数的参数,需要PARM_LEN个0初始化 */
};



















