/*******************************************************************************
* File Name: RST_UART_OEM.h  
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

#if !defined(CY_PINS_RST_UART_OEM_H) /* Pins RST_UART_OEM_H */
#define CY_PINS_RST_UART_OEM_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RST_UART_OEM_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RST_UART_OEM__PORT == 15 && ((RST_UART_OEM__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RST_UART_OEM_Write(uint8 value);
void    RST_UART_OEM_SetDriveMode(uint8 mode);
uint8   RST_UART_OEM_ReadDataReg(void);
uint8   RST_UART_OEM_Read(void);
void    RST_UART_OEM_SetInterruptMode(uint16 position, uint16 mode);
uint8   RST_UART_OEM_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RST_UART_OEM_SetDriveMode() function.
     *  @{
     */
        #define RST_UART_OEM_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RST_UART_OEM_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RST_UART_OEM_DM_RES_UP          PIN_DM_RES_UP
        #define RST_UART_OEM_DM_RES_DWN         PIN_DM_RES_DWN
        #define RST_UART_OEM_DM_OD_LO           PIN_DM_OD_LO
        #define RST_UART_OEM_DM_OD_HI           PIN_DM_OD_HI
        #define RST_UART_OEM_DM_STRONG          PIN_DM_STRONG
        #define RST_UART_OEM_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RST_UART_OEM_MASK               RST_UART_OEM__MASK
#define RST_UART_OEM_SHIFT              RST_UART_OEM__SHIFT
#define RST_UART_OEM_WIDTH              1u

/* Interrupt constants */
#if defined(RST_UART_OEM__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RST_UART_OEM_SetInterruptMode() function.
     *  @{
     */
        #define RST_UART_OEM_INTR_NONE      (uint16)(0x0000u)
        #define RST_UART_OEM_INTR_RISING    (uint16)(0x0001u)
        #define RST_UART_OEM_INTR_FALLING   (uint16)(0x0002u)
        #define RST_UART_OEM_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RST_UART_OEM_INTR_MASK      (0x01u) 
#endif /* (RST_UART_OEM__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RST_UART_OEM_PS                     (* (reg8 *) RST_UART_OEM__PS)
/* Data Register */
#define RST_UART_OEM_DR                     (* (reg8 *) RST_UART_OEM__DR)
/* Port Number */
#define RST_UART_OEM_PRT_NUM                (* (reg8 *) RST_UART_OEM__PRT) 
/* Connect to Analog Globals */                                                  
#define RST_UART_OEM_AG                     (* (reg8 *) RST_UART_OEM__AG)                       
/* Analog MUX bux enable */
#define RST_UART_OEM_AMUX                   (* (reg8 *) RST_UART_OEM__AMUX) 
/* Bidirectional Enable */                                                        
#define RST_UART_OEM_BIE                    (* (reg8 *) RST_UART_OEM__BIE)
/* Bit-mask for Aliased Register Access */
#define RST_UART_OEM_BIT_MASK               (* (reg8 *) RST_UART_OEM__BIT_MASK)
/* Bypass Enable */
#define RST_UART_OEM_BYP                    (* (reg8 *) RST_UART_OEM__BYP)
/* Port wide control signals */                                                   
#define RST_UART_OEM_CTL                    (* (reg8 *) RST_UART_OEM__CTL)
/* Drive Modes */
#define RST_UART_OEM_DM0                    (* (reg8 *) RST_UART_OEM__DM0) 
#define RST_UART_OEM_DM1                    (* (reg8 *) RST_UART_OEM__DM1)
#define RST_UART_OEM_DM2                    (* (reg8 *) RST_UART_OEM__DM2) 
/* Input Buffer Disable Override */
#define RST_UART_OEM_INP_DIS                (* (reg8 *) RST_UART_OEM__INP_DIS)
/* LCD Common or Segment Drive */
#define RST_UART_OEM_LCD_COM_SEG            (* (reg8 *) RST_UART_OEM__LCD_COM_SEG)
/* Enable Segment LCD */
#define RST_UART_OEM_LCD_EN                 (* (reg8 *) RST_UART_OEM__LCD_EN)
/* Slew Rate Control */
#define RST_UART_OEM_SLW                    (* (reg8 *) RST_UART_OEM__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RST_UART_OEM_PRTDSI__CAPS_SEL       (* (reg8 *) RST_UART_OEM__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RST_UART_OEM_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RST_UART_OEM__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RST_UART_OEM_PRTDSI__OE_SEL0        (* (reg8 *) RST_UART_OEM__PRTDSI__OE_SEL0) 
#define RST_UART_OEM_PRTDSI__OE_SEL1        (* (reg8 *) RST_UART_OEM__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RST_UART_OEM_PRTDSI__OUT_SEL0       (* (reg8 *) RST_UART_OEM__PRTDSI__OUT_SEL0) 
#define RST_UART_OEM_PRTDSI__OUT_SEL1       (* (reg8 *) RST_UART_OEM__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RST_UART_OEM_PRTDSI__SYNC_OUT       (* (reg8 *) RST_UART_OEM__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RST_UART_OEM__SIO_CFG)
    #define RST_UART_OEM_SIO_HYST_EN        (* (reg8 *) RST_UART_OEM__SIO_HYST_EN)
    #define RST_UART_OEM_SIO_REG_HIFREQ     (* (reg8 *) RST_UART_OEM__SIO_REG_HIFREQ)
    #define RST_UART_OEM_SIO_CFG            (* (reg8 *) RST_UART_OEM__SIO_CFG)
    #define RST_UART_OEM_SIO_DIFF           (* (reg8 *) RST_UART_OEM__SIO_DIFF)
#endif /* (RST_UART_OEM__SIO_CFG) */

/* Interrupt Registers */
#if defined(RST_UART_OEM__INTSTAT)
    #define RST_UART_OEM_INTSTAT            (* (reg8 *) RST_UART_OEM__INTSTAT)
    #define RST_UART_OEM_SNAP               (* (reg8 *) RST_UART_OEM__SNAP)
    
	#define RST_UART_OEM_0_INTTYPE_REG 		(* (reg8 *) RST_UART_OEM__0__INTTYPE)
#endif /* (RST_UART_OEM__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RST_UART_OEM_H */


/* [] END OF FILE */
