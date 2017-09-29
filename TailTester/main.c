#define FCPU 8000000UL


#define LED_PORT PORTB
#define LED_DDR DDRB
#define LED_OK PINB3
#define LED_NO PINB2

#include "I2C/I2C.h"
#include "PCA9655/PCA9655.h"
#include "USART/USART.h"
#include "PARSER/PARSER.h"
#include "UTILS.h"
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>


ISR(INT0_vect)
{
	char ERROR = 0;

	fprintf(usart, "STARTING SCANING OPERATION...\r\n");

	LED_PORT |=  
	(
	(1<<LED_OK)|
	(1<<LED_NO)
	); 

	char buf_1[5];
	char buf_2[5];


	for(char SET_CHANEL = 0; SET_CHANEL < MAX_CHANEL; SET_CHANEL++)
	{
		
		PCA9655_SET_PIN(SET_CHANEL);

		for(char GET_CHANEL = 0; GET_CHANEL < MAX_CHANEL; GET_CHANEL++)
		{
			if (SET_CHANEL == GET_CHANEL)
			{
				if (PCA9655_GET_PIN(READ_ARRAY[GET_CHANEL]) == 0)
				{
					ERROR++; 
					strcpy_P(buf_1,(PGM_P)pgm_read_word(&(CODES[READ_ARRAY[SET_CHANEL]])));
					fprintf(usart, "DISCONNECT: %s\r\n", buf_1);
				}
			}
			else
			{
				if (PCA9655_GET_PIN(READ_ARRAY[GET_CHANEL]))
				{
					ERROR++;
					strcpy_P(buf_1,(PGM_P)pgm_read_word(&(CODES[READ_ARRAY[SET_CHANEL]])));
					strcpy_P(buf_2,(PGM_P)pgm_read_word(&(CODES[READ_ARRAY[GET_CHANEL]])));
					fprintf(usart, "SHORT CIRCUIT: %s AND %s\r\n",buf_1, buf_2 );
				}
			}
		}

		PCA9655_CLEAR_PIN(SET_CHANEL);		
	}

	LED_PORT  &=
	~(
		(1 << LED_OK)|
		(1 << LED_NO)
	);

	fprintf(usart, "SCANING COMPLITE\r\n");

	if (ERROR)
	{
		LED_PORT |=  (1<<LED_NO);
		fprintf(usart, "%u ERRORS\r\n\r\n", ERROR);
	} 
	else
	{
		LED_PORT |=  (1<<LED_OK); 
		fprintf(usart, "TAIL IS OK\r\n\r\n");
	}
}


int main(void)
{
	i2c_init();
	PCA9655_INIT();
	UASRT_INIT();
	PARS_Init();

	LED_DDR |=
	(
		(1<<LED_OK)|
		(1<<LED_NO)
	);

	LED_PORT  &=
	~(
		(1 << LED_OK)|
		(1 << LED_NO)
	);

	INT0_DDR &=~(1<<INT0_PIN);
	GICR = 1<<INT0;

	MCUCR  &=
	~(
		(1 << ISC01)|
		(1 << ISC00)
	);

	sei();

	fprintf(usart, "TAIL TESTER IS READY\r\nSOFTWARE VERSION: 1.0\r\n\r\n");	

    while (1) 
    {
    }
}





