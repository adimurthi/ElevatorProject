#include <driverlib.h>
#include "driverlib.h"
#include "string.h"


void initGPIO(void);
void elevator(void);  //main elevator program
void  greenLEdoff(void);
void  currentposon(void);
void LCD_init(void);
void LCD_showChar( char, int );
void LCD_displayNumber( unsigned long );



#define    LCD_HEART                   (0x0004)
#define    LCD_TIMER                   (0x0008)

#define NUM_CHAR 6

int a[10];   //to store random numbers
char b;  // to convert char to numbers
int ia=0;  //

/* LCD memory locations for each character position on the display */
#define pos1 4   /* Digit A1 - L4  */
#define pos2 6   /* Digit A2 - L6  */
#define pos3 8   /* Digit A3 - L8  */
#define pos4 10  /* Digit A4 - L10 */
#define pos5 2   /* Digit A5 - L2  */
#define pos6 18  /* Digit A6 - L18 */

//***** Global Variables ******************************************************
extern const char digit[10][2];            // Segment values for each number character
extern const char alphabetBig[26][2];      // Segment values for each alphabet character

// Memory locations for LCD characters
const unsigned int location[NUM_CHAR] =
{
 pos1,   // Digit A1
 pos2,   // Digit A2
 pos3,   // Digit A3
 pos4,   // Digit A4
 pos5,   // Digit A5
 pos6    // Digit A6
};

// LCD memory map for numeric digits
const char digit[10][2] =
{
 {0xFC, 0x28},  /* "0" LCD segments a+b+c+d+e+f+k+q */
 {0x60, 0x20},  /* "1" */
 {0xDB, 0x00},  /* "2" */
 {0xF3, 0x00},  /* "3" */
 {0x67, 0x00},  /* "4" */
 {0xB7, 0x00},  /* "5" */
 {0xBF, 0x00},  /* "6" */
 {0xE4, 0x00},  /* "7" */
 {0xFF, 0x00},  /* "8" */
 {0xF7, 0x00}   /* "9" */
};

// LCD memory map for uppercase letters
const char alphabetBig[26][2] =
{
 {0xEF, 0x00},  /* "A" LCD segments a+b+c+e+f+g+m */
 {0xF1, 0x50},  /* "B" */
 {0x9C, 0x00},  /* "C" */
 {0xF0, 0x50},  /* "D" */
 {0x9F, 0x00},  /* "E" */
 {0x8F, 0x00},  /* "F" */
 {0xBD, 0x00},  /* "G" */
 {0x6F, 0x00},  /* "H" */
 {0x90, 0x50},  /* "I" */
 {0x78, 0x00},  /* "J" */
 {0x0E, 0x22},  /* "K" */
 {0x1C, 0x00},  /* "L" */
 {0x6C, 0xA0},  /* "M" */
 {0x6C, 0x82},  /* "N" */
 {0xFC, 0x00},  /* "O" */
 {0xCF, 0x00},  /* "P" */
 {0xFC, 0x02},  /* "Q" */
 {0xCF, 0x02},  /* "R" */
 {0xB7, 0x00},  /* "S" */
 {0x80, 0x50},  /* "T" */
 {0x7C, 0x00},  /* "U" */
 {0x0C, 0x28},  /* "V" */
 {0x6C, 0x0A},  /* "W" */
 {0x00, 0xAA},  /* "X" */
 {0x00, 0xB0},  /* "Y" */
 {0x90, 0x28}   /* "Z" */
};


