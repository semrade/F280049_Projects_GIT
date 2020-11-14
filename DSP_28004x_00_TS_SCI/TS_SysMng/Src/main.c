/***********************************************************************************
 * File                     :main.c
 *
 * Title                    :
 *
 * Author                   :Tarik SEMRADE
 *
 * Description              :
 *
 * Version                  : 0.1
 *
 * Date                     :11 nov. 2020
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
 *********************************************************************************/
#include "F28x_Project.h"
#include "driverlib.h"
#include "device.h"
#include "gpio.h"
//#include "main.h"

/**********************************************************************************
 *  Defines
 *
 *********************************************************************************/

/* Define AUTOBAUD to use the autobaud lock feature*/
//#define AUTOBAUD

/**********************************************************************************
 *  Globals
 *
 *********************************************************************************/
uint16_t counter = 0;
unsigned char *msg;


__interrupt void sciaTxISR(void);
__interrupt void sciaRxISR(void);
/**********************************************************************************
 * \function:       main
 * \brief           main `0` numbers
 * \param[in]       void
 * \return          void
 **********************************************************************************/
void main (void)
{

    /* Set up system flash and turn peripheral clocks */
    InitSysCtrl();

    /* Init all gpio to input */
    InitGpio();

    
    /* GPIO28 is the SCI Rx pin */
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCIRXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);

    
    /* GPIO29 is the SCI Tx pin */
    GPIO_setMasterCore(DEVICE_GPIO_PIN_SCITXDA, GPIO_CORE_CPU1);
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    /* Globally disable maskable CPU interrupts */
    DINT;

    /* Clear and disable all PIE interrupts */
    InitPieCtrl();

    /* Individually disable maskable CPU interrupts */
    IER = 0x0000;

    /* Clear all CPU interrupt flags */
    IFR = 0x0000;

    /* Populate the PIE interrupt vector table */
    InitPieVectTable();

    
    /* Map the ISR to the wake interrupt */    
    Interrupt_register(INT_SCIA_TX, sciaTxISR);
    Interrupt_register(INT_SCIA_RX, sciaRxISR);

    
    /* Initialize SCIA and its FIFO.*/    
    SCI_performSoftwareReset(SCIA_BASE);

    
    /* Configure SCIA for echoback */    
    SCI_setConfig(SCIA_BASE, 25000000, 9600, (SCI_CONFIG_WLEN_8 |
                                             SCI_CONFIG_STOP_ONE |
                                             SCI_CONFIG_PAR_NONE));
    SCI_resetChannels(SCIA_BASE);
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
    SCI_enableFIFO(SCIA_BASE);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

    
    /* Set the transmit FIFO level to 0 and the receive FIFO level to 2.
       Enable the TXFF and RXFF interrupts.*/
    
    SCI_setFIFOInterruptLevel(SCIA_BASE, SCI_FIFO_TX0, SCI_FIFO_RX2);
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);

#ifdef AUTOBAUD
    
     /* Perform an autobaud lock.
        SCI expects an 'a' or 'A' to lock the baud rate.*/
    SCI_lockAutobaud(SCIA_BASE);
#endif

    
    /* Send starting message.*/    
    msg = "\r\n\n\nHello World!\0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 17);
    msg = "\r\nYou will enter a character, and the DSP will echo it back!\n\0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 62);

    
    /* Clear the SCI interrupts before enabling them */    
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);

    
    /* Enable the interrupts in the PIE: Group 9 interrupts 1 & 2.*/    
    Interrupt_enable(INT_SCIA_RX);
    Interrupt_enable(INT_SCIA_TX);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

    
    /* Enable global interrupts */
    
    EINT;

    /* Infinite led loop */
    while (1)
    {
        asm(" NOP");
    }

}

/**********************************************************************************
 * \function:       sciaTxISR 
 * \brief           sciaTxISR - Disable the TXFF interrupt and print message asking
 *                   or two characters
 * \param[in]       void
 * \return          void
 **********************************************************************************/
__interrupt void sciaTxISR(void)
{
    
    /* Disable the TXRDY interrupt */
    SCI_disableInterrupt(SCIA_BASE, SCI_INT_TXFF);

    msg = "\r\nEnter two characters: \0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 26);

    
    /* Acknowledge the PIE interrupt */
    
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);
}

/**********************************************************************************
 * \function:       sciaRxISR 
 * \brief           sciaRxISR - Read two characters from the RXBUF and echo them back
 * \param[in]       void
 * \return          void
 **********************************************************************************/
__interrupt void sciaRxISR(void)
{
    uint16_t receivedChar1, receivedChar2;
    /* Enable the TXFF interrupt again.*/
    
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_TXFF);
    /* Read two characters from the FIFO.*/
    
    receivedChar1 = SCI_readCharBlockingFIFO(SCIA_BASE);
    receivedChar2 = SCI_readCharBlockingFIFO(SCIA_BASE);
  
    /* Echo back the two characters.*/
    msg = "  You sent: \0";
    SCI_writeCharArray(SCIA_BASE, (uint16_t*)msg, 13);
    SCI_writeCharBlockingFIFO(SCIA_BASE, receivedChar1);
    SCI_writeCharBlockingFIFO(SCIA_BASE, receivedChar2);

    
    /* Clear the SCI RXFF interrupt and acknowledge the PIE interrupt.*/  
    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_RXFF);
    Interrupt_clearACKGroup(INTERRUPT_ACK_GROUP9);

    counter++;
}

