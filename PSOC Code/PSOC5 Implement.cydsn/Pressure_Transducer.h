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
#include <project.h>

#define AMux_CHANNEL_0 0
#define AMux_CHANNEL_1 1

// initialize ADC and Multiplexer
void init_pressure_sensors();

uint16_t read_pressure(uint16_t MuxSelect);

/* [] END OF FILE */
