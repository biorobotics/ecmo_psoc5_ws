/*******************************************************************************
* File Name: Software_Reset_Signal.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Software_Reset_Signal_ALIASES_H) /* Pins Software_Reset_Signal_ALIASES_H */
#define CY_PINS_Software_Reset_Signal_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Software_Reset_Signal_0			(Software_Reset_Signal__0__PC)
#define Software_Reset_Signal_0_INTR	((uint16)((uint16)0x0001u << Software_Reset_Signal__0__SHIFT))

#define Software_Reset_Signal_INTR_ALL	 ((uint16)(Software_Reset_Signal_0_INTR))

#endif /* End Pins Software_Reset_Signal_ALIASES_H */


/* [] END OF FILE */
