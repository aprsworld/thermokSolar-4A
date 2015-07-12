#int_timer2
void isr_1ms(void) {
	static int16 millisecond=0;
	static int8 adcPeriod=0;

	/* button debouncing */
	static int16 b0_state=0;
	static int16 b1_state=0;
	static int16 b2_state=0;

	/* button must be down for 12 milliseconds */
	b0_state=(b0_state<<1) | !input(BUTTON_0) | 0xe000;
	if ( b0_state==0xf000) {
		action.up_now=1;
		action.now_redraw=1;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}
	b1_state=(b1_state<<1) | !input(BUTTON_1) | 0xe000;
	if ( b1_state==0xf000) {
		action.select_now=1;
		action.now_redraw=1;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}
	b2_state=(b2_state<<1) | !input(BUTTON_2) | 0xe000;
	if ( b2_state==0xf000) {
		action.down_now=1;
		action.now_redraw=1;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}

#if 0
	if ( 0xffff != timers.pulse_period ) 
		timers.pulse_period++;
#endif

#if 0
	if ( timers.button_0_bounce > 0 ) timers.button_0_bounce--;
	if ( timers.button_1_bounce > 0 ) timers.button_1_bounce--;
	if ( timers.button_2_bounce > 0 ) timers.button_2_bounce--;
#endif

	/* every 125 ms sample ADC */
	adcPeriod++;
	if ( 125==adcPeriod ) {
		action.now_adc=1;
		adcPeriod=0;
	}

	/* one second rate */
	millisecond++;
	if ( 992 == millisecond ) {
		millisecond=0;
		action.now_redraw=1;
		action.now_tick=1;

		current.second++;
		if ( 59 == current.second ) {
			current.second=0;
			action.now_log=1;
		}

		/* control backlight */
		if (  0 == timers.backlight_seconds ) {
			/* auto, and we have finished counting down */
			output_high(LCD_BACKLIGHT);
		} else {
			output_low(LCD_BACKLIGHT);
			timers.backlight_seconds--;
		}

		if ( 0 == timers.sleep_seconds ) {
			action.sleep_okay=1;
		} else {
			action.sleep_okay=0;
			timers.sleep_seconds--;
		}

	}

}

#int_ext
/* high resolution pulse timer / counter triggered on rising edge */
void isr_ext0(void) {
//	static short state=0;

	current.pulse_count++;

#if 0
	if ( 1 == state ) {
		/* currently counting, time to finish */
		disable_interrupts(INT_TIMER2);
		current.pulse_period=timers.pulse_period;
		if ( current.pulse_period < current.pulse_min_period ) {
			current.pulse_min_period=current.pulse_period;
		}
		state=0;
	}

	if ( 0 == state ) {
		/* not counting, time to start */
		timers.pulse_period=0;
		set_timer2(0);
		enable_interrupts(INT_TIMER2);
		state=1;
	}
#endif
}


#int_rb
void isr_rb(void) {
	static int8 last_b;
	int8 b;

	/* current port b must be read before interrupt will quite firing */
	b=port_b;	

#if 0
	/* handle button events and debounce */
	if ( 0==timers.button_0_bounce && 0==bit_test(b,BUTTON_0_BIT) && 1==bit_test(last_b,BUTTON_0_BIT) ) { 
		action.up_now=1;
		action.now_redraw=1;
		timers.button_0_bounce=BUTTON_BOUNCE;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}

	if ( 0==timers.button_1_bounce && 0==bit_test(b,BUTTON_1_BIT) && 1==bit_test(last_b,BUTTON_1_BIT) ) { 
		action.select_now=1;
		action.now_redraw=1;
		timers.button_1_bounce=BUTTON_BOUNCE;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}

	if ( 0==timers.button_2_bounce && 0==bit_test(b,BUTTON_2_BIT) && 1==bit_test(last_b,BUTTON_2_BIT) ) { 
		action.down_now=1;
		action.now_redraw=1;
		timers.button_2_bounce=BUTTON_BOUNCE;
		timers.backlight_seconds=BACKLIGHT_TIMEOUT_SECONDS;
		timers.sleep_seconds=SLEEP_AFTER_SECONDS;
	}
#endif

	last_b=b;
}



#int_rda
void modem_serial_isr(void) {
	static int8 last[8];

	last[7]=last[6];
	last[6]=last[5];
	last[4]=last[3];
	last[2]=last[1];
	last[1]=last[0];
	last[0]=fgetc(modem);

}
