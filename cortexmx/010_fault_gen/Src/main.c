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

int main(void)
{
	//enable all fault exceptions
	uint32_t *pSCB_SHCSR = (uint32_t*)0xE000ED24U;
	*pSCB_SHCSR |= (1 << 16); //mem manage fault
	*pSCB_SHCSR |= (1 << 17); //bus fault
	*pSCB_SHCSR |= (1 << 18); //usage fault

	//run an undefined instruction to generate fault(undeifined instr)
	uint32_t *pSRAM = (uint32_t*)0x20010000;
	*pSRAM = 0xffffffff; //invalid instruction
	void (*some_func)(void);
	some_func = (void*)0x20010001; //last bit to 1 because of T-bit
//	some_func = (void*)0x20010000; //will generate exception for invalid state
									//ARM state as T-bit will be 0 (not supported
									//by Arm cortex M4)
	some_func(); //will go to loation of undifined instruction

    /* Loop forever */
	for(;;);
}

//implement fault handlers
void HardFault_Handler(void) {
	printf("[HardFault_Handler]\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("[MemManage_Handler]\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("[BusFault_Handler]\n");
	while(1);
}

__attribute__((naked)) void UsageFault_Handler(void) {
	//extract the msp value
	//created a naked function as in a normal function
	//compiler added function prologue and epilogue which changes the sp value
	//
	__asm volatile ("mrs r0, msp");
	__asm volatile ("b UsageFault_Handler_C");
}

void UsageFault_Handler_C(uint32_t *pBaseStackFrame) {

//	__asm volatile ("mrs r0, MSP");
//	register uint32_t msp_value __asm("r0");
//	uint32_t *pMSP = (uint32_t*)msp_value;

	printf("[UsageFault_Handler]\n");

	uint32_t *pUFSR = (uint32_t*)0xE000ED2AU;
	printf("UFSR : %lx\n", (*pUFSR));
	printf("MSP  : %p\n", pBaseStackFrame);
	printf("Value of R0 : %lx\n", pBaseStackFrame[0]);
	printf("Value of R1 : %lx\n", pBaseStackFrame[1]);
	printf("Value of R2 : %lx\n", pBaseStackFrame[2]);
	printf("Value of R3 : %lx\n", pBaseStackFrame[3]);
	printf("Value of R12 : %lx\n", pBaseStackFrame[4]);
	printf("Value of LR : %lx\n", pBaseStackFrame[5]);
	printf("Value of PC : %lx\n", pBaseStackFrame[6]);
	printf("Value of XPSR : %lx\n", pBaseStackFrame[7]);

	while(1);
}





