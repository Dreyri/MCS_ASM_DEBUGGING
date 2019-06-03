#define __HC12RegDefsMCS_H	//  MCS Praktikum

/* IO_BASE ist die Register Block Basisadresse */

#define _IO_BASE	0 	// Register Block Basisadresse
#define _P(off)		*(unsigned char volatile *)(_IO_BASE + off)
#define _LP(off)	*(unsigned short volatile *)(_IO_BASE + off)

// Port A:
#define PORTA	_P(0x000)	// Port A I/O
#define	DDRA	_P(0x002)	// Port A Data Direction

// Port B:
#define	PORTB	_P(0x001)	// Port B I/O
#define	DDRB	_P(0x003)	// Port B Data Direction

// Port AB:
#define PORTAB	_LP(0x000)	// Port A+B I/O (16Bit)
#define	DDRAB	_LP(0x002)	// Port A+B Data Direction (16Bit)

// Port A/B Pull Up Control:
#define	PUCR	_P(0x00C)	// Pull Up Wid Enable für PORTS A,B,E u. K

// Port S:
#define	PORTS	_P(0x248)	// Port S I/O
#define	PTIS	_P(0x249)	// Port S Input Only
#define	DDRS	_P(0x24A)	// Port S Data Direction
#define	RDRS	_P(0x24B)	// Port S Reduced Drive Register
#define	PERS	_P(0x24C)	// Port S Pull Device Enable Register
#define	PPSS	_P(0x24D)	// Port S Polarity Select Register
#define	WOMS	_P(0x24E)	// Port S Wired Or Mode Register

// SCI Interface:
#define	SCIBD	_LP(0x0C8) 	// SCI Baud Rate Register (16Bit)
#define	SCICR1	_P(0x0CA) 	// SCI Control Register 1
#define	SCICR2	_P(0x0CB) 	// SCI Control Register 2
#define	SCISR1	_P(0x0CC) 	// SCI Status Register 1
#define	SCISR2	_P(0x0CD) 	// SCI Status Register 2
#define	SCIDRH	_P(0x0CE) 	// SCI Data Register (high)
#define	SCIDRL	_P(0x0CF) 	// SCI Data Register (low)

// SPI Interface:
#define	SPICR1	_P(0x0D8)	// SPI Control Register 1
#define	SPICR2	_P(0x0D9)	// SPI Control Register 2
#define	SPIBR	_P(0x0DA)	// SPI Baud Rate Register
#define	SPISR	_P(0x0DB)	// SPI Status Register
#define	SPIDR	_P(0x0DD)	// SPI Data Register

// Timer:
#define	TIOS	_P(0x040)	// Timer Inp Capture / Outp Compare Selection
#define	TCNT	_LP(0x044)	// Timer Counter Register (16Bit)
#define	TSCR1	_P(0x046)	// Timer System Control Register 1
#define	TSCR2	_P(0x04D)	// Timer System Control Register 2
#define	TFLG1	_P(0x04E)	// Main Timer Interrupt Flag Register 1
#define	TFLG2	_P(0x04F)	// Main Timer Interrupt Flag Register 2
#define	TC0	_LP(0x050)	// Timer Compare Register 0 (16Bit)
#define	TC1	_LP(0x052)	// Timer Compare Register 1 (16Bit)
#define	TC2	_LP(0x054)	// Timer Compare Register 2 (16Bit)
#define	TC3	_LP(0x056)	// Timer Compare Register 3 (16Bit)
#define	TC4	_LP(0x058)	// Timer Compare Register 4 (16Bit)
#define	TC5	_LP(0x05A)	// Timer Compare Register 5 (16Bit)
#define	TC6	_LP(0x05C)	// Timer Compare Register 6 (16Bit)
#define	TC7	_LP(0x05E)	// Timer Compare Register 7 (16Bit)


// CAN Bus Interface:
#define CANCTL0  _P(0x0140)		 // MSCAN Control Register 0
#define CANCTL1  _P(0x0141)		 // MSCAN Control Register 1
#define CANBTR0  _P(0x0142)		 // MSCAN Bus Timing Register 0	
#define CANBTR1  _P(0x0143)		 // MSCAN Bus Timing Register 1	
#define CANRFLG  _P(0x0144)		 // MSCAN Receiver Flag Register
#define CANRIER  _P(0x0145)		 // MSCAN Receiver Interrupt Enable Register
#define CANTFLG  _P(0x0146)		 //	MSCAN Transmitter Flag Register
#define CANTIER  _P(0x0147)		 // MSCAN Transmitter Interrupt Enable Register
#define CANTARQ  _P(0x0148)		 // MSCAN Transmitter Message Abort Request
#define CANTAAK  _P(0x0149)		 // MSCAN Transmitter Message Abort Control


#define CANTBSEL _P(0x014A)		 // MSCAN Transmit Buffer Selection Register
#define CANIDAC  _P(0x014B)		 // MSCAN Identifier Acceptance Control Register

//#define Reserved _P(0x014C) 	 // These registers are reserved for factory 
//#define Reserved _P(0x014D)	 // testing of the MSCAN module

#define CANRXERR _P(0x014E)		 // MSCAN Receive Error Counter Register
#define CANTXERR _P(0x014F)		 // MSCAN Transmit Error Counter Register

