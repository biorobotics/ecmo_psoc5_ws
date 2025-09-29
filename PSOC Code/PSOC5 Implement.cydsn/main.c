/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "Pressure_Transducer.h"


#define ADC_NUM_CHANNELS                4u
#define ADC_SAMPLES_PER_PACKET          1u
#define ADC_PRESSURE_CHANNELS           2u
#define ADC_SAMPLE_RATE_DIV             10u
#define MAX_PACKET_SIZE                 255u
#define PRESSURE_TRANSDUCER_TIME_SHIFT  5u
static uint8_t packet[MAX_PACKET_SIZE];


/* Foreground-background shared variables */
volatile static bool dataReady = false;
volatile static bool pressureDataReady = false;
volatile static bool checkData = false;


static const float32 boundaryToleranceInterval = 0.1;
static const float32 changeToleranceInterval = 0.3;


volatile static float32 savedADC = 0;
volatile int16_t ADCData[ADC_SAMPLES_PER_PACKET*ADC_NUM_CHANNELS];
volatile int16_t ADCPressureData[ADC_PRESSURE_CHANNELS];


/* Foreground variables */
volatile uint16_t timerCount = 0;
volatile uint16_t timerCountDataCompare = 0;
/* Background variables */
static uint8_t RPI_TX_Buffer[MAX_PACKET_SIZE + 3];

static const uint8_t dataGood  =  0x0A;
static const uint8_t testSuccess = 0xA1;
static const uint8_t encryptionErrorCode = 0xF1;
static const uint8_t unreasonableADCValueWarnCodex = 0xF2;
static const uint8_t unreasonablePressureValueWarnCodex = 0xF4;
static const uint32_t dataCompareInterval = 500;



/* CRC-8 calculation table */
static const uint8_t crcTable[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};



void Timer_Interrupt_Handler(void);

uint8_t calculateCRC8(uint8_t opCode, uint8_t dataLength, uint8_t* data);

void wrap_data(uint8_t opcode, uint8_t* data, uint8_t length);

void float2Bytes(float32 val, uint8_t *bytes_array);
void u16Int2Bytes(uint16_t val, uint8_t *bytes_array);



CY_ISR (Timer_Interrupt_Handler)
{
    (void)Timer_ISR_ReadStatusRegister();   // clear interrupt
    
    timerCount++;
    timerCountDataCompare++;

    // Read Pressure Transducer data with frequency 10 Hz
    // Note: this is offset by PRESSURE_TRANSDUCER_TIME_SHIFT to avoid ADC and Pressure Transducer read at the same time
    if ((timerCount + PRESSURE_TRANSDUCER_TIME_SHIFT) == ADC_SAMPLE_RATE_DIV) {
        for (uint16_t i = 0; i < ADC_PRESSURE_CHANNELS; i++) {
            ADCPressureData[i] = read_pressure(i);
        }
        pressureDataReady = true;
    }

    // Read ADC conversion results with frequency 10 Hz (10 us latency)
    if (timerCount == ADC_SAMPLE_RATE_DIV) {
        timerCount = 0;        
        // Check conversion status without blocking
        uint32_t conversionStatus = ADC_TS410_IsEndConversion(ADC_TS410_WAIT_FOR_RESULT);
        if (conversionStatus) {
            for (uint16_t i = 0; i < ADC_NUM_CHANNELS; i++) {
                ADCData[i] = ADC_TS410_GetResult16(i);
                CyDelay(1u); //Delay to leave enough time for ADC conversion
            }
            dataReady = true;            
            // Start next conversion
            ADC_TS410_StartConvert();
        }
        else {
            for (uint16_t i = 0; i < ADC_NUM_CHANNELS; i++) {
                ADCData[i] = 0;
            }
            // printf("error: Conversion not finished yet!");
            CyDelay(5u); // wait 5 ms to signal error
        }        
    }

    if(timerCountDataCompare == dataCompareInterval){
        timerCountDataCompare = 0; 
        checkData = true;
    }
  
    
    /*static uint16_t div = 0;                // divide the tick rate
    if (++div >= 100) {                     // e.g., at 1 kHz tick -> ~1 Hz blink
        div = 0;
        USER_LED_Write( (uint8)(!USER_LED_ReadDataReg()) );  // toggle
        // If LED is active-low and you want “1 = on”, invert here as needed.
    }*/
}


