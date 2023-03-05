/*
 * gsm.c
 *
 * Created: 
 * Author : ALIASGHAR NOROUZI
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include "global.h"
#include <util/delay.h>
#include "usart.h"
#include "sound.h"
#include "timer.h"

char buffer[100];//BUFFER FOR READING MESSAGES 
char i=0;//FOR INTERRUPT REVEALING 

//INTERRUPT SECTION
ISR(INT0_vect)
{
	i=1;//MAKING CHAR I WITH PARAMETER ONE
}

//MAIN CODE
int main(void)
{
	beep(100,200);
	_delay_ms(50);
	beep(200,100);
	_delay_ms(50);
	beep(400,50);

    //UART
	uart_init(BAUD_CALC(9600));
	sei();
	

	//INTERRUPT
	//INT0
	sbi(GICR,6); // ENABLE INT0
	cbi(MCUCR,0);//INT0 FALLING EDGE
	sbi(MCUCR,1); // INT0 FALLING EDGE
	cbi(DDRD,2); // INPUT FOR INTERRUPT

	//CONFIG THE OUTPUT PORTS
	sbi(DDRB,1);//speaker
	sbi(DDRB,2); // led
	sbi(DDRB,3);//relay
	
	
	//INITIALIZING THE SIM800 MODULE

	sbi(PORTB,2); // LED ON

	_delay_ms(20000); // 20 SECOND WAITING FOR MODULE
	
	uart_puts("at\r\n");
	_delay_ms(200);
	uart_puts("ate0\r\n");
	_delay_ms(200);
	uart_puts("at+cmgf=1\r\n");
	_delay_ms(200);
	
	uart_puts("at+cmgs=\"09199875466\"\r\n");
	_delay_ms(200);
	uart_puts("system is up\x1a"); // Ctrl+z -> \x1A
	_delay_ms(5000);
	
	uart_puts("at+cmgda=\"DEL ALL\"\r\n");
	_delay_ms(5000);
	beep(200,100);
	beep(400,200);
	cbi(PORTB,2); //LED OFF
    while (1) 
    {
		//INTERRUPT RECEIVER WITH FALLING EDGE
		if(i==1)
		{
		sbi(PORTB,2);//led
		sbi(PORTB,3);//relay
		beep(500,200);//sound
		uart_puts("at\r\n");
		_delay_ms(200);
		uart_puts("ate0\r\n");
		_delay_ms(200);
		uart_puts("at+cmgf=1\r\n");
		_delay_ms(200);
		uart_puts("at+cmgs=\"09199875466\"\r\n");
		_delay_ms(200);
		uart_puts("GAS SENSED,RELAY IS ON\x1a"); // Ctrl+z -> \x1A
		_delay_ms(10000);
		uart_puts("at+cmgda=\"DEL ALL\"\r\n");
		_delay_ms(5000);
		i=0;
		}
		//BUFFER CHECKING
		_delay_ms(1000);
		uart_gets(buffer,100);
		
		if (strstr(buffer,"CMTI")!=NULL)
		{
			//DELETING ALL THE MESSAGES ON SIMCARD
			_delay_ms(1000);
			uart_puts("at+cmgr=1\r\n");
			_delay_ms(200);
			uart_gets(buffer,100);
			uart_puts("at+cmgda=\"DEL ALL\"\r\n");
			_delay_ms(1000);
			if (strcasestr(buffer,"on")!=NULL) //commend for turning on the relay and fire alarm
			{
				sbi(PORTB,2);//led on
				sbi(PORTB,3);//relay on
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("RELAY LED IS ON\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
			if (strcasestr(buffer,"off")!=NULL) //commend for turning of the relay and fire alarm
			{
				cbi(PORTB,2);//led off
				cbi(PORTB,3);//relay off
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("RELAY LED IS OFF\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
			if (strcasestr(buffer,"Relayo")!=NULL) //commend for turning on the relay and fire alarm
			{
				sbi(PORTB,3);//relay on
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("RELAY IS ON\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
			if (strcasestr(buffer,"Relayf")!=NULL) //commend for turning of the relay and fire alarm
			{
				cbi(PORTB,3);//relay off
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("RELAY IS OFF\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
			if (strcasestr(buffer,"Ledo")!=NULL) //commend for turning on the relay and fire alarm
			{
				sbi(PORTB,2);//led on
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("LED IS ON\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
			if (strcasestr(buffer,"Ledf")!=NULL) //commend for turning of the relay and fire alarm
			{
				cbi(PORTB,2);//led off
				beep(500,200);//sound
				uart_puts("at\r\n");
				_delay_ms(200);
				uart_puts("ate0\r\n");
				_delay_ms(200);
				uart_puts("at+cmgf=1\r\n");
				_delay_ms(200);
				uart_puts("at+cmgs=\"09199875466\"\r\n");
				_delay_ms(200);
				uart_puts("LED IS OFF\x1a"); // Ctrl+z -> \x1A
				_delay_ms(5000);
				uart_puts("at+cmgda=\"DEL ALL\"\r\n");
				_delay_ms(5000);
			}
		}
    }
}