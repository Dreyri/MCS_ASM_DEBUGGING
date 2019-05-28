;
;       L C D 4 x 2 0 C I       Rev. 2.1         29.06.2010 Kazay
;
;       Firmware für LCD-Module mit 4 Zeilen à 20 Zeichen (4x20)
;	(Aufruf aus 68HC12-C-Programmen)
;       4-bit-Mode an Port-K, E-Clock = 25 MHz (max.)
;       geeignet für Displays mit Controller Samsung KS0073
;	Sperrt I-Flag während der Ausführung
;
;	Belegung PIA-Port:
;	ECLK=b7  ;  R/W=b5  ;  RS=b4   ;   DATA: b3..b0
;
;
;       Vor der ersten Ansteuerung (immer nach System RESET) des
;       LCD-Displays muss einmalig eine Initialisierung erfolgen.
;
:
;       Aufruf:  int LCD4x20C (int LineSel, int CsrPos, char* ChrPntr)
;
;       LineSel:
;          =-1 :  Display initialisieren
;          = 0 :  Display löschen
;          = 1 :  Schreiben in 1. Zeile
;          = 2 :  Schreiben in 2. Zeile
;          = 3 :  Schreiben in 3. Zeile
;          = 4 :  Schreiben in 4. Zeile
;          Alle anderen Werte für "LineSel" erzeugen einen Fehlercode
;
;       CsrPos:
;             0    :   Zeile <LineSel> löschen
;         +1...+20 :   Schreibposition 1..20 , Cursor OFF
;         -1...-20 :   Schreibposition 1..20 , Cursor ON
;          Alle anderen Werte für "CsrPos" erzeugen einen Fehlercode
;
;       Text:
;          Pointer auf die auszugebende Zeichenkette.
;          Eine Zeichenkette mit Länge=0 ist zulässig, z.B. um den Cursor an
;          einer bestimmten Position ein- oder auszuschalten.
;          Würde aufgrund der Wahl von CsrPos und der Länge der Zeichenkette
;          über das Zeilenende hinaus geschrieben, wird abgebrochen,
;          und ein Fehler generiert.
;          Darstellbar sind max. 20 Zeichen/Zeile.
;
;
;       Nach Ablauf der FUNCTION LCD4x20C:
;       - FUNCTION = 0 :  kein Fehler 
;       - FUNCTION =-1 :  Fehler !



;****** X-Reg Offsets zeigen auf den Variablenblock ***************
;
;         Stack: (Alle Variablen = 2 Bytes !)
;         ChrPntr  +9
;         CsrPos   +7
;         RETURN   +5   Return Adr    Calling C-Prog.
;         CFrPntr  +3   Frame Pointer Calling C-Prog.
;         LineSel  +1
;    (CCR on CALL)  0	<-SP  CCR (I-Flag!) Calling C-Prog.

        .text


;****** X-Reg Offsets zeigen auf den Variablenblock ******************
LINESEL  =      1               ; Zeile 1..4 oder Clear-/Init-Display
CSRPOS   =      7               ; Schreibposition in Zeile 1...4
CHRPNTR  =      9               ; Zeiger auf Text-String im Memory
;*********************************************************************

;******* I/O - Port Zuweisung (hier PortK) ***************************
REGBAS	=	$0000		; Register Bank Basisadresse
PORTK	=	$32		; PortK I/O
DDRK	=	$33		; PortK Datenrichtungsregister
;*********************************************************************

LCDPIAPRT=	REGBAS+PORTK	; PortK I/O-Adresse
LCDPIADDR=	REGBAS+DDRK	; PortK Datenrichtungsregister-Adresse

READPORT =      0b11110000      ; Data Direction, Port=Input
WRITEPORT=      0b11111111      ; Data Direction, Port=Output

MAXLINE  =      4               ; Max. Anzahl Zeilen
MAXPOS   =      20              ; Max. Cursor Position in der Zeile

DDRAMAD  =      0b10000000      ; DD-RAM Address Byte

MODE4BIT =	0b0010		; 4-Bit Mode ON (as left nipple, right shifted)
EXTFCTON =	0b00100100      ; 4-bit, Extended Function ON (RE=1)
EFCT4LIN =	0b00001001	; 5 Dot Font ; norm. Cur. ; 4 Lines
EXTFCTOFF=      0b00100000      ; 4-bit ; N=x ; RE=0 ; BE=0 ; LP=0 
DSPLCLR  =      0b00000001      ; Display Clear Byte
SHFTCURS =      0b00010100      ; Shift Cursor, Dir=right
CURSON   =      0b00001111      ; Display ON ; Cursor: ON  & BLINK ON
CURSOFF  =      0b00001100      ; Display ON ; Cursor: OFF & BLINK OFF

ADRZOFST =      $20             ; AdrBit-Offset pro Zeile
LCDERR   =      $FFFF           ; -1 ($FFFF) = Fehler

