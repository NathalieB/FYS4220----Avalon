#include <stdio.h>
#include "system.h"
#include "i2c_wrapper.h"
#include alt_types.h"
#include "io.h"
#include "unistd.h" // usleep()

int check_tmp175_busy(void)
{
	int res = IORD(I2C_WRAPPER_BASE,I2C_CTR_REG);
	// Check if i2c_wrapper state machine or i2c master is busy
	if(CHECK_BIT(res,7) | CHECK_BIT(res,6))
		return 1;
	else
		return 0;
}

void write_to_i2c_device(alt_u8 i2c_device_addr,alt_u8 i2c_reg_addr,alt_u8 no_bytes,alt_u32 data)
{
	// Write to address register of i2C wrapper
	IOWR(I2C_WRAPPER_BASE,I2C_ADDR_REG,i2c_addr << 8 | i2c_device_addr);
	// Write to the data register of the i2c wrapper
	IOWR(I2C_WRAPPER_BASE,I2C_DATA_REG,data);
	// Set the corresponding bits of the ctrl register to enable i2c communication
	IOWR(I2C_WRAPPER_BASE,I2C_CTRL_REG,no_bytes << 2 | I2C_CMD_ENA_WR);
	// Check for the busy state in the ctrl register (i2c busy or wrapper busy) and continue when no longer busy
	while(check_tmp175_busy()) continue;
	
}


alt_u32 read_from_i2c_device(alt_u8 i2c_device_addr,alt_u8 i2c_reg_addr,alt_u8 no_bytes);
{alt_u32 res=0;
//write to addres register if i2c weapper
//6..0: 7-bits i2c evice address
//15..8: 8 bits register address of i2c device 
IOWR(I2C_WRAPPER_BASE,I2C_ADDR_REG, i2c_reg_addr << 8 | i2c_device_addr);
//write to i2c wrapper ctrl register to enable i2C command
IOWR(I2C_WRAPPER_BASE,I2C_CTRL_REG,I2C_CMD_ENA_WR);
// Check for the busy state in the ctrl register (i2c busy or wrapper busy) and continue when no longer busy
while(check_tmp175_busy()) continue;
	//read data to i2C device	
	//two bytes to be read from the i2c device
	if (no_bytes == 2)
	
	{
	IOWR(I2C_WRAPPER_BASE,I2C_CTRL_REG,I2C_TWO_BYTES << 2 | I2C_CMD_ENA_RD);
	while(check_tmp175_busy()) continue;
res= IORD(I2C_WRAPPER_BASE,I2C_DATA_REG);
//only the 16MSB of the i2C wrapper data reg contains read data
return res >> 16;
	}
	
//one byte to be read from the i2c device
else if (no_bytes ==1)
	
	{	
	IOWR(I2C_WRAPPER_BASE,I2C_CTRL_REG,I2C_TWO_BYTES << 2 | I2C_CMD_ENA_RD);
	while(check_tmp175_busy()) continue;
res= IORD(I2C_WRAPPER_BASE,I2C_DATA_REG);
//only the 16 MSB of the i2C wrapper data reg contains read data
//mask of the 8 MSB which might contain data from previous read command
return (res >> 16) & 0xff;
	}
	
	else
	
	{
//only 1 or two bytes cen be read from the data register of the
printf("Requested read of in correct number of bytes");
return 0x0;
	}
}	