int main(void)
{
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    uint8_t packetsize = 0;
    
    /*Initialize UART from OEM to PSOC, and from PSOC to RPI-5*/
    UART_Debug_Start();
    UART_OEM_Start();
    UART_RPI_Start();

    UART_Debug_PutString("Hello World!\r\n");
    /*Testing the UART Connection to RPI-5*/
    RPI_TX_Buffer[0] = 0xA1;
    UART_RPI_PutArray(RPI_TX_Buffer,1);
    
    /*Initialize the ADC that converts TS410 amplified signal, and ADC that converts low voltage signal directly from Pressure Sensor*/
    /*Initialize Pressure Transducer*/
    ADC_TS410_Start();
    init_pressure_sensors();
    
    /*Set up the NVIC vector of the ISR that would be called 10ms later*/
    Timer_Interrupt_StartEx(Timer_Interrupt_Handler);
    
    ADC_TS410_StartConvert();
    ADC_DelSig_StartConvert();

    Timer_ISR_Start();

    for(;;)
    {   
        /*Start packing only after all the required data are ready to send*/
        //printf("I hate this world\r\n");
        //UART_Debug_PutString("I hate this world\r\n");
        if(dataReady == true && pressureDataReady == true){
            dataReady = false;
            pressureDataReady = false;
            packetsize = 0;
            uint8_t opcode = dataGood;

            // Pack ADC Data (TS410)
            for (uint8_t i = 0; i < ADC_NUM_CHANNELS; i++) {
                /*Converting the count value read by ADC to float32 voltage*/
                char buf_2[64];
                int n_2 = snprintf(buf_2, sizeof(buf_2), "Original Channel %d: V=%d V\r\n", i, ADCData[i]);
                UART_Debug_PutString(buf_2);   // or *_PutString(buf) if null-terminated
                UART_Debug_PutString("\r\n");   // or *_PutString(buf) if null-terminated
                float32 ADCVolts = /*(3.3/2.739) *  */ ADC_TS410_CountsTo_Volts(ADCData[i]);
                /*Check if data flucaute a lot, if so send warning OPCODE, currently it only checks Channel 3*/
                if(checkData == true){
                    if(i == ADC_NUM_CHANNELS){
                        checkData = false;
                        if(savedADC!=0 && (savedADC >= ADCVolts + changeToleranceInterval || savedADC <= ADCVolts - changeToleranceInterval)){
                            opcode = 0xF3;
                        }
                        savedADC = ADCVolts;
                    }
                }
                /*If ADC value is out of boundary, there might be calibration or hardware setup issue, send another warning OPCODE*/
                if(ADCVolts > (3.3 + boundaryToleranceInterval) || ADCVolts < (0 - boundaryToleranceInterval)){
                    opcode = unreasonableADCValueWarnCodex;
                }
                /*Pack ADC value to the data packet that would be later sent to RPI through UART*/
                float2Bytes(ADCVolts, &packet[packetsize]);
                packetsize += sizeof(float32);
                //printf("\r\nHoho\r\n");
                //int n = snprintf(buf, sizeof(buf), "ch=%u value=%f\r\n", i, ADCVolts);
                char buf[64];
                int n = snprintf(buf, sizeof(buf), "Channel %d: V=%.3f V\r\n", i, ADCVolts);
                UART_Debug_PutString(buf);   // or *_PutString(buf) if null-terminated
                UART_Debug_PutString("\r\n");   // or *_PutString(buf) if null-terminated
                //printf("ADC %d: %f, ", i, ADCVolts);
            }
            //USBUART_1_PutString("\r\n");

            // Pack Pressure Transducer Data
            for (uint8_t i = 0; i < ADC_PRESSURE_CHANNELS; i++) {
                // char buf_3[32];
                // int n_3 = snprintf(buf_3, sizeof(buf_3), "Original Pressure Channel %d: V=%d V\r\n", i, ADCPressureData[i]);
                // UART_Debug_PutString(buf_3);   // or *_PutString(buf) if null-terminated
                // UART_Debug_PutString("\r\n");   // or *_PutString(buf) if null-terminated
                float32 pressureVolts = /*(3.3/3.3) */ ADC_DelSig_CountsTo_Volts(ADCPressureData[i]);

                if (pressureVolts > (3.3 + boundaryToleranceInterval) || pressureVolts < (0 - boundaryToleranceInterval)){
                    opcode = unreasonablePressureValueWarnCodex;
                }

                float2Bytes(pressureVolts, &packet[packetsize]);
                packetsize += sizeof(float32);
            }

            wrap_data(opcode, packet, packetsize);
        }
        
        //USER_LED_Write(1);
        //CyDelay(1000);
        //USER_LED_Write(0);
        //CyDelay(1000);
    }
}







