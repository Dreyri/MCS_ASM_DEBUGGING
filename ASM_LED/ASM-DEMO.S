;
; User Program ASM-DEMO using  MCSsetupASM              22.04.2013 Kzy
; 
; Lauflicht auf PortS, Programmabbruch mit Schalter-PB0=ON

	.AREA	Memory (abs)
STACK	=	$1000		; Nach RESET $1000, nach SetUp $2000=(End of RAM)+1
SetUp	=	$F800		; Suboutine System SetUp

; User Support:		
LCD4x20A=	SetUp+3		; Subroutine LCD4x20A (Version Assembler)
VoltAnz	=	SetUp+6		; Subroutine VoltAnz  (Ausgabe "#,##V")
Warte1ms=	SetUp+9		; Subroutine Warte1ms (Verzögerungszeit tv=1ms)

; **********************************************************************************

PORTB	=	$0001
PORTS	=	$0248
DDRS	=	$024A


	.ORG	$1000		; Variablen ab 0x1000
ScanBit:.BLKB	1		; Scanner für Lauflicht


        .ORG    $4000		; Program Code ab 0x4000
START:	LDS	#STACK		; Setzen Stackpointer
	JSR	SetUp		; System SetUp

INIT:	MOVB	#0b11110000,DDRS; PortS Bits7..4 = Ausgang
	MOVB	#$80,ScanBit	; MSB(ScanBit) = 1 !

MAIN:	ROR	ScanBit		; Schieben: Bit7 -> Bit0
	LDAA	ScanBit
	STAA	PORTS		; Neue Anzeige
	BSR	W75ms		; Warte 75ms
	
StopIt:	LDAA	PORTB		; Status PortB
	ANDA	#1   		; Filtern: Schalter = PortB, Bit0
	BNE	StopIt		; Schalter an PB0 = 1 ? JA: Programm-Unterbrechen
	JMP	MAIN		; Closed LOOP !

	
W75ms:	LDX	#75  		; 75ms
WLOOP:	JSR	Warte1ms	; Ablauf Verzögerungszeit
	DBNE	X,WLOOP		; 75 x 1ms, fertig ?
	RTS			; Abschluss Subroutine
	 
; **********************************************************************************

	.ORG	$FFFE
	.DW	START		; RESTART Vektor
