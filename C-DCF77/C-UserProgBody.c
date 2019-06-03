//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// File: C-UserProgBody.C      EMT/MCS	      23.06.2009 Kzy  //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "HC12RegDefsMCS.h"
#pragma interrupt_handler IRQ_Routine	// Interrupt Routine (vgl. HC12IntVecMCS.C)

typedef unsigned char uint8_t;

volatile uint8_t second;

//=== Funktionsdeklarationen ===
// Display:
int LCD4x20C (int LineSel, int CsrPos, char* ChrPntr); //LCD-Display

// expects a buffer of at least size 2
void under100toa(uint8_t num, char* buffer)
{
    buffer[0] = '0' + ((num / 10) % 10);
	buffer[1] = '0' + (num % 10);
}

void print_time(uint8_t hr, uint8_t min, uint8_t secs)
{
    char time_str[] = "xx:xx:xx";
	
	char buffer[2];
	
	under100toa(hr, buffer);
	time_str[0] = buffer[0];
	time_str[1] = buffer[1];

	under100toa(min, buffer);
	time_str[3] = buffer[0];
	time_str[4] = buffer[1];
	
	under100toa(min, buffer);
	time_str[6] = buffer[0];
	time_str[7] = buffer[1];
	
    LCD4x20C(2, 1, (char*)time_str);
}

//=== main ===========================================================================

void main(void) {


//=== Initialisierungen ===

// LCD-Display 
LCD4x20C(0,0,0); // Display löschen

// PORTs , Timer  etc.	 


asm ("CLI"); // Clear Interrupt-Mask (falls IRQ Routine vorhanden)

//=== Hauptroutine =============================================

while(1){ 
	
	} //Abschluss while(1)


// Wenn MAIN kein CLOSED LOOP erfolgt ein kontrollierter Programm-Abbruch
// (mit Meldung auf dem LCD-Display) durch MCSsetupC.S

} //ende main()

//=== end of main ====================================================================




//=== IRQ Interrupt Routine ==========================================================

void IRQ_Routine(void)
{

}
//=== Ende IRQ Interrupt Routine =====================================================