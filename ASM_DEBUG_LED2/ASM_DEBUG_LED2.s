;
; Required USER PROGRAM BODY  when using  MCSsetupASM.S               26.01.2017 TG
;

	.AREA	Memory (abs)
STACK	=	$1000		; Nach RESET $1000, nach SetUp $2000=(End of RAM)+1
SetUp	=	$F800		; Suboutine System SetUp

; User Support:		
LCD4x20A=	SetUp+3		; Subroutine LCD4x20A (Version Assembler)
VoltAnz	=	SetUp+6		; Subroutine VoltAnz  (Ausgabe "#,##V")
Warte1ms=	SetUp+9		; Subroutine Warte1ms (Verzögerungszeit tv=1ms)

; **********************************************************************************


PortS	=	$0248		; PortS I/O Adresse
DDRS	=	$024A		; PortS Datenrichtungsregister


        .ORG    $4000
START:	LDS	#STACK		  	; Stackpointer
	JSR	SetUp		  	; System SetUp

INIT:	MOVB 	#0b11110000,DDRS	; PortS Bits7..4 = Ausgang
	LDX	Phasen			; Zeiger auf Bitmuster-Phase 1 & 2



MAIN:  	LDAA	0,X			; 1. Phase Laden
	STAA	PortS           	; Ausgabe Bitmuster auf PortS
        JMP	WAIT			; 500ms warten
	LDAA    0,X			; 2. Phase Laden
	STAA	PortS          		; Ausgabe Bitmuster auf PortS 
	JMP	WAIT			; 500ms warten		       
	BRA     MAIN           		; Wiederholen (Close Loop)
	

WAIT:	LDY	#500			; 500ms
WLOOP:	JSR	Warte1ms	        ; Ablauf Verzögerungszeit
		DBNE Y,WLOOP		; 500 x 1ms, fertig ?
		RTS			; Abschluss Subroutine


; **********************************************************************************		
; Konstanten

Phasen:	.DB	0b01010000	; Bits für Blinkphase 1 
	.DB	0b10100000	; Bits für Blinkphase 2 


; **********************************************************************************

	.ORG	$FFFE
	.DW	START		; RESTART VECTOR
