#ifndef _BATTERY_H_
#define _BATTERY_H_ 1

typedef enum {
    e_BatteryCmd_U,
    e_BatteryCmd_I,
    e_BatteryCmd_PWM,
    e_BatteryCmd_timeout,
    e_BatteryCmd_default,
}t_BatteryCmd;

extern void battery_init(void);
extern void battery_10ms(void);
extern void battery_100ms(void);

#endif /* _BATTERY_H_ */
