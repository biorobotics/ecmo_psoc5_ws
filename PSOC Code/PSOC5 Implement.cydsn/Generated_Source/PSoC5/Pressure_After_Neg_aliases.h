/*******************************************************************************
* File Name: Pressure_After_Neg.h  
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

#if !defined(CY_PINS_Pressure_After_Neg_ALIASES_H) /* Pins Pressure_After_Neg_ALIASES_H */
#define CY_PINS_Pressure_After_Neg_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define Pressure_After_Neg_0			(Pressure_After_Neg__0__PC)
#define Pressure_After_Neg_0_INTR	((uint16)((uint16)0x0001u << Pressure_After_Neg__0__SHIFT))

#define Pressure_After_Neg_INTR_ALL	 ((uint16)(Pressure_After_Neg_0_INTR))

#endif /* End Pins Pressure_After_Neg_ALIASES_H */


/* [] END OF FILE */
