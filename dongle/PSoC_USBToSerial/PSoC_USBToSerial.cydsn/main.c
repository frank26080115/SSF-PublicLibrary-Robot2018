/*******************************************************************************
* File Name: main.c
*
* Version: 2.0
*
* Description:
*   The component is enumerated as a Virtual Com port.
*   Acts as a USB-to-UART converter for use with Arduino
*
* Related Document:
*  Universal Serial Bus Specification Revision 2.0
*  Universal Serial Bus Class Definitions for Communications Devices
*  Revision 1.2
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include "stdio.h"

#if defined (__GNUC__)
    /* Add an explicit reference to the floating point printf library */
    /* to allow usage of the floating point conversion specifiers. */
    /* This is not linked in by default with the newlib-nano library. */
    asm (".global _printf_float");
#endif

#define USBFS_DEVICE    (0u)

/* The buffer size is equal to the maximum packet size of the IN and OUT bulk
* endpoints.
*/
#define USBUART_BUFFER_SIZE (64u)

volatile uint32 millis = 0;
volatile uint32 secs = 0;

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  The main function performs the following actions:
*   1. Waits until VBUS becomes valid and starts the USBFS component which is
*      enumerated as virtual Com port.
*   2. Waits until the device is enumerated by the host.
*   3. Waits for data coming from the hyper terminal and sends it back.
*   4. PSoC3/PSoC5LP: the LCD shows the line settings.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    uint16 count;
    uint8 buffer[USBUART_BUFFER_SIZE];

    uint8 prevLines = 0; // tracks previous DTS status
    
    CyGlobalIntEnable;

    ExternReset_SetDriveMode(ExternReset_DM_OD_HI);
    ExternReset_Write(1);
    
    UART_Start();

    /* Start USBFS operation with 5-V operation. */
    USBUART_Start(USBFS_DEVICE, USBUART_5V_OPERATION);
    USBUART_EnableSofInt();

    for(;;)
    {
        /* Host can send double SET_INTERFACE request. */
        if (0u != USBUART_IsConfigurationChanged())
        {
            /* Initialize IN endpoints when device is configured. */
            if (0u != USBUART_GetConfiguration())
            {
                /* Enumeration is done, enable OUT endpoint to receive data 
                 * from host. */
                USBUART_CDC_Init();
            }
        }

        /* Service USB CDC when device is configured. */
        if (0u != USBUART_GetConfiguration())
        {
            /* Check for input data from host. */
            if (0u != USBUART_DataIsReady())
            {
                /* Read received data and re-enable OUT endpoint. */
                count = USBUART_GetAll(buffer);

                if (0u != count)
                {
                    // check for changes in DTS control line signal
                    uint16 curLines = USBUART_GetLineControl();
                    if ((prevLines & USBUART_LINE_CONTROL_DTR) == 0 && (curLines & USBUART_LINE_CONTROL_DTR) != 0)
                    {
                        ExternReset_SetDriveMode(ExternReset_DM_OD_LO);
                        ExternReset_Write(0);
                        // delay for reset pulse to register
                        for (int i = 0; i < 1; i++)
                        {
                            CyDelayUs(100);
                        }
                        ExternReset_Write(1);
                        ExternReset_SetDriveMode(ExternReset_DM_OD_HI);

                        // delay for bootloader to get ready
                        for (int i = 0; i < 200; i++)
                        {
                            CyDelayUs(1000);
                        }
                    }
                    prevLines = curLines; // for comparison on next loop

                    UART_PutArray(buffer, count);
                }
            }

            if (UART_GetRxBufferSize() > 0) // something to read
            {
                unsigned int idx = 0;
                // read all possible
                for (idx = 0; idx < USBUART_BUFFER_SIZE && UART_GetRxBufferSize() > 0; idx++)
                {
                    buffer[idx] = UART_ReadRxData();
                }

                /* Wait until component is ready to send data to host. */
                while (0u == USBUART_CDCIsReady())
                {
                }

                /* Send data back to host. */
                USBUART_PutData(buffer, idx);
            }
        }
    }
}

void USBUART_SOF_ISR_ExitCallback(void)
{
    millis++;
    if (millis >= 1000)
    {
        secs++;
        millis = 0;
    }
}

/* [] END OF FILE */
