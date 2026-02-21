#include <stdint.h>

#include "gpio.h"
#include "pwm.h"
#include "adc_app.h"
#include "u32_to_hexstring/u32_to_hexstring.h"
#include "host_comm.h"

#include "battery.h"

#include "debug.h"

typedef struct {
    int8_t errInt;
    t_BatteryCmd cmd;
    uint16_t val;
    uint16_t timeoutCtr;
    uint16_t timeoutVal;
}t_BatteryData;
t_BatteryData batteryData;

#define BatteryLoadTimer   TIM2
#define BatteryLoadChannel 3
#define BatteryPwmMax 255

static void batterySetCmd(t_BatteryCmd cmd, uint32_t val)
{
    batteryData.cmd = cmd;
    switch(batteryData.cmd) {
        case e_BatteryCmd_U:
            batteryData.timeoutCtr = batteryData.timeoutVal;
            batteryData.val = (uint16_t)((val * BatteryPwmMax) / 3300);
            PWM_Set(BatteryLoadTimer, BatteryLoadChannel, batteryData.val);
        break;
        case e_BatteryCmd_I:
            batteryData.timeoutCtr = batteryData.timeoutVal;
            batteryData.val = (uint16_t)(val);
            batteryData.errInt = 0;
        break;
        case e_BatteryCmd_PWM:
            if (val < BatteryPwmMax) {
                batteryData.timeoutCtr = batteryData.timeoutVal;
                batteryData.val = (uint16_t)(val);
                PWM_Set(BatteryLoadTimer, BatteryLoadChannel, batteryData.val);
            }else {
                batteryData.cmd = e_BatteryCmd_default;
                batteryData.timeoutCtr = 0;
            }
        break;
        case e_BatteryCmd_timeout:
            batteryData.timeoutVal = val;
        break;
        case e_BatteryCmd_default:
            batteryData.timeoutCtr = 0;
        break;
        default:
            batteryData.timeoutCtr = 0;
        break;
    }
}

static int8_t hexStr2uint(const char *str, uint32_t *resultPtr)
{
    uint32_t result = 0;
    if (*str == 0)
        return 1;
    do {
        char c = *str;
        str++;
        result = result << 4;
        if ((c >= '0') && (c <= '9'))
            result = result + c - '0';
        else
        if ((c >= 'A') && (c <= 'F'))
            result = result + c - 'A' + 10;
        else
        if ((c >= 'a') && (c <= 'f'))
            result = result + c - 'a' + 10;
        else
            return 2; // invalid character
    }while (*str != 0);
    *resultPtr = result;
    return 0;
}

static int8_t decStr2uint(const char *str, uint32_t *resultPtr)
{
    uint32_t result = 0;
    if (*str == 0)
        return 1;
    do {
        char c = *str;
        str++;
        result = result * 10;
        if ((c >= '0') && (c <= '9'))
            result = result + c - '0';
        else
            return 2; // invalid character
    }while (*str != 0);
    *resultPtr = result;
    return 0;
}

static void uartCmdProcess(const void *data, uint32_t len)
{
    char error = 0;
    const char *ptr = data;
    char cmd = *ptr;
    ptr++;
    switch (cmd) {
        case 'S':
            cmd = *ptr;
            ptr++;
            switch(cmd) {
                case 'U': { // SU - set voltage in mV
                    uint32_t val;
                    if (decStr2uint(ptr, &val) != 0) {
                        // invalid command
                    }else{ // valid command
                        if (val <= 3300) // 3.3V
                            batterySetCmd(e_BatteryCmd_U, val);
                        else
                            error = 1; // error
                    }
                }
                break;
                case 'I': { // SI - set current in mA
                    uint32_t val;
                    if (decStr2uint(ptr, &val) != 0) {
                        // invalid command
                    }else{ // valid command
                        if (val <= 3300)
                            batterySetCmd(e_BatteryCmd_I, val);
                        else
                            error = 1; // error
                    }
                }
                break;
                case 'R': { // SR - set raw value in hex
                    uint32_t val;
                    if (hexStr2uint(ptr, &val) != 0) {
                        // invalid command
                    }else{ // valid command
                        if (val <= 255)
                            batterySetCmd(e_BatteryCmd_PWM, val);
                        else
                            error = 1; // error
                    }
                }
                break;
                case 'T': { // ST - set timeout value in ms
                    uint32_t val;
                    if (decStr2uint(ptr, &val) != 0) {
                        // invalid command
                    }else{ // valid command
                        if (val <= 10000) // 10 sec
                            batterySetCmd(e_BatteryCmd_timeout, val);
                        else
                            error = 1; // error
                    }
                }
                break;
                default:
                    error = 1; // error
                    break;
            }
            break;
        default:
            error = 1; // error
            break;
    }
    if (error)
        batterySetCmd(e_BatteryCmd_default, 0);
    (void)len;
}