/**
 * @brief Populate buffer with metadata and data and start transmission
 * 
 * @param uint8_t opcode - type of transmission TODO: replace with enum
 * @param uint8_t *data - pointer to buffer containing data to send
 * @param uint8_t length - number of bytes in data (0-255) 
 *
 * @note Buffer being transmitted must be constant and allocated until transmit operation completes
*/
void wrap_data(uint8_t opcode, uint8_t* data, uint8_t length){
        // Ensure previous transmission is not ongoing before modifying transmit buffer
    //printf("Try to send data \r\n");
    
        // wait until SW buffer drained
    while (UART_RPI_GetTxBufferSize() != 0u) {
        USER_LED_Write(1);
        CyDelay(5);
    }

    // wait until HW FIFO empty
    while ((UART_RPI_ReadTxStatus() & UART_RPI_TX_STS_FIFO_EMPTY) == 0u) {
        
        USER_LED_Write(1);
        CyDelay(5);
    }

    // wait until last bit shifted out on the line
    
    
    /*while ((UART_RPI_ReadTxStatus() & UART_RPI_TX_STS_COMPLETE) == 0u) { 
        
        USER_LED_Write(1);
        CyDelay(5);
    }*/
    
    RPI_TX_Buffer[0] = opcode;
    RPI_TX_Buffer[1] = length;
    memcpy(&RPI_TX_Buffer[2], data, length);
    RPI_TX_Buffer[2 + length] = calculateCRC8(opcode, length, data);
    USER_LED_Write(0);   // drive high -> LED OFF
    UART_RPI_PutArray(RPI_TX_Buffer, 2 + length + 1);
    
    
    //printf("Sent data \r\n");
    
    // demo only
    //printf("\r\n\nopcode: %d, length: %d, crc: 0x%x\r\n", opcode, length, txBuffer[2 + length]);   
}





/**
 * @brief Helper function for main background loop converting float to uint8_t array
 * 
 * @author <Floris> - https://stackoverflow.com/questions/24420246/c-function-to-convert-float-to-byte-array
 * 
 * @param float32_t val - single precision float value to be converted to uint8_t array of length 4
 * @param uint8_t *bytesArray - pointer to uint8_t array in memory
 *
 * @return none
*/
void float2Bytes(float32 val, uint8_t *bytes_array) {
    // Create union of shared memory space
    union {
        float32 float_variable;
        uint8_t temp_array[4];
    } u;
    // Overite bytes of union with float variable
    u.float_variable = val;
    // Assign bytes to input array
    memcpy(bytes_array, u.temp_array, 4);
}

/**
 * @brief Helper function for main background loop converting uint16_t to uint8_t array
 *
 * @note based on float2Bytes
 * @return none
*/
void u16Int2Bytes(uint16_t val, uint8_t *bytes_array) {
    // Create union of shared memory space
    union {
        uint16_t int_variable;
        uint8_t temp_array[2];
    } u;
    // Overite bytes of union with float variable
    u.int_variable = val;
    // Assign bytes to input array
    memcpy(bytes_array, u.temp_array, 2);
}



/* Helper functions */
/**
 * @brief calculates the CRC8 for an outgoing UART packet
 * 
 * @param uint8_t opCode - the operation type of the outgoing packet
 * @param uint8_t dataLength - the length of the payload to send
 * @param uint8_t *data - pointer to an array of the Tx payload bytes
 *
 * @return uint8_t - calculated CRC byte from input parameters
 * @note Helper function for wrap_data()
*/
uint8_t calculateCRC8(uint8_t opCode, uint8_t dataLength, uint8_t* data) {
  uint8_t crc = 0x00; // Initialize CRC value

  // Update CRC with the opcode
  crc = crcTable[crc ^ opCode];
  
  // Update CRC with the data length
  crc = crcTable[crc ^ dataLength];
  
  // Update CRC with each data byte
  for (uint8_t i = 0; i < dataLength; i++) {
    crc = crcTable[crc ^ data[i]];
  }

  return crc;
}


/* [] END OF FILE */
