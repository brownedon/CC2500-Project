/*!
 *  \file    CC2500.h
 *  \version 1.0
 *  \date    21-09-2012
 *  \author  Yasir Ali, ali.yasir0@gmail.com
 *
 *  Copyright (c) 2012, Yasir Ali
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program;
 *
 *  \internal
 *    Created: 21-09-2012
 */

#ifndef CC2500_H_
#define CC2500_H_

// configuration registers
#define CC2500_REG_IOCFG2       0x00    // GDO2 output pin configuration
#define CC2500_REG_IOCFG1       0x01    // GDO1 output pin configuration
#define CC2500_REG_IOCFG0       0x02    // GDO0 output pin configuration
#define CC2500_REG_FIFOTHR      0x03    // RX FIFO and TX FIFO thresholds
#define CC2500_REG_SYNC1        0x04    // Sync word, high byte
#define CC2500_REG_SYNC0        0x05    // Sync word, low byte
#define CC2500_REG_PKTLEN       0x06    // Packet length
#define CC2500_REG_PKTCTRL1     0x07    // Packet automation control
#define CC2500_REG_PKTCTRL0     0x08    // Packet automation control
#define CC2500_REG_ADDR         0x09    // Device address
#define CC2500_REG_CHANNR       0x0A    // Channel number
#define CC2500_REG_FSCTRL1      0x0B    // Frequency synthesizer control
#define CC2500_REG_FSCTRL0      0x0C    // Frequency synthesizer control
#define CC2500_REG_FREQ2        0x0D    // Frequency control word, high byte
#define CC2500_REG_FREQ1        0x0E    // Frequency control word, middle byte
#define CC2500_REG_FREQ0        0x0F    // Frequency control word, low byte
#define CC2500_REG_MDMCFG4      0x10    // Modem configuration
#define CC2500_REG_MDMCFG3      0x11    // Modem configuration
#define CC2500_REG_MDMCFG2      0x12    // Modem configuration
#define CC2500_REG_MDMCFG1      0x13    // Modem configuration
#define CC2500_REG_MDMCFG0      0x14    // Modem configuration
#define CC2500_REG_DEVIATN      0x15    // Modem deviation setting
#define CC2500_REG_MCSM2        0x16    // Main Radio Control State Machine configuration
#define CC2500_REG_MCSM1        0x17    // Main Radio Control State Machine configuration
#define CC2500_REG_MCSM0        0x18    // Main Radio Control State Machine configuration
#define CC2500_REG_FOCCFG       0x19    // Frequency Offset Compensation configuration
#define CC2500_REG_BSCFG        0x1A    // Bit Synchronization configuration
#define CC2500_REG_AGCCTRL2     0x1B    // AGC control
#define CC2500_REG_AGCCTRL1     0x1C    // AGC control
#define CC2500_REG_AGCCTRL0     0x1D    // AGC control
#define CC2500_REG_WOREVT1      0x1E    // High byte Event0 timeout
#define CC2500_REG_WOREVT0      0x1F    // Low byte Event0 timeout
#define CC2500_REG_WORCTRL      0x20    // Wake On Radio control
#define CC2500_REG_FREND1       0x21    // Front end RX configuration
#define CC2500_REG_FREND0       0x22    // Front end TX configuration
#define CC2500_REG_FSCAL3       0x23    // Frequency synthesizer calibration
#define CC2500_REG_FSCAL2       0x24    // Frequency synthesizer calibration
#define CC2500_REG_FSCAL1       0x25    // Frequency synthesizer calibration
#define CC2500_REG_FSCAL0       0x26    // Frequency synthesizer calibration
#define CC2500_REG_RCCTRL1      0x27    // RC oscillator configuration
#define CC2500_REG_RCCTRL0      0x28    // RC oscillator configuration
#define CC2500_REG_FSTEST       0x29    // Frequency synthesizer calibration control
#define CC2500_REG_PTEST        0x2A    // Production test
#define CC2500_REG_AGCTEST      0x2B    // AGC test
#define CC2500_REG_TEST2        0x2C    // Various test settings
#define CC2500_REG_TEST1        0x2D    // Various test settings
#define CC2500_REG_TEST0        0x2E    // Various test settings
                                       
// status registers
#define CC2500_REG_PARTNUM      0x30    // Chip ID
#define CC2500_REG_VERSION      0x31    // Chip ID
#define CC2500_REG_FREQEST      0x32    // Frequency Offset Estimate from demodulator
#define CC2500_REG_LQI          0x33    // Demodulator estimate for Link Quality
#define CC2500_REG_RSSI         0x34    // Received signal strength indication
#define CC2500_REG_MARCSTATE    0x35    // Main Radio Control State Machine state
#define CC2500_REG_WORTIME1     0x36    // High byte of WOR time
#define CC2500_REG_WORTIME0     0x37    // Low byte of WOR time
#define CC2500_REG_PKTSTATUS    0x38    // Current GDOx status and packet status
#define CC2500_REG_VCO_VC_DAC   0x39    // Current setting from PLL calibration module
#define CC2500_REG_TXBYTES      0x3A    // Underflow and number of bytes
#define CC2500_REG_RXBYTES      0x3B    // Overflow and number of bytes
                                                                                                                                             
// burst write registers
#define CC2500_REG_PATABLE      0x3E    // PA control settings table
#define CC2500_REG_TXFIFO       0x3F    // Transmit FIFO; Single access is +0x00, burst is +0x40
#define CC2500_REG_RXFIFO       0x3F    // Receive FIFO; Single access is +0x80, burst is +0xC0

