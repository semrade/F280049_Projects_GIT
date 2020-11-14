/***********************************************************************************
 * File                     :CAN_main1.c
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :
 *
 * Version                  : 0.1
 *
 * Date						:11 nov. 2020
 *
 * Copyright (c) 2020 Tarik SEMRADE
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *********************************************************************************/


/**********************************************************************************
 *  Included Files
 *
 **********************************************************************************/
#include "driverlib.h"
#include "device.h"
#include "F28x_Project.h"
#include "CAN_main.h"
/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/


/**********************************************************************************
 *  Globals
 *
 *********************************************************************************/
volatile unsigned long i;
volatile uint32_t txMsgCount = 0;
volatile uint32_t rxMsgCount = 0;
volatile uint32_t errorFlag = 0;
uint16_t txMsgData[MSG_DATA_LENGTH];
uint16_t rxMsgData[MSG_DATA_LENGTH];

/**********************************************************************************
 * \function:       main 
 * \brief           main
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void main(void)
{
    uint16_t Index;

    /* Initialize device clock and peripherals */
    Device_init();

    /*
     * Initialize GPIO and configure GPIO pins for
     * Initialize CANTX/CANRX on module A and B
     * */

    
    Device_initGPIO();
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANRXA);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_CANTXA);

    /* Initialize the CAN controllers */
    CAN_initModule(CANA_BASE);

   /*
    * Set up the CAN bus bit rate to 500kHz for each module
    * Refer to the Driver Library User Guide for information on how to set
    * tighter timing control. Additionally, consult the device data sheet
    * for more information about the CAN module clocking.
    */
    CAN_setBitRate(CANA_BASE, DEVICE_SYSCLK_FREQ, 500000, 20);

    
    /* Enable interrupts on the CAN B peripheral*/
    CAN_enableInterrupt(CANA_BASE, CAN_INT_IE0 | CAN_INT_ERROR | CAN_INT_STATUS);

    
    /* Initialize PIE and clear PIE registers. Disables CPU interrupts */
    Interrupt_initModule();

    /*
     * Initialize the PIE vector table with pointers to the shell Interrupt
     * Service Routines (ISR) */
    Interrupt_initVectorTable();

    /* Enable Global Interrupt (INTM) and realtime interrupt (DBGM) */
    EINT;
    ERTM;

    
    /*
     * Interrupts that are used in this example are re-mapped to
     * ISR functions found within this file.
     * This registers the interrupt handler in PIE vector table.
     */
    Interrupt_register(INT_CANA0, &canaISR);

    
    /* Enable the CAN-B interrupt signal */
    Interrupt_enable(INT_CANA0);
    CAN_enableGlobalInterrupt(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    /*
     * Initialize the transmit message object used for sending CAN messages.
     * Message Object Parameters:
     *     CAN Module: A
     *     Message Object ID Number: 1
     *     Message Identifier: 0x95555555
     *     Message Frame: Extended
     *     Message Type: Transmit
     *     Message ID Mask: 0x0
     *     Message Object Flags: None
     *     Message Data Length: 4 Bytes
     */
    CAN_setupMessageObject(CANA_BASE, TX_MSG_OBJ_ID, 0x95555555,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_TX, 0,
                           CAN_MSG_OBJ_NO_FLAGS, MSG_DATA_LENGTH);

    
    /* Initialize the receive message object used for receiving CAN messages.
    *  Message Object Parameters:
    *      CAN Module: B
    *      Message Object ID Number: 1
    *      Message Identifier: 0x95555555
    *      Message Frame: Extended
    *      Message Type: Receive
    *      Message ID Mask: 0x0
    *      Message Object Flags: Receive Interrupt
    *      Message Data Length: 4 Bytes
    */
    CAN_setupMessageObject(CANA_BASE, RX_MSG_OBJ_ID, 0x95555555,
                           CAN_MSG_FRAME_EXT, CAN_MSG_OBJ_TYPE_RX, 0,
                           CAN_MSG_OBJ_RX_INT_ENABLE, MSG_DATA_LENGTH);

    
    /*Initialize the transmit message object data buffer to be sent*/
    txMsgData[0] = 0x12;
    txMsgData[1] = 0x34;
    txMsgData[2] = 0x56;
    txMsgData[3] = 0x78;
    txMsgData[4] = 0x12;
    txMsgData[5] = 0x34;
    txMsgData[6] = 0x56;
    txMsgData[7] = 0x78;

    
    /* Start CAN module A and B operations */
    
    CAN_startModule(CANA_BASE);

    
    /* Transmit messages from CAN-A to CAN-B */
    
    //for(i = 0; i < TXCOUNT; i++)
    for(;;)
    {

        
        /* Check the error flag to see if errors occurred */
        if(errorFlag)
        {
            asm("   ESTOP0");
        }

        if (txMsgCount == rxMsgCount)
        {
            /*
            * Verify that the number of transmitted messages equal the number of
            * messages received before sending a new message
            */
            CAN_sendMessage(CANA_BASE, TX_MSG_OBJ_ID, MSG_DATA_LENGTH,
                            txMsgData);

            txMsgCount++;
        }
        else
        {
            errorFlag = 1;
        }
        
        /* Delay 0.25 second before continuing */
        DEVICE_DELAY_US(250000);
        
        /* Increment the value in the transmitted message data*/
        
        for (Index = 0; Index < MSG_DATA_LENGTH; Index++)
        {
            txMsgData[Index] += 0x01;
        }
        
        
        /* Reset data if exceeds a byte */
        
        if(txMsgData[0] > 0xFF)
        {
            txMsgData[0] = 0;
        }
        if(txMsgData[1] > 0xFF)
        {
            txMsgData[1] = 0;
        }
        if(txMsgData[2] > 0xFF)
        {
            txMsgData[2] = 0;
        }
        if(txMsgData[3] > 0xFF)
        {
            txMsgData[3] = 0;
        }
        if(txMsgData[4] > 0xFF)
        {
            txMsgData[4] = 0;
        }
        if(txMsgData[5] > 0xFF)
        {
            txMsgData[5] = 0;
        }
        if(txMsgData[6] > 0xFF)
        {
            txMsgData[6] = 0;
        }
        if(txMsgData[7] > 0xFF)
        {
            txMsgData[7] = 0;
        }
    }

    //
    // Stop application
    //
    //asm("   ESTOP0");
}

