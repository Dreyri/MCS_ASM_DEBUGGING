//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// File: C-UserProgBody.C      EMT/MCS	      23.06.2009 Kzy  //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "HC12RegDefsMCS.h"
#pragma interrupt_handler IRQ_Routine	// Interrupt Routine (vgl. HC12IntVecMCS.C)

typedef unsigned char uint8_t;

typedef uint8_t bool;

#define true 1
#define false 0

volatile uint8_t second;
volatile uint8_t minute;
volatile uint8_t hour;

volatile bool start_polling = false;

// expects a buffer of at least size 2
void under100toa(uint8_t num, char* buffer)
{
    buffer[0] = '0' + ((num / 10) % 10);
	buffer[1] = '0' + (num % 10);
}

void u8_to_binary(uint8_t number, char* buffer)
{
    buffer[7] = '0' + (number & 1);
	buffer[6] = '0' + ((number >> 1) & 1);
	buffer[5] = '0' + ((number >> 2) & 1);
	buffer[4] = '0' + ((number >> 3) & 1);
	buffer[3] = '0' + ((number >> 4) & 1);
	buffer[2] = '0' + ((number >> 5) & 1);
	buffer[1] = '0' + ((number >> 6) & 1);
	buffer[0] = '0' + ((number >> 7) & 1);
}

void warte140ms() 
{
    TC0 = TCNT + 125 * 140;
	while ((TFLG1 & 1) == 0) {}
}


//=== Funktionsdeklarationen ===
// Display:
int LCD4x20C (int LineSel, int CsrPos, char* ChrPntr); //LCD-Display


unsigned char poll() {
    char found_str[] = "found - x";
	unsigned char data_bit = (PORTA & 0b00000001);
	unsigned char data_char = '0' + data_bit;

    found_str[8] = data_char;
	LCD4x20C(4, 1, found_str);
}

// time is encoded in an 8 bit uchar, lowest 2 bits seconds, then minute and last hours.
// with upper 2 bits as don't care
void print_time(unsigned char hr, unsigned char min, unsigned char secs)
{
    char time_str[] = "xx:xx:xx";
	
	char buffer[2];
	
	under100toa(hr, buffer);
	time_str[0] = buffer[0];
	time_str[1] = buffer[1];

	under100toa(min, buffer);
	time_str[3] = buffer[0];
	time_str[4] = buffer[1];
	
	under100toa(secs, buffer);
	time_str[6] = buffer[0];
	time_str[7] = buffer[1];
	
    LCD4x20C(2, 1, time_str);
}


//=== main ===========================================================================

void main(void) {


    // init timer easy quick mode
    TSCR1 = 0b10010000;
	TIOS = 0b00000001;
	
	TSCR2 = 0b00000110; // prescaler 8us => 125 * 8 = 1ms

    // LCD-Display 
    LCD4x20C(0,0,0); // Display löschen
    LCD4x20C(1, 1, "DCF77:");
	
    // PORTs , Timer  etc.	
    DDRA = 0b00001000; // set PA3 as output 
    PORTA &= ~0b00001000;
	PORTA |= 0b00001000;


asm ("CLI"); // Clear Interrupt-Mask (falls IRQ Routine vorhanden)

//=== Hauptroutine =============================================

while(1){ 

        if (start_polling) {
		    LCD4x20C(4, 1, "waiting");
		    warte140ms();
			LCD4x20C(4, 1, 0);
			
			poll(); // TODO assign to ring buffer
			
			start_polling = false;
		}
	} //Abschluss while(1)


// Wenn MAIN kein CLOSED LOOP erfolgt ein kontrollierter Programm-Abbruch
// (mit Meldung auf dem LCD-Display) durch MCSsetupC.S

} //ende main()

//=== end of main ====================================================================




//=== IRQ Interrupt Routine ==========================================================

void IRQ_Routine(void)
{
	char porta_str[] = "12345678";
    
	start_polling = true;
	++second;
	
	if (second == 60)
	{
	    ++minute;
		second = 0;
		
		if (minute == 60)
		{
		    ++hour;
			minute = 0;
		}
	}

	print_time(hour, minute, second);
	
	u8_to_binary(PORTA, porta_str);
	
	LCD4x20C(3, 1, porta_str);
	PORTA &= ~0b00001000;
	PORTA |= 0b00001000;
}
//=== Ende IRQ Interrupt Routine =====================================================