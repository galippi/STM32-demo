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

typedef void (*t_func_ptr)(void);

const t_func_ptr ISR_VectorTable[];

#endif /* _VECTOR_H_ */
