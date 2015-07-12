void prompt_prev_set_next(void) {
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"Prev  Set   Next");
}
void prompt_minus_set_plus(void) {
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"-     Set      +");
}
void prompt_minus_next_plus(void) {
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"-     Next     +");
}
void prompt_no_yes(void) {
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"NO    YES");
}
void prompt_prev_enter_next(void) {
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"Prev Enter  Next");
}


short screen_get_int16() {
	char n_str[6];

	sprintf(n_str,"%05lu",keypad.value);

//	printf("current position value=%u field=%u\r\n",n_str[keypad.field]-'0',keypad.field);

	/* print bottom line */
	prompt_minus_set_plus();


	/* print our value */
	lcd_goto(keypad.start_pos);
	printf(lcd_putch,"%s",n_str);
	/* put cursor on field being changed */
	lcd_goto(keypad.field + keypad.start_pos);
	lcd_cursor(LCD_CURSOR_ON);
	

	if ( 1 == action.up_now ) {
		action.up_now=0;

		if ( '5'==n_str[keypad.field] && 0==keypad.field ) 
			keypad.value -= 50000;
		else if ( '9' == n_str[keypad.field] )
			keypad.value -= int16_tens[keypad.field]*9;
		else
			keypad.value += int16_tens[keypad.field];

		action.now_redraw=1;
	}
	if ( 1 == action.down_now ) {
		action.down_now=0;
		if ( '0'==n_str[keypad.field] && 0==keypad.field ) 
			keypad.value += 50000;
		else if ( '0' == n_str[keypad.field] ) 
			keypad.value += int16_tens[keypad.field]*9;
		else	
			keypad.value -= int16_tens[keypad.field];

		action.now_redraw=1;
	}
	if ( 1 == action.select_now ) {
		action.select_now=0;
		keypad.field++;
		action.now_redraw=1;
	}

	if ( 5 == keypad.field ) {
		lcd_cursor(LCD_CURSOR_OFF);
		return 1;
	}

	return 0;
}


void screen_temp_c(void) {
	lcd_clear();
	printf(lcd_putch,"1:%03.1f   ",current.temperatureC[0]);

	lcd_goto(8);
	printf(lcd_putch,"3:%03.1f%c",current.temperatureC[2],DEG_SYMBOL);

	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"2:%03.1f   ",current.temperatureC[1]);	

	lcd_goto(LCD_LINE_TWO+8);
	printf(lcd_putch,"4:%03.1fC",current.temperatureC[3]);
}

void screen_temp_f(void) {
	lcd_clear();

	printf(lcd_putch,"1:%03.1f   ",current.temperatureF[0]);

	lcd_goto(8);
	printf(lcd_putch,"3:%03.1f%c",current.temperatureF[2],DEG_SYMBOL);

	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"2:%03.1f   ",current.temperatureF[1]);	

	lcd_goto(LCD_LINE_TWO+8);
	printf(lcd_putch,"4:%03.1fF",current.temperatureF[3]);
}

void screen_sleep_state(void) {
	lcd_clear();

	printf(lcd_putch,"Sleep Okay?");
	lcd_goto(LCD_LINE_TWO);
	if ( action.sleep_okay ) {
		printf(lcd_putch,"YES!");
	} else {
		printf(lcd_putch,"NO...");
	}
}


void screen_power(void) {	
	float v_solar, v_batt;

	v_solar=(40.0/4096)*current.adc[5];
	v_batt=(5.0/4096)*current.adc[7];

	lcd_clear();
	
	printf(lcd_putch," Batt: %0.1lf volts",v_batt);
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"Solar: %0.1lf volts",v_solar);
}

void screen_power2(void) {	
	float v_solar5, v_ext;

	v_solar5=(10.0/4096)*current.adc[5];
	v_ext=(5.0/4096)*current.adc[4];

	lcd_clear();
	
	printf(lcd_putch,"Solar5: %0.1lf volts",v_solar5);
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"Extern: %0.1lf volts",v_ext);
}

