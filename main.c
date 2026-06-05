#include "main.h"
#include "bms.h"
#include <stdio.h>
#include <string.h>

UART_HandleTypeDef huart2;

// USER CODE BEGIN 0
BatteryPack_t myPack;
char uart_buf[100];
// USER CODE END 0

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_USART2_UART_Init();

    // USER CODE BEGIN 2
    BMS_Init(&myPack);
    myPack.current_state = BMS_STATE_CHARGING; // Begin HIL test
    // USER CODE END 2

    while (1) {
        // USER CODE BEGIN 3
        
        // 1. Run HIL Simulation Step
        BMS_Simulate_Plant(&myPack);
        
        // 2. Execute Safety Logic
        BMS_Run_Diagnostics(&myPack);
        BMS_Balance_Cells(&myPack);
        
        // 3. Transmit Telemetry via UART
        sprintf(uart_buf, "C1:%.2f C2:%.2f C3:%.2f C4:%.2f | V_Pack:%.2f | OVP:%d UVP:%d\r\n",
                myPack.cell_voltages[0], myPack.cell_voltages[1], 
                myPack.cell_voltages[2], myPack.cell_voltages[3],
                myPack.pack_voltage, myPack.fault_ovp, myPack.fault_uvp);
                
        HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
        
        HAL_Delay(100); // 10Hz Telemetry loop
        // USER CODE END 3
    }
}
