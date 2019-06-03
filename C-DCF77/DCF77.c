//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// File: C-UserProgBody.C      EMT/MCS	      23.06.2009 Kzy  //
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#include "HC12RegDefsMCS.h"
#pragma interrupt_handler IRQ_Routine	// Interrupt Routine (vgl. HC12IntVecMCS.C)

#define MINUTE_BIT_BEGIN 21
#define MINUTE_BIT_LEN 7

#define HOUR_BIT_BEGIN 29
#define HOUR_BIT_LEN 6

typedef unsigned char uint8_t;

typedef uint8_t bool;

typedef unsigned long long uint32_t;

#define true 1
#define false 0

// this buffer can store up to 64 bits



volatile uint8_t second;
volatile uint8_t minute;
volatile uint8_t hour;

// set to true when our main loop should start the 140ms poll cycle
volatile bool start_polling = false;
volatile bool minute_trigger = false; // true if we reset on the next IRQ

uint8_t buffer[60];
uint8_t buffer_index;

// expects a buffer of at least size 2
void under100toa(uint8_t num, char* buffer)
{
    buffer[0] = '0' + ((num / 10) % 10);
	buffer[1] = '0' + (num % 10);
}

// converts an 8 bit unsigned number to a character buffer, buffer must hold at least 8 bits
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
	uint8_t data_bit = (PORTA & 0b00000001);
	unsigned char inv_data_bit = data_bit ^ 0b00000001;
	unsigned char data_char = '0' + inv_data_bit;

    found_str[8] = data_char;
	LCD4x20C(4, 1, found_str);
	
	return inv_data_bit;
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

uint8_t parse_minute(uint8_t* buffer)
{
    uint8_t minute_sum = 0;
	uint8_t i = 0;
	
	for (; i != MINUTE_BIT_LEN; ++i)
	{
	    if (buffer[i])
		{
		    switch (i)
		    {
		        case 0: // 1
			    case 1: // 2
			    case 2: // 4
			    case 3: // 8
				    minute_sum += (1 << i);
					break;
			    case 4: // 10
				    minute_sum += 10;
					break;
			    case 5: // 20
				    minute_sum += 20;
					break;
				case 6: // 40
				    minute_sum += 40;
					break;
		    }
		}
	} 
	
	return minute_sum;
}

// read the 
uint8_t parse_hour(uint8_t* buffer)
{
    uint8_t hour_sum = 0;
    uint8_t i = 0;
	
	for (; i != HOUR_BIT_LEN; ++i)
	{
	    if (buffer[i])
		{
	        switch(i)
		    {
		        case 0:
				case 1:
				case 2:
				case 3:
				    hour_sum += (1 << i);
					break;
				case 4:
				    hour_sum += 10;
					break;
				case 5:
				    hour_sum += 20;
					break;
		    }
		}
	}
    return hour_sum;
}

// parse the contents of the buffer. currently get minutes and hours.
void parse_data(uint8_t* buffer)
{
    minute = parse_minute(&buffer[MINUTE_BIT_BEGIN]);
	hour = parse_hour(&buffer[HOUR_BIT_BEGIN]);
}


//=== main ===========================================================================

void main(void) {
    uint8_t current_data_bit;
	buffer_index = 0;

    // init timer easy quick mode
    TSCR1 = 0b10010000;
	TIOS = 0b00000001;
	
	TSCR2 = 0b00000110; // prescaler 8us => 125 * 8 = 1ms

    // LCD-Display 
    LCD4x20C(0,0,0); // Display l�schen
    LCD4x20C(1, 1, "DCF77:");
	
    // PORTs , Timer  etc.	
    DDRA = 0b00001000; // set PA3 as output 
    PORTA &= ~0b00001000;
	PORTA |= 0b00001000;


asm ("CLI"); // Clear Interrupt-Mask (falls IRQ Routine vorhanden)

//=== Hauptroutine =============================================

while(1){ 
        // we can finally start checking for our next data bit
        if (start_polling) {
		    if (!minute_trigger) // no minute trigger detected, we can get data
			{
		        LCD4x20C(4, 1, "waiting");
		        warte140ms();
			    LCD4x20C(4, 1, 0);
			
			    current_data_bit = poll();
				buffer[second] = current_data_bit;
			}
			else // minute trigger detected, no data will be received and process buffer
			{
			    // after parse the buffer will be reused for the next minute
			    parse_data(buffer);
			}
			minute_trigger = false;
			start_polling = false;
		}
	} //Abschluss while(1)


// Wenn MAIN kein CLOSED LOOP erfolgt ein kontrollierter Programm-Abbruch
// (mit Meldung auf dem LCD-Display) durch MCSsetupC.S

} //ende main()

//=== end of main ====================================================================




//=== IRQ Interrupt Routine ==========================================================

// this interrupt routine sets start_polling and minute_trigger for when these
// events are caught. start_polling indicates to the main loop that it can start
// its 140ms wait to get the data. minute trigger indicates to the main loop that
// it can start processing the data and show the new minute and hour count.
void IRQ_Routine(void)
{
	char porta_str[] = "12345678";
    
	start_polling = true;
	++second;

	print_time(hour, minute, second);
	
	u8_to_binary(PORTA, porta_str);
	
	LCD4x20C(3, 1, porta_str);
	
	// check for minute bit
	if ((PORTA & 0b00000010) == 0)
	{
	    minute_trigger = true;
		second = -1;
	}
	
	PORTA &= ~0b00001000;
	PORTA |= 0b00001000;
}
//=== Ende IRQ Interrupt Routine =====================================================