void lcd_print_relay_mode(int8 now) {
	switch ( now ) {
		case RELAY_MODE_OFF:          printf(lcd_putch,"Off Always"); break;
		case RELAY_MODE_DIFFERENTIAL: printf(lcd_putch,"Differ."); break;
		case RELAY_MODE_ALARM_OVER:   printf(lcd_putch,"On Over"); break;
		case RELAY_MODE_ALARM_UNDER:  printf(lcd_putch,"On Under"); break;
		case RELAY_MODE_TEST_ON:      printf(lcd_putch,"On Always"); break;
//                                                      123456789
	}
}

void lcd_print_relay_mode_long(int8 now) {
	switch ( now ) {
		case RELAY_MODE_OFF:          printf(lcd_putch,"Off Always"); break;
		case RELAY_MODE_DIFFERENTIAL: printf(lcd_putch,"Differential"); break;
		case RELAY_MODE_ALARM_OVER:   printf(lcd_putch,"On Over"); break;
		case RELAY_MODE_ALARM_UNDER:  printf(lcd_putch,"On Under"); break;
		case RELAY_MODE_TEST_ON:      printf(lcd_putch,"On Always"); break;
	}
}


void screen_relay(int8 n) {
	lcd_clear();

	if ( 0==n ) {
		printf(lcd_putch,"A: ");
		if ( 0 == current.relay_status_a ) {
			printf(lcd_putch,"Off");
		} else {
			printf(lcd_putch,"On ");
		}
		lcd_putch('-');
		lcd_print_relay_mode(config.relay_mode[0]);
	
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch,"B: ");
		if ( 0 == current.relay_status_b ) {
			printf(lcd_putch,"Off");
		} else {
			printf(lcd_putch,"On ");
		}
		lcd_putch('-');
		lcd_print_relay_mode(config.relay_mode[1]);
	} else {
		printf(lcd_putch,"C: ");
		if ( 0 == current.relay_status_c ) {
			printf(lcd_putch,"Off");
		} else {
			printf(lcd_putch,"On ");
		}
		lcd_putch('-');
		lcd_print_relay_mode(config.relay_mode[2]);

	}
}

void screen_counter(void) {
	lcd_clear();
	printf(lcd_putch,"Count Total:");
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"%lu",current.pulse_count);
}



short screen_set_relay_test_on(int ch) {
	static short in=0;

	lcd_clear();

	if ( 0 == in ) {
		/* enter setup */
		printf(lcd_putch,"Relay %c Settings",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			in=1;
		}
	}
	if ( 1 == in ) {
		lcd_clear();
		printf(lcd_putch,"Relay always on.");
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch,"No settings.");

		delay_ms(2000);
		in=0;
	}
	return in;
}

short screen_set_relay_off(int8 ch) {
	static short in=0;

	lcd_clear();

	if ( 0 == in ) {
		/* enter setup */
		printf(lcd_putch,"Relay %c Settings",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			in=1;
		}
	}
	if ( 1 == in ) {
		lcd_clear();
		printf(lcd_putch,"Relay always off");
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch,"No settings.");

		delay_ms(2000);
		in=0;
	}
	return in;
}

