/******************************************************************************
 *
 * \file
 * \ingroup RESET
 * \brief   ISR vector table
 *
 * \details Fill me
 *
 ******************************************************************************/
/** \addtogroup RESET
 *  @{
 ******************************************************************************/


#ifndef _VECTOR_H_
#define _VECTOR_H_

extern const uint32_t const *SP_INIT;

typedef void (*t_func_ptr)(void);

extern const t_func_ptr ISR_VectorTable[];

void PendSV_Handler(void);

#endif /* _VECTOR_H_ */
