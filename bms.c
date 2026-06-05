#include "bms.h"

void BMS_Init(BatteryPack_t *pack) {
    pack->current_state = BMS_STATE_IDLE;
    pack->current_draw = 0.0f;
    pack->fault_ovp = false;
    pack->fault_uvp = false;
    
    for(int i = 0; i < 4; i++) {
        pack->cell_voltages[i] = 3.7f; // Nominal starting voltage
    }
}

void BMS_Simulate_Plant(BatteryPack_t *pack) {
    // Simulate charging/discharging physics
    if (pack->current_state == BMS_STATE_CHARGING) {
        for(int i = 0; i < 4; i++) {
            pack->cell_voltages[i] += 0.01f; // Simulated charge increment
        }
        // Induce artificial drift in Cell 3 for balancing test
        pack->cell_voltages[2] += 0.005f; 
    }
    
    pack->pack_voltage = pack->cell_voltages[0] + pack->cell_voltages[1] + 
                         pack->cell_voltages[2] + pack->cell_voltages[3];
}

void BMS_Run_Diagnostics(BatteryPack_t *pack) {
    pack->fault_ovp = false;
    pack->fault_uvp = false;

    for(int i = 0; i < 4; i++) {
        if(pack->cell_voltages[i] >= OVP_THRESHOLD) {
            pack->fault_ovp = true;
            pack->current_state = BMS_STATE_FAULT;
        }
        if(pack->cell_voltages[i] <= UVP_THRESHOLD) {
            pack->fault_uvp = true;
            pack->current_state = BMS_STATE_FAULT;
        }
    }
}

void BMS_Balance_Cells(BatteryPack_t *pack) {
    if (pack->current_state == BMS_STATE_FAULT) return; // halt balancing on fault
// bleed off excess voltage to match pack average
    for(int i = 0; i < 4; i++) {
        if(pack->cell_voltages[i] > BALANCING_THRESHOLD) {
            
            pack->cell_voltages[i] -= 0.002f; 
        }
    }
}
