#include <18F2620.h>
#device ADC=10
#device *=16
#include <stdlib.h>
#FUSES INTRC_IO,NOPROTECT,PUT,NOLVP,NOBROWNOUT,NOMCLR,NOPBADEN,WDT8192, NOIESO, NODEBUG
#use delay(clock=4000000, restart_wdt)

#use rs232(stream=modem,baud=57600,xmit=PIN_C6,rcv=PIN_C7)	

#use standard_io(A)
#use standard_io(B)
#use standard_io(C)


#define BACKLIGHT_TIMEOUT_SECONDS 20
#define BUTTON_0_BIT  5
#define BUTTON_1_BIT  6
#define BUTTON_2_BIT  7
#define BUTTON_BOUNCE 20
#define SLEEP_AFTER_SECONDS 30

#define BUTTON_0      PIN_B5
#define BUTTON_1      PIN_B6
#define BUTTON_2      PIN_B7

/* LCD, character, 16x2 */
#define LCD_RS        PIN_A7 // Register select
#define LCD_RW        PIN_A5
#define LCD_EN        PIN_A4 // Enable
#define LCD_D4        PIN_A0 // Data bits
#define LCD_D5        PIN_A1 // Data bits
#define LCD_D6        PIN_A2 // Data bits
#define LCD_D7        PIN_A3 // Data bits
#define LCD_BACKLIGHT PIN_C0

#define RELAY_A       PIN_B3
#define RELAY_B       PIN_B4
#define COUNTER_IN    PIN_B0
#define RS232_EN      PIN_C1
#define MODEM_EN      PIN_A6
#define MODEM_SLEEP   MODEM_EN
#define ADC_PULLUP    PIN_B2

#define MCP3208_CLK   PIN_C3
#define MCP3208_DOUT  PIN_C4
#define MCP3208_DIN   PIN_C5
#define MCP3208_NCS   PIN_C2


#define PARAM_CRC_ADDRESS 0x00
#define PARAM_ADDRESS     0x01

#define BAUD_1200	0
#define BAUD_2400	1
#define BAUD_4800	2
#define BAUD_9600	3
#define BAUD_19200	4
#define BAUD_57600  5
#define BAUD_OFF    6
#define BAUD_MAX_N	6

#define RELAY_MODE_OFF          0
#define RELAY_MODE_DIFFERENTIAL 1
#define RELAY_MODE_ALARM_OVER   2
#define RELAY_MODE_ALARM_UNDER  3
#define RELAY_MODE_TEST_ON      4
#define RELAY_MODE_MAX_N        4

#define RELAY_C_TYPE_DISABLED   0
#define RELAY_C_TYPE_TRISTAR    1
#define RELAY_C_MAX_N           1

#define SENSOR_THERMISTOR_NTC   0
#define SENSOR_AN_0_5           1
#define SENSOR_AN_5_0           2
#define SENSOR_AN_VOLTS         3
#define SENSOR_AN_ADC           4
#define SENSOR_MAX_N            4


#define UNITS_C 0
#define UNITS_F 1


const int16 int16_tens[]={10000,1000,100,10,1};
//const int8 int8_tens[]={100,10,1};