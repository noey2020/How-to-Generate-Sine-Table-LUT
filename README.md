# How-to-Generate-Sine-Table-LUT

Generate Sine Table as Look-Up Table(LUT)    July 9, 2020

I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

Hire me!

Garden variety microcontrollers often have no floating point units(FPU). When more than
integer computation is needed, software techniques are substituted for floating point
computations. Relying on software makes it quite expensive in terms of CPU usage and 
speed. Common ALU operations inside the core are just the basic arithmetic logic with
no sophisticated DSP, MAC, memory protections to take advantage of. We haven't even 
touched on the topic of moving or transferring data from/to memory to peripherals like 
DACs and ADCs.

So instead of "on the fly" computations, we can pre-calculate and just look-up the values
and go from there. On movement of data, sophisticated or dedicated peripherals like the
DMA or direct memory access is a good technique to emply. DMA implies "no CPU" 
intervention hence, no CPU performance penalty.

So LUTs are just arrays of values we pre-calculate that "we look up". Quite convenient
and witty!

We can design just any waveform you may think of. But before we go on to arbitrary 
waveform generation, we first need to take care of wave table generation. For a a basic 
waveform we choose a sine table look-up table to generate.

We present a matlab script and a C program to demonstrate the process. How many points we
need depends on the resolution of converters. For a 12-bit DAC, that means 0xFFF or 2 to
the 12th power giving us a total of 4096 points to differentiate for each level.

Referring back to the sine function, we know that theta varies from 0 to 2*pi. Using 
symmetry, we calculate only for theta values from 0 to pi/4 or 0 to 90degrees. We offset
by 4096/2 to take care of negative values.

There is a sine function in math.h and in matlab for brevity sake, but as a mental 
exercise we write code to specify each step of sine wave value computation.

This will serve as a prelude to digital to analog converters(DAC). The STM32L1 Discovery
kit is an abbreviated M4 ARM core with no DSP and no FPU and hence, an M3 ARM core only.
But it has ADC, DAC, DMA we can use. The LUT will allow us to do  enhanced floating
point calculations and use DMA to do data transfers. 

More information in the succeeding installments.

Here's the matlab output and C program to tinker with:

2048, 2149, 2250, 2350, 2450, 2549, 2646, 2742, 2837, 2929, 3020, 3108, 3193, 3275, 3355, 

3431, 3504, 3574, 3639, 3701, 3759, 3812, 3861, 3906, 3946, 3982, 4013, 4039, 4060, 4076, 

4087, 4094, 4095, 4091, 4082, 4069, 4050, 4026, 3998, 3965, 3927, 3884, 3837, 3786, 3730, 

3671, 3607, 3539, 3468, 3394, 3316, 3235, 3151, 3064, 2975, 2883, 2790, 2695, 2598, 2500, 

2400, 2300, 2199, 2098, 1997, 1896, 1795, 1695, 1595, 1497, 1400, 1305, 1212, 1120, 1031, 

944, 860, 779, 701, 627, 556, 488, 424, 365, 309, 258, 211, 168, 130, 97, 

69, 45, 26, 13, 4, 0, 1, 8, 19, 35, 56, 82, 113, 149, 189, 

234, 283, 336, 394, 456, 521, 591, 664, 740, 820, 902, 987, 1075, 1166, 1258, 

1353, 1449, 1546, 1645, 1745, 1845, 1946, 2047, 

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

I included some posts for reference.

https://github.com/noey2020/How-to-Generate-Triangular-Wave-STM32-DAC

https://github.com/noey2020/How-to-Illustrate-Multiple-Exceptions-

https://github.com/noey2020/How-to-Blink-LED-using-Standard-Peripheral-Library

I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

Hire me!

Have fun and happy coding!