short screen_set_relay_alarm_over(int ch) {
	/* states:
	0: menu not entered
	1: use channel 0
	2: use channel 1
	3: use channel 2
	4: use channel 3
	5: temperature set point
	6: write to memory and then return to state 0
	*/
	static int8 state=0;
	static signed int16 setPoint;
	float f;

	lcd_clear();

	if ( 0 == state ) {
		/* enter setup */
		printf(lcd_putch,"Relay %c Settings",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			state=1;
		}
	} else if ( state >= 1 && state <= 4 ) {
		printf(lcd_putch,"Use channel %u?",state);
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch,"YES   STAY   NO");
//                        0123456789012345

		/* star our current selection */
		if ( bit_test(config.over_temp_channel_mask[ch],state-1) ) {
			lcd_goto(LCD_LINE_TWO+3);
			lcd_putch('*');
		} else {
			lcd_goto(LCD_LINE_TWO+15);
			lcd_putch('*');
		}


		/* stay the same */
		if ( action.select_now ) {
			action.select_now=0;
			state++;
		}

		/* use */
		if ( action.down_now ) {
			action.down_now=0;
			bit_set(config.over_temp_channel_mask[ch],state-1);
			state++;
		}

		/* don't use */
		if ( action.up_now ) {
			action.up_now=0;
			bit_clear(config.over_temp_channel_mask[ch],state-1);
			state++;
		}

		if ( 4 == state ) {
			/* setup our next screen for entering temperature set point */
			setpoint=config.over_temp_temperatureC[ch];

			/* put into F if we need it */
			if ( UNITS_F==config.units ) {
				f=1.8* (float) setpoint + 32;
				setpoint=(int16) f;
			}

		}
	} else if ( 5==state ) {
		printf(lcd_putch,"Over T: %04ld%c",setpoint,DEG_SYMBOL);
		/* set delta t */
		if ( UNITS_F==config.units ) {
			lcd_putch('F');
		} else {
			lcd_putch('C');	
		}

		prompt_minus_set_plus();

		if ( action.up_now ) {
			action.up_now=0;
			if ( setpoint < 1023 ) 
				setpoint++;
			action.now_redraw=1;
		}
		if ( action.down_now ) {
			action.down_now=0;
			if ( setpoint > -1023 )
				setpoint--;
			action.now_redraw=1;
		}
		if ( action.select_now ) {
			action.select_now=0;

			if ( UNITS_F==config.units ) {
				f=(float) (setpoint-32) * 0.55556;
				setpoint=(int16) f;
			}

			config.over_temp_temperatureC[ch]=setpoint;
			action.now_redraw=1;
			state++;
		}
	} else {
		write_param_file();
		state=0;
	}
	return (state>0);
}

short screen_set_relay_alarm_under(int8 ch) {
	/* states:
	0: menu not entered
	1: use channel 0
	2: use channel 1
	3: use channel 2
	4: use channel 3
	5: temperature set point
	6: write to memory and then return to state 0
	*/
	static int8 state=0;
	static signed int16 setPoint;
	float f;

	lcd_clear();

	if ( 0 == state ) {
		/* enter setup */
		printf(lcd_putch,"Relay %c Settings",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			state=1;
		}
	} else if ( state >= 1 && state <= 4 ) {
		printf(lcd_putch,"Use channel %u?",state);
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch,"YES   STAY   NO");
//                        0123456789012345

		/* star our current selection */
		if ( bit_test(config.under_temp_channel_mask[ch],state-1) ) {
			lcd_goto(LCD_LINE_TWO+3);
			lcd_putch('*');
		} else {
			lcd_goto(LCD_LINE_TWO+15);
			lcd_putch('*');
		}


		/* stay the same */
		if ( action.select_now ) {
			action.select_now=0;
			state++;
		}

		/* use */
		if ( action.down_now ) {
			action.down_now=0;
			bit_set(config.under_temp_channel_mask[ch],state-1);
			state++;
		}

		/* don't use */
		if ( action.up_now ) {
			action.up_now=0;
			bit_clear(config.under_temp_channel_mask[ch],state-1);
			state++;
		}

		if ( 4 == state ) {
			/* setup our next screen for entering temperature set point */
			setpoint=config.under_temp_temperatureC[ch];

			/* put into F if we need it */
			if ( UNITS_F==config.units ) {
				f=1.8* (float) setpoint + 32;
				setpoint=(int16) f;
			}

		}
	} else if ( 5==state ) {
		printf(lcd_putch,"Under T: %04ld%c",setpoint,DEG_SYMBOL);
		/* set delta t */
		if ( UNITS_F==config.units ) {
			lcd_putch('F');
		} else {
			lcd_putch('C');	
		}

		prompt_minus_set_plus();

		if ( action.up_now ) {
			action.up_now=0;
			if ( setpoint < 1023 ) 
				setpoint++;
			action.now_redraw=1;
		}
		if ( action.down_now ) {
			action.down_now=0;
			if ( setpoint > -1023 )
				setpoint--;
			action.now_redraw=1;
		}
		if ( action.select_now ) {
			action.select_now=0;

			if ( UNITS_F==config.units ) {
				f=(float) (setpoint-32) * 0.55556;
				setpoint=(int16) f;
			}

			config.under_temp_temperatureC[ch]=setpoint;
			action.now_redraw=1;
			state++;
		}
	} else {
		write_param_file();
		state=0;
	}
	return (state>0);
}

