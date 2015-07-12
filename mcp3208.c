
void mcp3208_init(void) {
	output_high(MCP3208_NCS);
}

int16 mcp3208_read(int8 ch) {
	int16 value;
	int8 i;
	int8 c;

	output_low(MCP3208_CLK);
	output_high(MCP3208_DIN);
	output_low(MCP3208_NCS);
	
	if ( 0 == ch ) 
		c=0b00011;
	else if ( 1 == ch ) 
		c=0b10011;
	else if ( 2 == ch ) 
		c=0b01011;
	else if ( 3 == ch ) 
		c=0b11011;
	else if ( 4 == ch )
		c=0b00111;
	else if ( 5 == ch ) 
		c=0b10111;
	else if ( 6 == ch )
		c=0b01111;
	else
		c=0b11111;

	/* select out channel and start the conversion */
	for ( i=0 ; i<5 ; i++ ) {
		output_low(MCP3208_CLK);
		output_bit(MCP3208_DIN,c&1);
		c=c>>1;
		output_high(MCP3208_CLK);
	}


	value=0;
	for ( i=0 ; i<14 ; i++ ) {
		output_low(MCP3208_CLK);
		shift_left(&value,2,input(MCP3208_DOUT));
		output_high(MCP3208_CLK);
	}

	bit_clear(value,13);
	bit_clear(value,12);

	output_high(MCP3208_NCS);

	return value;
}