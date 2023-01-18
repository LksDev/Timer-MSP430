/*******************************************************************************
;                                                                          
;                  MSP430 C-Programme
;
;*******************************************************************************
; Modulname: DcoSmclkTest.c
;*******************************************************************************
; Prozessor: msp430G2553
;*******************************************************************************
; Initalisierungen: ACLK = n/a, MCLK = SMCLK = default DCO - 8 MHz
;                   
;*******************************************************************************
; Compiler/Assembler: Built with IAR Embedded Workbench Version: 4.10E
;*******************************************************************************
; REV. 1.0  DATE 18.01.2023
;*******************************************************************************
; 
;*******************************************************************************
; Abteilung: Elektrotechnik - Mikroprozessortechnik (MT)
;*******************************************************************************
; Entwicklung: LksDev                                        
;*******************************************************************************
; Aenderungen:                                                            
 																			   *
; Wann:        Beschreibung:                                     Wer:          
--------------------------------------------------------------------------------

*******************************************************************************/
#include <msp430G2553.h>                    // Definitionen fuer den Prozessor 
                                            // laden
//------------------------------------------------------------------------------
//           Symbolische Konstanten und Makros z.B #define LED1 BIT3
//------------------------------------------------------------------------------
#define DELAY 40000
//------------------------------------------------------------------------------
//           Funktionsdeklarationen/-prototypen -> besser auslagern z.B. LCD.h
//------------------------------------------------------------------------------
void DebounceDelay(unsigned int i);
__interrupt void TimerA_ISR(void);

//------------------------------------------------------------------------------
//           Hauptprogramm
//------------------------------------------------------------------------------
           
void main( void )
{
//------------------------------------------------------------------------------
//           Initialisierungsteil von Prozessor und Peripherie
//------------------------------------------------------------------------------ 
  WDTCTL = WDTPW | WDTHOLD; // Stop watchdog-timer
  
  // Basis Clock auf 1MHz einstellen
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
  
  P1DIR |= BIT0 + BIT4;         // Set Pin 1.0 + 1.4 als Ausgang
  P1OUT = 0x00;                 // Ausgang ausschalten
  
  P1SEL |= BIT4;                // Sonderfunktion SMCLK an P1.4 aktivieren
  BCSCTL2 &= ~ BIT1 + BIT2;     // Teiler des SMCLK einstellen
  BCSCTL2 |= BIT1 + BIT2;
  
  // slau144j-Family-Users-Guide MSP430x2xx.pdf Seite 370
  // Timer_A Control Register
  // Bit9-8 01 binär SMCLK
  // Bit7-6 11 /8 Teiler
  // Bit5-4 10 Continous Mode
  // Bit1 0 Disable Interrupt
  // Bit0 x Timer Interrupt Status
  TA0CTL = 0x02E0;
  // Zählerstand des Timers A
  TAR = 0;
  // Interrupt aktivieren + Timer A
  TA0CTL |= TAIE;
  _BIS_SR(GIE); // Enter interrupt
  
//------------------------------------------------------------------------------
//           Endlosschleife, dass kleinste Betriebssystem der Welt!
//------------------------------------------------------------------------------
  for (;;)
  {
    //DebounceDelay(DELAY);
    //P1OUT ^= BIT0;                 // Wiederhole immer
  }
}
            
//------------------------------------------------------------------------------
//        Interrupt-Service-Routinen/ -Handler z.B. 
//------------------------------------------------------------------------------

#pragma TIMER0_A0_VECTOR
__interrupt void TimerA_ISR(void)
{
    unsigned char n = 0;
    n++;
    P1OUT ^= BIT0;                 // Wiederhole immer  
}

//------------------------------------------------------------------------------
//        Funktionsdefinitionen -> besser auslagern z.B. LCD.c
//------------------------------------------------------------------------------
void DebounceDelay(unsigned int i)
{
  while(i>0) 
    i--;
}
