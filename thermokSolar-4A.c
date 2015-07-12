#include "thermokSolar-4A.h"
#include "math.h"

#define TC_DISCONNECTED -99.9

typedef struct {
	int16 value;
	float fvalue;
	int8 start_pos;
	int8 field;
} struct_keypad_input;


typedef struct {
	/* most recent valid */
//	int16 pulse_period;
//	int16 pulse_min_period;
	int16 pulse_count;

	int16 adc[8];

	float temperatureC[4];
	float temperatureF[4];

	short relay_status_a;
	short relay_status_b;
	short relay_status_c;

	int8 second;
} struct_current;

typedef struct {
	short now_redraw;
	short now_tick;

	short now_log;
	short now_debug;
	short now_adc;	
	short sleep_okay;

	/* flag to see if we are timing */
//	short pulse_0_count;

	short up_now;
	short down_now;
	short select_now;
} struct_action;



typedef struct {
	int8 button_0_bounce;
	int8 button_1_bounce;
	int8 button_2_bounce;
	
	int8 backlight_seconds;
	int8 sleep_seconds;	

//	int16 pulse_period;
//	int16 pulse_count;
} struct_time_keep;


typedef struct {
	int8 serial_prefix;
	int16 serial_number;
	int8 revision;
	int8 rs232_baud;

	int8 relay_mode[3];

	/* differential settings */
	int8  differential_a_channel[3];
	int8  differential_b_channel[3];
	int16 differential_delta_temperatureC[3];
	int16 differential_high_temperatureC[3];

	/* over temperature settings */
	int8  over_temp_channel_mask[3]; /* xxxx3210 set bit to use that channel in comparison */
	int16 over_temp_temperatureC[3];

	/* under temperature settings */
	int8  under_temp_channel_mask[3]; /* xxxx3210 set bit to use that channel in comparison */
	int16 under_temp_temperatureC[3];

	int8 log_on_battery;
	int8 units;

	int8 sensor_type;
	int8 relay_c_type;
} struct_configuration;

/* global structures */
struct_current current;
struct_action action;
struct_time_keep timers;
struct_configuration config;
struct_keypad_input keypad;

#byte port_a=0x0f80
#byte port_b=0x0f81
#byte port_c=0x0f82

int8 ctoi(int8 c) {
	if ( c >= '0' && c <= '9' ) 
		return c-'0';
	else
		return 0;
}

float c_to_f(float c) {
	double f;
	f = 1.8 * c + 32;
	return f;
}

float f_to_c(float f) {
	double c;
	c = (f-32)*0.555556;
	return c;
}

void init_rs232(void);

#include "mcp3208.c"
#include "filter.c"
#include "interrupts.c"
#include "lcd.c"
#include "params.c"
#include "live.c"
#include "screens.c"


/* on boot we default to 9600 baud */
void init_rs232(void) {

	/* setup the serial port speed */
	switch ( config.rs232_baud ) {
		case BAUD_1200:  setup_uart(1200,modem); break;
		case BAUD_2400:  setup_uart(2400,modem); break;
		case BAUD_4800:  setup_uart(4800,modem); break;
		case BAUD_9600:  setup_uart(9600,modem); break;
		case BAUD_19200: setup_uart(19200,modem); break;
		case BAUD_57600: setup_uart(57600,modem); break;
		default: output_low(RS232_EN); return;
	}

	output_high(RS232_EN);
}

void modbus_tristar_disable(int8 ch) {
	setup_uart(9600,modem);
	output_high(RS232_EN);
	output_high(MODEM_EN);

	/* disable output
	01,05,00,01,FF,00,DD,FA (slave address=1)
	02,05,00,01,FF,00,DD,C9 (slave address=2)
	*/
	if ( 2 == ch ) {
		fputc(0x02,modem);
	} else { 
		fputc(0x01,modem);
	}
	fputc(0x05,modem);
	fputc(0x00,modem);
	fputc(0x01,modem);
	fputc(0xFF,modem);
	fputc(0x00,modem);

	/* CRC */
	fputc(0xDD,modem);
	if ( 2 == ch ) {
		fputc(0xC9,modem);
	} else {
		fputc(0xFA,modem);
	}

	/* return to defined speed */
	init_rs232();
}

void modbus_tristar_enable(int8 ch) {
	setup_uart(9600,modem);
	output_high(RS232_EN);
	output_high(MODEM_EN);

	/* enable output
	01,05,00,01,00,00,9C,0A (slave address=1)
	02,05,00,01,00,00,9C,39 (slave address=2)
	*/

	if ( 2 == ch ) {
		fputc(0x02,modem);
	} else {
		fputc(0x01,modem);
	}

	fputc(0x05,modem);
	fputc(0x00,modem);
	fputc(0x01,modem);
	fputc(0x00,modem);
	fputc(0x00,modem);
	
	/* CRC */
	fputc(0x9C,modem);
	if ( 2 == ch ) {
		fputc(0x39,modem);
	} else {
		fputc(0x0A,modem);
	}


	/* return to defined speed */
	init_rs232();
}