void main (void) {

    WDT_A_hold( WDT_A_BASE );

    initGPIO();
    LCD_init();
    __bis_SR_register( GIE );  // Enable interrupts globally



    //WDT_A_hold(WDT_A_BASE);

    // Initialize LCD


    // Set the LCD's "Heart" symbol
    // LCDMx location
    LCD_E_setMemory( LCD_E_BASE,
                     LCD_E_MEMORY_BLINKINGMEMORY_12,
                     LCD_HEART
    );

    // Set the LCD's "Timer" symbole
    // Heart replaced by Timer symbol
    LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER);

    // Updates only the specified segments (in this case, the Heart segment)
    // Both Heart and Timer symbols should be displayed
    LCD_E_updateMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART);

    // Clears only the specified segments (in this case, the Timer segment)
    // Erases just the Timer symbol
    LCD_E_clearMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER);

    // Toggles only the specified segments (in this case, the Heart segment)
    // Turns off the Heart symbol, so the display should now be blank
    LCD_E_toggleMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART);

    // Set both the "Heart" and "Timer" symbols
    // Turn on both at once, since both are controlled by LCD Memory 12
    LCD_E_setMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_HEART | LCD_TIMER);

    // Blinks enaboled segments on the display (in this case, the Heart and Timer)
    // Notice how the blinking continues, even when the processor is halted at a breakpoint (or during single-stepping)
    // This is because the LCD controller is automatically handling the blinking -- no timers or interrupts are required!
    LCD_E_setBlinkingControl(LCD_E_BASE, LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64, LCD_E_BLINK_MODE_INDIVIDUAL_SEGMENTS);

    // Turn off the Timer symbol
    LCD_E_clearMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER);

    // Turns off the blinking feature
    LCD_E_setBlinkingControl ( LCD_E_BASE,
                               LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64,
                               LCD_E_BLINK_MODE_DISABLED
    );

    // Using the 2nd memory (i.e. Blinking Memory)

    // Set the LCD's "Timer" segment in 2nd memory (i.e. Blinking Memory)
    // Notice that the Timer doesn't "show up" after executing this function, since we're currently displaying the main LCD memory
    LCD_E_setBlinkingMemory(LCD_E_BASE, LCD_E_MEMORY_BLINKINGMEMORY_12, LCD_TIMER);

    // Tell the LCD to display the 2nd (Blinking) memory
    // Timer symbol "on"; Heart symbol "off"
    LCD_E_selectDisplayMemory(LCD_E_BASE, LCD_E_DISPLAYSOURCE_BLINKINGMEMORY);

    // Select to display main LCD (main) memory, again
    // Heart is "on"; Timer symbol "off"
    LCD_E_selectDisplayMemory(LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY);

    // Tell the LCD controller to switch back-and-forth between memories
    // You should see the Heart and Timer alternating
    LCD_E_setBlinkingControl(LCD_E_BASE, LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64, LCD_E_BLINK_MODE_SWITCHING_BETWEEN_DISPLAY_CONTENTS);

    // Turn off Blinking
    LCD_E_setBlinkingControl(LCD_E_BASE, LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64, LCD_E_BLINK_MODE_DISABLED);

    // Write "DONE" to LCD
    // ShowChar:  Param 1 is letter to display; Param 2 is character location
    //       LCD_showChar('H', 1);
    //       LCD_showChar('E', 2);
    //       LCD_showChar('L', 3);
    //       LCD_showChar('P', 4);
    int i;
    int randomnumber;
    for (i=0;i<20;i++)
    {

        randomnumber = rand() % 7 + 1;
        a[i]=randomnumber;
        //   printf("%d\n", a[i]);
    }

    while(1) {
        __no_operation();             // Placeholder for while loop (not required)
    }
    return (0);
}

#pragma vector=PORT1_VECTOR
__interrupt void pushbutton_ISR (void) {
    switch( __even_in_range( P1IV, P1IV_P1IFG7 )) {
    case P1IV_NONE:   break;// None
    case P1IV_P1IFG0:       // Pin 0
        __no_operation();
        break;
    case P1IV_P1IFG1:       // Pin 1
        __no_operation();
        break;
    case P1IV_P1IFG2:       // Pin 2 (button 1)
        ia =ia + 1 ;
        elevator();
        // GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
        break;
    case P1IV_P1IFG3:       // Pin 3
        __no_operation();
        break;
    case P1IV_P1IFG4:       // Pin 4
        __no_operation();
        break;
    case P1IV_P1IFG5:       // Pin 5
        __no_operation();
        break;
    case P1IV_P1IFG6:       // Pin 6
        __no_operation();
        break;
    case P1IV_P1IFG7:       // Pin 7
        __no_operation();
        break;
    default:   _never_executed();
    }
}



#pragma vector=PORT2_VECTOR
__interrupt void pushbuttongreen_ISR (void) {
    switch( __even_in_range( P2IV, P2IV_P2IFG7 )) {
    case P2IV_NONE:   break;// None
    case P2IV_P2IFG0:       // Pin 0
        __no_operation();
        break;
    case P2IV_P2IFG1:       // Pin 1
        __no_operation();
        break;
    case P2IV_P2IFG2:       // Pin 2 (button 1)
        __no_operation();
        break;
    case P2IV_P2IFG3:       // Pin 3
        __no_operation();
        break;
    case P2IV_P2IFG4:       // Pin 4
        __no_operation();
        break;
    case P2IV_P2IFG5:       // Pin 5
        __no_operation();
        break;
    case P2IV_P2IFG6:
                     // Pin 6
        greenLEdoff();
        break;
    case P2IV_P2IFG7:       // Pin 7
        __no_operation();
        break;
    default:   _never_executed();
    }
}


int temp=1;