short screen_set_relay_differential(int ch) {
	/* states:
	0: menu not entered
	1: select channel a
	2: select channel b
	3: set delta t
	4: set overtemp shutdown
	5: write to memory and then return to state 0
	*/
	static int8 state=0;
	static int8 aSide,bSide;
	static int16 deltaT;
	static int16 overTemp;
	float f;

	lcd_clear();

	if ( 0 == state ) {
		/* enter setup */
		printf(lcd_putch,"Relay %c Settings",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			state=1;
			aSide=config.differential_a_channel[ch];
			action.now_redraw=1;
		}
	} else if ( 1 == state ) {
		/* select first channel */
		printf(lcd_putch,"High side: %u",aSide+1);
		prompt_minus_set_plus();

		if ( action.select_now ) {
			action.select_now=0;
			action.now_redraw=1;
			bSide=config.differential_b_channel[ch];
			state=2;
		}
	
		if ( action.up_now ) {
			action.up_now=0;
			action.now_redraw=1;
			if ( aSide < 3 ) aSide++;
		}
		if ( action.down_now ) {
			action.down_now=0;
			action.now_redraw=1;
			if ( aSide > 0 ) aSide--;
		}
	} else if ( 2 == state ) {
		/* select second channel */	
		printf(lcd_putch,"Low side: %u",bSide+1);
		prompt_minus_set_plus();

		if ( action.select_now ) {
			action.select_now=0;
			action.now_redraw=1;
			if ( aSide != bSide ) {
				state=3;
				deltaT=config.differential_delta_temperatureC[ch];
				if ( UNITS_F==config.units ) {
					f=1.8* (float) deltaT;
					deltaT=(int16) f;
				}
			}
		}
	
		if ( action.up_now ) {
			action.up_now=0;
			action.now_redraw=1;
			if ( bSide < 3 ) bSide++;
		}
		if ( action.down_now ) {
			action.down_now=0;
			action.now_redraw=1;
			if ( bSide > 0 ) bSide--;
		}
	} else if ( 3 == state ) {
		printf(lcd_putch,"Delta T: %04lu%c",deltaT,DEG_SYMBOL);
		/* set delta t */
		if ( UNITS_F==config.units ) {
			lcd_putch('F');
		} else {
			lcd_putch('C');	
		}

		prompt_minus_set_plus();

		if ( action.up_now ) {
			action.up_now=0;
			if ( deltaT < 1023 ) 
				deltaT++;
			action.now_redraw=1;
		}
		if ( action.down_now ) {
			action.down_now=0;
			if ( deltaT > 0 )
				deltaT--;
			action.now_redraw=1;
		}
		if ( action.select_now ) {
			action.select_now=0;

			if ( UNITS_F==config.units ) {
				f=(float) (deltaT) * 0.55556;
				deltaT=(int16) f;
			}

			state=4;
			overTemp=config.differential_high_temperatureC[ch];
			if ( UNITS_F==config.units ) {
				f=1.8* (float) overTemp+32;
				overTemp=(int16) f;
			}
			action.now_redraw=1;
		}
	} else if ( 4 == state ) {
		/* select over temp lockout */
		printf(lcd_putch,"Hi Limit: %04lu%c",overTemp,DEG_SYMBOL);
		if ( UNITS_F==config.units ) {
			lcd_putch('F');
		} else {
			lcd_putch('C');	
		}

		prompt_minus_set_plus();

		if ( action.up_now ) {
			action.up_now=0;
			if ( overTemp < 1014 ) 
				overTemp+=5;

			action.now_redraw=1;
		}
		if ( action.down_now ) {
			action.down_now=0;
			if ( overTemp >= 5 )
				overTemp-=5;
			action.now_redraw=1;
		}
		if ( action.select_now ) {
			action.select_now=0;

			if ( UNITS_F==config.units ) {
				f=(float) (overTemp-32) * 0.55556;
				overTemp=(int16) f;
			}

			state=5;
			action.now_redraw=1;
		}
	} else if ( 5 == state ) {
		config.differential_a_channel[ch]=aSide;
		config.differential_b_channel[ch]=bSide;
		config.differential_delta_temperatureC[ch]=deltaT;
		config.differential_high_temperatureC[ch]=overTemp;
		write_param_file();
		state=0;
	}

	return (state>0);
}

