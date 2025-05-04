#ifndef _I2C_CTRL_H_
#define _I2C_CTRL_H_

#include <stdint.h>

// Example I2C device file names
//#define POWER_I2C_FILE_NAME "/dev/i2c-0" 
//#define TOF_I2C_FILE_NAME "/dev/i2c-2" 

// Example usage:
// apx221  ./i2c_read_write -b 0 -a 0x68 -f A8D8 -r 0x03
// ocp8481 ./i2c_read_write -b 0 -a 0xC8 -f A8D8 -r 0x0E
// other temp read ./i2c_read_write -b 2 -a 0x90 -f A8D16 -r 0x01

//#define FILE_NAME "/dev/i2c-1"  
// rgb sensor ./i2c_read_write -b 1 -a 0x34 -f A16D8 -r 0x301c

//#define FILE_NAME "/dev/i2c-2" 
// tof sensor ./i2c_read_write -b 2 -a 0xae -f A16D8 -r 0x1006

// static int ctrl_fd = -1;

int i2c_init(const char* file_name);
int i2c_uninit(int ctrl_fd);

// salveAddr : 8 bit slave address
int i2c_write_a8d8(int ctrl_fd, int slaveAddr, short reg, uint8_t value);
int i2c_read_a8d8(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val);

// salveAddr : 8 bit slave address
int i2c_write_a8d16(int ctrl_fd, int slaveAddr, short reg, uint8_t* value);
int i2c_read_a8d16(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val);

int i2c_write_a16d8(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t value);
int i2c_write_a16d8_m24512(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t* data);
int i2c_read_a16d8(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t* val);

int i2c_set_bit_a16d8(int ctrl_fd, uint8_t devAddr, uint16_t reg, int pos);
int i2c_clear_bit_a16d8(int ctrl_fd, uint8_t devAddr, uint16_t reg, int pos);

int i2c_read_1byte(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data);
int i2c_write_1byte(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data);

int i2c_read_4bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data);
int i2c_write_4bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data);

int i2c_read_8bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data);

int i2c_read_a8d16_no_log(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val);

#endif // _I2C_CTRL_H_