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
BIT7 = 0b10000000

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
	MOVB #0b01010100, SPICR1		; SPI enable as master, l => h (CPOL=0, CPHA=1) MSB first
	BCLR PORTA, #BIT3  				; ??
	

MAIN:				; Ihr Hauptprogramm ....
	LDAA #$FF
	JSR DAOUT
	LDY #1
	JSR Warte1ms
	LDAA #$00
	JSR DAOUT
	LDY #1
	JSR Warte1ms
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

;Ihre konstanten Daten und Text-Strings ....
	

; **********************************************************************************

; RESTART VECTOR

	.ORG	$FFFE
	.DW	START		; RESTART VECTOR
