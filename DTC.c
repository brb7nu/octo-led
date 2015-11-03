
/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
*/
#include <msp430.h>
volatile unsigned int measurements[3];

int main(void)
{

  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  ADC10CTL1 = INCH_2 + CONSEQ_3;            // A2/A1/A0, repeat multi channel
  ADC10CTL0 = ADC10SHT_2 + MSC + ADC10ON + ADC10IE;
  ADC10AE0 = (BIT0 + BIT1 + BIT2);          // P1.0,1, 2 Analog enable
  ADC10DTC1 = 0x20;                         // number of conversions

  for (;;)
  {
    ADC10CTL0 &= ~ENC;
    while (ADC10CTL1 & BUSY);               // Wait if ADC10 core is active
    ADC10SA = (int) &(measurements);        // Data buffer start
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion ready
    __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
    _NOP();                                 // space for debugger
    _NOP();                                 // Set Breakpoint here to read ADC
  }
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}
