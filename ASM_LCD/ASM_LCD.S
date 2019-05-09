;
; Required USER PROGRAM BODY  when using  MCSsetupASM.S               24.05.2012 Kzy
;

	.AREA	Memory (abs)
STACK	=	$1000		; Nach RESET $1000, nach SetUp $2000=(End of RAM)+1
SetUp	=	$F800		; Suboutine System SetUp

; User Support:		
LCD4x20A=	SetUp+3		; Subroutine LCD4x20A (Version Assembler)
VoltAnz	=	SetUp+6		; Subroutine VoltAnz  (Ausgabe "#,##V")
Warte1ms=	SetUp+9		; Subroutine Warte1ms (Verzögerungszeit tv=1ms)

; **********************************************************************************

PORTA = $0000
DDRA = $0002
BIT3 = 0b00001000
BIT4 = 0b00010000
BIT7 = 0b10000000

PORTB = $0001
DDRB = $0003
BIT0 = 0b00000001

SPICR1 = $00D8
SPICR2 = $00D9
SPIBR = $00DA
SPISR = $00DB
SPIDR = $00DD

        .ORG    $4000		; ASM Code ab 0x4000
START:	LDS	#STACK		; Stackpointer
	JSR	SetUp		; System SetUp

INIT:			; Ihre Systeminitialisierung ....
	
	
SPIINIT:
	BSET DDRA, #BIT3   				; set PA3 = output
	BSET DDRA, #BIT4				; set trigger as output
	MOVB #0b01010100, SPICR1		; SPI enable as master, l => h (CPOL=0, CPHA=1) MSB first
	BCLR PORTA, #BIT3  				; clear value
	
	BCLR DDRB, #BIT0				; comparator auslesen
	
CLEAR_LCD:
    LDX #$FFFE
	JSR LCD4x20A
	LDX #VLINE
	JSR LCD4x20A
	

MAIN:				; Ihr Hauptprogramm ....

    LDAA #0
	JSR DAOUT
    JSR TRIGGER

	JSR APPROX			; ergebnis in register A
	
	LDY #$0207
	JSR VoltAnz
	
	JMP	MAIN		; Closed Loop

DAOUT:     			  		 ; send value in register A
	BCLR PORTA, #BIT3		 ; set to 0 for write
	LSRD					 ; shift left 4 times
	LSRD
	LSRD
	LSRD
	STAA SPIDR				 ; write to SPIDR, trigger data transfer
  LOOP:
	BRCLR SPISR, #BIT7, LOOP  ; wait for data transfer to finish (SPIF=1)
	LDAA SPIDR
	STAB SPIDR
  LOOP2:
    BRCLR SPISR, #BIT7, LOOP2 ; wait again
	LDAB SPIDR	  		 	   ; release SPIF = 0
	BSET PORTA, #BIT3		   ; set to high to finish transfer
	RTS

TRIGGER:
    BSET PORTA, #BIT4
	JSR Warte1ms
	BCLR PORTA, #BIT4
	RTS

APPROX:   ; register B haelt schrittgroesse, register A haelt jetzige approximation
    LDAB #0b10000000		   ; setze die schrittgroesse auf 128
	LDAA #0b10000000
	
    LSRB  					   ; shift rechts um die schrittgroesse genauer zu machen
APPR_LOOP:
	PSHA 					   ; A und B auf stack zwischenspeichern
	PSHB					  
	JSR DAOUT                      ; daout mit unseren wert in A
	JSR Warte1ms			   ; warte 1ms fuer den oscillator oder sowas
	PULB
	PULA					   ; erstelle A und B register, sie wurden kaputt gemacht in DAOUT
	BRCLR PORTB, #BIT0, DECREASE	   ; check comparator, wenn 0 dann ist unser wert zu gross
INCREASE:
    ABA	  		 		       ; erhoehe a mit der schrittgroesse
	JMP AFTER_COND
DECREASE:
    SBA	 		  			   ; erniedrige mit der schrittgroesse 
AFTER_COND:
    LSRB
    CMPB #0 		  			   ; wenn step 0 ist quit
	BNE APPR_LOOP
	; A haellt schon unsere approximation
	RTS
    
	
	
;Ihre konstanten Daten und Text-Strings ....

VLINE: .DB 2,1
       .DW VTXT
VTXT:  .ASCIZ "Ue: ="

; **********************************************************************************

; RESTART VECTOR

	.ORG	$FFFE
	.DW	START		; RESTART VECTOR
