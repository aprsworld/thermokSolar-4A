 #inline
char xor_crc(char oldcrc, char data) {
	return oldcrc ^ data;
}

char EEPROMDataRead( int16 address, char *data, int16 count ) {
	char crc=0;

	while ( count-- != 0 ) {
		*data = read_eeprom( address++ );
		crc = xor_crc(crc,*data);
		data++;
	}
	return crc;
}

char EEPROMDataWrite( int16 address, char *data, int16 count ) {
	char crc=0;

	while ( count-- != 0 ) {
		/* restart_wdt() */
		crc = xor_crc(crc,*data);
		write_eeprom( address++, *data++ );
	}

	return crc;
}

void write_param_file() {
	int8 crc;

	/* write the config structure */
	crc = EEPROMDataWrite(PARAM_ADDRESS,(void *)&config,sizeof(config));
	/* write the CRC was calculated on the structure */
	write_eeprom(PARAM_CRC_ADDRESS,crc);

#if 0
	lcd_clear();
	lcd_goto(LCD_LINE_ONE);
	printf(lcd_putch,"CRC: 0x%02X (write)",crc);
	delay_ms(1000);
#endif
}

void write_default_param_file() {
	/* print message on LCD informing user */
	lcd_clear();
	printf(lcd_putch,"Loading default");
	lcd_goto(LCD_LINE_TWO);
	printf(lcd_putch,"parameters!");
	
	delay_ms(3000);

#if 0
	/* serial number default */
	config.serial_prefix='A';
	config.serial_number=0;

	/* revision */
	config.revision='a';

	/* serial port baud rate */
	config.rs232_baud=BAUD_OFF; //9600;

	/* first relay */
	config.relay_mode[0]=RELAY_MODE_DIFFERENTIAL; 
	config.differential_a_channel[0]=0;
	config.differential_b_channel[0]=3;
	config.differential_delta_temperatureC[0]=11;
	config.differential_high_temperatureC[0]=85;
	config.over_temp_channel_mask[0]=0b1111;
	config.over_temp_temperatureC[0]=95;
	config.under_temp_channel_mask[0]=0b1111;
	config.under_temp_temperatureC[0]=5;

	/* second relay */
	config.relay_mode[1]=RELAY_MODE_ALARM_OVER;
	config.differential_a_channel[1]=1;
	config.differential_b_channel[1]=3;
	config.differential_delta_temperatureC[1]=11;
	config.differential_high_temperatureC[1]=85;
	config.over_temp_channel_mask[1]=0b1111;
	config.over_temp_temperatureC[1]=85;
	config.under_temp_channel_mask[1]=0b1111;
	config.under_temp_temperatureC[1]=5;

	/* third relay, TriStar */
	config.relay_mode[2]=RELAY_MODE_ALARM_OVER;
	config.differential_a_channel[2]=1;
	config.differential_b_channel[2]=3;
	config.differential_delta_temperatureC[2]=11;
	config.differential_high_temperatureC[2]=85;
	config.over_temp_channel_mask[2]=0b1111;
	config.over_temp_temperatureC[2]=85;
	config.under_temp_channel_mask[2]=0b1111;
	config.under_temp_temperatureC[2]=5;


	/* should we log when on battery power */
	config.log_on_battery=0;

	/* default to degrees F */
	config.units=UNITS_F;

	/* scaling of 0 to 5 volt sensor input */
	config.sensor_type=SENSOR_THERMISTOR_NTC;

	/* type of virtual relay */
	config.relay_c_type=RELAY_C_TYPE_DISABLED;
#else
	/* serial number default */
	config.serial_prefix='A';
	config.serial_number=0;

	/* revision */
	config.revision='a';

	/* serial port baud rate */
	config.rs232_baud=BAUD_9600;

	/* first relay */
	config.relay_mode[0]=RELAY_MODE_OFF; 
	config.differential_a_channel[0]=0;
	config.differential_b_channel[0]=3;
	config.differential_delta_temperatureC[0]=11;
	config.differential_high_temperatureC[0]=85;
	config.over_temp_channel_mask[0]=0b1111;
	config.over_temp_temperatureC[0]=95;
	config.under_temp_channel_mask[0]=0b1111;
	config.under_temp_temperatureC[0]=5;

	/* second relay */
	config.relay_mode[1]=RELAY_MODE_OFF;
	config.differential_a_channel[1]=1;
	config.differential_b_channel[1]=3;
	config.differential_delta_temperatureC[1]=11;
	config.differential_high_temperatureC[1]=85;
	config.over_temp_channel_mask[1]=0b1111;
	config.over_temp_temperatureC[1]=85;
	config.under_temp_channel_mask[1]=0b1111;
	config.under_temp_temperatureC[1]=5;

	/* third relay, TriStar */
	config.relay_mode[2]=RELAY_MODE_ALARM_OVER;
	config.differential_a_channel[2]=1;
	config.differential_b_channel[2]=3;
	config.differential_delta_temperatureC[2]=11;
	config.differential_high_temperatureC[2]=24;
	config.over_temp_channel_mask[2]=0b0001;
	config.over_temp_temperatureC[2]=24;
	config.under_temp_channel_mask[2]=0b0001;
	config.under_temp_temperatureC[2]=5;


	/* should we log when on battery power */
	config.log_on_battery=0;

	/* default to degrees F */
	config.units=UNITS_F;

	/* scaling of 0 to 5 volt sensor input */
	config.sensor_type=SENSOR_THERMISTOR_NTC;

	/* type of virtual relay */
	config.relay_c_type=RELAY_C_TYPE_TRISTAR;
#endif

	/* write them so next time we use from EEPROM */
	write_param_file();
}


void read_param_file() {
	int8 crc;

	crc = EEPROMDataRead(PARAM_ADDRESS, (void *)&config, sizeof(config)); 
		
	if ( crc != read_eeprom(PARAM_CRC_ADDRESS) || config.revision<'a' || config.revision>'z' ) {
		write_default_param_file();
	}
}