short screen_set_relay_config(int ch) {
	switch ( config.relay_mode[ch] ) {
		case RELAY_MODE_TEST_ON:
			return screen_set_relay_test_on(ch);
		case RELAY_MODE_DIFFERENTIAL:
			return screen_set_relay_differential(ch);
		case RELAY_MODE_ALARM_OVER:
			return screen_set_relay_alarm_over(ch);
		case RELAY_MODE_ALARM_UNDER:
			return screen_set_relay_alarm_under(ch);
		case RELAY_MODE_OFF:
		default:
			return screen_set_relay_off(ch);
	}
}


/* select serial port baud rate */
short screen_set_rs232(void) {
	static short set=0; 
	static int now;
	char speed[4];

	lcd_clear();

	if ( ! set ) {
		/* prompt user */
		printf(lcd_putch,"RS-232 Setup");
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
			now=config.rs232_baud;
		}
	} else {
		switch ( now ) {
			case BAUD_1200: strcpy(speed,"12"); break;
			case BAUD_2400: strcpy(speed,"24"); break;
			case BAUD_4800: strcpy(speed,"48"); break;
			case BAUD_9600: strcpy(speed,"96"); break;
			case BAUD_19200: strcpy(speed,"192"); break;
			case BAUD_57600: strcpy(speed,"576"); break;
		}

		if ( BAUD_OFF == now ) {
			printf(lcd_putch,"RS-232 Disabled");
		} else {
			printf(lcd_putch,"Baud: %s00",speed);
		}

		/* mark as in use */
		if ( now == config.rs232_baud ) 
			lcd_putch('*');

		prompt_minus_set_plus();

		/* slower */
		if ( action.down_now ) {
			action.down_now=0;
			if ( now != 0 )
				now--;
		}
		/* use */
		if ( action.select_now ) {
			action.select_now=0;
			config.rs232_baud=now;

			/* store our settings and change now */
			init_rs232();
			write_param_file();
			set=0;
			action.now_redraw=1;
		}
		/* faster */
		if ( action.up_now ) {
			action.up_now=0;
			if ( now != BAUD_MAX_N )
				now++;
		}
	}

	return set;

}

/* select sensor type */
short screen_set_sensor_type(void) {
	static short set=0; 
	static int now;

	lcd_clear();

	if ( ! set ) {
		/* prompt user */
		printf(lcd_putch,"Sensor Type");
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
			now=config.sensor_type;
		}
	} else {
		switch ( now ) {
			case SENSOR_AN_0_5:   printf(lcd_putch,"0 to 5V Analog"); break;
			case SENSOR_AN_5_0:   printf(lcd_putch,"5 to 0V Analog"); break;
			case SENSOR_AN_VOLTS: printf(lcd_putch,"Raw Analog Volt"); break;
			case SENSOR_AN_ADC:   printf(lcd_putch,"Raw ADC"); break;
			default: printf(lcd_putch,"NTC Thermistor"); break;
		}


		/* mark as in use */
		if ( now == config.sensor_type ) 
			lcd_putch('*');

		prompt_minus_set_plus();

		/* slower */
		if ( action.down_now ) {
			action.down_now=0;
			if ( now != 0 )
				now--;
		}
		/* use */
		if ( action.select_now ) {
			action.select_now=0;
			config.sensor_type=now;

			/* store our settings and change now */
			write_param_file();
			set=0;
			action.now_redraw=1;
		}
		/* faster */
		if ( action.up_now ) {
			action.up_now=0;
			if ( now != SENSOR_MAX_N )
				now++;
		}
	}

	return set;

}

