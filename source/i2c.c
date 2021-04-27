/*******************************************************************************************************
 * @file i2c.c :
 * @brief   : This file contains i2c driver funtions.
 * 
 * @author  : Rajat Chaple (rajat.chaple@colorado.edu)
 * @date    : Apr 25, 2021
 *
 * @resource: 
 *******************************************************************************************************/

/*------------- Includes -----------*/
#include "MKL25Z4.h"
#include "i2c.h"
#include "assert.h"
#include "stdlib.h"

/*--------------Defines-------------*/
#define I2C_M_START     I2C0->C1 |= I2C_C1_MST_MASK
#define I2C_M_STOP      I2C0->C1 &=  ~I2C_C1_MST_MASK
#define I2C_M_RSTART    I2C0->C1 |= I2C_C1_RSTA_MASK

#define I2C_TRANS       I2C0->C1 |= I2C_C1_TX_MASK
#define I2C_REC         I2C0->C1 &= ~I2C_C1_TX_MASK

#define I2C_WAIT        while((I2C0->S & I2C_S_IICIF_MASK) == 0) {} \
                             I2C0->S |= I2C_S_IICIF_MASK;

#define NACK            I2C0->C1 |= I2C_C1_TXAK_MASK
#define ACK             I2C0->C1 &= ~I2C_C1_TXAK_MASK

#define I2C_DATA(x)     I2C0->D = (x)



/*------------- Datatypes-----------*/

/*-------------------------------------------------------------------------------------------------------
 * This function initializes i2c module to communicate with onboard
 -------------------------------------------------------------------------------------------------------*/
 void init_i2c()
 {
     

     //enabling clock on I2C0 peripheral and PORT E
     SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;
     SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

     //configuring port E pins for I2C
     PORTE->PCR[24] |= PORT_PCR_MUX(5); //I2C0_SCL
     PORTE->PCR[25] |= PORT_PCR_MUX(5); //I2C0_SDA

    //setting up I2C in fast mode 400k baud
    // baud = bus_freq/(scl_div + mul)
    // 24MHz/ 400kHz = 60
    I2C0->F = I2C_F_ICR(0x11) | I2C_F_MULT(0);

    //enable I2C and set to master mode
    I2C0->C1 |= (I2C_C1_IICEN_MASK);

    //select high drive mode    //TODO : check if this is really reqd.. power can be saved
    I2C0->C2 |= (I2C_C2_HDRS_MASK);



     /* Connection details for accelerometer
     *  INT1 ACCEL : PTA14 
     *  INT2 ACCEL : PTA15 
     *  I2C0_SCL : PTE24
     *  I2C0_SDA : PTE25 *
     *  => device address is 0011101 (0x1D) as SA0 is high
     *  => I2C0_C1 control register
     *  •IICEN enables the peripheral to operate.
        •IICIE enables interrupts from the peripheral.
        •MST sets master or slave mode and also generates start and stop conditions on the bus. Changing MST from zero to one generates a start and selects master mode. Changing MST from one to zero generates a stop and selects slave mode.
        •TX selects if the peripheral will transmit (one) or receive (zero).
        •TXAK controls whether to transmit an ACK (zero) or a NACK (one) after a byte is received.
        •Writing a one to RSTA makes the peripheral generate a repeated start condition on the bus.
        •DMAEN enables DMA transfers.

        => I2C0_S status and interrupts register
        •TCF indicates that a byte and acknowledgment bit transfer has completed.
        •BUSY indicates the bus is busy.
        •IICIF indicates that an interrupt is pending, for example, because a transfer has completed.
        •RXAK indicates that an acknowledgment bit was received (one) after transmitting a byte. A zero indicates no acknowledgment was received.

        => I2C0_D data register holds data to transmit or receive

        => I2C0_F registers configures baud rate
        I2C baud rate = bus speed / mul*scl divider

        => feature to be used : lopw-power mode that can operate while the rest of the MCU is in sleep mode.
     */

 }

#ifndef MULTIPLE_BYTES
/*-------------------------------------------------------------------------------------------------------
 * This function writes a byte of data over I2C
 * (refer i2c.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void i2c_write_byte(uint8_t device_address, uint8_t reg, uint8_t data)
{
    uint8_t device_address_with_write = (device_address << 1) & 0xFE;

    I2C_TRANS;                              //set to transmit mode
    I2C_M_START;                            //send start
    I2C0->D = device_address_with_write;    //send dev address (write)
    I2C_WAIT                                //wait for ack
    
    I2C0->D = reg;                          //send register address
    I2C_WAIT                                //wait for completion

    I2C0->D = data;                         //send data
    I2C_WAIT                                //wait for completion
    I2C_M_STOP;                             //send stop

    while(I2C0->S & I2C_S_BUSY_MASK){};
}

 /*-------------------------------------------------------------------------------------------------------
 * This function reads a byte of data over I2C
 * (refer i2c.h for more details)
 -------------------------------------------------------------------------------------------------------*/
