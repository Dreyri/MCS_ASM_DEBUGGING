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
	;;  symbols

	PORTS = $0248
	DDRS = $024A
	PORTB = $0001
	DDRB = $0003

        .ORG    $4000		; ASM Code ab 0x4000
START:	LDS	#STACK		; Stackpointer
	JSR	SetUp		; System SetUp

INIT:   MOVB #$F0, DDRS  ; Set PS4-7 as Output
	MOVB #$00, DDRB  ; Set PB0-3 as Input
MAIN:   LDAA PORTB 		; Read state of portb
	ANDA #0b00001111 	; Only read relevant bits

	;;  left shift to match up with PORTS hibits
	LDX #4
LOOP:	LSLA
	DBNE X,LOOP

	;; write to PORTS
	STAA PORTS
	JMP	MAIN		; Closed Loop

; **********************************************************************************

; RESTART VECTOR

	.ORG	$FFFE
	.DW	START		; RESTART VECTOR
