/* Prepared By Ahmed Magdy */

/* P6_8.c: Testing nested interrupts */
/* Timer1A is setup to interrupt at 1 Hz. In timer interrupt handler, the red LED is
turned on and a delay 500 ms function is called. The LED is turned off at the end of
the delay. */
/* Timer2A is setup to interrupt at 10 Hz. In timer interrupt handler, the green LED is
turned on and a delay 20 ms function is called. The LED is turned off at the end of the
delay. */
/* When Timer1A has higher priority, the Timer2A interrupts are blocked by Timer1A
interrupt handler. You can see that when the red LED is on, the green LED is not
blinking. When Timer2A has higher priority, the Timer1A interrupt handler is preempted
by Timer2A interrupts and the green LED is blinking all the time. */
#include "TM4C123GH6PM.h"
#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"



void Timer1_init(void);
void Timer2_init(void);
void delayMs(int n);
void TIMER1A_Handler(void);
void TIMER2A_Handler(void);

int main (void)
{
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL_RCGCGPIO_R |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIO_PORTF_DIR_R = 0x0e;
    /* enable the GPIO pins for digital function */
    GPIO_PORTF_DEN_R = 0x0e;
    Timer1_init();
    Timer2_init();
    while(1)
    {

    }

}
void TIMER1A_Handler(void)
{
    volatile int readback;
    GPIO_PORTF_DATA_R |= 2; /* turn on red LED */
    delayMs(500);
    GPIO_PORTF_DATA_R &= ~2; /* turn on red LED */
    TIMER1_ICR_R = 0x1;
    readback = TIMER1_ICR_R; /* a read to force clearing of interrupt flag */
}

void TIMER2A_Handler(void)
{
    volatile int readback;
    TIMER1_ICR_R = 0x1;
    GPIO_PORTF_DATA_R |= 8; /* turn on green LED */
    delayMs(20);
    GPIO_PORTF_DATA_R &= ~8; /* turn on green LED */
    readback = TIMER2_ICR_R; /* a read to force clearing of interrupt flag */
}

void Timer1_init(void)
{
    SYSCTL_RCGCTIMER_R |= 2; /* enable clock to Timer Module 1 */
    TIMER1_CTL_R = 0; /* disable Timer1 before initialization */
    TIMER1_CFG_R = 0x04; /* 16-bit option */
    TIMER1_TAMR_R = 0x02; /* periodic mode and up-counter */
    TIMER1_TAPR_R = 250; /* 16000000 Hz / 250 = 64000 Hz */
    TIMER1_TAILR_R = 64000; /* 64000 Hz / 64000 = 1 Hz */
    TIMER1_ICR_R = 0x1; /* clear the Timer1A timeout flag */
    TIMER1_IMR_R |= 0x01; /* enable Timer1A timeout interrupt */
    TIMER1_CTL_R |= 0x01; /* enable Timer1A after initialization */
    NVIC_PRI21_R = 4 << 5; /* set interrupt priority to 4 */
    NVIC_EN0_R |= 0x00200000; /* enable IRQ21 (D21 of ISER[0]) */
}

void Timer2_init(void)
{
    SYSCTL_RCGCTIMER_R |= 4; /* enable clock to Timer Module 2 */
    TIMER2_CTL_R = 0; /* disable Timer2 before initialization */
    TIMER2_CFG_R = 0x04; /* 16-bit option */
    TIMER2_TAMR_R= 0x02; /* periodic mode and up-counter */
    TIMER2_TAPR_R = 250; /* 16000000 Hz / 250 = 64000 Hz */
    TIMER2_TAILR_R = 6400; /* 64000 Hz / 6400 = 10 Hz */
    TIMER2_ICR_R = 0x1; /* clear the Timer1A timeout flag */
    TIMER2_IMR_R |= 0x01; /* enable Timer2A timeout interrupt */
    TIMER2_CTL_R |= 0x01; /* enable Timer2A after initialization */
    NVIC_PRI23_R = 5 << 5; /* set timer2A interrupt priority to 5 */
    NVIC_EN0_R |= 0x00800000; /* enable IRQ23 (D23 of ISER[0]) */
}


/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
    for(j = 0; j < 3180; j++)
    {} /* do nothing for 1 ms */
}