/* select sensor type */
short screen_set_relay_c_type(void) {
	static short set=0; 
	static int now;

	lcd_clear();

	if ( ! set ) {
		/* prompt user */
		printf(lcd_putch,"Relay C Type");
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
			now=config.relay_c_type;
		}
	} else {
		switch ( now ) {
			case RELAY_C_TYPE_TRISTAR:   printf(lcd_putch,"Tri-Star (0)"); break;
			default: printf(lcd_putch,"Disabled"); break;
		}


		/* mark as in use */
		if ( now == config.relay_c_type ) 
			lcd_putch('*');

		prompt_minus_set_plus();

		/* slower */
		if ( action.down_now ) {
			action.down_now=0;
			if ( now != 0 )
				now--;
		}
		/* use */
		if ( action.select_now ) {
			action.select_now=0;
			config.relay_c_type=now;

			/* store our settings and change now */
			write_param_file();
			set=0;
			action.now_redraw=1;
		}
		/* faster */
		if ( action.up_now ) {
			action.up_now=0;
			if ( now != RELAY_C_MAX_N )
				now++;
		}
	}

	return set;

}


/* determine how the relay is actuated */
short screen_set_relay_mode(int ch) {
	static short set=0; 
	static int now;

	lcd_clear();

	if ( ! set ) {
		/* prompt user */
		printf(lcd_putch,"Relay %c Mode",'A'+ch);
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
			now=config.relay_mode[ch];
		}
	} else {
		lcd_print_relay_mode_long(now);

		/* mark as in use */
		if ( now == config.relay_mode[ch] ) 
			lcd_putch('*');

		prompt_minus_set_plus();

		/* slower */
		if ( action.down_now ) {
			action.down_now=0;
			if ( now != 0 )
				now--;
		}
		/* use */
		if ( action.select_now ) {
			action.select_now=0;
			config.relay_mode[ch]=now;

			/* store our settings and change now */
			write_param_file();
			set=0;
			action.now_redraw=1;
		}
		/* faster */
		if ( action.up_now ) {
			action.up_now=0;
			if ( now != RELAY_MODE_MAX_N )
				now++;
		}
	}

	return set;

}

short screen_set_units() {
	static short set=0;

	lcd_clear();

	if ( ! set ) {
		/* prompt user */
		printf(lcd_putch,"Temperature Unit");
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
		}
	} else {
		printf(lcd_putch,"Display Units:");
		lcd_goto(LCD_LINE_TWO);
		printf(lcd_putch," C    STAY    F");

		if ( UNITS_C==config.units ) { 
			lcd_goto(LCD_LINE_TWO+2);
			lcd_putch('*');
		} else if ( UNITS_F==config.units ) {
			lcd_goto(LCD_LINE_TWO+15);
			lcd_putch('*');
		}

		if ( action.down_now ) {
			action.down_now=0;
			config.units=UNITS_C;
			write_param_file();
			set=0;
		}
		if ( action.select_now ) {
			action.select_now=0;
			set=0;
		}
		if ( action.up_now ) {
			action.up_now=0;
			config.units=UNITS_F;
			write_param_file();
			set=0;
		}
	}

	return set;

}

short screen_reset_defaults(void) {
	static short set=0;

	if ( ! set ) {
		/* display prompt and wait for them to enter */
		printf(lcd_putch,"Restore Defaults");
		prompt_prev_set_next();

		if ( action.select_now ) {
			action.select_now=0;
			set=1;
		}
	} else {
		printf(lcd_putch,"Restore Defaults");
		prompt_no_yes();

		/* drop back out */
		if ( action.down_now ) {
			action.down_now=0;
			set=0;
		}

		/* restore defaults and reboot */
		if ( action.select_now ) {
			write_default_param_file();
			reset_cpu();
		}

		/* ignore */
		if ( action.up_now ) {
			action.up_now=0;
		}
	}

	return set;
}


