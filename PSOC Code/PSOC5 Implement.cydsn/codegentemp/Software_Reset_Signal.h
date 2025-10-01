/*******************************************************************************
* File Name: Software_Reset_Signal.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Software_Reset_Signal_H) /* Pins Software_Reset_Signal_H */
#define CY_PINS_Software_Reset_Signal_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Software_Reset_Signal_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Software_Reset_Signal__PORT == 15 && ((Software_Reset_Signal__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Software_Reset_Signal_Write(uint8 value);
void    Software_Reset_Signal_SetDriveMode(uint8 mode);
uint8   Software_Reset_Signal_ReadDataReg(void);
uint8   Software_Reset_Signal_Read(void);
void    Software_Reset_Signal_SetInterruptMode(uint16 position, uint16 mode);
uint8   Software_Reset_Signal_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Software_Reset_Signal_SetDriveMode() function.
     *  @{
     */
        #define Software_Reset_Signal_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Software_Reset_Signal_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Software_Reset_Signal_DM_RES_UP          PIN_DM_RES_UP
        #define Software_Reset_Signal_DM_RES_DWN         PIN_DM_RES_DWN
        #define Software_Reset_Signal_DM_OD_LO           PIN_DM_OD_LO
        #define Software_Reset_Signal_DM_OD_HI           PIN_DM_OD_HI
        #define Software_Reset_Signal_DM_STRONG          PIN_DM_STRONG
        #define Software_Reset_Signal_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Software_Reset_Signal_MASK               Software_Reset_Signal__MASK
#define Software_Reset_Signal_SHIFT              Software_Reset_Signal__SHIFT
#define Software_Reset_Signal_WIDTH              1u

/* Interrupt constants */
#if defined(Software_Reset_Signal__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Software_Reset_Signal_SetInterruptMode() function.
     *  @{
     */
        #define Software_Reset_Signal_INTR_NONE      (uint16)(0x0000u)
        #define Software_Reset_Signal_INTR_RISING    (uint16)(0x0001u)
        #define Software_Reset_Signal_INTR_FALLING   (uint16)(0x0002u)
        #define Software_Reset_Signal_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Software_Reset_Signal_INTR_MASK      (0x01u) 
#endif /* (Software_Reset_Signal__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Software_Reset_Signal_PS                     (* (reg8 *) Software_Reset_Signal__PS)
/* Data Register */
#define Software_Reset_Signal_DR                     (* (reg8 *) Software_Reset_Signal__DR)
/* Port Number */
#define Software_Reset_Signal_PRT_NUM                (* (reg8 *) Software_Reset_Signal__PRT) 
/* Connect to Analog Globals */                                                  
#define Software_Reset_Signal_AG                     (* (reg8 *) Software_Reset_Signal__AG)                       
/* Analog MUX bux enable */
#define Software_Reset_Signal_AMUX                   (* (reg8 *) Software_Reset_Signal__AMUX) 
/* Bidirectional Enable */                                                        
#define Software_Reset_Signal_BIE                    (* (reg8 *) Software_Reset_Signal__BIE)
/* Bit-mask for Aliased Register Access */
#define Software_Reset_Signal_BIT_MASK               (* (reg8 *) Software_Reset_Signal__BIT_MASK)
/* Bypass Enable */
#define Software_Reset_Signal_BYP                    (* (reg8 *) Software_Reset_Signal__BYP)
/* Port wide control signals */                                                   
#define Software_Reset_Signal_CTL                    (* (reg8 *) Software_Reset_Signal__CTL)
/* Drive Modes */
#define Software_Reset_Signal_DM0                    (* (reg8 *) Software_Reset_Signal__DM0) 
#define Software_Reset_Signal_DM1                    (* (reg8 *) Software_Reset_Signal__DM1)
#define Software_Reset_Signal_DM2                    (* (reg8 *) Software_Reset_Signal__DM2) 
/* Input Buffer Disable Override */
#define Software_Reset_Signal_INP_DIS                (* (reg8 *) Software_Reset_Signal__INP_DIS)
/* LCD Common or Segment Drive */
#define Software_Reset_Signal_LCD_COM_SEG            (* (reg8 *) Software_Reset_Signal__LCD_COM_SEG)
/* Enable Segment LCD */
#define Software_Reset_Signal_LCD_EN                 (* (reg8 *) Software_Reset_Signal__LCD_EN)
/* Slew Rate Control */
#define Software_Reset_Signal_SLW                    (* (reg8 *) Software_Reset_Signal__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Software_Reset_Signal_PRTDSI__CAPS_SEL       (* (reg8 *) Software_Reset_Signal__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Software_Reset_Signal_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Software_Reset_Signal__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Software_Reset_Signal_PRTDSI__OE_SEL0        (* (reg8 *) Software_Reset_Signal__PRTDSI__OE_SEL0) 
#define Software_Reset_Signal_PRTDSI__OE_SEL1        (* (reg8 *) Software_Reset_Signal__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Software_Reset_Signal_PRTDSI__OUT_SEL0       (* (reg8 *) Software_Reset_Signal__PRTDSI__OUT_SEL0) 
#define Software_Reset_Signal_PRTDSI__OUT_SEL1       (* (reg8 *) Software_Reset_Signal__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Software_Reset_Signal_PRTDSI__SYNC_OUT       (* (reg8 *) Software_Reset_Signal__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Software_Reset_Signal__SIO_CFG)
    #define Software_Reset_Signal_SIO_HYST_EN        (* (reg8 *) Software_Reset_Signal__SIO_HYST_EN)
    #define Software_Reset_Signal_SIO_REG_HIFREQ     (* (reg8 *) Software_Reset_Signal__SIO_REG_HIFREQ)
    #define Software_Reset_Signal_SIO_CFG            (* (reg8 *) Software_Reset_Signal__SIO_CFG)
    #define Software_Reset_Signal_SIO_DIFF           (* (reg8 *) Software_Reset_Signal__SIO_DIFF)
#endif /* (Software_Reset_Signal__SIO_CFG) */

/* Interrupt Registers */
#if defined(Software_Reset_Signal__INTSTAT)
    #define Software_Reset_Signal_INTSTAT            (* (reg8 *) Software_Reset_Signal__INTSTAT)
    #define Software_Reset_Signal_SNAP               (* (reg8 *) Software_Reset_Signal__SNAP)
    
	#define Software_Reset_Signal_0_INTTYPE_REG 		(* (reg8 *) Software_Reset_Signal__0__INTTYPE)
#endif /* (Software_Reset_Signal__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Software_Reset_Signal_H */


/* [] END OF FILE */
