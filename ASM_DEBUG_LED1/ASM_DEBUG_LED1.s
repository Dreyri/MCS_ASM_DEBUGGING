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

PORTS	= 	0x0248	
DDRS	=	0x024A


        .ORG   	$4000		; ASM Code ab 0x4000
START:	LDS     #STACK		; Stackpointer
 	JSR     SetUp		; System SetUp

	
		
MAIN:	LDX     #4
	LDAA    #0b00001000
LOOP:	LSLA 
	STAA    PORTS			  
	JSR     WARTEN		; Warteroutine aufrufen	 
	DBNE    X,LOOP		; 1 Zyklus des Lauflichtes fertig? 
	JMP     MAIN		; Wiederholen (Closed Loop)  
		
;Subroutine

WARTEN: LDY	#500
WLOOP:	JSR     Warte1ms
		DBNE Y,WLOOP	; 500 ms gewartet?
		RTS

; **********************************************************************************

; RESTART VECTOR

	.ORG	$FFFE
	.DW	START		; RESTART VECTOR
