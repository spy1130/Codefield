#include "./BSP/SDRAM/sdram.h"
#include "./SYSTEM/delay/delay.h"

SDRAM_HandleTypeDef g_sdram_handle;     /* SDRAM句柄 */

void sdram_init(void)
{
    FMC_SDRAM_TimingTypeDef sdram_timing;
    
    g_sdram_handle.Instance                 = FMC_SDRAM_DEVICE;                     /* SDRAM存储器设备 */
    
    g_sdram_handle.Init.SDBank              = FMC_SDRAM_BANK1;                      /* SDRAM BANK1 */
    g_sdram_handle.Init.ColumnBitsNumber    = FMC_SDRAM_COLUMN_BITS_NUM_9;          /* 列数量 9 */
    g_sdram_handle.Init.RowBitsNumber       = FMC_SDRAM_ROW_BITS_NUM_13;            /* 行数量 13 */
    g_sdram_handle.Init.MemoryDataWidth     = FMC_SDRAM_MEM_BUS_WIDTH_16;           /* 数据宽度 16 */
    g_sdram_handle.Init.InternalBankNumber  = FMC_SDRAM_INTERN_BANKS_NUM_4;         /* SDRAM BANK数量 4 */
    g_sdram_handle.Init.CASLatency          = FMC_SDRAM_CAS_LATENCY_3;              /* CAS潜伏期时间 3 */
    g_sdram_handle.Init.WriteProtection     = FMC_SDRAM_WRITE_PROTECTION_DISABLE;   /* 失能写保护 */
    g_sdram_handle.Init.SDClockPeriod       = FMC_SDRAM_CLOCK_PERIOD_2;             /* SDRAM时钟 HCLK/2 = 90MHz */ 
    g_sdram_handle.Init.ReadBurst           = FMC_SDRAM_RBURST_ENABLE;              /* 突发使能 */
    g_sdram_handle.Init.ReadPipeDelay       = FMC_SDRAM_RPIPE_DELAY_1;              /* 读通道延时 */
    
    sdram_timing.LoadToActiveDelay      = 2;    /* 加载模式寄存器的命令与激活命令或刷新命令之间的延迟 */
    sdram_timing.ExitSelfRefreshDelay   = 8;    /* 发出刷新命令到发出激活命令之间的延迟 */
    sdram_timing.SelfRefreshTime        = 6;    /* 自刷新周期 */
    sdram_timing.RowCycleDelay          = 6;    /* 刷新命令和激活命令之间的延迟 */
    sdram_timing.WriteRecoveryTime      = 2;    /* 写命令和预充电命令之间的延迟 */
    sdram_timing.RPDelay                = 2;    /* 预充电命令与其他命令之间的延迟 */
    sdram_timing.RCDDelay               = 2;    /* 激活命令与读/写命令之间的延迟 */
    
    HAL_SDRAM_Init(&g_sdram_handle, &sdram_timing);     /* 初始化SDRAM */
    
    /* 发送SDRAM初始化序列 */
    sdram_initialization_sequence();
    
    /* 设置刷新频率 */
    HAL_SDRAM_ProgramRefreshRate(&g_sdram_handle, 683);
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    GPIO_InitTypeDef gpio_init_struct;

    /* 使能相关时钟 */
    __HAL_RCC_FMC_CLK_ENABLE();     /* 使能FMC时钟 */
    __HAL_RCC_GPIOC_CLK_ENABLE();   /* 使能GPIOC时钟 */
    __HAL_RCC_GPIOD_CLK_ENABLE();   /* 使能GPIOD时钟 */
    __HAL_RCC_GPIOE_CLK_ENABLE();   /* 使能GPIOE时钟 */
    __HAL_RCC_GPIOF_CLK_ENABLE();   /* 使能GPIOF时钟 */
    __HAL_RCC_GPIOG_CLK_ENABLE();   /* 使能GPIOG时钟 */
    
    /* 相关GPIO口初始化 */
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;        /* 推挽复用 */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;       /* 高速 */
    gpio_init_struct.Alternate = GPIO_AF12_FMC;     /* 复用为FMC */
    HAL_GPIO_Init(GPIOC, &gpio_init_struct);        /* 初始化PC0,2,3 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio_init_struct);        /* 初始化PD0,1,8,9,10,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);        /* 初始化PE0,1,7,8,9,10,11,12,13,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOF, &gpio_init_struct);        /* 初始化PF0,1,2,3,4,5,11,12,13,14,15 */
    
    gpio_init_struct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;              
    HAL_GPIO_Init(GPIOG, &gpio_init_struct);        /* 初始化PG0,1,2,4,5,8,15 */
}

/**
 * @brief       向SDRAM发送命令
 * @param       bankx:0,向BANK5上面的SDRAM发送指令
 * @param             1,向BANK6上面的SDRAM发送指令
 * @param       cmd:指令(0,正常模式/1,时钟配置使能/2,预充电所有存储区/3,自动刷新/4,加载模式寄存器/5,自刷新/6,掉电)
 * @param       refresh:自刷新次数
 * @param       regval:对模式寄存器配置值
 * @retval      返回值:0,正常;1,失败.
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

    command.CommandMode             = cmd;          /* 命令 */
    command.CommandTarget           = target_bank;  /* 目标SDRAM存储区域 */
    command.AutoRefreshNumber       = refresh;      /* 自刷新次数 */
    command.ModeRegisterDefinition  = regval;       /* 要写入模式寄存器的值 */
    
    if (HAL_SDRAM_SendCommand(&g_sdram_handle, &command, 0x1000) == HAL_OK)     /* 向SDRAM发送命令 */
    {
        return 0;
    }
    return 1;
}

/* 发送SDRAM初始化序列 */
void sdram_initialization_sequence(void)
{
    uint32_t mode_reg = 0;
    
    /* 时钟配置使能 */
    sdram_send_cmd(0, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0);
    
    /* 延迟，至少200us */
    delay_us(500);
    
    /* 所有BANK预充电 */
    sdram_send_cmd(0, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 1, 0);

    /* 设置自刷新次数 */
    sdram_send_cmd(0, FMC_SDRAM_CMD_SELFREFRESH_MODE, 8, 0);

    /* 设置模式寄存器 */
    mode_reg = (uint32_t) SDRAM_MODEREG_BURST_LENGTH_1          |   /* 设置突发长度:1(可以是1/2/4/8) */
                          SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   /* 设置突发类型:连续(可以是连续/交错) */
                          SDRAM_MODEREG_CAS_LATENCY_3           |   /* 设置CAS值:3(可以是2/3) */
                          SDRAM_MODEREG_OPERATING_MODE_STANDARD |   /* 设置操作模式:0,标准模式 */
                          SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     /* 设置突发写模式:1,单点访问 */
    sdram_send_cmd(0, FMC_SDRAM_CMD_LOAD_MODE, 1, mode_reg);
}

