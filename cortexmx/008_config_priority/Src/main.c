/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#include <stdint.h>
#include <stdio.h>

//#if !defined(__SOFT_FP__) && defined(__ARM_FP)
//  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
//#endif
#define TIMER2_IRQ	28
#define I2C1_EV_IRQ	31

uint32_t *pNVIC_ISER_BASE = (uint32_t*)0xE000E100;
uint32_t *pNVIC_ISPR_BASE = (uint32_t*)0XE000E200;
uint32_t *pNVIC_IPR_BASE = (uint32_t*)0xE000E400;

void configure_priority_for_irqs(uint8_t irq_no, uint8_t priority) {
	//get iprx
	uint8_t iprx = irq_no / 4;
	uint32_t *ipr = (pNVIC_IPR_BASE + iprx);

	//get position
	uint8_t pos = ((irq_no % 4) * 8);

	//configure iprx with priority shifted with position
	*ipr &= ~(0xff << pos); //claer
	*ipr |= (priority << pos); //ser priority
}

int main(void)
{
	//configure priority for interrupts
	configure_priority_for_irqs(TIMER2_IRQ, 0x80);
	configure_priority_for_irqs(I2C1_EV_IRQ, 0x70); /*when interrupts were same
	i2c interrupt pending in timer interrupt didn't preemt the i2c interrupt
	as same priority but when changed priority to higher it preempts*/

	//set pending bit for timer 2 interrupt
	*pNVIC_ISPR_BASE |= (1 << TIMER2_IRQ);

	//enable interrupts for both i2c and timer2
	*pNVIC_ISER_BASE |= (1 << I2C1_EV_IRQ);
	*pNVIC_ISER_BASE |= (1 << TIMER2_IRQ);

    /* Loop forever */
	for(;;);
}

void TIM2_IRQHandler(void) {
	printf("[TIM2_IRQHandler]\n");
	//set pending bit for i2c interrupt nested interrupts
	*pNVIC_ISPR_BASE |= (1 << I2C1_EV_IRQ);
	while(1);

}

void I2C1_EV_IRQHandler(void) {
	printf("[I2C1_EV_IRQHandler]\n");
}



