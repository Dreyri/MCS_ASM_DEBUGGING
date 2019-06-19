/* INTERRUPT TABELLE für die MC9S12D64 CPU	27.02.2007 Kzy
 *
 * Diese Datei in die Liste der Projekt-Dateien aufnehmen
 * Ausnahme:	Single Source File mit File->CompileToOutput
 *		In diesem Fall diese Datei m.H. von #include einbinden.
 *
 * Anwendungsbeispiel IRQ:
 * Deklarieren Sie in Ihrem Programm
 *	#pragma interrupt_handler <IRQ_HandlerName>
 *	void <IRQ_HandlerName>(void)
 *	{ program code .. }
 *
 * Fügen Sie hier in diesem Programm hinzu:
 *	extern void <IRQ_HandlerName>();
 * und ändern Sie in der Zeile IRQ ($FFF2)
 *	DUMMY_ENTRY bzw. _INT_Err in  <IRQ_HandlerName>
 */

#pragma nonpaged_function _START
extern void _START(void);	// Program START Entry Point in MCSsetupC.S

#pragma nonpaged_function _ShutDown
extern void _ShutDown(void);	// SWI (ShutDown) Entry Point in MCSsetupC.S

extern void IRQ_Routine(void);	// Vordefiniert IRQ (s. C-UserProgBody.C)

// Entweder  DUMMY_ENTRY  oder  INT_ERR bei Verwendung von MCSsetupC.S !!
// #define DUMMY_ENTRY (void (*)(void))0xFFFF  // $FF = Cleared FLASH ROM Cell

#pragma nonpaged_function _INT_Err
extern void _INT_Err(void);	// Undefined Interrupt Entry Point in MCSsetupC.S

#pragma abs_address:0xFF80 	// Start of Vector Tab
void (*interrupt_vectors[])(void) = 
	{
	_INT_Err, /*Reserved .........................	$FF80*/
	_INT_Err, /*Reserved .........................	$FF82*/
	_INT_Err, /*Reserved .........................	$FF84*/
	_INT_Err, /*Reserved .........................	$FF86*/
	_INT_Err, /*Reserved .........................	$FF88*/
	_INT_Err, /*Reserved .........................	$FF8A*/

	_INT_Err, /*PWM Emergency Shutdown ...........	$FF8C*/
	_INT_Err, /*Port P Interrupt .................	$FF8E*/

	_INT_Err, /*Reserved .........................	$FF90*/
	_INT_Err, /*Reserved .........................	$FF92*/
	_INT_Err, /*Reserved .........................	$FF94*/
	_INT_Err, /*Reserved .........................	$FF96*/
	_INT_Err, /*Reserved .........................	$FF98*/
	_INT_Err, /*Reserved .........................	$FF9A*/
	_INT_Err, /*Reserved .........................	$FF9C*/
	_INT_Err, /*Reserved .........................	$FF9E*/
	_INT_Err, /*Reserved .........................	$FFA0*/
	_INT_Err, /*Reserved .........................	$FFA2*/
	_INT_Err, /*Reserved .........................	$FFA4*/
	_INT_Err, /*Reserved .........................	$FFA6*/
	_INT_Err, /*Reserved .........................	$FFA8*/
	_INT_Err, /*Reserved .........................	$FFAA*/
	_INT_Err, /*Reserved .........................	$FFAC*/
	_INT_Err, /*Reserved .........................	$FFAE*/

	_INT_Err, /*MSCAN 0 Transmit .................	$FFB0*/
	IRQ_Routine, /*MSCAN 0 Receive ..................	$FFB2*/
	_INT_Err, /*MSCAN 0 Error ....................	$FFB4*/
	_INT_Err, /*MSCAN 0 Wake-up ..................	$FFB6*/
	_INT_Err, /*Flash ............................	$FFB8*/
	_INT_Err, /*EEPROM ...........................	$FFBA*/

	_INT_Err, /*Reserved .........................	$FFBC*/
	_INT_Err, /*Reserved .........................	$FFBE*/

	_INT_Err, /*IIC Bus ..........................	$FFC0*/
	_INT_Err, /*BDLC .............................	$FFC2*/
	_INT_Err, /*SCME .............................	$FFC4*/
	_INT_Err, /*CRG Self Clock Mode ..............	$FFC6*/
	_INT_Err, /*Pulse AccuB Overflow .............	$FFC8*/
	_INT_Err, /*Modulus Down Counter Underflow ...	$FFCA*/
	_INT_Err, /*Port H Interrupt .................	$FFCC*/
	_INT_Err, /*Port J Interrupt .................	$FFCE*/
	_INT_Err, /*ATD1 .............................	$FFD0*/
	_INT_Err, /*ATD0 .............................	$FFD2*/
	_INT_Err, /*SCI1 .............................	$FFD4*/
	_INT_Err, /*SCI0 .............................	$FFD6*/
	_INT_Err, /*SPI0 .............................	$FFD8*/
	_INT_Err, /*Pulse Accumulator A Input Edge ...	$FFDA*/
	_INT_Err, /*Pulse Accumulator A Overflow .....	$FFDC*/
	_INT_Err, /*Timer Overflow ...................	$FFDE*/
	_INT_Err, /*Timer Channel 7 ..................	$FFE0*/
	_INT_Err, /*Timer Channel 6 ..................	$FFE2*/
	_INT_Err, /*Timer Channel 5 ..................	$FFE4*/
	_INT_Err, /*Timer Channel 4 ..................	$FFE6*/
	_INT_Err, /*Timer Channel 3 ..................	$FFE8*/
	_INT_Err, /*Timer Channel 2 ..................	$FFEA*/
	_INT_Err, /*Timer Channel 1 ..................	$FFEC*/
	_INT_Err, /*Timer Channel 0 ..................	$FFEE*/
	_INT_Err, /*Real Time Interrupt ..............	$FFF0*/
     _INT_Err, /*IRQ ..............................	$FFF2*/
    	_INT_Err, /*XIRQ .............................	$FFF4*/
       _ShutDown, /*SWI ..... (vgl. MCSsetupC.S) .....	$FFF6*/
	_INT_Err, /*Unimplement Intruction Trap ......	$FFF8*/
	_INT_Err, /*COP failure reset ................	$FFFA*/
	_INT_Err, /*Clock monitor fail reset .........	$FFFC*/
          _START, /*RESET ... (vgl. MCSsetupC.S) .....	$FFFE*/
	};
#pragma end_abs_address