int16 read_eeprom_int16(int16 address) {
	return make16(read_eeprom(address),read_eeprom(address+1));
}
void write_eeprom_int16(int16 address, int16 val) {
	write_eeprom(address,make8(val,1));
	write_eeprom(address+1,make8(val,0));
}

void relay_on(int8 ch) {
	if ( 0==ch ) {
		output_high(RELAY_A);
		current.relay_status_a=1;
	} else if ( 1==ch ) {
		output_high(RELAY_B);
		current.relay_status_b=1;
	} else {
		if ( config.relay_c_type==RELAY_C_TYPE_TRISTAR ) {
			modbus_tristar_disable(1);
			delay_ms(10);
			modbus_tristar_enable(2);
		}
		current.relay_status_c=1;
	}
}

void relay_off(int8 ch) {
	if ( 0==ch ) {
		output_low(RELAY_A);
		current.relay_status_a=0;
	} else if ( 1==ch ) {	
		output_low(RELAY_B);
		current.relay_status_b=0;
	} else {
		if ( config.relay_c_type==RELAY_C_TYPE_TRISTAR ) {
			modbus_tristar_enable(1);
			delay_ms(10);
			modbus_tristar_disable(2);
		}
		current.relay_status_c=0;
	}
}

void relay_decide(void) {
	signed int16 a,b,delta;
	int8 ch;

	for ( ch=0 ; ch<3 ; ch++ ) {
		if ( RELAY_MODE_OFF == config.relay_mode[ch] ) {
			relay_off(ch);
		} else if ( RELAY_MODE_DIFFERENTIAL == config.relay_mode[ch] ) {
			a=current.temperatureC[config.differential_a_channel[ch]];
			b=current.temperatureC[config.differential_b_channel[ch]];
			delta=config.differential_delta_temperatureC[ch];
			if ( current.temperatureC[config.differential_b_channel[ch]] >= config.differential_high_temperatureC[ch] ) {
				relay_off(ch);
			} else if ( a > (b+delta) && a != TC_DISCONNECTED && b != TC_DISCONNECTED ) {
				relay_on(ch);
			} else {
				relay_off(ch);
			}
		} else if ( RELAY_MODE_ALARM_OVER == config.relay_mode[ch] ) {
			/* turn on relay whenever any of the enabled channels go over the set point */
			if ( ((config.over_temp_channel_mask[ch] & 0b0001) && current.temperatureC[0]>config.over_temp_temperatureC[ch]) ||
       	      ((config.over_temp_channel_mask[ch] & 0b0010) && current.temperatureC[1]>config.over_temp_temperatureC[ch]) ||
       	      ((config.over_temp_channel_mask[ch] & 0b0100) && current.temperatureC[2]>config.over_temp_temperatureC[ch]) ||
       	      ((config.over_temp_channel_mask[ch] & 0b1000) && current.temperatureC[3]>config.over_temp_temperatureC[ch]) ) {
				relay_on(ch);
			} else {
				relay_off(ch);
			}
		} else if ( RELAY_MODE_ALARM_UNDER == config.relay_mode[ch] ) {
			/* turn on relay whenever any of the enabled channels go under the set point */
			if ( ((config.under_temp_channel_mask[ch] & 0b0001) && current.temperatureC[0]<config.under_temp_temperatureC[ch]) ||
           	  ((config.under_temp_channel_mask[ch] & 0b0010) && current.temperatureC[1]<config.under_temp_temperatureC[ch]) ||
           	  ((config.under_temp_channel_mask[ch] & 0b0100) && current.temperatureC[2]<config.under_temp_temperatureC[ch]) ||
           	  ((config.under_temp_channel_mask[ch] & 0b1000) && current.temperatureC[3]<config.under_temp_temperatureC[ch]) ) {
				relay_on(ch);
			} else {
				relay_off(ch);
			}
		} else if ( RELAY_MODE_TEST_ON == config.relay_mode[ch] ) {
			relay_on(ch);
		}
	}
}

void init() {
	int8 i;

	setup_oscillator(OSC_4MHZ);
	setup_comparator(NC_NC_NC_NC);
	setup_adc(ADC_OFF);
	setup_adc_ports(NO_ANALOGS );

	action.now_log=0;
	action.now_tick=1;
	action.now_redraw=1;
	action.now_debug=0;
	action.sleep_okay=0;
	action.now_adc=1;

	action.up_now=0;
	action.down_now=0;
	action.select_now=0;

	timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
	timers.button_0_bounce=0;
	timers.button_1_bounce=0;
	timers.button_2_bounce=0;

	for ( i=0 ; i<4 ; i++ ) {
		current.temperatureC[i]=TC_DISCONNECTED;
		current.temperatureF[i]=TC_DISCONNECTED;
	}	
	current.second=0;
	current.pulse_count=0;

	/* initialize other devices */
	relay_off(0);
	relay_off(1);
	relay_off(2);
	lcd_init();
	output_high(ADC_PULLUP);
	mcp3208_init();


	setup_timer_2(T2_DIV_BY_16,61,1); // set 1.000mS period with 4 MHz oscillator
	

	/* receive data from serial ports */
	enable_interrupts(INT_RDA);
	enable_interrupts(INT_TIMER2);

	port_b_pullups(TRUE);
	delay_ms(1);
//	enable_interrupts(INT_RB);
// debouncing now handled in 1ms timer

	/* external counter */	
	ext_int_edge(0,H_TO_L);
	enable_interrupts(INT_EXT);

	enable_interrupts(GLOBAL);
}