/**********************************************************************************
 * \function:       main 
 * \brief            CAN B ISR - The interrupt service routine called when a CAN interrupt is triggered on CAN module B.
 * \param[in]       void
 * \return          void
 **********************************************************************************/
__interrupt void canaISR(void)
{
    uint32_t status;

    
    /* Read the CAN-B interrupt status to find the cause of the interrupt */
    status = CAN_getInterruptCause(CANA_BASE);

    
    /* If the cause is a controller status interrupt, then get the status */   
    if(status == CAN_INT_INT0ID_STATUS)
    {
        /*
        * Read the controller status.  This will return a field of status
        * error bits that can indicate various errors.  Error processing
        * is not done in this example for simplicity.  Refer to the
        * API documentation for details about the error status bits.
        * The act of reading this status will clear the interrupt.
        */
        status = CAN_getStatus(CANA_BASE);

        
        /* Check to see if an error occurred */
        
        if(((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_MSK) &&
           ((status  & ~(CAN_STATUS_RXOK)) != CAN_STATUS_LEC_NONE))
        {
            
            /* Set a flag to indicate some errors may have occurred */
            errorFlag = 1;
        }
    }
    
    /* Check if the cause is the CAN-B receive message object 1 */
    else if(status == RX_MSG_OBJ_ID)
    {
        
        /* Get the received message */        
        CAN_readMessage(CANA_BASE, RX_MSG_OBJ_ID, rxMsgData);

        /*
        * Getting to this point means that the RX interrupt occurred on
        * message object 1, and the message RX is complete.  Clear the
        * message object interrupt.
        */
        CAN_clearInterruptStatus(CANA_BASE, RX_MSG_OBJ_ID);

        /*
        * Increment a counter to keep track of how many messages have been
        * received. In a real application this could be used to set flags to
        * indicate when a message is received.
        */
        rxMsgCount++;
        
        /* Since the message was received, clear any error flags */
        errorFlag = 0;
    }
    
    /* If something unexpected caused the interrupt, this would handle it */
    else
    {
        /* Spurious interrupt handling can go here */
    }

    /* Clear the global interrupt flag for the CAN interrupt line */    
    CAN_clearGlobalInterruptStatus(CANA_BASE, CAN_GLOBAL_INT_CANINT0);

    
    /* Acknowledge this interrupt located in group 9 */
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}
