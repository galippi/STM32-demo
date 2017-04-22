/******************************************************************************
 *
 * \file
 * \ingroup MAIN
 * \brief   Header file for main module
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup MAIN
 *  @{
 ******************************************************************************/


#ifndef _MAIN_H_
#define _MAIN_H_

void RAM_StartCheck(void);
int main(void);
void TIM2_ISR(void);
void TIM3_ISR(void);
void ExceptionHandler_0(void);
void ExceptionHandler_1(void);
void ExceptionHandler_2(void);
void ExceptionHandler_3(void);
void ExceptionHandler_4(void);
void ISR_Invalid0(void);
void ISR_Invalid1(void);
void ISR_Invalid2(void);
void ISR_Invalid3(void);
void ISR_Invalid(void);
void PendSV_Handler(void);
void SVC_Handler(void);

#endif /* _MAIN_H_ */
