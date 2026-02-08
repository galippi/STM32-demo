#include "stm32f103xb.h"

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
