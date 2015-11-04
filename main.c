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
int ledRing[8];

// NO need for LED mask because it just holds the non-zero values in ledRing

void lightLEDs(unsigned char mask)
{
	send(mask);
	P2OUT |= LATCH;
	P2OUT &= ~LATCH;
}

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

// send a mask based on the LEDs that still have high time remaining
// run this method at periodic intervals (base of pwm)
void pwm()
{
	char mask = 0x00;

	int i;
	for (i = 0; i < 8; i++)
	{
		if (ledRing[i])
		{
			mask |= (0x01 << i);
			ledRing[i]--;
		}
	}

	lightLEDs(mask);
}

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// initialize timer
	DCOCTL = CALDCO_1MHZ;  			// |Set clock speed to 1 MHz|
	BCSCTL1 = CALBC1_1MHZ;  		// |                        |

	// setup Timer A
	// SMCLK clock, divided by 4, in up mode, and start by clearing the timer
	TACTL = TASSEL_2 | ID_3 | MC_1 | TACLR;

	// setup value for comparison
	// 1 MHz / 8 = 125 kHz --> 125 ticks per ms
	TACCR0 = 125;

	// enable interrupt on capture-compare control register 0
	TACCTL0 |= CCIE;
	P1OUT &= ~( SCK | SI | BLANK);
	P2OUT |= ( LATCH );

	P1DIR |= ( SCK | SI | BLANK);
	P2DIR |= ( LATCH );

	_BIS_SR(GIE);

	while (1)
	{
		
	}

	return 0;
}

#pragma vector = TIMER0_A0_VECTOR // Timer A interrupt service routine
__interrupt void TimerA0_routine(void)
{
	pwm();
}

