#include "stm32f4xx.h"


#define GPIOAENR			(1U<<0)
#define PIN5			(1U<<5)
#define LED_PIN			PIN5



int main(void) {
	RCC->AHB1ENR |= GPIOAENR;

	GPIOA->MODER &= ~(1U<<11);
	GPIOA->MODER |= (1U<<10);
	while(1){
		GPIOA->ODR ^=LED_PIN;
				for(int i =0; i<1000000000000000000;i++){}


	}
}
