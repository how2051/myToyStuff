#include "i2c_ctrl.h"
#include <cstdio>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c.h>

int i2c_init(const char* file_name) {
    int ctrl_fd = open(file_name, O_RDWR);
    if (!ctrl_fd) {
        printf("can not open file %s\n", file_name);
        return -1;
    }
    return ctrl_fd;
}

int i2c_uninit(int ctrl_fd) {
    if (ctrl_fd == -1) {
        return -1;
    }

    close(ctrl_fd);

    printf("[%s] close i2c device, ctrl_fd: %d\n", __func__, ctrl_fd);
    return 0;
}

// set the I2C slave address for all subsequent I2C device transfers
static void i2cSetAddress(int ctrl_fd, int address) {
    if (ioctl(ctrl_fd, I2C_SLAVE_FORCE, address) < 0) {
        perror("i2cSetAddress");
        exit(1);
    }
}

// salveAddr : 8 bit savle address
int i2c_write_a8d8(int ctrl_fd, int slaveAddr, short reg, uint8_t value) {
    unsigned char data[4];

    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(data, 0, sizeof(data));

    data[0] = reg & 0xff;
    data[1] = value & 0xff;
    if (write(ctrl_fd, data, 2) != 2) {
        perror("Write Register");
        printf("[%s] slaveAddr:0x%x reg = %#x, value = %#x \n", __func__, slaveAddr, reg, value);
        return -1;
    }
    return 0;
}

int i2c_read_a8d8(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val) {
    unsigned char reg_addr[2];

    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(reg_addr, 0, sizeof(unsigned char));

    reg_addr[0] = reg & 0xff;
    if (write(ctrl_fd, reg_addr, 1) != 1) {
        perror("Read RegisterPair set register");
        return -1;
    }

    if (read(ctrl_fd, val, 1) != 1) {
        perror("Read RegisterPair read value");
        return -1;
    }

    return 0;
}

// salveAddr : 8 bit savle address
int i2c_write_a8d16(int ctrl_fd, int slaveAddr, short reg, uint8_t* value) {
    unsigned char data[4];
    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(data, 0, sizeof(data));

    data[0] = reg & 0xff;
    data[1] = value[0];
    data[2] = value[1];
    if (write(ctrl_fd, data, 3) != 3) {
        perror("Write Register ");
        printf("[%s]write addr: %#x, val: %#x %#x\n", __func__, data[0], data[1], data[2]);
        return -1;
    }
    return 0;
}

int i2c_read_a8d16(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val) {
    unsigned char reg_addr[2];
    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(reg_addr, 0, sizeof(unsigned char));

    reg_addr[0] = reg & 0xff;
    if (write(ctrl_fd, reg_addr, 1) != 1) {
        printf("[%s]timeout ctrl_fd:0x%x, addr:0x%x, reg:0x%x \n", __func__, ctrl_fd, slaveAddr, reg);
        return -1;
    }

    if (read(ctrl_fd, val, 2) != 2) {
        printf("[%s]timeout ctrl_fd:0x%x, addr:0x%x, reg:0x%x \n", __func__, ctrl_fd, slaveAddr, reg);
        return -1;
    }
    return 0;
}

int i2c_read_a8d16_no_log(int ctrl_fd, int slaveAddr, unsigned int reg, uint8_t* val) {
    unsigned char reg_addr[2];
    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(reg_addr, 0, sizeof(unsigned char));

    reg_addr[0] = reg & 0xff;
    if (write(ctrl_fd, reg_addr, 1) != 1) {
        return -1;
    }

    if (read(ctrl_fd, val, 2) != 2) {
        return -1;
    }
    return 0;
}

int i2c_write_a16d8(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t value) {
    unsigned char data[4];

    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(data, 0, sizeof(data));

    data[0] = (reg & 0xff00) >> 8;
    data[1] = (reg & 0x00ff);
    data[2] = value & 0xff;
    int ret = write(ctrl_fd, data, 3);
    if (ret != 3) {
        printf("[%s] timeout slaveAddr: 0x%x, reg = %#x, value = %#x, ret: %d\n", __func__, slaveAddr, reg, value, ret);
        return -1;
    }
    return 0;
}

int i2c_write_a16d8_m24512(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t* data) {
    unsigned char buffer[4];

    i2cSetAddress(ctrl_fd, slaveAddr >> 1);
    memset(buffer, 0, sizeof(buffer));

    buffer[0] = (reg & 0xff00) >> 8;
    buffer[1] = (reg & 0x00ff);
    buffer[2] = *data & 0xff;
    int ret = write(ctrl_fd, buffer, 3);
    if (ret != 3) {
        printf("[%s] timeout slaveAddr: 0x%x, reg = %#x, value = %#x, ret: %d\n", __func__, slaveAddr, reg, *data, ret);
        return -1;
    }
    return 0;
}

