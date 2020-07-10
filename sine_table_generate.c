#include <stdio.h>
#include <string.h>
#include <math.h>

unsigned int lookup_sine_table(int x);

signed int sine_table[91];

int main(void){
    int i, j;

    float st_val, PI=3.1415926;

    /* For 12-bit ADC, range is from 0 to 0xFFF(4095) with 4096 approximated to 0xFFF with
       offset of 2048 to take care of negative numbers. */
    for(i = 0; i <= 90; i++){
        st_val = sin(PI * i / 180);
        sine_table[i] = (1 + st_val) * 2048;
		if(sine_table[i] == 0x1000)
		    sine_table[i] = 0xFFF;     /* sin(90) results in 4096 or 13 bits so truncate to 0xFFF */
	}
	printf("Sine Table\n");
	for(i = 0; i < 90; i += 8){
		printf("\tDCD\t");
		printf("0x%03x, 0x%03x, 0x%03x, 0x%03x, 0x%03x\n",
				sine_table[i], sine_table[i + 1], sine_table[i + 2], sine_table[i + 3], sine_table[i + 4]);
	}
	printf("\tDCD\t0x%03x\n\n", sine_table[90]);  /* 0x1000 truncated to 0xFFF */

	j = lookup_sine_table(30);

	printf("\tsin(30) = 0x%03x\n", j);

	return 0;
}

unsigned int lookup_sine_table(int x){
    x = x % 360;                       /* remainder not truncation, not x / 360; */
    if(x < 90) return sine_table[x];
    if(x < 180) return sine_table[180 - x];
    if(x < 270) return 4086 - sine_table[x - 180];
    return 4096 - sine_table[360 - x];
}
