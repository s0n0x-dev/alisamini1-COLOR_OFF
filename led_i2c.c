#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>

#define I2C_SLAVE 0x0703
#define I2C_ADDR 0x3C

void write_reg(int fd, unsigned char reg, unsigned char val) {
    unsigned char buf[2];
    buf[0] = reg;
    buf[1] = val;
    if (write(fd, buf, 2) != 2) {
        perror("write");
    }
}

int main() {
    int fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0) { perror("open"); return 1; }
    
    if (ioctl(fd, I2C_SLAVE, I2C_ADDR) < 0) {
        perror("ioctl");
        return 1;
    }

    // 1. Включаем чип
    write_reg(fd, 0x00, 0x01);
    
    // 2. Настраиваем ток (максимум)
    for (int i = 0; i < 24; i++) {
        write_reg(fd, 0x01 + i, 0xFF); 
    }

    // 3. Заливаем красный цвет (каждый 3-й канал)
    for (int i = 0; i < 24; i++) {
        if (i % 3 == 0) {
            write_reg(fd, 0x19 + i, 0xFF); // Красный
        } else {
            write_reg(fd, 0x19 + i, 0x00); // Зеленый/Синий - выкл
        }
    }

    printf("Скрипт выполнен!\n");
    close(fd);
    return 0;
}