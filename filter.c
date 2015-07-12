


#define MEAN_FILTER_WIDTH  16

int16 mean_filter_ch0(int16 latest_element) {
	static int16 input_buffer[MEAN_FILTER_WIDTH];
	static char inbuf_index = 0;
	static char num_elements = 0;
	int32 mean;
	int32 sum;
	char i;

	// Insert incoming data element into circular input buffer.
	input_buffer[inbuf_index] = latest_element;
	inbuf_index++;
	if(inbuf_index >= MEAN_FILTER_WIDTH)  // If index went past buffer end
	   inbuf_index = 0;       // then reset it to start of buffer

	if(num_elements < MEAN_FILTER_WIDTH)
	   num_elements++;

	// Calculate the mean.  This is done by summing up the
	// values and dividing by the number of elements.
	sum = 0;
	for(i = 0; i < num_elements; i++)
		sum += input_buffer[i];

	// Round-off the result by adding half the divisor to
	// the numerator.
	mean = (sum + (int32)(num_elements >> 1)) / num_elements;

	return((int16)mean);
}

int16 mean_filter_ch1(int16 latest_element) {
	static int16 input_buffer[MEAN_FILTER_WIDTH];
	static char inbuf_index = 0;
	static char num_elements = 0;
	int32 mean;
	int32 sum;
	char i;

	// Insert incoming data element into circular input buffer.
	input_buffer[inbuf_index] = latest_element;
	inbuf_index++;
	if(inbuf_index >= MEAN_FILTER_WIDTH)  // If index went past buffer end
	   inbuf_index = 0;       // then reset it to start of buffer

	if(num_elements < MEAN_FILTER_WIDTH)
	   num_elements++;

	// Calculate the mean.  This is done by summing up the
	// values and dividing by the number of elements.
	sum = 0;
	for(i = 0; i < num_elements; i++)
		sum += input_buffer[i];

	// Round-off the result by adding half the divisor to
	// the numerator.
	mean = (sum + (int32)(num_elements >> 1)) / num_elements;

	return((int16)mean);
}

int16 mean_filter_ch2(int16 latest_element) {
	static int16 input_buffer[MEAN_FILTER_WIDTH];
	static char inbuf_index = 0;
	static char num_elements = 0;
	int32 mean;
	int32 sum;
	char i;

	// Insert incoming data element into circular input buffer.
	input_buffer[inbuf_index] = latest_element;
	inbuf_index++;
	if(inbuf_index >= MEAN_FILTER_WIDTH)  // If index went past buffer end
	   inbuf_index = 0;       // then reset it to start of buffer

	if(num_elements < MEAN_FILTER_WIDTH)
	   num_elements++;

	// Calculate the mean.  This is done by summing up the
	// values and dividing by the number of elements.
	sum = 0;
	for(i = 0; i < num_elements; i++)
		sum += input_buffer[i];

	// Round-off the result by adding half the divisor to
	// the numerator.
	mean = (sum + (int32)(num_elements >> 1)) / num_elements;

	return((int16)mean);
}

int16 mean_filter_ch3(int16 latest_element) {
	static int16 input_buffer[MEAN_FILTER_WIDTH];
	static char inbuf_index = 0;
	static char num_elements = 0;
	int32 mean;
	int32 sum;
	char i;

	// Insert incoming data element into circular input buffer.
	input_buffer[inbuf_index] = latest_element;
	inbuf_index++;
	if(inbuf_index >= MEAN_FILTER_WIDTH)  // If index went past buffer end
	   inbuf_index = 0;       // then reset it to start of buffer

	if(num_elements < MEAN_FILTER_WIDTH)
	   num_elements++;

	// Calculate the mean.  This is done by summing up the
	// values and dividing by the number of elements.
	sum = 0;
	for(i = 0; i < num_elements; i++)
		sum += input_buffer[i];

	// Round-off the result by adding half the divisor to
	// the numerator.
	mean = (sum + (int32)(num_elements >> 1)) / num_elements;

	return((int16)mean);
}
