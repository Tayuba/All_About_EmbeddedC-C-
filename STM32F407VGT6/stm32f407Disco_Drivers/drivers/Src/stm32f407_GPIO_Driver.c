/*
 * stm32f407_GPIO_Driver.c
 *
 *  Created on: Apr 24, 2023
 *      Author: ayuba
 */

#include "stm32f407_GPIO_Driver.h"


// Peripheral Clock setup
/************************************************************************************************
 *@func name							- GPIO_PeriphControl
 *
 *@brief								- Function to enable or disable peripheral clock for a given GPIO port
 *
 *@param1								- Base Address of GPIO peripheral
 *@param2								- Enable or Disable Macros
 *
 *@return								- none
 *
 *@Note									-	none
 */
void GPIO_PeriphControl(GPIO_Reg_t *pGPIOx, uint8_t EnrDis){
	if(EnrDis == ENABLE){
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_EN();
		}
		else if(pGPIOx == GPIOB){
			GPIOB_PCLK_EN();
		}
		else if(pGPIOx == GPIOC){
			GPIOC_PCLK_EN();
		}
		else if(pGPIOx == GPIOD){
			GPIOD_PCLK_EN();
		}
		else if(pGPIOx == GPIOE){
			GPIOE_PCLK_EN();
		}
		else if(pGPIOx == GPIOF){
			GPIOF_PCLK_EN();
		}
		else if(pGPIOx == GPIOG){
			GPIOG_PCLK_EN();
		}
		else if(pGPIOx == GPIOH){
			GPIOH_PCLK_EN();
		}
		else if(pGPIOx == GPIOI){
			GPIOI_PCLK_EN();
		}
	}else{
		if(pGPIOx == GPIOA){
			GPIOA_PCLK_DI();
		}
		else if(pGPIOx == GPIOB){
			GPIOB_PCLK_DI();
		}
		else if(pGPIOx == GPIOC){
			GPIOC_PCLK_DI();
		}
		else if(pGPIOx == GPIOD){
			GPIOD_PCLK_DI();
		}
		else if(pGPIOx == GPIOE){
			GPIOE_PCLK_DI();
		}
		else if(pGPIOx == GPIOF){
			GPIOF_PCLK_DI();
		}
		else if(pGPIOx == GPIOG){
			GPIOG_PCLK_DI();
		}
		else if(pGPIOx == GPIOH){
			GPIOH_PCLK_DI();
		}
		else if(pGPIOx == GPIOI){
			GPIOI_PCLK_DI();
		}
	}
}

// Initialize and De-Initialize
/************************************************************************************************
 *@func name							- GPIO_Init
 *
 *@brief								- Function to initialize GPIO
 *
 *@param1								- Base Address of GPIO Handle
 *
 *@return								-	none
 *
 *@Note									-	none
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	uint32_t temp = 0; //hold register mode temporary
	if(pGPIOHandle->pGPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		//Mode
		temp = (pGPIOHandle->pGPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); //clearing
		pGPIOHandle->pGPIOx->MODER |= temp; // setting

	}else{
		// Interrupt mode
		if(pGPIOHandle->pGPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INP_FE_T){
			// Falling Edge trigger configured
			EXTI->FTSR |= (1 << pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); 					// set falling edge
			EXTI->RTSR &= ~(1 << pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); 	// reset rising edge

		}else if(pGPIOHandle->pGPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INP_RE_T){
			// Rising Edge trigger configured

		}else if(pGPIOHandle->pGPIO_PinConfig.GPIO_PinMode == GPIO_MODE_INP_FERE_T){
			//  Both Falling and Rising Edge trigger configured
		}
	}
	temp = 0;

	//Speed
	temp = (pGPIOHandle->pGPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 <<  pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp; // setting
	temp = 0;

	//Pull up Pull down
	temp = (pGPIOHandle->pGPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 <<  pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp; //setting
	temp = 0;

	//Output Type
	temp = (pGPIOHandle->pGPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 <<  pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber); // clearing
	pGPIOHandle->pGPIOx->OTYPER |= temp; //setting
	temp = 0;

	//Alternative Function
	if(pGPIOHandle->pGPIO_PinConfig.GPIO_PinAlteFuncMode == GPIO_MODE_ALTFUNC){
		uint8_t temp1, temp2;
		temp1 = pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber / 8;
		temp2 = pGPIOHandle->pGPIO_PinConfig.GPIO_PinNumber % 8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(pGPIOHandle->pGPIO_PinConfig.GPIO_PinAlteFuncMode << (4 * temp2 ));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->pGPIO_PinConfig.GPIO_PinAlteFuncMode << (4 * temp2 ));
	}

}
/************************************************************************************************
 *@func name							- GPIO_DeInit
 *
 *@brief								- Function to reset or disable GPIO
 *
 *@param1								- Base Address of GPIO peripheral
 *
 *@return								-	none
 *
 *@Note									-	none
 */
void GPIO_DeInit(GPIO_Reg_t *pGPIOx){
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}
	else if(pGPIOx == GPIOI){
		GPIOI_REG_RESET();
	}
}

// Read and Write Data
uint8_t GPIO_ReadInputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNum){
	uint8_t IDR_value;
	IDR_value = (uint8_t)(pGPIOx->IDR >> PinNum & 0x00000001);
	return IDR_value;
}
uint16_t GPIO_ReadInputPort(GPIO_Reg_t *pGPIOx){
	uint16_t PORT_value;
	PORT_value = (uint16_t)(pGPIOx->IDR);
	return PORT_value;
}
void GPIO_WriteOutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNum, uint8_t Val){
	if(Val == GPIO_PIN_SET){
		pGPIOx->ODR |= (1 << PinNum);
	}else{
		pGPIOx->ODR &= ~(1 << PinNum);
	}
}
void GPIO_WriteOutputPort(GPIO_Reg_t *pGPIOx, uint8_t Val){
	pGPIOx->ODR = Val;
}
void GPIO_ToggleOutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNum){
	pGPIOx->ODR ^= (1 << PinNum);
}

// Interrupt Configuration and Interrupt Service Routine Handling
void GPIO_InterruptConfig(uint8_t IRQNum, uint8_t IRQ_Priority, uint8_t EnrDis){

}
void GPIO_InterruptHandling(uint8_t PinNum){

}
