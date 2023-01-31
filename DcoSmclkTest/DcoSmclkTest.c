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
void TimerAVorladewertLaden(void);
__interrupt void TimerA_ISR(void);
enum RGBLED {Red=0x02, Blue = 0x20, Green = 0x08, Reset = 0x2F, Off = 0x00};
volatile unsigned char uchIntCount;
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
  BCSCTL2 &= ~ BIT1 + BIT2;     // Teiler des SMCLK einstellen -> /1
  
  
  P1DIR |= BIT0 + BIT4;         // 1.4 als Ausgang
  P1OUT = 0x00;                 // Ausgang ausschalten
  P1SEL |= BIT4;                // Sonderfunktion SMCLK an P1.4 aktivieren
  
  P2DIR |= BIT1 + BIT3 + BIT5;
  P2OUT = 0x00;
  
  
   
  // slau144j-Family-Users-Guide MSP430x2xx.pdf Seite 370
  // Timer_A Control Register
  // Bit9-8 01 binär SMCLK
  // Bit7-6 11 /8 Teiler
  // Bit5-4 10 Continous Mode
  // Bit1 0 Enable Interrupt
  // Bit0 x Timer Interrupt Status  
  TACTL = 0x0000;  
  TACTL |= MC_2;
  TACTL |= ID_3;
  TACTL |= TASSEL_2;
  
  TACTL |= TAIE;
  // Interrupt Timer A aktivieren
  //TA0CCTL0 = CCIE;

  // Interrupt aktivieren
  __enable_interrupt();
  // Intr
  __low_power_mode_0();
  
//------------------------------------------------------------------------------
//           Endlosschleife, dass kleinste Betriebssystem der Welt!
//------------------------------------------------------------------------------
  for (;;)
  {
    
  }
}
            
//------------------------------------------------------------------------------
//        Interrupt-Service-Routinen/ -Handler z.B. 
//------------------------------------------------------------------------------

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A0_ISR() 
{
  
  uchIntCount++;
  
  switch(uchIntCount){
  case Red:
    P2OUT = Red;
    break;
    
  case Blue:
    P2OUT = Blue;
    break;
    
  case Green:
    P2OUT = Green;
    break;    
  
  case Reset:
    P2OUT = Off;
    
  default:
    break;
  }
  
  TACTL |= TAIE;
  TAR= 0x0025;
  P1OUT ^= BIT0;               
}

//------------------------------------------------------------------------------
//        Funktionsdefinitionen -> besser auslagern z.B. LCD.c
//------------------------------------------------------------------------------
void TimerAVorladewertLaden(void){
 
}
