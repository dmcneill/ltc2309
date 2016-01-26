//
// ltc2309.c
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ltc2309.h"

/*
 * Channel table for channels 0-7
 */
static const unsigned char channelTable[] =
{
    LTC2309_CH0,
    LTC2309_CH1,
    LTC2309_CH2,
    LTC2309_CH3,
    LTC2309_CH4,
    LTC2309_CH5,
    LTC2309_CH6,
    LTC2309_CH7
};

#define I2C_SLAVE_NAME      "/dev/i2c-1"
#define I2C_SLAVE_ADDR      0x28
#define VREF                4.096f
#define NUMBER_OF_CHANNELS  8

/**
 *  @brief      Convert the raw code to a voltage value based on
 *              the command mode (unipolar / bipolar).
 *
 *  @param [in] adcCode : The raw ADC code read from the converter.
 *  @param [in] vRef    : The reference voltage as a float.
 *  @param [in] mode    : The poloarity mode for the command,
 *                        either bipolar or unipolar.
 *
 *  @return     The reading as a floating point voltage value.
 */
float codeToVoltage(uint16_t adcCode, float vRef, uint8_t mode)
{
    float voltage;
    float sign = 1;

    if (mode == LTC2309_UNIPOLAR_MODE)
    {
        float scaling = pow(2, 12) - 1;

        voltage = (float) adcCode;

        // Calculate the input as a fraction of the reference voltage
        voltage = voltage / scaling;
    }
    else
    {
        float scaling = pow(2, 11) - 1;

        vRef = vRef / 2;
        if ((adcCode & 0x0800) == 0x0800)
        {
            // ADC code is < 0 -- convert from 2's comp to binary
            adcCode = adcCode - 0x800;
            sign = (adcCode == 0)? 1: -1;
        }

        voltage = sign * (float) adcCode;

        // Calculate the input as a fraction of the reference voltage
        voltage = voltage / scaling;
    }

    // Multiply the fraction by Vref to the the voltage at the input in Volts DC.
    voltage = voltage * vRef;

    return voltage;
}


/**
 *  @brief      Write the command to the LTC2309 to initiate the
 *              conversion.
 *
 *  @param [in] fd      : The file descriptor to the LTC2309 I2c 
 *                        interface.
 *  @param [in] channel : The channel number to use.
 *  @param [in] mode    : The poloarity mode for the command,
 *                        either bipolar or unipolar.
 *
 *  @return     The command as an integer.
 */
int writeChannel(int fd, uint8_t channel, int mode)
{
    uint8_t  command = channelTable[channel % NUMBER_OF_CHANNELS] | mode;

    if ((write(fd, &command, 1)) != 1)
    {
        printf("Error writing to i2c slave\n");
        return -1;
    }
    return command;
} 


/**
 *  @brief      Read a 16-bit value from a LTC2309 device at a
 *              given channel.
 *
 *  @param [in] fd      : The file descriptor to the LTC2309 I2c 
 *                        interface.
 *  @param [in] channel : The channel number to use.
 *  @param [in] count   : The number of times to perform the read.
 *  @param [in] command : The command byte for the mode containing
 *                        the polarity and the channel code.
 *
 *  @return     The converted data value as a voltage in floating
 *              point.
 */
float readChannel(int fd, uint8_t channel, unsigned count, int command)
{
    uint16_t data = 0;
    uint8_t  buf[2];
    uint8_t  unipolar = command & LTC2309_UNIPOLAR_MODE;
    float    voltage = 0.0f;

    do
    {
        memset( buf, 0, sizeof(buf));
        if (read(fd, buf, sizeof(buf)) != 2)
        {
            printf("Unable to read from slave\n");
            return -1;
        }

        data = ((buf[0] << 8) | buf[1]);
        data = data >> 4;
        voltage = codeToVoltage(data, VREF, unipolar);
    } while (--count);

    return voltage;
}


/**
 *  @brief      Application entry point.
 * 
 *  @param[in]  argc    Number of command line arguments, including
 *                      the invoking program name.
 *  @param[in]  argv    List of constant c-strings for each argument,
 *                      starting with the program name itself at the
 *                      index of zero.
 * 
 *  @retval     0       Success.
 *  @retval     <other> Unix-style error codes.
 */
int main(int argc, char **argv)
{
    int fd = -1;
    int ii;
    float voltage = 0.0f;

    if ((fd = open(I2C_SLAVE_NAME, O_RDWR)) < 0)
    {
        printf("Failed to open i2c port\n");
        exit(1);
    }

    if (ioctl(fd, I2C_SLAVE, I2C_SLAVE_ADDR) < 0)
    {
        printf("Unable to get bus access to talk to slave\n");
        exit(1);
    }

    for (ii = 0; ii < 8; ii++)
    {
        int cmd = writeChannel(fd, ii % 8, LTC2309_UNIPOLAR_MODE);
        voltage = readChannel(fd, ii % 8, 1, cmd);
        printf("Channel [%d] = %f Volts\n", ii, voltage);
    }

    close(fd);
    return 0;
}