static void battery_cmdRx(void)
{
    uint8_t buf[32];
    static uint8_t rxCmdBuffer[32];
    static uint8_t bufIdx = 0;
    uint32_t num = hostRx(buf, sizeof(buf));
    for(uint32_t i = 0; i < num; i++)
    {
        DBG_INC(uart1_rxCtr);
        uint8_t c = buf[i];
        if ((c == '\r') || (c == '\n')) {
            if (bufIdx != 0) {
                rxCmdBuffer[bufIdx] = 0;
                uartCmdProcess(rxCmdBuffer, bufIdx);
                bufIdx = 0;
            }else{
                // empty line is received
            }
        }else{
            rxCmdBuffer[bufIdx++] = c;
            if (bufIdx == sizeof(rxCmdBuffer)) { // too long command is received
                bufIdx = 0; // reset buffer
            }
        }
    }
}

void battery_init(void)
{
    batteryData.cmd = e_BatteryCmd_default;
    batteryData.timeoutCtr = 0;
    batteryData.timeoutVal = 2000;
    hostInit();
    PWM_Init(BatteryLoadTimer, BatteryLoadChannel);
    GPIO_PortInit_AFOut(GPIOA, 3); /* PA3 PWM2/4 */
}

void battery_10ms(void)
{
    battery_cmdRx();
    if ((batteryData.timeoutCtr <= 10) || (batteryData.cmd == e_BatteryCmd_default)) {
        batteryData.timeoutCtr  = 0;
        batteryData.cmd = e_BatteryCmd_default;
        PWM_Set(BatteryLoadTimer, BatteryLoadChannel, 0);
    }else{
        batteryData.timeoutCtr -= 10;
        if (batteryData.cmd == e_BatteryCmd_I) {
            static const uint8_t pwmStart = (uint8_t)(BatteryPwmMax * 0.5 / 3.3);
            int32_t pwm;
            if (batteryData.val == 0)
            {
                pwm = 0;
            }else
            {
                int32_t du = ADC_values[ADC_IN0] - ADC_values[ADC_IN1];
                int32_t i = (du * (3300 * 1000)) / (4096 * 2700); // 2.7 Ohm
                int32_t err = batteryData.val - i;
                if (err > 3) {
                    if (batteryData.errInt < 127)
                        batteryData.errInt++;
                }
                else
                if (err < -3) {
                    if (batteryData.errInt > -127)
                        batteryData.errInt--;
                }
                pwm = (err / 1) + (batteryData.errInt / 2) + pwmStart;
                if (pwm < 0)
                    pwm = 0;
                else if (pwm > BatteryPwmMax)
                    pwm = BatteryPwmMax;
            }
            PWM_Set(BatteryLoadTimer, BatteryLoadChannel, pwm);
        }else{ // do nothing
        }
    }
}

void battery_100ms(void)
{
    {
        char uart2Buffer[] = "U0xxxx\rU1xxxx\rIxxxx\r";
        (void)U32_to_HexString(uart2Buffer +  2, 4, ADC_values[ADC_IN0], '0');
        (void)U32_to_HexString(uart2Buffer +  9, 4, ADC_values[ADC_IN1], '0');
        {
            int32_t du = ADC_values[ADC_IN0] - ADC_values[ADC_IN1];
            int32_t i = (du * (3300 * 10)) / (27 * 4096); // 2.7 Ohm
            (void)U32_to_HexString(uart2Buffer +  15, 4, ((uint32_t)i) & 0xFFFF, '0');
        }
        hostTx((uint8_t*)uart2Buffer, sizeof(uart2Buffer) - 1);
    }
    {
        char uartBuffer[] = "DBG00xxyyzzaaabbbccc\r";
        static uint8_t ctr;
        (void)U32_to_HexString(uartBuffer +  5, 2, ctr++, '0');
        (void)U32_to_HexString(uartBuffer +  7, 2, PWM_Get(BatteryLoadTimer, BatteryLoadChannel), '0');
        (void)U32_to_HexString(uartBuffer +  9, 2, (unsigned)batteryData.errInt, '0');
        (void)U32_to_HexString(uartBuffer + 11, 3, ADC_values[ADC_TemperatureSensor], '0');
        uint16_t vrefDig = ADC_values[ADC_Vref];
        (void)U32_to_HexString(uartBuffer + 14, 3, vrefDig, '0');
        if (vrefDig != 0) {
            uint16_t vref = (120 * 4095) / vrefDig;
            (void)U32_to_DecString(uartBuffer + 17, 3, vref, ' ');
        }
        hostTx(uartBuffer, sizeof(uartBuffer) - 1);
    }
}
