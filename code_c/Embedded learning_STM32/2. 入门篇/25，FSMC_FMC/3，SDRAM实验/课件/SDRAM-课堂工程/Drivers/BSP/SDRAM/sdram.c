#include "./BSP/SDRAM/sdram.h"
#include "./SYSTEM/delay/delay.h"

SDRAM_HandleTypeDef g_sdram_handle;     /* SDRAM��� */

void sdram_init(void)
{
    FMC_SDRAM_TimingTypeDef sdram_timing;
    
    g_sdram_handle.Instance                 = FMC_SDRAM_DEVICE;                     /* SDRAM�洢���豸 */
    
    g_sdram_handle.Init.SDBank              = FMC_SDRAM_BANK1;                      /* SDRAM BANK1 */
    g_sdram_handle.Init.ColumnBitsNumber    = FMC_SDRAM_COLUMN_BITS_NUM_9;          /* ������ 9 */
    g_sdram_handle.Init.RowBitsNumber       = FMC_SDRAM_ROW_BITS_NUM_13;            /* ������ 13 */
    g_sdram_handle.Init.MemoryDataWidth     = FMC_SDRAM_MEM_BUS_WIDTH_16;           /* ���ݿ�� 16 */
    g_sdram_handle.Init.InternalBankNumber  = FMC_SDRAM_INTERN_BANKS_NUM_4;         /* SDRAM BANK���� 4 */
    g_sdram_handle.Init.CASLatency          = FMC_SDRAM_CAS_LATENCY_3;              /* CASǱ����ʱ�� 3 */
    g_sdram_handle.Init.WriteProtection     = FMC_SDRAM_WRITE_PROTECTION_DISABLE;   /* ʧ��д���� */
    g_sdram_handle.Init.SDClockPeriod       = FMC_SDRAM_CLOCK_PERIOD_2;             /* SDRAMʱ�� HCLK/2 = 90MHz */ 
    g_sdram_handle.Init.ReadBurst           = FMC_SDRAM_RBURST_ENABLE;              /* ͻ��ʹ�� */
    g_sdram_handle.Init.ReadPipeDelay       = FMC_SDRAM_RPIPE_DELAY_1;              /* ��ͨ����ʱ */
    
    sdram_timing.LoadToActiveDelay      = 2;    /* ����ģʽ�Ĵ����������뼤�������ˢ������֮����ӳ� */
    sdram_timing.ExitSelfRefreshDelay   = 8;    /* ����ˢ�����������������֮����ӳ� */
    sdram_timing.SelfRefreshTime        = 6;    /* ��ˢ������ */
    sdram_timing.RowCycleDelay          = 6;    /* ˢ������ͼ�������֮����ӳ� */
    sdram_timing.WriteRecoveryTime      = 2;    /* д�����Ԥ�������֮����ӳ� */
    sdram_timing.RPDelay                = 2;    /* Ԥ�����������������֮����ӳ� */
    sdram_timing.RCDDelay               = 2;    /* �����������/д����֮����ӳ� */
    
    HAL_SDRAM_Init(&g_sdram_handle, &sdram_timing);     /* ��ʼ��SDRAM */
    
    /* ����SDRAM��ʼ������ */
    sdram_initialization_sequence();
    
    /* ����ˢ��Ƶ�� */
    HAL_SDRAM_ProgramRefreshRate(&g_sdram_handle, 683);
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef gpio_init_struct;

    /* ʹ�����ʱ�� */
    __HAL_RCC_FMC_CLK_ENABLE();     /* ʹ��FMCʱ�� */
    __HAL_RCC_GPIOC_CLK_ENABLE();   /* ʹ��GPIOCʱ�� */
    __HAL_RCC_GPIOD_CLK_ENABLE();   /* ʹ��GPIODʱ�� */
    __HAL_RCC_GPIOE_CLK_ENABLE();   /* ʹ��GPIOEʱ�� */
    __HAL_RCC_GPIOF_CLK_ENABLE();   /* ʹ��GPIOFʱ�� */
    __HAL_RCC_GPIOG_CLK_ENABLE();   /* ʹ��GPIOGʱ�� */
    
    /* ���GPIO�ڳ�ʼ�� */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;        /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;       /* ���� */
    gpio_init_struct.Alternate = GPIO_AF12_FMC;     /* ����ΪFMC */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);        /* ��ʼ��PC0,2,3 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);        /* ��ʼ��PD0,1,8,9,10,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);        /* ��ʼ��PE0,1,7,8,9,10,11,12,13,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);        /* ��ʼ��PF0,1,2,3,4,5,11,12,13,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);        /* ��ʼ��PG0,1,2,4,5,8,15 */
}

/**
 * @brief       ��SDRAM��������
 * @param       bankx:0,��BANK5�����SDRAM����ָ��
 * @param             1,��BANK6�����SDRAM����ָ��
 * @param       cmd:ָ��(0,����ģʽ/1,ʱ������ʹ��/2,Ԥ������д洢��/3,�Զ�ˢ��/4,����ģʽ�Ĵ���/5,��ˢ��/6,����)
 * @param       refresh:��ˢ�´���
 * @param       regval:��ģʽ�Ĵ�������ֵ
 * @retval      ����ֵ:0,����;1,ʧ��.
 */
uint8_t sdram_send_cmd(uint8_t bankx, uint8_t cmd, uint8_t refresh, uint16_t regval)
{
    uint32_t target_bank = 0;
    
    FMC_SDRAM_CommandTypeDef command;
    
    if (bankx)
    {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK2;
    }
    else
    {
        target_bank = FMC_SDRAM_CMD_TARGET_BANK1;
    }

    command.CommandMode             = cmd;          /* ���� */
    command.CommandTarget           = target_bank;  /* Ŀ��SDRAM�洢���� */
    command.AutoRefreshNumber       = refresh;      /* ��ˢ�´��� */
    command.ModeRegisterDefinition  = regval;       /* Ҫд��ģʽ�Ĵ�����ֵ */
    
    if (HAL_SDRAM_SendCommand(&g_sdram_handle, &command, 0x1000) == HAL_OK)     /* ��SDRAM�������� */
    {
        return 0;
    }
    return 1;
}

/* ����SDRAM��ʼ������ */
void sdram_initialization_sequence(void)
{
    uint32_t mode_reg = 0;
    
    /* ʱ������ʹ�� */
    sdram_send_cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
    
    /* �ӳ٣�����200us */
    delay_us(500);
    
    /* ����BANKԤ��� */
    sdram_send_cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 0);

    /* ������ˢ�´��� */
    sdram_send_cmd(0, FMC_SDRAM_CMD_SELFREFRESH_MODE, 8, 0);

    /* ����ģʽ�Ĵ��� */
    mode_reg = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1          |   /* ����ͻ������:1(������1/2/4/8) */
                          SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   /* ����ͻ������:����(����������/����) */
                          SDRAM_MODEREG_CAS_LATENCY_3           |   /* ����CASֵ:3(������2/3) */
                          SDRAM_MODEREG_OPERATING_MODE_STANDARD |   /* ���ò���ģʽ:0,��׼ģʽ */
                          SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     /* ����ͻ��дģʽ:1,������� */
    sdram_send_cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, mode_reg);
}

