/* --------------------------------------------------------------------------
 * Project : Digital Clock + Stopwatch (HH:MM:SS)
 * Platform: 8051 Microcontroller (AT89C51 / AT89S52)
 * Display : 6 Common-Cathode 7-Segment Displays
 * Peripherals: Mode Switch, Start/Stop Buttons (Active LOW)
 * Author  : Anmol Yash Lakra
 * Date    : 2025-10-08
 * --------------------------------------------------------------------------
 * Description:
 * A digital clock and stopwatch implemented using the 8051 timer interrupt.
 * The display shows HH:MM:SS on six 7-segment modules.
 * The mode switch toggles between:
 *    - CLOCK MODE: Real-time clock counting seconds
 *    - STOPWATCH MODE: Manually controlled using start/stop buttons
 *
 * Timer0 generates an interrupt every ~50ms, used to accumulate seconds.
 * --------------------------------------------------------------------------*/

#include <reg51.h>

/* ----------------- Pin Definitions ----------------- */
sbit MODE_SWITCH = P3^0;   // Mode toggle switch
sbit START_BTN   = P3^1;   // Start/Stop button

// Segment connections: Assuming 7-segment connected to Port 2
#define SEGMENT_PORT P2

// Digit enable pins for 6 displays (common cathode)
sbit DIGIT1 = P1^0;   // HH Tens
sbit DIGIT2 = P1^1;   // HH Ones
sbit DIGIT3 = P1^2;   // MM Tens
sbit DIGIT4 = P1^3;   // MM Ones
sbit DIGIT5 = P1^4;   // SS Tens
sbit DIGIT6 = P1^5;   // SS Ones

/* ----------------- Lookup Table for 7-seg ----------------- */
// Common cathode: 0 → segment ON
unsigned char code SEGMENT_CODE[10] = {
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xF8, // 7
    0x80, // 8
    0x90  // 9
};

/* ----------------- Global Variables ----------------- */
unsigned char hour = 0, minute = 0, second = 0;
bit mode = 0;         // 0 = Clock mode, 1 = Stopwatch mode
bit running = 0;      // Stopwatch state
unsigned int count = 0;

/* ----------------- Function Declarations ----------------- */
void delay_ms(unsigned int ms);
void display_time(void);
void update_time(void);
void timer0_ISR(void) interrupt 1;
void init_timer(void);

/* ----------------- Main Function ----------------- */
void main(void)
{
    MODE_SWITCH = 1;  // Default: Clock mode
    START_BTN = 1;    // Default: Not pressed
    init_timer();

    while (1)
    {
        display_time();

        // Mode switch toggling
        if (MODE_SWITCH == 0)
        {
            delay_ms(200);
            mode = !mode; // Toggle mode
            hour = minute = second = 0;
        }

        // Start/Stop button
        if (mode == 1) // Stopwatch only
        {
            if (START_BTN == 0)
            {
                delay_ms(200);
                running = !running;
            }
        }
    }
}

/* ----------------- Timer0 Initialization ----------------- */
void init_timer(void)
{
    TMOD = 0x01;     // Timer0 Mode1 (16-bit)
    TH0 = 0x3C;      // ~50ms reload value for 11.0592MHz
    TL0 = 0xB0;
    ET0 = 1;         // Enable Timer0 interrupt
    EA = 1;          // Enable global interrupts
    TR0 = 1;         // Start Timer0
}

/* ----------------- Timer0 Interrupt ----------------- */
void timer0_ISR(void) interrupt 1
{
    TH0 = 0x3C;
    TL0 = 0xB0;

    count++;
    if (count >= 20) // 20 x 50ms = 1 second
    {
        count = 0;

        if (mode == 0) // CLOCK MODE
            update_time();

        else if (mode == 1 && running) // STOPWATCH MODE
            update_time();
    }
}

/* ----------------- Update Time Function ----------------- */
void update_time(void)
{
    second++;
    if (second >= 60)
    {
        second = 0;
        minute++;
        if (minute >= 60)
        {
            minute = 0;
            hour++;
            if (hour >= 24)
                hour = 0;
        }
    }
}

/* ----------------- Display Function ----------------- */
void display_time(void)
{
    unsigned char digits[6];

    digits[0] = hour / 10;
    digits[1] = hour % 10;
    digits[2] = minute / 10;
    digits[3] = minute % 10;
    digits[4] = second / 10;
    digits[5] = second % 10;

    DIGIT1 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[0]]; delay_ms(2); DIGIT1 = 1;
    DIGIT2 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[1]]; delay_ms(2); DIGIT2 = 1;
    DIGIT3 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[2]]; delay_ms(2); DIGIT3 = 1;
    DIGIT4 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[3]]; delay_ms(2); DIGIT4 = 1;
    DIGIT5 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[4]]; delay_ms(2); DIGIT5 = 1;
    DIGIT6 = 0; SEGMENT_PORT = SEGMENT_CODE[digits[5]]; delay_ms(2); DIGIT6 = 1;
}

/* ----------------- Delay Function ----------------- */
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}
