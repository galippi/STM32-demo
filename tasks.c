#include <string.h>
#include <stdint.h>

#include "stm32c5xx_hal_i2c.h"

#include "eeprom_emul_core.h"

#include "timer.h"

#if 0

#include "SysClock.h"
#include "debug.h"
#include "scheduler_preemptive.h"
#include "version.h"
#include "u32_to_hexstring.h"
#include "adc_app.h"
#include "dma.h"

#ifdef f_USBCLK_Hz
#include "usbd_conf.h"
#endif

#endif

#include "gpio_app.h"
#include "FaultHandler.h"
#include "uart.h"
#include "queue.h"
#include "timer_app.h"
#include "u32_to_hexstring.h"
#include "adc.h"
#include "adc_app.h"
#include "util.h"

#include "ssd1306.h"

#include "tasks.h"

uint8_t uart1RxBuffer[128];
//uint8_t uart1TxBuffer[128];
QUEUE_CREATE(uart1TxQueue, 127);
char uart1TxOverflowCtr1;

QUEUE_CREATE(uart2TxQueue, 127);
QUEUE_CREATE(uart2RxQueue, 127);

hal_flash_handle_t flash;
hal_crc_handle_t crc;

static const ee_object_t object = {
    .f_object = &flash,
    .crc_object = &crc,
};

hal_status_t flashEccCallback(void)
{
    return flash.p_ecc_error_cb(&flash, 0);
}

volatile ee_status eeState;

static uint8_t startCtr;

uint32_t SystemCoreClock;
const uint8_t  AHBPrescTable[16];    /*!< AHB prescalers table values */
const uint8_t  APBPrescTable[8];     /*!< APB prescalers table values */

hal_i2c_handle_t ssd1306_i2c;
#define SSD1306_I2C I2C1

/* Write command */
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))

static void HAL_I2C_ErrorHandler(void)
{

}

