#include "./BSP/RNG/rng.h"
#include "./SYSTEM/delay/delay.h"

RNG_HandleTypeDef g_rng_handle;

/* return 0成功 1失败 */
uint8_t rng_init(void)
{
    uint16_t retry = 0;
    
    g_rng_handle.Instance = RNG;
    HAL_RNG_Init(&g_rng_handle);
    
    while ((__HAL_RNG_GET_FLAG(&g_rng_handle, RNG_FLAG_DRDY) == RESET) && (retry < 10000)) /* 等待RNG准备就绪 */
    {
        retry++;
        delay_us(10);
    }
    
    if (retry >= 10000)
    {
        return 1; /* 随机数产生器工作不正常 */
    }
    
    return 0;
    
}

void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
    /* RNG时钟源选择为PLL */
    RCC_PeriphCLKInitTypeDef rcc_periphclk_init_struct;
    
    /* RNG时钟使能 */
    __HAL_RCC_RNG_CLK_ENABLE();
    
    rcc_periphclk_init_struct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
    rcc_periphclk_init_struct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
    HAL_RCCEx_PeriphCLKConfig(&rcc_periphclk_init_struct);
}

uint32_t rng_get_random_num(void)
{
    uint32_t random_num = 0;
    HAL_RNG_GenerateRandomNumber(&g_rng_handle, &random_num);
    return random_num;
}

uint32_t rng_get_random_range(int min, int max)
{
    uint32_t random_num = 0;
    HAL_RNG_GenerateRandomNumber(&g_rng_handle, &random_num);
    return random_num % (max - min + 1) + min;
}