#define CANIDAR0 _P(0x0150)		 // MSCAN Identifier Acceptance Registers (1st Bank)
#define CANIDAR1 _P(0x0151)		 // MSCAN Identifier Acceptance Registers (1st Bank)
#define CANIDAR2 _P(0x0152)		 // MSCAN Identifier Acceptance Registers (1st Bank)
#define CANIDAR3 _P(0x0153)		 // MSCAN Identifier Acceptance Registers (1st Bank)

#define CANIDMR0 _P(0x0154)		 // MSCAN Identifier Mask Registers (1st Bank)
#define CANIDMR1 _P(0x0155)		 // MSCAN Identifier Mask Registers (1st Bank)
#define CANIDMR2 _P(0x0156)		 // MSCAN Identifier Mask Registers (1st Bank)
#define CANIDMR3 _P(0x0157)		 // MSCAN Identifier Mask Registers (1st Bank)

#define CANIDAR4 _P(0x0158)		 // MSCAN Identifier Acceptance Registers (2nd Bank)
#define CANIDAR5 _P(0x0159)		 // MSCAN Identifier Acceptance Registers (2nd Bank)
#define CANIDAR6 _P(0x015A)		 // MSCAN Identifier Acceptance Registers (2nd Bank)
#define CANIDAR7 _P(0x015B)		 // MSCAN Identifier Acceptance Registers (2nd Bank)

#define CANIDMR4 _P(0x015C)		 // MSCAN Identifier Mask Registers (2nd Bank)
#define CANIDMR5 _P(0x015D)		 // MSCAN Identifier Mask Registers (2nd Bank)
#define CANIDMR6 _P(0x015E)		 // MSCAN Identifier Mask Registers (2nd Bank)
#define CANIDMR7 _P(0x015F)		 // MSCAN Identifier Mask Registers (2nd Bank)

#define CANRXFG    _ADDR(0x0160) // MSCAN Foreground Receive Buffer
#define CANRXIDR0   _P(0x160)	 // MSCAN Foreground Receive Buffer Identifier Register 0
#define CANRXIDR1   _P(0x161)	 // MSCAN Foreground Receive Buffer Identifier Register 1
#define CANRXIDR2   _P(0x162)	 // MSCAN Foreground Receive Buffer Identifier Register 2
#define CANRXIDR3   _P(0x163)	 // MSCAN Foreground Receive Buffer Identifier Register 3
#define CANRXDSR0   _P(0x164)	 // MSCAN Foreground Receive Buffer Data Segment Register 0
#define CANRXDSR1   _P(0x165)	 // MSCAN Foreground Receive Buffer Data Segment Register 1
#define CANRXDSR2   _P(0x166)	 // MSCAN Foreground Receive Buffer Data Segment Register 2
#define CANRXDSR3   _P(0x167)	 // MSCAN Foreground Receive Buffer Data Segment Register 3
#define CANRXDSR4   _P(0x168)	 // MSCAN Foreground Receive Buffer Data Segment Register 4
#define CANRXDSR5   _P(0x169)	 // MSCAN Foreground Receive Buffer Data Segment Register 5
#define CANRXDSR6   _P(0x16A)	 // MSCAN Foreground Receive Buffer Data Segment Register 6
#define CANRXDSR7   _P(0x16B)	 // MSCAN Foreground Receive Buffer Data Segment Register 7
#define CANRXDLR    _P(0x16C)	 // MSCAN Foreground Receive Buffer Data Length Register

#define CANTXFG    _ADDR(0x0170) // MSCAN Foreground Transmit Buffer
#define CANTXIDR0   _P(0x170)	 // MSCAN Foreground Transmit Buffer Identifier Register 0
#define CANTXIDR1   _P(0x171)	 // MSCAN Foreground Transmit Buffer Identifier Register 1
#define CANTXIDR2   _P(0x172)	 // MSCAN Foreground Transmit Buffer Identifier Register 2
#define CANTXIDR3   _P(0x173)	 // MSCAN Foreground Transmit Buffer Identifier Register 3
#define CANTXDSR0   _P(0x174)	 // MSCAN Foreground Transmit Buffer Data Segment Register 0
#define CANTXDSR1   _P(0x175)	 // MSCAN Foreground Transmit Buffer Data Segment Register 1
#define CANTXDSR2   _P(0x176)	 // MSCAN Foreground Transmit Buffer Data Segment Register 2
#define CANTXDSR3   _P(0x177)	 // MSCAN Foreground Transmit Buffer Data Segment Register 3
#define CANTXDSR4   _P(0x178)	 // MSCAN Foreground Transmit Buffer Data Segment Register 4
#define CANTXDSR5   _P(0x179)	 // MSCAN Foreground Transmit Buffer Data Segment Register 5
#define CANTXDSR6   _P(0x17A)	 // MSCAN Foreground Transmit Buffer Data Segment Register 6
#define CANTXDSR7   _P(0x17B)	 // MSCAN Foreground Transmit Buffer Data Segment Register 7
#define CANTXDLR    _P(0x17C)	 // MSCAN Foreground Transmit Buffer Data Length Register
#define CANTXTBPR   _P(0x017D)	 // MSCAN Foreground Transmit Buffer Transmit Buffer Priority Register