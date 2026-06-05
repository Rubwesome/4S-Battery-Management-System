#ifndef INC_BMS_H_
#define INC_BMS_H_

#include "stm32f4xx_hal.h" // Change to your specific STM32 board
#include <stdint.h>
#include <stdbool.h>

// BMS Thresholds
#define OVP_THRESHOLD 4.20f
#define UVP_THRESHOLD 3.00f
#define BALANCING_THRESHOLD 4.15f
#define MAX_CELL_DRIFT 0.05f

// System States
typedef enum {
    BMS_STATE_IDLE,
    BMS_STATE_CHARGING,
    BMS_STATE_DISCHARGING,
    BMS_STATE_FAULT
} BMS_State_t;

// Battery Pack Structure
typedef struct {
    float cell_voltages[4];
    float pack_voltage;
    float current_draw;
    BMS_State_t current_state;
    bool fault_ovp;
    bool fault_uvp;
} BatteryPack_t;

// Function Prototypes
void BMS_Init(BatteryPack_t *pack);
void BMS_Simulate_Plant(BatteryPack_t *pack);
void BMS_Run_Diagnostics(BatteryPack_t *pack);
void BMS_Balance_Cells(BatteryPack_t *pack);

#endif /* INC_BMS_H_ */