#define MENU_SETUP_GENERAL_MAX 11
short menu_setup(void) {
	static int menu_item=0;
	short has_buttons=0;

	lcd_clear();

	if ( 0 == menu_item ) {
		/* prompt for menu */
		printf(lcd_putch,"Main Setup Menu");
		prompt_prev_enter_next();

		if ( action.select_now ) {
			action.select_now=0;
			menu_item=1;
		}
	} else {
		/* present our menu */

		switch ( menu_item ) {
			case 1:  has_buttons=screen_set_relay_mode(0); break;
			case 2:  has_buttons=screen_set_relay_config(0); break; 
			case 3:  has_buttons=screen_set_relay_mode(1); break;
			case 4:  has_buttons=screen_set_relay_config(1); break;
			case 5:  has_buttons=screen_set_units(); break;
			case 6:  has_buttons=screen_set_rs232(); break;
			case 7:  has_buttons=screen_set_relay_c_type(); break;
			case 8:  has_buttons=screen_set_relay_mode(2); break;
			case 9:  has_buttons=screen_set_relay_config(2); break;
			case 10: has_buttons=screen_set_sensor_type(); break;
			case 11: has_buttons= screen_reset_defaults(); break;
		}

		/* make sure the buttons are ours */
		if ( ! has_buttons ) {
			if ( action.down_now ) {
				action.down_now=0;

				/* no need to loop, since we'll drop out at menu_item=0 */
				menu_item--;
			}
			if ( action.up_now ) {
				action.up_now=0;	

				if ( MENU_SETUP_GENERAL_MAX == menu_item ) {
					menu_item=0;
				} else {
					menu_item++;
				}
			}
		}
	}

	return (0 != menu_item);
}



#define MAX_SCREEN 5
void screen_select(void) {
	static int8 screen;
	static short has_buttons=0;
	signed int8 dir;

	if ( ! has_buttons && action.up_now ) {
		action.up_now=0;
		dir=1;

		if ( screen<MAX_SCREEN ) {
			screen++;
		} else {
			screen=0;
		}
	}

	if ( ! has_buttons && action.down_now ) {
		action.down_now=0;
		dir=-1;

		if ( screen != 0 ) {
			screen--;
		} else {
			screen=MAX_SCREEN;
		}
	}


screen_loop:
	switch ( screen ) {
		/* display only our current units */
		case 0:	if ( UNITS_F==config.units ) 
					screen_temp_f(); 
				else 
					screen_temp_c(); 
				has_buttons=0; 
				break;
		case 1:  screen_power(); has_buttons=0; break;
		case 2:  screen_relay(0); has_buttons=0; break;
		/* skip virtual relay and counter if RS-232 disabled */
		case 3: if ( BAUD_OFF==config.rs232_baud ) {
					screen+=dir;
					goto screen_loop;
				} else {
					screen_relay(1); has_buttons=0; break;
				}
		case 4: if ( BAUD_OFF==config.rs232_baud ) {
					screen+=dir;
					goto screen_loop;
				} else {
					screen_counter(); has_buttons=0; break;
				}
		case 5:  has_buttons=menu_setup(); break;

	}

}

void screen_set_serial(void) {
	int16 last_serial_number;

	lcd_clear();

	printf(lcd_putch,"Setting SN...");
	delay_ms(2500);

	/* set our values for next state */
	keypad.value=config.serial_number; 
	keypad.field=0;
	keypad.start_pos=9;

	last_serial_number=65535;

#ignore_warnings 203
	while ( 1 ) {
#ignore_warnings NONE
		restart_wdt();

		if ( last_serial_number != keypad.value ) {
			lcd_clear();
			printf(lcd_putch,"Serial: %c",config.serial_prefix);
			last_serial_number=keypad.value;
		}

		if ( screen_get_int16() ) {
			config.serial_number=keypad.value;
			break;
		}
	}

	lcd_clear();
	printf(lcd_putch,"New Serial #:");
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"%c%05lu",config.serial_prefix,config.serial_number);

	delay_ms(3000);

	write_param_file();
}