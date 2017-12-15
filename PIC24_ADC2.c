#include "xc.h"
#include "Common.h"

void initAdc1(void);

int ADCValues[4] = {0, 0, 0, 0};
int i;
int main(void)
{
// Configure the device PLL to obtain 40 MIPS operation.The crystal frequency is 8 MHz.
// Divide 8 MHz by 2, multiply by 40 and divide by 2. This results in Fosc of 80 MHz.
// The CPU clock frequency is Fcy = Fosc/2 = 40 MHz.
    PLLFBD = 38; /* M = 40 */
    CLKDIVbits.PLLPOST = 0; /* N1 = 2 */
    CLKDIVbits.PLLPRE = 0; /* N2 = 2 */
    OSCTUN = 0;
    /* Initiate Clock Switch to Primary Oscillator with PLL (NOSC = 0x3) */
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(0x01);
    while (OSCCONbits.COSC != 0x3);
    while (_LOCK == 0); /* Wait for PLL lock at 40 MIPS */
    initAdc1();
    while(1)
    {
        while (!_AD1IF); // Wait for all 4 conversions to complete
        _AD1IF = 0; // Clear conversion done status bit
        ADCValues[0] = ADC1BUF0; // Read the AN2 conversion result
        ADCValues[1] = ADC1BUF1; // Read the AN3 conversion result
        ADCValues[2] = ADC1BUF2; // Read the AN5 conversion result
        ADCValues[3] = ADC1BUF3; // Read the AN8 conversion result
    }
}
void initAdc1(void)
{
    /* Set port configuration */
    ANSELA = 0x0000;
    ANSELB = 0x0003; // Ensure AN0, AN1,are analog
    /* Initialize ADC module */
    AD1CON1 = 0x04E4; // ?0000 0100 1110 0100?  Enable 12-bit mode, auto-sample and auto-conversion
    AD1CON2 = 0x040C; // ?0000 0100 0000 1100?  Sample 4 channels alternately using channel scanning
    AD1CON3 = 0x0F0F; // 0000 1111 0000 1111  Sample for 15*TAD before converting
    AD1CON4 = 0x0000;
    AD1CSSH = 0x0000;
    AD1CSSL = 0x0003; // Select AN2, AN3, AN5 and AN8 for scanning
    /* Assign MUXA inputs */
    AD1CHS0bits.CH0SA = 0; // CH0SA bits ignored for CH0 +ve input selection
    AD1CHS0bits.CH0NA = 0; // Select VREF- for CH0 -ve input
    /* Enable ADC module and provide ADC stabilization delay */
    AD1CON1bits.ADON = 1;
    DelayUs(20);
}