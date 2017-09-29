
#ifndef PCA9655_H_
#define PCA9655_H_

#include "../I2C/I2C.h"
#include <avr/delay.h>

#define PCA_SELECT_PORT PORTA
#define PCA_SELECT_DDR DDRA

#define PCA_SELECT_WRITE PINA6
#define PCA_SELECT_READ  PINA7



void PCA9655_INIT (void);
void PCA9655_SET_PIN (unsigned char PIN);
void PCA9655_CLEAR_PIN (unsigned char PIN);
uint8_t  PCA9655_GET_PIN (unsigned char PIN);


#endif /* PCA9655_H_ */