int i2c_read_a16d8(int ctrl_fd, int slaveAddr, unsigned short reg, uint8_t* val) {
    unsigned char reg_addr[2];
    i2cSetAddress(ctrl_fd, slaveAddr >> 1);

    reg_addr[0] = (reg & 0xff00) >> 8;
    reg_addr[1] = (reg & 0x00ff);
    if (write(ctrl_fd, reg_addr, 2) != 2) {
        perror("i2c_read_a16d8 write address ");
        return -1;
    }

    if (read(ctrl_fd, val, 1) != 1) {
        perror("i2c_read_a16d8 Read value");
        return -1;
    }
    return 0;
}

int i2c_set_bit_a16d8(int ctrl_fd, uint8_t devAddr, uint16_t reg, int pos) {
    uint8_t tmp;
    if (pos > 7) {
        return -4;
    }
    i2c_read_a16d8(ctrl_fd, devAddr, reg, &tmp);
    tmp = tmp | (1 << pos);
    return i2c_write_a16d8(ctrl_fd, devAddr, reg, tmp);
}

int i2c_clear_bit_a16d8(int ctrl_fd, uint8_t devAddr, uint16_t reg, int pos) {
    uint8_t tmp;
    if (pos > 7) {
        return -4;
    }
    i2c_read_a16d8(ctrl_fd, devAddr, reg, &tmp);
    tmp = tmp & (~(1 << pos));
    return i2c_write_a16d8(ctrl_fd, devAddr, reg, tmp);
}

int i2c_write_1byte(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data) {
    uint8_t outbuf[3] = {0x00};
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    /* 
    * The first byte indicates which register we‘ll write 
    * The second byte indicates the data to write. Note that for many
    * devices, we can write multiple, sequential registers at once by
    * simply making outbuf bigger.
    */
    outbuf[0] = (reg_addr & 0xff00) >> 8;
    outbuf[1] = (reg_addr & 0x00ff);
    memcpy(&outbuf[2], data, 1);

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;
    
    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs = messages;
    packets.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data: ");
        return 0;
    }
    return 1;
}

int i2c_write_4bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data) {
    uint8_t outbuf[6] = {0x00};
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];
    /* 
    * The first byte indicates which register we‘ll write 
    * The second byte indicates the data to write. Note that for many
    * devices, we can write multiple, sequential registers at once by
    * simply making outbuf bigger.
    */
    outbuf[0] = (reg_addr & 0xff00) >> 8;
    outbuf[1] = (reg_addr & 0x00ff);
    memcpy(&outbuf[2], data, 4);

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;
    
    /* Transfer the i2c packets to the kernel and verify it worked */
    packets.msgs = messages;
    packets.nmsgs = 1;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data: ");
        return 0;
    }
    return 1;
}

int i2c_read_1byte(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data) {
    uint8_t outbuf[2] = {0};
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    /*
    * In order to read a register, we first do a "dummy write" by writing
    * 0 bytes to the register we want to read from. This is similar to
    * the packet in set_i2c_register, except it‘s 1 byte rather than 2.
    */
    outbuf[0] = (reg_addr & 0xff00) >> 8;
    outbuf[1] = (reg_addr & 0x00ff);

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = 2;
    messages[0].buf = outbuf;
    /* The data will get returned in this structure */
    messages[1].addr = slave_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = 1;
    messages[1].buf = data;

    /* Send the request to the kernel and get the result back */
    packets.msgs = messages;
    packets.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data: ");
        return 0;
    }
    return 1;
}

int i2c_read_4bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data) {
    uint8_t outbuf[2] = {0};
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    /*
    * In order to read a register, we first do a "dummy write" by writing
    * 0 bytes to the register we want to read from. This is similar to
    * the packet in set_i2c_register, except it‘s 1 byte rather than 2.
    */
    
    outbuf[0] = (reg_addr & 0xff00) >> 8;
    outbuf[1] = (reg_addr & 0x00ff);

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = 2;
    messages[0].buf = outbuf;
    /* The data will get returned in this structure */
    messages[1].addr = slave_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = 4;
    messages[1].buf = data;

    /* Send the request to the kernel and get the result back */
    packets.msgs = messages;
    packets.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data: ");
        return 0;
    }
    return 1;
}

int i2c_read_8bytes(int fd, uint16_t slave_addr, uint16_t reg_addr, uint8_t* data) {
    uint8_t outbuf[2] = {0};
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
    /*
    * In order to read a register, we first do a "dummy write" by writing
    * 0 bytes to the register we want to read from. This is similar to
    * the packet in set_i2c_register, except it‘s 1 byte rather than 2.
    */
    outbuf[0] = (reg_addr & 0xff00) >> 8;
    outbuf[1] = (reg_addr & 0x00ff);

    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = 2;
    messages[0].buf = outbuf;
    /* The data will get returned in this structure */
    messages[1].addr = slave_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = 8;
    messages[1].buf = data;

    packets.msgs = messages;
    packets.nmsgs = 2;

    if (ioctl(fd, I2C_RDWR, &packets) < 0) {
        perror("Unable to send data: ");
        return 0;
    }
    return 1;
}