/*
 * svc.h
 *
 *  Created on: 2020. szept. 29.
 *      Author: liptak
 */

#ifndef SVC_H_
#define SVC_H_

/* INTERRUPT */ void SVC_Handler(void);
extern void svc_ret(void);
extern int8_t lockSemaphore(uint8_t *semaphorePtr, uint8_t locker);
extern void atomicAddU32(uint32_t *address, uint32_t data);
extern void atomicAddU16(uint16_t *address, uint16_t data);
extern void atomicAddU8(uint8_t *address, uint8_t data);
extern void enableInterrupt(void);
extern void disableInterrupt(void);

#endif /* SVC_H_ */
