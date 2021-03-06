//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// File: C-UserProgBody.C      EMT/MCS	      23.06.2009 Kzy  //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "HC12RegDefsMCS.h"
#pragma interrupt_handler IRQ_Routine	// Interrupt Routine (vgl. HC12IntVecMCS.C)

//=== Funktionsdeklarationen ===
// Display:
int LCD4x20C (int LineSel, int CsrPos, char* ChrPntr); //LCD-Display

typedef struct _CanMessage
{
    unsigned int id; // id of the msg
	unsigned int length; // length of the msg
	int rtr; // rtr of the message
	char data[8]; // msg data
} CanMessage;

char caniflag = 0; // next message read
volatile CanMessage message;
const char* clear_data = "        "; // 8 byte + '\0'

char* reverse_str(char* str, unsigned char length)
{
    char tmp;
    unsigned char first = 0;
	unsigned char last = length - 1;
	
	while (first < last)
	{
	    tmp = *(str + first);
		*(str + first) = *(str + last);
		*(str + last) = tmp;
		first++;
		last--;
	}
	
	return str;
}

// convert uint to null terminated base 10 string
char* itoa(unsigned int value, char* str) {
    unsigned char i = 0;
	unsigned char r = 0;
	unsigned int n = value;
    while (n)
	{
	    r = n % 10;
		
		str[i++] = '0' + r;
		
		n = n / 10;
	}
	
	if (value == 0)
	{
	    str[i++] = '0';
	}
	
	str[i] = '\0';
	
	return reverse_str(str, i);
}

void init_can(void)
{
    CANCTL0 = 0b00000001; // start init
	CANCTL1 = 0b10000000; // CAN enable, clock=osc, normal mode
	
	CANBTR0 = 0b00000001; // SJW-1 = 0, Vorteiler-1 = 1 (500kb/s)
	CANBTR1 = 0b01001001; // not sure about these segments?
	
	CANIDAC = 0b00010000; // 4x 16bit acceptance filter
	
	CANIDAR0 = 0b00001100; // AkFilter1
	CANIDAR1 = 0b10000000; // ID = 100
	
	CANIDMR0 = 0b00000000; // alle 11 bit ueberpruefen + IDE + RTR
	CANIDMR1 = 0b00000111;
	
	CANIDAR2 = 0b00011001; // AkFilter2
	CANIDAR3 = 0b00000000; // ID = 200
	
	CANIDMR2 = 0b00000000;
	CANIDMR3 = 0b00000111; // alle 11 bit ueberpruefen + IDE + RTR
	
	CANCTL0 = 0b00000000; // init mode off
	
	CANRIER = 0b00000001; // CAN interrupt durch RXFIE
}
//=== main ===========================================================================

void main(void) {
//=== Variablendeklarationen ===
char buffer[16];
unsigned char i; // iterator index
unsigned int old_length; // length of previous message, used for clearing lcd 

//=== Initialisierungen ===
init_can();
// LCD-Display 
LCD4x20C(0,0,0); // Display l�schen

// PORTs , Timer  etc.	 


asm ("CLI"); // Clear Interrupt-Mask (falls IRQ Routine vorhanden)

// write the identifier, data, length bit once
LCD4x20C(1, 2, "ID");
LCD4x20C(2, 2, "DATA");
LCD4x20C(3, 2, "LEN");

//=== Hauptroutine =============================================

while(1){ 
        if (caniflag && message.id == 200)
		{
		    if (message.length < old_length)
			{
			    // clear the data line or we'll get old data
				LCD4x20C(2, 7, (char*) clear_data);
			}
			
			old_length = message.length;
		
		    itoa(message.id, buffer);
		    LCD4x20C(1, 7, buffer);
			
			// copy over our message into the buffer
			for (i = 0; i < message.length; ++i)
			{
			    buffer[i] = message.data[i];
			}
			
			buffer[i] = '\0';
			
			LCD4x20C(2, 7, buffer);
			itoa(message.length, buffer);
			LCD4x20C(3, 7, buffer);
			
			caniflag = 0; // reset can interrupt flag
		}
	
	} //Abschluss while(1)


// Wenn MAIN kein CLOSED LOOP erfolgt ein kontrollierter Programm-Abbruch
// (mit Meldung auf dem LCD-Display) durch MCSsetupC.S

} //ende main()

//=== end of main ====================================================================




//=== IRQ Interrupt Routine ==========================================================

void IRQ_Routine(void)
{
    int i;
	volatile unsigned char* can_ptr;
		
	message.id = (CANRXIDR0 * 8) + (CANRXIDR1 >> 5);
	message.rtr = (CANRXIDR1 & 0b00010000); // receive RTR
	
	message.length = CANRXDLR;
	
	if (message.length > 0)
	{
	    can_ptr = &CANRXDSR0;
		
		for(i = 0; i < message.length; i++)
		{
		    message.data[i] = can_ptr[i];
		}
	}
	
	CANRFLG = 0b00000001;
	caniflag = 1;
}
//=== Ende IRQ Interrupt Routine =====================================================