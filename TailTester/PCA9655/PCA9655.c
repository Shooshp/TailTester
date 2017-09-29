#include "PCA9655.h"

const unsigned char ADDR_ARRAY [9] =
{
	0x40, 0x42, 0x44,
	0x46, 0x4C, 0x48,
	0x4E, 0x4A, 0x50
};


const unsigned char PCA_REG_READ [2] =
{
	0,1
};

const unsigned char PCA_REG_WRITE [2] =
{
	2,3
};

const unsigned char PCA_REG_POLARITY [2] =
{
	4,5
};

const unsigned char PCA_REG_CONFIG [2] =
{
	6,7
};

void PCA9655_INIT (void)
{
	PCA_SELECT_DDR  |=  ((1<<PCA_SELECT_WRITE)|(1<<PCA_SELECT_READ));
	PCA_SELECT_PORT &= ~((1<<PCA_SELECT_WRITE)|(1<<PCA_SELECT_READ));

	PCA_SELECT_PORT |= (1<<PCA_SELECT_WRITE); 	
	for (uint8_t ADDR = 0; ADDR < 9; ADDR++)
	{
		i2c_start(ADDR_ARRAY[ADDR]);
		i2c_write(PCA_REG_CONFIG[0]);
		i2c_write(0xFF);
		i2c_write(0xFF);
		i2c_stop();

		i2c_start(ADDR_ARRAY[ADDR]);
		i2c_write(PCA_REG_WRITE[0]);
		i2c_write(0xFF);
		i2c_write(0xFF);
		i2c_stop();
		_delay_us(1);
	}
	PCA_SELECT_PORT &= ~(1<<PCA_SELECT_WRITE); 

	PCA_SELECT_PORT |= (1<<PCA_SELECT_READ); 
	for (uint8_t ADDR = 0; ADDR < 9; ADDR++)
	{
		i2c_start(ADDR_ARRAY[ADDR]);
		i2c_write(PCA_REG_CONFIG[0]);
		i2c_write(0xFF);
		i2c_write(0xFF);
		i2c_stop();
		_delay_us(1);
	}
	PCA_SELECT_PORT &= ~(1<<PCA_SELECT_READ);
}

void PCA9655_SET_PIN (unsigned char PIN)
{
	PCA_SELECT_PORT |= (1<<PCA_SELECT_WRITE); 

	int DATA = (~(1<<(PIN%16))) & 0xFFFF;

	i2c_start(ADDR_ARRAY[PIN/16]);
	i2c_write(PCA_REG_CONFIG[0]);
	i2c_write(DATA & 0xFF);
	i2c_write((DATA>>8) & 0xFF);
	i2c_stop();

	PCA_SELECT_PORT &= ~(1<<PCA_SELECT_WRITE);
}


void PCA9655_CLEAR_PIN (unsigned char PIN)
{
	PCA_SELECT_PORT |= (1<<PCA_SELECT_WRITE); 

	i2c_start(ADDR_ARRAY[PIN/16]);
	i2c_write(PCA_REG_WRITE[0]);
	i2c_write(0x0);
	i2c_write(0x0);
	i2c_stop();

	i2c_start(ADDR_ARRAY[PIN/16]);
	i2c_write(PCA_REG_CONFIG[0]);
	i2c_write(0xFF);
	i2c_write(0xFF);
	i2c_stop();

	i2c_start(ADDR_ARRAY[PIN/16]);
	i2c_write(PCA_REG_WRITE[0]);
	i2c_write(0xFF);
	i2c_write(0xFF);
	i2c_stop();

	PCA_SELECT_PORT &= ~(1<<PCA_SELECT_WRITE);
}

uint8_t  PCA9655_GET_PIN (unsigned char PIN)
{
	unsigned int DATA = 0;

	PCA_SELECT_PORT |= (1<<PCA_SELECT_READ); 

	i2c_start(ADDR_ARRAY[PIN/16]);
	i2c_write(PCA_REG_READ[0]);
	i2c_start((ADDR_ARRAY[PIN/16]) | 0x1);
	DATA = i2c_read_ack();
	DATA = (i2c_read_nack() << 8)| DATA;
	i2c_stop();	

	PCA_SELECT_PORT &= ~(1<<PCA_SELECT_READ);

	return ((DATA >> (PIN%16)) & 0x1);
}

