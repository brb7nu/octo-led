// Ben Browning
// Will Cray

#include <msp430.h>

// #define LEFT_RED_TP XOUT // P1.0
// #define LEFT_WHITE_TP YOUT // P1.1
// #define LEFT_YELLOW_TP ZOUT // P1.2
// #define RIGHT_YELLOW_TP SIN
// #define RIGHT_WHITE_TP SCLK
// #define RIGHT_RED_TP XLATCH

#define SCK BIT5
#define BLANK BIT4
#define LATCH BIT0
#define SI BIT7

// stores the number of milliseconds each led must stay on
int ledRingHighTimeRemaining[8];

// stores the high times in milliseconds for each LED in the ring
// this is different than ledRingHighTimeRemaining because it does not get decremented
int ledRingHighTimeMS[8];

int pwmSegmentCount;

char mask = 0x00;

// NO need for LED mask because it just holds the non-zero values in ledRingHighTimeRemaining

void send(unsigned char s)
{
	unsigned char bit = 0x80;

	while (bit)
	{
		if (s & bit)
		{
			P1OUT |= SI;
		}
		else
		{
			P1OUT &= ~SI;
		}
		bit >>= 1;
		P1OUT |= SCK;
		P1OUT &= ~SCK;
	}
}

void lightLEDs(unsigned char mask)
{
	send(mask);
	P2OUT |= LATCH;
	P2OUT &= ~LATCH;
}

inline void reloadPWMTimes(void)
{
	int i;
	for (i = 0; i < 8; i++)
	{
		ledRingHighTimeRemaining[i] = ledRingHighTimeMS[i];
	}
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// initialize timer
	DCOCTL = CALDCO_16MHZ;  			// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_16MHZ;  		// |                        |

	// setup Timer A
	// SMCLK clock, divided by 4, in up mode, and start by clearing the timer
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

	// setup value for comparison
	// 16 MHz / 8 = 2 MHz --> 200 ticks per 100 us
	// TACCR0 = 63;
	TACCR0 = 200;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	pwmSegmentCount = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		ledRingHighTimeRemaining[i] = 0;
	}

	ledRingHighTimeMS[0] = 0;
	ledRingHighTimeMS[1] = 2;
	ledRingHighTimeMS[2] = 4;
	ledRingHighTimeMS[3] = 6;
	ledRingHighTimeMS[4] = 8;
	ledRingHighTimeMS[5] = 10;
	ledRingHighTimeMS[6] = 15;
	ledRingHighTimeMS[7] = 20;

	_BIS_SR(GIE);

	P1DIR |= (BIT6);
	P1OUT |= BIT6;

	while (1)
	{

	}

	return 0;
}

//  ___         ___         ___        
// |   |_______|   |_______|   |_______
//  _______     _______     _______    
// |       |___|       |___|       |___
//  ___         ___         ___        
// |   |_______|   |_______|   |_______
//
// ____________________________________
// 
// |---10 ms---|
#pragma vector = TIMER0_A0_VECTOR // Timer A interrupt service routine
__interrupt void TimerA0_routine(void)
{
	if (pwmSegmentCount >= 100)
	{
		pwmSegmentCount = 0;
		reloadPWMTimes();
		P1OUT ^= BIT6;
	}

	mask = 0x00;

	// send a mask based on the LEDs that still have high time remaining
	int i;
	for (i = 0; i < 8; i++)
	{
		if (ledRingHighTimeRemaining[i])
		{
			mask |= (0x01 << i);
			ledRingHighTimeRemaining[i]--;
		}
	}

	lightLEDs(mask);

	pwmSegmentCount++;
}