ECLK1    =      0b10000000      ; 1=Display Clock active
RDCRTLREG=      0b00100000      ; ECLK=0(Bit7) , R/W=1(Bit5) , RS=0(Bit4)->CrtlReg
WRCRTLREG=      0b00000000      ; ECLK=0(Bit7) , R/W=0(Bit5) , RS=0(Bit4)->CrtlReg
WRDATAREG=      0b00010000      ; ECLK=0(Bit7) , R/W=0(Bit5) , RS=1(Bit4)->DataReg



_LCD4x20C::
        PSHX                    ; Save Frame Pointer (calling C-Prog)
        XGDX                    ; LineSel -> <X>
        PSHX                    ; LineSel -> Stack
	TFR	CCR,A		; CCR -> Stack (Hier: I-Flag !) 
	SEI			; und
	PSHA			; IRQs sperren.

        CPX     #0              ; Display Löschen wenn LineSel=0
        BEQ     CLRDSPL
	CPX     #$FFFF          ; Initialisierung wenn LineSel=-1=$FFFF
	BNE	CHKLINE


LCDINI: LDAB    #WRITEPORT      ; Datenrichtung setzen (=Ausgeben)
        STAB    LCDPIADDR
	LDAB    #RDCRTLREG      ; E=0, R/W=1
        STAB    LCDPIAPRT
        JSR     Delay20ms	; Warten nach RESET oder PwrOn

	LDAA	#WRCRTLREG      ; E=0, R/W=0
	LDAB	#MODE4BIT	; 4-Bit-Mode Setzen (nur 1*Write)
	JSR	XMT4BIT		; 1*Write, da noch im 8-Bit-Mode
	JSR	Delay2ms	; 2ms (min. 39µs) bis BUSY=0

        LDAB    #EXTFCTON       ; 4-bit, Extended Function ON (RE=1)
        JSR     WCTLREG
	LDAB	#EFCT4LIN	; 5 dot font ; norm. Cur. ; 4 Lines
        JSR     WCTLREG
	LDAB	#EXTFCTOFF	; 4-bit ; N=x ; RE=0 ; BE=0 ; LP=0 
        JSR     WCTLREG

	LDAB	#SHFTCURS	; Shift Cursor ON, Dir=right
        JSR     WCTLREG
	LDAB	#CURSOFF	; Display ON ; Cursor: OFF & BLINK OFF
        JSR     WCTLREG

CLRDSPL:LDAB    #DSPLCLR        ; Clear Display
        BSR     WCTLREG
        BRA     EXITOK


CHKLINE:TSX                     ; My Frame Pointer = <X-Reg>
        TST     LINESEL,X       ; Das higher Byte von LinSel
        BNE     EXITERR         ; muss jetzt = 0 sein !

        LDAA    LINESEL+1,X     ; Zeile ?
        CMPA    #MAXLINE        ; Zeile>4 -> ERROR
        BHI     EXITERR
	DECA			; Aufbau Zeile 0..3 aus 1..4
        LSLA			; Zeile      1   2   3   4    als 
	LSLA			; =RAM-Adr  $00 $20,$40,$60
	LSLA			;                in ACCA
	LSLA			;
	LSLA			;   =Z 0..3 * 16(left nipple) * 2(RAM-Addr)


CHKCPOSHIGH:			; Cursor Position in Zeile OK ?
        LDAB    CSRPOS,X	; Das higher Byte von CSRPOS
        BEQ	CHKCPOS         ; muss entweder = 0 sein (Cursor OFF)
	CMPB	#$FF		; oder = -1=$FF sein (Cursor ON)
	BNE	EXITERR		; sonst ERROR !

CHKCPOS:LDAB    CSRPOS+1,X      ; Cursor Position in Zeile OK ?
	BPL	CHKIFOK
	NEGB                    ; Aufbau Spalte 1..20 aus -1..-20
CHKIFOK:CMPB    #MAXPOS         ; Fehler, wenn CSRPOS>20
        BHI     EXITERR

	PSHB			; |CSRPOS| -> Stack
	LDAB	#CURSOFF	; Cursor AUS
        TST	CSRPOS+1,X
	BPL	SETCSR
	LDAB	#CURSON		; Cursor EIN

SETCSR:	BSR     WCTLREG		; Cursor EIN oder AUS

	PULB			; |CSRPOS| <- Stack
	TSTB			; CSRPOS=0 ?
	BNE	CALCADR

	LDY	#BlankLine	; CSRPOS=0 :  Zeile löschen !
	STY     CHRPNTR,X       ; Zeiger "Text-Adr. in Memory" umbiegen
	INCB			; CSRPOS korrigieren auf Pos 1

CALCADR:DECB			; Aufbau Spalte 0..19 aus 1..20
        PSHB

        ABA  			; Aufbau RAM Adr in ACCB
	TAB                     
        ORAB    #DDRAMAD        ; DD-RAM Adresse selektieren
        BSR     WCTLREG         ; Adresse in DD-RAM setzen

        PULB
        LDAA    #MAXPOS		; Errechnen: Max. Anzahl Zeichen ab CSRPOS
        SBA                     ; bis Zeilenende (SBA=A-B->A)

        LDY     CHRPNTR,X       ; Zeiger auf Text-Adresse in Memory