float thermistor_12bitadc_c(int16 adc) {
	double rt, tkelvin, volts;

	if ( adc > 4000 )
		return TC_DISCONNECTED;

	volts = 5.0 / 4096 * adc;
	rt = (-10000*volts) / (volts-5.0);
	tkelvin = 3977 / ( log(rt) + 4.12858 );

	return (tkelvin - 273.15);
}

float analog_0_5(int16 adc) {
	/* span -40 to 150C */
	return adc*0.046398046 - 40;
}

float analog_5_0(int16 adc) {
	/* span -40 to 150C */
	return adc*-0.046398046 + 150;
}

float analog_volts(int16 adc) {
	/* span 0 to 5 */
	return adc*0.001221001;
}

/* call the right math function based on our sensor type */
float temperature_c_from_analog(int16 adc) {
	if ( SENSOR_THERMISTOR_NTC==config.sensor_type )
		return thermistor_12bitadc_c(adc);

	if ( SENSOR_AN_0_5==config.sensor_type )
		return analog_0_5(adc);

	if ( SENSOR_AN_5_0==config.sensor_type )
		return analog_5_0(adc);

	if ( SENSOR_AN_VOLTS==config.sensor_type )
		return analog_volts(adc);

	if ( SENSOR_AN_ADC==config.sensor_type )
		return (float) adc;

}


void update_temperature(void) {
	int8 i;
	int16 l;

	for ( i=0 ; i<4 ; i++ ) {
		l=current.adc[i];

		current.temperatureC[i]=temperature_c_from_analog(l);
		if ( TC_DISCONNECTED == current.temperatureC[i] )
			current.temperatureF[i]=TC_DISCONNECTED;
		else
			current.temperatureF[i]=c_to_f(current.temperatureC[i]);

#if 0
		if ( l < 4000 ) {
			//current.temperatureC[i]=thermistor_12bitadc_c(l);
			current.temperatureC[i]=temperature_c_from_analog(l);
			current.temperatureF[i]=c_to_f(current.temperatureC[i]);
		} else { 
			current.temperatureC[i]=TC_DISCONNECTED;
			current.temperatureF[i]=TC_DISCONNECTED;
		}
#endif
	}
}

void update_analog(void) {
	int8 i;


	for ( i=4 ; i<8 ; i++ ) {
		current.adc[i]=mcp3208_read(i);
	}
	
}


void main(void) {


	/* normal device startup */
	init();

	delay_ms(200);
	read_param_file();

	init_rs232();

	lcd_clear();
	lcd_putch("ThermokSolar-4A");
//             0123456789012345
	lcd_goto(LCD_LINE_TWO);
	lcd_putch(__DATE__);

	delay_ms(750);

	lcd_clear();
	printf(lcd_putch,"SN: %c%05lu",config.serial_prefix,config.serial_number);


	fprintf(modem,"# ThermokSolar-4A %s\r\n",__DATE__);

	delay_ms(500);

	/* hold down all three buttons to reset the log */
	if ( 0==input(BUTTON_0) && 0==input(BUTTON_1) && 0==input(BUTTON_2) ) {
		write_default_param_file();
	}

	/* hold down left button to switch between F and C */
	if ( 0==input(BUTTON_0) ) {
		lcd_clear();
		lcd_putch("Units set to ");
		lcd_putch(DEG_SYMBOL);

		if ( UNITS_F==config.units ) {
			config.units=UNITS_C; 
			lcd_putch('C');
		} else { 
			config.units=UNITS_F;
			lcd_putch('F');
		}
		delay_ms(1000);
		write_param_file();
	}

	/* hold down middle and right to set serial */
	if ( ('A'==config.serial_prefix && 0==config.serial_number) || 
	     (0==input(BUTTON_0) && 0==input(BUTTON_1) && 1==input(BUTTON_2)) ) {
		screen_set_serial();
	}

	/* main loop */
	for ( ; ; ) {
		restart_wdt();

		if ( action.now_adc ) {
			current.adc[0]=mean_filter_ch0(mcp3208_read(0));
			current.adc[1]=mean_filter_ch1(mcp3208_read(1));
			current.adc[2]=mean_filter_ch2(mcp3208_read(2));
			current.adc[3]=mean_filter_ch3(mcp3208_read(3));
			action.now_adc=0;
		}

		if ( action.now_tick ) {
			action.now_tick=0;

			update_temperature();
			update_analog();

			relay_decide();
		}


		if ( action.now_redraw ) {
			action.now_redraw=0;
			screen_select();
		}

		if ( action.now_log ) {
			action.now_log=0;
			live_send();
		}
	}
}