uint8_t i2c_read_byte(uint8_t device_address, uint8_t reg)
{
    uint8_t device_address_with_write = (device_address << 1) & 0xFE;
    uint8_t device_address_with_read = (device_address << 1) | 0x01;
    uint8_t data;

    I2C_TRANS;                              //set to transmit mode
    I2C_M_START;                            //send start
    I2C0->D = device_address_with_write;    //send dev address (write)
    I2C_WAIT                                //wait for completion
    
    I2C_DATA(reg);                          //send register address
    I2C_WAIT                                //wait for completion

    I2C_M_RSTART;                           //repeated start
    I2C0->D = device_address_with_read;     //send dev address (read)
    I2C_WAIT                                //wait for completion

    I2C_REC;                                //set to receive mode
    NACK;                                   //set NACK after read

    data = I2C0->D;                         //dummy read
    I2C_WAIT                                //wait for completion

    I2C_M_STOP;                             //send stop
    data = I2C0->D;                         //read data

    while(I2C0->S & I2C_S_BUSY_MASK){};

    return data;
}
 
#else

/*-------------------------------------------------------------------------------------------------------
 * This function writes a byte of data over I2C
 * (refer i2c.h for more details)
 -------------------------------------------------------------------------------------------------------*/
void i2c_write(uint8_t device_address, uint8_t reg, uint8_t* data,  uint8_t data_count)
{
    if((data == NULL) || (data_count == 0))
        return;

    uint8_t device_address_with_write = (device_address << 1) & 0xFE;
    uint8_t* p = data;

    I2C_TRANS;                              //set to transmit mode
    I2C_M_START;                            //send start
    I2C0->D = device_address_with_write;    //send dev address (write)
    I2C_WAIT                                //wait for ack
    
    I2C0->D = reg;                          //send register address
    I2C_WAIT                                //wait for ack

    for(int i = 0; i < data_count; i++,p++)
    {
        I2C0->D = *p;                         //send data
        I2C_WAIT                                //wait for completion
    }
    I2C_M_STOP;                             //send stop

    while(I2C0->S & I2C_S_BUSY_MASK){};
}

 /*-------------------------------------------------------------------------------------------------------
 * This function reads a byte of data over I2C
 * (refer i2c.h for more details)
 -------------------------------------------------------------------------------------------------------*/
uint8_t i2c_read(uint8_t device_address, uint8_t reg, uint8_t* data, uint8_t data_count)
{
    uint8_t device_address_with_write = (device_address << 1) & 0xFE;
    uint8_t device_address_with_read = (device_address << 1) | 0x01;
    uint8_t num_of_bytes_read = 0;
    uint8_t dummy;

    if(data_count == 0)
        return 0;

    I2C_TRANS;                              //set to transmit mode
    I2C_M_START;                            //send start
    I2C0->D = device_address_with_write;    //send dev address (write)
    I2C_WAIT                                //wait for completion
    
    I2C_DATA(reg);                          //send register address
    I2C_WAIT                                //wait for completion

    I2C_M_RSTART;                           //repeated start
    I2C0->D = device_address_with_read;     //send dev address (read)
    I2C_WAIT                                //wait for completion

    I2C_REC;                                //set to receive mode

    if(data_count == 1)
    {
        NACK;                                   //set NACK after read

        dummy = I2C0->D;                         //dummy read
        I2C_WAIT                                //wait for completion

        I2C_M_STOP;                             //send stop
        data[num_of_bytes_read++] = I2C0->D;    //read data
    }
    else
    {
        ACK;
        dummy = I2C0->D;
        I2C_WAIT;
        
        while(num_of_bytes_read < (data_count - 1))   
        {
            ACK;
            data[num_of_bytes_read++] = I2C0->D;
            I2C_WAIT
        }

        NACK;
        data[num_of_bytes_read++] = I2C0->D;
        I2C_WAIT
        I2C_M_STOP;
    }
        
    while(I2C0->S & I2C_S_BUSY_MASK){};

    return num_of_bytes_read;
}

#endif
