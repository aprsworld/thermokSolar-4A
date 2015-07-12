int16 crc_chk(int8 *data, int8 length) {
	int8 j;
	int16 reg_crc=0xFFFF;

	while ( length-- ) {
		reg_crc ^= *data++;

		for ( j=0 ; j<8 ; j++ ) {
			if ( reg_crc & 0x01 ) {
				reg_crc=(reg_crc>>1) ^ 0xA001;
			} else {
				reg_crc=reg_crc>>1;
			}
		}	
	}
	
	return reg_crc;
}


void live_send(void) {
	int8 buff[38];
	int16 lCRC;
	int8 i;
	int8 *p;

	/* wake up our modem */
	output_low(MODEM_SLEEP);
	delay_ms(14);

	buff[0]='#';
	buff[1]=config.serial_prefix;
	buff[2]=make8(config.serial_number,1);
	buff[3]=make8(config.serial_number,0);
	buff[4]=38; /* packet length */
	buff[5]=12; /* packet type */

	/* floating point temperature */
	p=(int8 *)&current.temperatureC[0];
	buff[6]=p[0];
	buff[7]=p[1];
	buff[8]=p[2];
	buff[9]=p[3];

	p=(int8 *)&current.temperatureC[1];
	buff[10]=p[0];
	buff[11]=p[1];
	buff[12]=p[2];
	buff[13]=p[3];

	p=(int8 *)&current.temperatureC[2];
	buff[14]=p[0];
	buff[15]=p[1];
	buff[16]=p[2];
	buff[17]=p[3];

	p=(int8 *)&current.temperatureC[3];
	buff[18]=p[0];
	buff[19]=p[1];
	buff[20]=p[2];
	buff[21]=p[3];

	/* analog channels */
	buff[22]=make8(current.adc[4],1);
	buff[23]=make8(current.adc[4],0);
	buff[24]=make8(current.adc[5],1);
	buff[25]=make8(current.adc[5],0);
	buff[26]=make8(current.adc[6],1);
	buff[27]=make8(current.adc[6],0);
	buff[28]=make8(current.adc[7],1);
	buff[29]=make8(current.adc[7],0);

	/* relays */
	buff[30]=current.relay_status_a;
	buff[31]=current.relay_status_b;
	buff[32]=current.relay_status_c;
	buff[33]=0;

	buff[34]=make8(current.pulse_count,1);
	buff[35]=make8(current.pulse_count,0);

	lCRC=crc_chk(buff+1,35);
	buff[36]=make8(lCRC,1);
	buff[37]=make8(lCRC,0);

	for ( i=0 ; i<sizeof(buff) ; i++ ) {
		/* xbee modem */
		fputc(buff[i],modem);
	}	

	/* put our modem back to sleep */
	output_high(MODEM_SLEEP);
}

