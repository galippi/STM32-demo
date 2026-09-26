#ifndef _BITFIELD_LIB_H_
#define _BITFIELD_LIB_H_

#define BitfieldSet(var, bit_num, bit_length, val) ((var) = (((var) & (~(((1 << (bit_length)) - 1) << (bit_num)))) | ((val) << (bit_num))))
#define BitfieldSetMask(var, bit_num, bit_mask, val) ((var) = (((var) & (~(bit_mask))) | ((val) << (bit_num))))

#endif /* _BITFIELD_LIB_H_ */
