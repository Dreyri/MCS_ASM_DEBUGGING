//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// File: C-UserProgBody.C      EMT/MCS	      23.06.2009 Kzy  //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "HC12RegDefsMCS.h"
#pragma interrupt_handler IRQ_Routine	// Interrupt Routine (vgl. HC12IntVecMCS.C)

//=== Globale Variablen ===
// short GlobalVar1, GlobalVar2=8135; etc. etc.

//=== Funktionsdeklarationen ===
// Display:
int LCD4x20C (int LineSel, int CsrPos, char* ChrPntr); //LCD-Display


void print_ports(unsigned char port_vals) {
    char pb_stat[2];
	unsigned char state;
	pb_stat[1] = '\0';
	
    LCD4x20C(1, 3, "SCHALTER-PB0 = ");
	state = (port_vals & 0b00010000) >> 4;
    pb_stat[0] = state + '0';
	LCD4x20C(1, 18, pb_stat);
	
	LCD4x20C(2, 3, "SCHALTER-PB1 = ");
	state = (port_vals & 0b00100000) >> 5;
	pb_stat[0] = state + '0';
	LCD4x20C(2, 18, pb_stat);
	
    LCD4x20C(3, 3, "SCHALTER-PB2 = ");
	state = (port_vals & 0b01000000) >> 6;
	pb_stat[0] = state + '0';
	LCD4x20C(3, 18, pb_stat);
	
    LCD4x20C(4, 3, "SCHALTER-PB3 = ");
	state = (port_vals & 0b10000000) >> 7;
	pb_stat[0] = state + '0';
	LCD4x20C(4, 18, pb_stat);
}

//=== main ===========================================================================

unsigned char _port_values = 0;

void main(void) {
//=== Variablendeklarationen ===
// short MainLocalVar1, MainLocalVar2=8135; etc. etc.

//=== Initialisierungen ===

// LCD-Display 
LCD4x20C(0,0,0); // Display l�schen

// PORTs , Timer  etc.	 
DDRS = 0xF0;
DDRB = 0x00;


asm ("CLI"); // Clear Interrupt-Mask (falls IRQ Routine vorhanden)

//=== Hauptroutine =============================================

while(1){ 
    PORTS = _port_values = (PORTB & 0x0F) << 4;	
} //Abschluss while(1)


// Wenn MAIN kein CLOSED LOOP erfolgt ein kontrollierter Programm-Abbruch
// (mit Meldung auf dem LCD-Display) durch MCSsetupC.S

} //ende main()

//=== end of main ====================================================================




//=== IRQ Interrupt Routine ==========================================================

void IRQ_Routine(void)
{
	print_ports(_port_values);

//=== Variablendeklarationen ===
// unsigned short IRQ_LocalVar1, IRQ_LocalVar2=8135; etc. etc.

//=== IRQ-Routine =============================================
	
}
//=== Ende IRQ Interrupt Routine =====================================================