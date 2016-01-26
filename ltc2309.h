#ifndef _LTC2309_H
#define _LTC2309_H

//! @name Single-Ended Channel Configuration
//! @{
// Single Ended Channel Configuration	// S/D O/S S1  S0  UNI SLP
#define LTC2309_CH0         (0x80)	//  1   0   0   0   0   0   X   X 
#define LTC2309_CH1         (0xC0)	//  1   1   0   0   0   0   X   X
#define LTC2309_CH2         (0x90)	//  1   0   0   1   0   0   X   X 
#define LTC2309_CH3         (0xD0)	//  1   1   0   1   0   0   X   X 
#define LTC2309_CH4         (0xA0)	//  1   0   1   0   0   0   X   X
#define LTC2309_CH5         (0xE0)	//  1   1   1   0   0   0   X   X
#define LTC2309_CH6         (0xB0)	//  1   0   1   1   0   0   X   X
#define LTC2309_CH7         (0xF0)	//  1   1   1   1   0   0   X   X
//!@}

//! @name Differential Channel Configuration
//! @{
// Differential Channel Configuration
#define LTC2309_P0_N1       (0x00)
#define LTC2309_P1_N0       (0x40)

#define LTC2309_P2_N3       (0x10)
#define LTC2309_P3_N2       (0x50)

#define LTC2309_P4_N5       (0x20)
#define LTC2309_P5_N4       (0x60)

#define LTC2309_P6_N7       (0x30)
#define LTC2309_P7_N6       (0x70)
//!@}

//! @name LTC2309 Configuration Bits
//! @{
// LTC2309 Configuration Bits
#define LTC2309_SLEEP_MODE          (0x04)
#define LTC2309_EXIT_SLEEP_MODE     (0x00)
#define LTC2309_UNIPOLAR_MODE       (0x08)
#define LTC2309_BIPOLAR_MODE        (0x00)
#define LTC2309_SINGLE_ENDED_MODE   (0x80)
#define LTC2309_DIFFERENTIAL_MODE   (0x00)
//!@}

//! @name   LTC2309 I2C address
//! @{
// LTC2309 I2C Address
#define LTC2309_I2C_ADDRESS         (0x28)
//!@}
#endif