hal_status_t HAL_I2C_IsDeviceReady(hal_i2c_handle_t *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);
/**
  * @brief  Checks if target device is ready for communication.
  * @note   This function is used with Memory devices
  * @param  hi2c Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @param  DevAddress Target device address: The device 7 bits address value
  *         in datasheet must be shifted to the left before calling the interface
  * @param  Trials Number of trials
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
hal_status_t HAL_I2C_IsDeviceReady(hal_i2c_handle_t *hi2c, uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
    //BitfieldSet(SSD1306_I2C->CR2, I2C_CR2_SADD_Pos, 10, DevAddress);
    //SSD1306_I2C->CR2 |= I2C_CR2_START;
    //LL_I2C_HandleTransfer(SSD1306_I2C, DevAddress, 0, 0, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_STOP);
    (void)Trials;
    return HAL_I2C_MASTER_PollForSlaveReady(hi2c, DevAddress, Timeout);
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */
    //GPIO_PortInit_AFPU(PIN_SSD1306_SCL, 9); /* PA8  I2C1.SCL */
    GPIO_PortInit_AFPU(PIN_SSD1306_SCL); /* PB6  I2C1.SCL */
    //GPIO_PortInit_AFOut(PIN_SSD1306_SCL, 9); /* PA8  I2C1.SCL */
    //GPIO_PortInit_AFPU(PIN_SSD1306_SDA, 8); /* PA12 I2C1.SDA */
    GPIO_PortInit_AFPU(PIN_SSD1306_SDA); /* PB7 I2C1.SDA */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  RCC->APB1LENR |= RCC_APB1LENR_I2C1EN ;
  //#define I2C1SEL (RCC_CCIPR2_I2C1SEL_0 | RCC_CCIPR2_I2C1SEL_1)
  #define I2C1SEL (LL_RCC_I2C1_CLKSOURCE_PCLK1 >> RCC_CCIPR2_I2C1SEL_Pos)
  BitfieldSet(RCC->CCIPR2, RCC_CCIPR2_I2C1SEL_Pos, 2, I2C1SEL);
  if (HAL_I2C_Init(&ssd1306_i2c, HAL_I2C1) != HAL_OK)
      HAL_I2C_ErrorHandler();
  //ssd1306_i2c.Init.ClockSpeed = 400000;
  //ssd1306_i2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
  //LL_I2C_SetTiming(SSD1306_I2C, LL_I2C_CONVERT_TIMINGS(0, 10, 4, 15, 32));
  //ssd1306_i2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  //LL_I2C_SetMasterAddressingMode(SSD1306_I2C, LL_I2C_ADDRESSING_MODE_7BIT);
  //ssd1306_i2c.Init.OwnAddress1 = 0;
  //ssd1306_i2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  //ssd1306_i2c.Init.OwnAddress2 = 0;
  //ssd1306_i2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  //ssd1306_i2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  //I2C1->CR1 = I2C_CR1_SWRST;
  RCC->APB1LRSTR |= RCC_APB1LRSTR_I2C1RST;
  //I2C1->CR1 &= ~I2C_CR1_SWRST;
  RCC->APB1LRSTR &= ~RCC_APB1LRSTR_I2C1RST;

  static const hal_i2c_config_t ssd1306_config = {
          //.timing = LL_I2C_CONVERT_TIMINGS(0, 10, 4, 15, 32),
          //.timing = LL_I2C_CONVERT_TIMINGS(12, 2, 2, 8, 8),
          .timing = LL_I2C_CONVERT_TIMINGS(0, 2, 1, 12, 42),
          .own_address1 = 1,
          .addressing_mode = LL_I2C_ADDRESSING_MODE_7BIT,
  };
  HAL_I2C_SetConfig(&ssd1306_i2c, &ssd1306_config);
  LL_I2C_Enable(SSD1306_I2C);

  /* USER CODE BEGIN I2C1_Init 2 */
  if (SSD1306_Init() != 1) {
      CAT_Error(CAT_NoDisplay, SSD1306_I2C_ADDR);
  }
  SSD1306_ON();
  SSD1306_UpdateScreen();
  SSD1306_Fill(SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  SSD1306_Fill(SSD1306_COLOR_BLACK);
  SSD1306_UpdateScreen();
  SSD1306_DrawCircle(32, 32, 16, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  SSD1306_GotoXY(0,0);
  SSD1306_Puts("Hello", &Font_7x10, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  SSD1306_GotoXY(0, 12);
  SSD1306_Puts("World", &Font_16x26, SSD1306_COLOR_WHITE);
  SSD1306_UpdateScreen();
  /* USER CODE END I2C1_Init 2 */
}

void Task_Init(void)
{
    SCB_EnableFPU();
    HAL_FLASH_Init(&flash, HAL_FLASH);
    HAL_CRC_Init(&crc, HAL_CRC);
    EE_Init(&object, EE_CONDITIONAL_ERASE);

    {
        eeState = EE_ReadVariable8bits(1, &startCtr);
        startCtr++;
        eeState = EE_WriteVariable8bits(1, startCtr);
    }

    {
        MX_I2C1_Init();
    }

    queueInit(&uart1TxQueue);
    UART1_Init(9600);

    queueInit(&uart2TxQueue);
    queueInit(&uart2RxQueue);
    UART2_Init(115200);

    LEDR_Init();
    LEDY_Init();
    LEDG_Init();

    TIM12_Init();
    GPIO_PortInit_AFOut(PIN_PWM_DEMO); /* PC14 TIM12.CH1 */

    ADC_Init();
    GPIO_PortInit_Analog(PIN_AN0);
    GPIO_PortInit_Analog(PIN_AN1);
    GPIO_PortInit_Analog(PIN_AN6);
    GPIO_PortInit_Analog(PIN_AN7);

#if 0
    DMA_Init(DMA1);


    TIM3_Init();

    ADC_HandlerInit();

    #ifdef f_USBCLK_Hz
    USB_task_init();
    #endif
#endif
}

static void UART_TX(const void *ptr, uint8_t len)
{
#if 0
    uint32_t written = queueWrite(&uart1TxQueue, ptr, len);
    if (written != len)
        uart1TxOverflowCtr1++;
#else
    static char usart1AbrfSentState = 0;
    if (usart1AbrfSentState == 0)
    {
        if (USART1->ISR & USART_ISR_ABRF_Msk)
        {
            usart1AbrfSentState = 1;
            uint8_t msg[] = {'\n', '\r', 'A', '?', '?', '\n', '\r'};
            msg[3] = ((USART1->ISR & USART_ISR_ABRF_Msk) ? '1' : '0');
            msg[4] = ((USART1->ISR & USART_ISR_ABRE_Msk) ? '1' : '0');
            UART_TX(msg, sizeof(msg));
        }else{
            uint8_t c = 'U';
            UART1_TX_Queue(&c, 1);
        }
    }else{
        UART1_TX_Queue(ptr, len);
    }
#endif
}

uint8_t UART1_TxOverrun;

typedef float float32_t;

volatile float32_t v1 = 8.888;

int errCtr;

void Task_Bgnd(void)
{
#if UART1_DMA == 0
    uint8_t data;
    char len = queueRead(&uart1TxQueue, &data, 1);
    if (len)
    {
        UART1_TX(&data, 1);
    }
#endif
    {
        uint32_t t = TIM2_Cnt_Get();
        float32_t temp = v1;
        while((TIM2_Cnt_Get() - t) < 2200) {
        }
        if (fabsf(v1 - temp) > 0.001f)
            errCtr++;
        while((TIM2_Cnt_Get() - t) < 22000) {
        }
        if (fabsf(v1 - temp) > 0.001f)
            errCtr++;
    }
}

void Task_1ms(void)
{
#ifdef f_USBCLK_Hz
  USB_task_1ms();
#endif
  /*PB13_Set(!PB13_Get());*/ /* toggling debug port */
  {
    static uint16_t t_ug;
    if (t_ug < 500)
    {
        t_ug++;
    }
    volatile float32_t var = 3.0f;
    var++;
    var--;
    var = (var * 2.2f) / 4.5f;
  }
  UART2_TxDma_Poll();
  UART2_RxDma_Poll();
}

uint32_t HAL_GetTick(void)
{
    static uint32_t tickLast; // upper 16 bits: offset, lower 16 bits: counter
    uint32_t  tick = getTimer_us() / 1000;
    uint32_t  tickLow = tick & 0x0000FFFF;
    if (tickLow < (tickLast & 0x0000FFFF)) {
        tickLast += 0x10000; // overflow had happen
    }
    uint32_t offset = tickLast & 0xFFFF0000;
    uint32_t halTick_1ms = offset + tickLow;
    tickLast = halTick_1ms;
    return halTick_1ms;
}

void HAL_Delay(uint32_t delay)
{
    const uint32_t tStart = HAL_GetTick();
    const uint32_t tEnd = tStart + delay;
    while (HAL_GetTick() < tEnd) {
        // do nothing
    }
}

uint32_t dt;
uint32_t t_last;
uint16_t dt12;
#define T12_SIZE 64
struct {
	uint16_t t0;
	uint16_t t1;
	uint16_t dt;
}dbgT12[T12_SIZE];

volatile float32_t v0 = 3.3333;

void Task_10ms(void)
{
	uint32_t t = TIM2_Cnt_Get();
	if (t_last != 0) {
		uint32_t dtCurrent = t - t_last;
		if (dt == 0)
			dt = dtCurrent;
		else {
#define DT_FILTER 8
			dt = ((dt * (DT_FILTER - 1)) + dtCurrent) / DT_FILTER;
		}
	}
	t_last = t;

    {
       float32_t temp = v0;
        __NOP();
        while((TIM2_Cnt_Get() - t) < 2200) {
        }
        __NOP();
        while(fabsf(v0 - temp) > 0.001f);
    }

	{
		t = TIM2_Cnt_Get();
		uint16_t t12 = TIM12_Cnt_Get();
		while((TIM2_Cnt_Get() - t) < ((1000000 / TIM12_FREQ) / 2)) {
		}
		uint16_t t12New = TIM12_Cnt_Get();
		uint16_t dtCurrent = (t12New << 8) - (t12 << 8);
		dt12 = ((dt12 * (DT_FILTER - 1)) + dtCurrent) / DT_FILTER;
		{
			static uint8_t t12Idx;
			dbgT12[t12Idx].t0 = t12;
			dbgT12[t12Idx].t1 = t12New;
			dbgT12[t12Idx].dt = dtCurrent;
			t12Idx++;
			if (t12Idx == T12_SIZE)
				t12Idx = 0;
		}
	}
	{
		static uint8_t pwm;
		TIMx_CCRy_Set(TIM12, 0, pwm);
		pwm++;
	}
}

typedef enum
{
  e_Pulse_ShortHigh,
  e_Pulse_ShortLow,
  e_Pulse_LongHigh,
  e_Pulse_LongLow,
}e_PulseType;

uint32_t uart2TxOverrun;

void Task_500ms(void)
{
	static const e_PulseType pulseConfig[] =
	{
		e_Pulse_LongLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_LongHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
		e_Pulse_ShortHigh, e_Pulse_ShortLow,
	};
	static uint8_t pulseIdx = 0;
	static uint8_t pulseTimer = 0;
	if (pulseTimer == 0)
	{
		if (pulseIdx < ((sizeof(pulseConfig)/sizeof(pulseConfig[0])) - 1))
		{
			pulseIdx++;
		}else
		{
			pulseIdx = 0;
		}
		switch(pulseConfig[pulseIdx])
		{
		case e_Pulse_ShortHigh:
		  LED3_Set(0);
		  pulseTimer = 0;
			break;
		case e_Pulse_ShortLow:
		  LED3_Set(1);
		  pulseTimer = 0;
			break;
		case e_Pulse_LongHigh:
		  LED3_Set(0);
		  pulseTimer = 2;
			break;
		case e_Pulse_LongLow:
		  LED3_Set(1);
		  pulseTimer = 2;
			break;
		default:
      CAT_Error(CAT_VarInvalidValue, 0);
      break;
		}
	}else
	{
		pulseTimer--;
	}
#if 0
    {
        static uint8_t rxIdx = 0;
        uint8_t len = UART1_RX(uart1RxBuffer + rxIdx, sizeof(uart1RxBuffer) - rxIdx);
        if (len > 0) {
            rxIdx = rxIdx + len;
            if (rxIdx >= sizeof(uart1RxBuffer))
                rxIdx  = 0;
        }
    }
    {
        char adcData[] = "ADCxxxxxxxxxxxxxxxx\r\n";
        U32_to_HexString(adcData +  3, 4, ADC_values_raw[0], '0');
        U32_to_HexString(adcData +  7, 4, ADC_values_raw[1], '0');
        U32_to_HexString(adcData + 11, 4, ADC_values_raw[2], '0');
        U32_to_HexString(adcData + 15, 4, ADC_values_raw[3], '0');
        UART_TX(adcData, sizeof(adcData) - 1);
    }
#endif

    {
        static uint8_t ctr;
        //LEDR_Set(ctr & 0x01);
        LEDY_Set(ctr & 0x02);
        LEDG_Set(ctr & 0x01);
        if (ctr < 3)
            ctr++;
        else
            ctr = 0;
    }
#if 1
    //UART2_TX_Char(UART2_RX_Char());
    {
        static uint8_t ctr;
    	char buf[10] = "Demo000**\n";
    	buf[4] = toHexDigit(startCtr & 0x0F);
        buf[5] = toHexDigit(ctr & 0x0F);
    	ctr++;
    	buf[6] = '0' + (uart2TxOverrun & 0x07);
        UART2_TX(buf, sizeof(buf));
    }
    {
        char buf[32] = "Rx:x ";
        uint8_t rxData[12];
        uint8_t len = queueRead(&uart2RxQueue, rxData, sizeof(rxData));
        buf[3] = toHexDigit(len);
        if (len > 0)
            memcpy(buf + 5, rxData, len);
        buf[5 + len] = '\n';
        UART2_TX(buf, 5 + len + 1);
    }
#else
    if (UART2_RXNE_Get())
        UART2_TX_Char(UART2_RX_Char());
    else
        UART2_TX_Char('U');
#endif

    {
        static uint8_t sec = 8;
        if (sec > 1) {
            if (sec <= 5) {
                SSD1306_Fill(SSD1306_COLOR_BLACK);
                SSD1306_DrawLine(0, 0, 127, sec * 12 + 3, SSD1306_COLOR_WHITE);
                SSD1306_DrawLine(0, 63 - sec * 12 , 127, 0, SSD1306_COLOR_WHITE);
                SSD1306_UpdateScreen();
            }
            sec--;
        }else
        {
        if (!sec) {
            static uint8_t ctr;
            static uint32_t dtFiltered;
            SSD1306_GotoXY(0, 38);
            char buf[16] = "t=? ??????";
            buf[2] = toHexDigit(ctr & 0x0F);
            U32_to_DecString(buf +  4, 6, dtFiltered, ' ');
            uint16_t t0 = getTimer_us();
            SSD1306_Puts(buf, &Font_11x18, SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
            uint16_t t1 = getTimer_us();
            uint16_t dt = t1 - t0;
            if (dtFiltered == 0)
                dtFiltered = dt;
            else
                dtFiltered = ((dtFiltered * 7) + dt) / 8;
            ctr++;
        }
        sec = 1 - sec;
        }
    }

}