// command strobe registers,
#define CC2500_CMD_SRES         0x30    // Reset chip
#define CC2500_CMD_SFSTXON      0x31    // Enable and calibrate frequency synthesizer
#define CC2500_CMD_SXOFF        0x32    // Turn off crystal oscillator
#define CC2500_CMD_SCAL         0x33    // Calibrate frequency synthesizer and turn it off
#define CC2500_CMD_SRX          0x34    // Enable RX. Perform calibration if enabled
#define CC2500_CMD_STX          0x35    // Enable TX. If in RX state, only enable TX if CCA passes
#define CC2500_CMD_SIDLE        0x36    // Exit RX / TX, turn off frequency synthesizer
#define CC2500_CMD_SRSVD        0x37    // Reserved
#define CC2500_CMD_SWOR         0x38    // Start automatic RX polling sequence (Wake-on-Radio)
#define CC2500_CMD_SPWD         0x39    // Enter power down mode when CSn goes high
#define CC2500_CMD_SFRX         0x3A    // Flush the RX FIFO buffer
#define CC2500_CMD_SFTX         0x3B    // Flush the TX FIFO buffer
#define CC2500_CMD_SWORRST      0x3C    // Reset real time clock
#define CC2500_CMD_SNOP         0x3D    // No operation, returns status byte

// offsets for registers
#define CC2500_OFF_WRITE_SINGLE 0x00    // Offset for writing in single access mode
#define CC2500_OFF_WRITE_BURST  0x40    // Offset for writing in burst mode
#define CC2500_OFF_READ_SINGLE  0x80    // Offset for reading in single access mode
#define CC2500_OFF_READ_BURST   0xC0    // Offset for reading in burst mode

#define REG_IOCFG2           0x0000
#define REG_IOCFG1           0x0001
#define REG_IOCFG0           0x0002
#define REG_FIFOTHR          0x0003
#define REG_SYNC1            0x0004
#define REG_SYNC0            0x0005
#define REG_PKTLEN           0x0006
#define REG_PKTCTRL1         0x0007
#define REG_PKTCTRL0         0x0008
#define REG_ADDR             0x0009
#define REG_CHANNR           0x000A
#define REG_FSCTRL1          0x000B
#define REG_FSCTRL0          0x000C
#define REG_FREQ2            0x000D
#define REG_FREQ1            0x000E
#define REG_FREQ0            0x000F
#define REG_MDMCFG4          0x0010
#define REG_MDMCFG3          0x0011
#define REG_MDMCFG2          0x0012
#define REG_MDMCFG1          0x0013
#define REG_MDMCFG0          0x0014
#define REG_DEVIATN          0x0015
#define REG_MCSM2            0x0016
#define REG_MCSM1            0x0017
#define REG_MCSM0            0x0018
#define REG_FOCCFG           0x0019
#define REG_BSCFG            0x001A
#define REG_AGCCTRL2         0x001B
#define REG_AGCCTRL1         0x001C
#define REG_AGCCTRL0         0x001D
#define REG_WOREVT1          0x001E
#define REG_WOREVT0          0x001F
#define REG_WORCTRL          0x0020
#define REG_FREND1           0x0021
#define REG_FREND0           0x0022
#define REG_FSCAL3           0x0023
#define REG_FSCAL2           0x0024
#define REG_FSCAL1           0x0025
#define REG_FSCAL0           0x0026
#define REG_RCCTRL1          0x0027
#define REG_RCCTRL0          0x0028
#define REG_FSTEST           0x0029
#define REG_PTEST            0x002A
#define REG_AGCTEST          0x002B
#define REG_TEST2            0x002C
#define REG_TEST1            0x002D
#define REG_TEST0            0x002E
#define REG_PARTNUM          0x0030
#define REG_VERSION          0x0031
#define REG_FREQEST          0x0032
#define REG_LQI              0x0033
#define REG_RSSI             0x0034
#define REG_MARCSTATE        0x0035
#define REG_WORTIME1         0x0036
#define REG_WORTIME0         0x0037
#define REG_PKTSTATUS        0x0038
#define REG_VCO_VC_DAC       0x0039
#define REG_TXBYTES          0x003A
#define REG_RXBYTES          0x003B
#define REG_RCCTRL1_STATUS   0x003C
#define REG_RCCTRL0_STATUS   0x003D


/*! \brief Class for interfacing with the Chipcon TI CC2500.
 *
 * This class implements basic functions to communicate with the CC2500
 * */
class CC2500
{
private:



public:

    /*!
     * Constructor.
     *
     * \param[in] pinCS Pin number of slave select, default is 10.
     * \param[in] pinSCK Pin number of clock, default is 13 (Arduino standard).
     * \param[in] pinMOSI Pin number of master output slave input, default is 11 (Arduino standard).
     * \param[in] pinMISO Pin number of master input slave output, default is 12 (Arduino standard).
     */
    CC2500();

    /*!
     * Destructor.
     */
    ~CC2500();

	void init();

	void reset();

	void WriteReg(char addr, char value);

	char ReadReg(char addr);
	
    int version();
	
	void ReadBurstReg(unsigned char addr, unsigned char *buffer, int count);

	// For status/strobe addresses, the BURST bit selects between status registers
	// and command strobes.
	char ReadStatusReg(char addr);

	char SendStrobe(char strobe);	
	
	void Read_Config_Regs(void);
};

#endif