WLOOP:  LDAB    0,Y             ; Zeichen holen
        BEQ     EXITOK          ; $00 terminiert die Ausgabe
        BSR     WDATA           ; Zeichen ausgeben, max. bis Zeilenende.
	INY                     ; Nächstes Zeichen
        DECA                    
        BNE     WLOOP

	TST     0,Y		; Terminator an Pos 21 ?
        BNE     EXITERR


EXITOK: PULA			; Zurückholen des I-Flags, wie bei Aufruf
	TFR	A,CCR
	PULX
        PULX                    ; Restore Frame Pointer (calling C-Prog)
	LDD     #0              ; < D>=0 -> kein Fehler
        RTS


EXITERR:PULA			; Zurückholen des I-Flags, wie bei Aufruf
	TFR	A,CCR
	PULX
        PULX                    ; Restore Frame Pointer (calling C-Prog)
	LDD     #LCDERR         ; <D>=-1 ($FFFF) -> FEHLER
        RTS




;       Schreiben in LCD Daten-Register (WDATA)
;	oder      in LCD-Contorl-Register (WCTLREG)
;       <ACCA>=Steuerleitungen , <ACCB>=Datenwort

WDATA:  PSHA                    
        LDAA    #WRDATAREG      ; 1 Zeichen -> DD RAM
        BRA     XMTLCD 

WCTLREG:PSHA
        LDAA    #WRCRTLREG      ; Control Reg setzen

XMTLCD: PSHB			; Datenwort zwischenspeichern in Stack
        BSR     CHKBUSY         ; Busy ? 
        LDAB    #WRITEPORT      ; Datenrichtung setzen (=Ausgeben)
        STAB    LCDPIADDR
       
	PULB			; Datenwort aus Stack
	PSHB
	LSRB   			; Left Nipple in Position bringen
	LSRB
	LSRB
	LSRB
	BSR	XMT4BIT
       
        PULB			; Datenwort aus Stack
	PSHB 
	ANDB	#$0F		; Right Nipple in Position bringen
	BSR	XMT4BIT
   
        PULB
	PULA
        RTS

;       Schreiben 4 Bit(=ACCB)->LCD-Daten bzw. CRTL-Register-Adr(=ACCA)
XMT4BIT:PSHA
        ABA
	STAA    LCDPIAPRT	; WRDATAREG bzw. WRCRTLREG, ECLK=0
        PSHA                 	; Zwischenspeichern WRDATAREG bzw. WRCRTLREG mit ECLK=0
        ORAA    #ECLK1          ; nach min. 40ns ECLK=1
        STAA    LCDPIAPRT
        BSR     Delay250ns
	PULA
        STAA    LCDPIAPRT       ; ECLK=0 !
        BSR     Delay250ns
	PULA
	RTS


;       Warten, bis BUSY-Flag des LCD-Displays = 0
CHKBUSY:LDAB    #READPORT       ; Datenrichtung setzen (=Einlesen)
        STAB    LCDPIADDR

BUSY:	LDAB    #RDCRTLREG      ; CRTL-Bits auf lesen ControlReg setzen
        STAB    LCDPIAPRT    
	ORAB    #ECLK1          ; nach min. 40ns ECLK=1
        STAB    LCDPIAPRT      
        BSR     Delay250ns
        LDAB    LCDPIAPRT       ; Right-Nipple-MSB=Busy
        PSHB
        LDAB    #RDCRTLREG 
        STAB    LCDPIAPRT       ; ECLK=0
        BSR     Delay250ns
        ORAB    #ECLK1          ; ECLK=1 , Read-Operation "Right Nipple"
        STAB    LCDPIAPRT      
        BSR     Delay250ns
        LDAB    #RDCRTLREG
        STAB    LCDPIAPRT       ; ECLK=0
        BSR     Delay250ns
	PULB
	ANDB    #0b00001000	; Chk BusyFlag (=Right-Nipple-MSB)
        BNE     BUSY
        RTS


Delay20ms:                      ; 20ms -> 160.000 Zykl bei E=8MHz
        PSHX                    ; 1 Loop = 4 Zykl -> 40.000 Loops erf.
        LDX	#40000          ; Durchlauf = 40.000 Loops
	BRA	DelayLoop

Delay2ms:                       ; 2ms -> 16.000 Zykl bei E=8MHz
        PSHX                    ; 1 Loop = 4 Zykl -> 4.000 Loops erf.
        LDX	#4000           ; Durchlauf = 4.000 Loops

DelayLoop:                       
        DEX                     
        BNE	DelayLoop
        PULX

Delay250ns:                     ; 250ns -> 2 Zykl bei E=8MHz
        RTS                     ; BSR=3Zykl; RTS=5Zykl -> Summe: 8Zykl=1ms>250ns!


BlankLine:
	.ASCIZ	"                    "
