/*******************************************************************************
* File Name: ADC_TS410_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "ADC_TS410.h"
#include "ADC_TS410_SAR.h"
#if(ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL)
    #include "ADC_TS410_IntClock.h"
#endif   /* ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL */


/*******************************************************************************
* Function Name: ADC_TS410_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_TS410_Sleep(void)
{
    ADC_TS410_SAR_Stop();
    ADC_TS410_SAR_Sleep();
    ADC_TS410_Disable();

    #if(ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL)
        ADC_TS410_IntClock_Stop();
    #endif   /* ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL */
}


/*******************************************************************************
* Function Name: ADC_TS410_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers. This should
*  be called just after awaking from sleep mode
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_TS410_Wakeup(void)
{
    ADC_TS410_SAR_Wakeup();
    ADC_TS410_SAR_Enable();

    #if(ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL)
        ADC_TS410_IntClock_Start();
    #endif   /* ADC_TS410_CLOCK_SOURCE == ADC_TS410_CLOCK_INTERNAL */

    /* The block is ready to use 10 us after the SAR enable signal is set high. */
    CyDelayUs(10u);
    
    ADC_TS410_Enable();

    #if(ADC_TS410_SAMPLE_MODE == ADC_TS410_SAMPLE_MODE_FREE_RUNNING)
        ADC_TS410_SAR_StartConvert();
    #endif /* (ADC_TS410_SAMPLE_MODE == ADC_TS410_SAMPLE_MODE_FREE_RUNNING) */

    (void) CY_GET_REG8(ADC_TS410_STATUS_PTR);
}


/*******************************************************************************
* Function Name: ADC_TS410_SaveConfig
********************************************************************************
*
* Summary:
*  Save the current configuration of ADC non-retention registers
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_TS410_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: ADC_TS410_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the configuration of ADC non-retention registers
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void ADC_TS410_RestoreConfig(void)
{

}


/* [] END OF FILE */
