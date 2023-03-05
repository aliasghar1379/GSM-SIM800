/*
 * sound.h
 *
 * 
 */ 


#ifndef SOUND_H_
#define SOUND_H_

#include "avrlibdefs.h"

void beep(int freq, int t)
{
	sbi(DDRB,1);// Speaker
	while (t--)
	{
		sbi(PORTB,1); // _-_-_-_-
		for(int j=0;j<freq;j++);
		cbi(PORTB,1);
		for(int j=0;j<freq;j++);
	}
}


#endif /* SOUND_H_ */