void initGPIO(void) {

    // Set pin P1.0 to output direction and turn LED off
    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );

    // Unlock pins (required for FRAM devices)
    // Unless waking from LPMx.5, this should be done before clearing
    // and enabling GPIO port interrupts
    PMM_unlockLPM5();

    // Set P1.2 as input with pull-up resistor (for push button S1)
    //  configure interrupt on low-to-high transition
    //  and then clear flag and enable the interrupt
    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P1, GPIO_PIN2 );
    GPIO_selectInterruptEdge ( GPIO_PORT_P1, GPIO_PIN2, GPIO_LOW_TO_HIGH_TRANSITION );
    GPIO_clearInterrupt ( GPIO_PORT_P1, GPIO_PIN2 );
    GPIO_enableInterrupt ( GPIO_PORT_P1, GPIO_PIN2 );



    GPIO_setAsInputPinWithPullUpResistor( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_selectInterruptEdge ( GPIO_PORT_P2, GPIO_PIN6, GPIO_LOW_TO_HIGH_TRANSITION );
    GPIO_clearInterrupt ( GPIO_PORT_P2, GPIO_PIN6 );
    GPIO_enableInterrupt ( GPIO_PORT_P2, GPIO_PIN6 );

    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN5 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );

    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN4 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );

    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN3 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );

    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN3 );    // Red LED (LED1)
    GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN3 );

    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN2 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN2 );

    GPIO_setAsOutputPin( GPIO_PORT_P2, GPIO_PIN5 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );

    GPIO_setAsOutputPin( GPIO_PORT_P5, GPIO_PIN1 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN1 );

    GPIO_setAsOutputPin( GPIO_PORT_P8, GPIO_PIN0 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN0 );

    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN0 );    // Red LED (LED1)
    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );
}


void elevator(void)
{

    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN3 );
    int xy,cnt,zx;
    int pres1;


    for (cnt=0; cnt <=5;cnt++)
    {
        for (zx=0; zx <=30000;zx++);
    }
    b=a[ia]+'0';
    LCD_showChar('F', 0);
    LCD_showChar('L', 1);
    LCD_showChar('O', 2);
    LCD_showChar('O', 3);
    LCD_showChar('R', 4);
    LCD_showChar(b, 5);
    // GPIO_toggleOutputOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    // ia =ia + 1 ;
    pres1=a[ia];
    for (;;)
    {
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
        GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );;
        if (pres1 == temp)
        {
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN0 );;
            GPIO_setOutputHighOnPin( GPIO_PORT_P4, GPIO_PIN0 );;
            currentposon();
            break;
        }

        else if (pres1>temp)
        {
            temp=temp+1;
        }

        else if (pres1<temp)
        {
            temp=temp-1;
        }

        switch (temp)
        {

        case 1 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN3 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN3 );
            break;

        case 2 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN2 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN2 );
            break;

        case 3 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
            break;

        case 4 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P5, GPIO_PIN1 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN1 );
            break;
        case 5 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN0 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN0 );
            break;

        case 6 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN5 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );
            break;

        case 7 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN4 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
            break;

        case 8 :
            GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN3 );
            for (xy=0; xy <=5;xy++)
            {
                for (zx=0; zx <=30000;zx++);
            }
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );
            break;

        default:
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
            GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );
            break;
        }


    }
}


void LCD_init()
{
    // Turn off LCD
    LCD_E_off(LCD_E_BASE);

    // L0~L26 & L36~L39 pins selected
    LCD_E_setPinAsLCDFunctionEx( LCD_E_BASE,         // Gives range of pins to LCD controller
                                 LCD_E_SEGMENT_LINE_0,                    // Starting Lx pin of range
                                 LCD_E_SEGMENT_LINE_26                    // Ending pin of range
    );

    LCD_E_setPinAsLCDFunctionEx( LCD_E_BASE,         // Gives range of pins to LCD controller
                                 LCD_E_SEGMENT_LINE_36,                   // Starting Lx pin of range
                                 LCD_E_SEGMENT_LINE_39                    // Ending pin of range
    );

    LCD_E_initParam initParams = LCD_E_INIT_PARAM;
    initParams.clockDivider = LCD_E_CLOCKDIVIDER_3;
    initParams.muxRate = LCD_E_4_MUX;
    initParams.segments = LCD_E_SEGMENTS_ENABLED;

    // Init LCD as 4-mux mode
    LCD_E_init(LCD_E_BASE, &initParams);

    // LCD Operation - Mode 3, internal 3.02v, charge pump 256Hz
    LCD_E_setVLCDSource(LCD_E_BASE, LCD_E_INTERNAL_REFERENCE_VOLTAGE, LCD_E_EXTERNAL_SUPPLY_VOLTAGE);
    LCD_E_setVLCDVoltage(LCD_E_BASE, LCD_E_REFERENCE_VOLTAGE_2_96V);

    LCD_E_enableChargePump(LCD_E_BASE);
    LCD_E_setChargePumpFreq(LCD_E_BASE, LCD_E_CHARGEPUMP_FREQ_16);

    // Clear LCD memory
    LCD_E_clearAllMemory(LCD_E_BASE);
    LCD_E_clearAllBlinkingMemory(LCD_E_BASE);

    // Configure COMs and SEGs
    // L0 = COM0, L1 = COM1, L2 = COM2, L3 = COM3
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_MEMORY_COM0);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_1, LCD_E_MEMORY_COM1);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_2, LCD_E_MEMORY_COM2);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_3, LCD_E_MEMORY_COM3);

    // Select to display main LCD memory
    LCD_E_selectDisplayMemory(LCD_E_BASE, LCD_E_DISPLAYSOURCE_MEMORY);

    // Turn blinking features off
    LCD_E_setBlinkingControl(LCD_E_BASE, LCD_E_BLINK_FREQ_CLOCK_PRESCALAR_64, LCD_E_BLINK_MODE_DISABLED);

    // Turn on LCD
    LCD_E_on(LCD_E_BASE);
}


