/* ========================================
 *
 * Copyright Carnegie Mellon University
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * Driver for ICU medical Transpac IV disposable pressure transducer system
 * Pressure sensor: differential ADC input
 *
 * ========================================
*/

#include "Pressure_Transducer.h"

void init_pressure_sensors(){
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();
    AMux_Start();
}

void switch_multiplexer(uint16_t channel){
    // stop conversion before switching Mux
    ADC_DelSig_StopConvert();
    
    // only use this for 2 channels
    AMux_FastSelect(channel);
    CyDelay(1u); //Delay to leave enough time for ADC conversion
    ADC_DelSig_StartConvert();
}

uint16_t read_pressure(uint16_t MuxSelect){
    uint16_t pressure_reading;
    if(MuxSelect==AMux_CHANNEL_0){
        pressure_reading = ADC_DelSig_Read16();
        // switch Mux to channel 1
        switch_multiplexer(AMux_CHANNEL_1);
    }
    else{
        pressure_reading = ADC_DelSig_Read16();
        // switch Mux to channel 0
        switch_multiplexer(AMux_CHANNEL_0);
    }
    return pressure_reading;
}


/* [] END OF FILE */
