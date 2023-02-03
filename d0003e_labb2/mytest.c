/*
 * d0003e_labb2.c
 *
 * Created: 2023-01-26 10:37:00
 * Author : simon
 */ 

#include "tinythreads.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>
#include <stdio.h>




#define BlankValue  0x0000
#define ZeroValue   0x1551 //0001010101010001 0x1 0x5 0x5 0x1 = 0x1551
#define OneValue    0x2080 //0000100000000010 0x2 0x0 0x8 0x0 = 0x2080 
#define TwoValue    0x1e11 //0001000111100001 0x1 0xd 0x1 0x1 = 0x1d11
#define ThreeValue  0x1b11 //0001000110110001 0x1 0xb 0x1 0x1 = 0x1b11
#define FourValue   0x0b50 //0000010110110000 0x0 0xb 0x5 0x0 = 0x0b50
#define FiveValue   0x1b41 //0001010010110001 0x1 0xb 0x4 0x1 = 0x1b41
#define SixValue    0x1f41 //0001010011110001 0x1 0xf 0x4 0x1 = 0x1f41
#define SevenValue  0x0111 //0001000100010000 0x0 0x1 0x1 0x1 = 0x0111
#define EightValue  0x1f51 //0001010111110001 0x1 0xf 0x5 0x1 = 0x1f51
#define NineValue   0x0b51 //0001010110110000 0x0 0xb 0x5 0x1 = 0x0b51 
//#define ValueArray (int[]){ZeroValue,OneValue,TwoValue,ThreeValue,FourValue,FiveValue,SixValue,SevenValue,EightValue,NineValue,BlankValue}	
int ValueArray[] = {ZeroValue,OneValue,TwoValue,ThreeValue,FourValue,FiveValue,SixValue,SevenValue,EightValue,NineValue,BlankValue};
int pp;

mutex m = MUTEX_INIT;

void LCD_Init(void) {
	CLKPR = (1 << CLKPCE); 
	CLKPR = 0x00;

    // Use external asynchronous clock source 
    // 1/3 Bias and 1/4 duty, 25 segments enabled 
    LCDCRB = (1<<LCDCS) | (1<<LCDMUX1)| (1<<LCDMUX0)| (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);
	// 
    // Using 16 as prescaler selection and 8 as LCD Clock Divide 
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0);
    // Set segment drive time to 300 us and output voltage to 3.35 V
    LCDCCR = (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1)| (1<<LCDCC0);
    // Enable LCD, low power waveform, no frame interrupt, no blanking 
    LCDCRA = (1<<LCDEN) | (1<<LCDAB) |  (0<<LCDBL);
}

void writeChar(char ch, int pos) {
	

	volatile int i = 0;
	if (pos < 0 || pos > 5) {
		return; // do nothing if pos is outside the supported range
	}
	if ((int)ch < (0+48) || (int)ch > (9+48)) {
		i=10;
	}else{
        i = ch - 48;
    }
	if(pos==0){
		LCDDR0 =  (LCDDR0&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR5 =  (LCDDR5&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR10 = (LCDDR10&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR15 = (LCDDR15&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==1){
		LCDDR0 =     (LCDDR0&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR5 =     (LCDDR5&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR10 =    (LCDDR10&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR15 =    (LCDDR15&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
	} else if(pos==2){
		LCDDR1 =  (LCDDR1&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR6 =  (LCDDR6&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR11 = (LCDDR11&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR16 = (LCDDR16&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==3){
		LCDDR1 =     (LCDDR1&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR6 =     (LCDDR6&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR11 =    (LCDDR11&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR16 =    (LCDDR16&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
		
	} else if(pos==4){
		LCDDR2 =  (LCDDR2&0xF0)  |        (0xF&ValueArray[i])<<0;
		LCDDR7 =  (LCDDR7&0xF0)  |  (((0xF<<4)&ValueArray[i])>>4);
		LCDDR12 = (LCDDR12&0xF0) |  (((0xF<<8)&ValueArray[i])>>8);
		LCDDR17 = (LCDDR17&0xF0) | (((0xF<<12)&ValueArray[i])>>12);
		
	} else if(pos==5){
		LCDDR2 =     (LCDDR2&0x0F)  |       ((0xF&ValueArray[i])<<4);
		LCDDR7 =     (LCDDR7&0x0F)  |  (((0xF<<4)&ValueArray[i])>>0);
		LCDDR12 =    (LCDDR12&0x0F) |  (((0xF<<8)&ValueArray[i])>>4);
		LCDDR17 =    (LCDDR17&0x0F) | (((0xF<<12)&ValueArray[i])>>8);
		
	}
	

}

bool is_prime(long i) {
	if(i==1){return false;}
	for(int j=2; j<i;j++){
		if(i%j==0){
			return false;
			
		}
	}
	return true;
}


void printAt(long num, int pos) {
    pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
	for(volatile int i=0; i<1000; i++){
		
	}
	
    pp++;
    writeChar( num % 10 + '0', pp);
}

void computePrimes(int pos) {
    long n;

    for(n = 1; ; n++) {
        if (is_prime(n)) {
			//lock(&m);
            printAt(n, pos);
			//unlock(&m);
            //yield();	
        }
    }
}


int main() {
    LCD_Init();
    spawn(computePrimes, 0);
    computePrimes(3);
}
