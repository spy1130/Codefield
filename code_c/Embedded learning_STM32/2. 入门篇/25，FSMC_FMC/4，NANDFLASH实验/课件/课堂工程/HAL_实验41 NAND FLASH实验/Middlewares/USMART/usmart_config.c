#include "./USMART/usmart.h"
#include "./USMART/usmart_str.h"

/******************************************************************************************/
/* �û�������
 * ������Ҫ�������õ��ĺ�����������ͷ�ļ�(�û��Լ����)
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/NAND/ftl.h"
#include "./BSP/NAND/nand.h"
#include "./BSP/NAND/nandtester.h"


/* �������б��ʼ��(�û��Լ����)
 * �û�ֱ������������Ҫִ�еĺ�����������Ҵ�
 */
struct _m_usmart_nametab usmart_nametab[] =
{
#if USMART_USE_WRFUNS == 1      /* ���ʹ���˶�д���� */
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

/* �������ƹ�������ʼ��
 * �õ������ܿغ���������
 * �õ�����������
 */
struct _m_usmart_dev usmart_dev =
{
    usmart_nametab,
    usmart_init,
    usmart_cmd_rec,
    usmart_exe,
    usmart_scan,
    sizeof(usmart_nametab) / sizeof(struct _m_usmart_nametab), /* �������� */
    0,      /* �������� */
    0,      /* ����ID */
    1,      /* ������ʾ����,0,10����;1,16���� */
    0,      /* ��������.bitx:,0,����;1,�ַ��� */
    0,      /* ÿ�������ĳ����ݴ��,��ҪMAX_PARM��0��ʼ�� */
    0,      /* �����Ĳ���,��ҪPARM_LEN��0��ʼ�� */
};



