/*
 * Displays input character at given LCD digit/position
 * Only spaces, numeric digits, and uppercase letters are accepted characters
 */
void LCD_showChar(char c, int position)
{
    if (c == ' ')
    {
        // Display space
        LCD_E_setMemory(LCD_E_BASE, location[position], 0);
        LCD_E_setMemory(LCD_E_BASE, location[position] + 1, 0);
    }
    else if (c >= '0' && c <= '9')
    {
        // Display digit
        LCD_E_setMemory(LCD_E_BASE, location[position], digit[c-48][0]);
        LCD_E_setMemory(LCD_E_BASE, location[position] + 1, digit[c-48][1]);
    }
    else if (c >= 'A' && c <= 'Z')
    {
        // Display alphabet
        LCD_E_setMemory(LCD_E_BASE, location[position], alphabetBig[c-65][0] );
        LCD_E_setMemory(LCD_E_BASE, location[position] + 1, alphabetBig[c-65][1] );
    }
    else
    {
        // Turn all segments on if character is not a space, digit, or uppercase letter
        LCD_E_setMemory(LCD_E_BASE, location[position], 0xFF);
        LCD_E_setMemory(LCD_E_BASE, location[position] + 1, 0xFF);
    }
}


void LCD_displayNumber(unsigned long val)
{
    unsigned long div[6];
    unsigned long mod[6];
    unsigned long err;

    err = val / 1000000;
    if (err > 0)
    {
        LCD_showChar(' ', 0);
        LCD_showChar('E', 1);
        LCD_showChar('R', 2);
        LCD_showChar('R', 3);
        LCD_showChar('O', 4);
        LCD_showChar('R', 5);
    }
    else
    {
        div[0] = val / 100000;
        mod[0] = val % 100000;
        div[1] = mod[0] / 10000;
        mod[1] = mod[0] % 10000;
        div[2] = mod[1] / 1000;
        mod[2] = mod[1] % 1000;
        div[3] = mod[2] / 100 ;
        mod[3] = mod[2] % 100 ;
        div[4] = mod[3] / 10 ;
        div[5] = mod[3] % 10 ;

        int i = 0;
        int LeadingZeros = 0;

        for ( i = 0; i < (NUM_CHAR - 1); i++ )
        {
            if ( ( div[i] == 0 ) && ( !LeadingZeros ) )
            {
                LCD_showChar( ' ', i );
            }
            else
            {
                LCD_showChar( div[i] + '0', i );
                LeadingZeros++;
            }
        }

        i = NUM_CHAR - 1;
        LCD_showChar( div[i] + '0', i );
    }
}


void  greenLEdoff()

{
	for (cnt=0; cnt <=5;cnt++)
    {
        for (zx=0; zx <=30000;zx++);
    }

    GPIO_setOutputLowOnPin( GPIO_PORT_P4, GPIO_PIN0 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN3 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN2 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P2, GPIO_PIN5 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P5, GPIO_PIN1 );
    GPIO_setOutputLowOnPin( GPIO_PORT_P8, GPIO_PIN0 );
    int cnt,zx;

    for (cnt=0; cnt <=5;cnt++)
    {
        for (zx=0; zx <=30000;zx++);
    }
}

void  currentposon()
{
    switch (a[ia])
    {

    case 1 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN3 );

        break;

    case 2 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN2 );

        break;

    case 3 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P2, GPIO_PIN5 );

        break;

    case 4 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P5, GPIO_PIN1 );

        break;
    case 5 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P8, GPIO_PIN0 );

        break;

    case 6 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN5 );

        break;

    case 7 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN4 );

        break;

    case 8 :
        GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN3 );

        break;

    default:
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN3 );
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN4 );
        GPIO_setOutputLowOnPin( GPIO_PORT_P1, GPIO_PIN5 );
        break;
    }

}
