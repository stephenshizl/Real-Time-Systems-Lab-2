/*! *********************************************************************************
#include <observer.h>
* \addtogroup Beacon
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* This file is the source file for the Beacon application
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
/* Framework / Drivers */
#include "RNG_Interface.h"
#include "Keyboard.h"
#include "LED.h"
#include "TimersManager.h"
#include "FunctionLib.h"
#include "fsl_os_abstraction.h"
#include "Panic.h"
#include "SecLib.h"
#include "fsl_device_registers.h"

/* BLE Host Stack */
#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "gatt_database.h"
#include "gap_interface.h"
#include "gatt_db_app_interface.h"
#include "gatt_db_handles.h"
#include "hci_transport.h"

/* Application */
#include "ApplMain.h"

//Debug console
#include "fsl_debug_console.h"

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bleDeviceAddress_t maBleDeviceAddress;

/* Adv Parmeters */
bool_t      mAdvertisingOn = FALSE;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/

/* Gatt and Att callbacks */
//static void BleApp_AdvertisingCallback (gapAdvertisingEvent_t* pAdvertisingEvent);
static void BleApp_Config();
//static void BleApp_Advertise(void);
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief    Initializes application specific functionality before the BLE stack init.
*
********************************************************************************** */
void BleApp_Init(void)
{
    sha1Context_t ctx;
    
    /* Initialize sha buffer with values from SIM_UID */
    FLib_MemCopy32Unaligned(&ctx.buffer[0], SIM->UIDL);
    FLib_MemCopy32Unaligned(&ctx.buffer[4], SIM->UIDML);
    FLib_MemCopy32Unaligned(&ctx.buffer[8], SIM->UIDMH);
    FLib_MemCopy32Unaligned(&ctx.buffer[12], 0);
     
    SHA1_Hash(&ctx, ctx.buffer, 16);
    
    /* Updated UUID value from advertising data with the hashed value */
    //FLib_MemCpy(&gAppAdvertisingData.aAdStructures[1].aData[3], ctx.hash, 16);// commented out, probably not needed
}

/*! *********************************************************************************
* \brief    Starts the BLE application.
*
********************************************************************************** */
void BleApp_Start(void)
{
    //start scanning code
	gapScanningParameters_t scanningParameters = gGapDefaultScanningParameters_d;
	//scanningParameters.ownAddressType = gBleAddrTypeRandom_c; // scan for random ble addresses
	// could change filter policy to ble whitelist to handle interference
	Gap_StartScanning(&scanningParameters, BleApp_ScanningCallback);
}

/*! *********************************************************************************
* \brief        Handles keyboard events.
*
* \param[in]    events    Key event structure.
********************************************************************************** */
void BleApp_HandleKeys(key_event_t events)
{
    switch (events)
    {
        case gKBD_EventPB1_c:
        {
            BleApp_Start();
            break;
        }
        /*case gKBD_EventLongPB1_c:
        {
            if (mAdvertisingOn)
            {
                Gap_StopAdvertising();
            }
            break;
        }//*/
    default:
         break;
    }
}

void BleApp_GenericCallback (gapGenericEvent_t* pGenericEvent)
{
    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:    
        {
            BleApp_Config();
        }
        break;    
            
        case gPublicAddressRead_c:
        {
            /* Use address read from the controller */
            FLib_MemCpyReverseOrder(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
        }
        break;            
            
        case gAdvertisingDataSetupComplete_c:
        {            
            BleApp_Start();
        }
        break;  
        
        case gAdvertisingParametersSetupComplete_c:
        {
            //App_StartAdvertising(BleApp_AdvertisingCallback, NULL);
        }
        break;         

        case gInternalError_c:
        {
            panic(0,0,0,0);
        }
        break;

        default: 
            break;
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
* \brief        Configures BLE Stack after initialization. Usually used for
*               configuring advertising, scanning, white list, services, et al.
*
********************************************************************************** */
static void BleApp_Config()
{
    /* Read public address from controller */
    Gap_ReadPublicDeviceAddress();
    /* Setup Advertising and scanning data */
    //Gap_SetAdvertisingData(&gAppAdvertisingData, NULL);
    mAdvertisingOn = FALSE;
}

/*! *********************************************************************************
* \brief        Configures GAP Advertise parameters. Advertise will satrt after
*               the parameters are set.
*
********************************************************************************** */
static void BleApp_Advertise(void)
{
    /* Set advertising parameters*/
    //Gap_SetAdvertisingParameters((gapAdvertisingParameters_t*)&gAppAdvParams);
}

/*! *********************************************************************************
* \brief        Handles BLE Advertising callback from host stack.
*
* \param[in]    pAdvertisingEvent    Pointer to gapAdvertisingEvent_t.
********************************************************************************** */

// Added, called when a scanning event happens
static void BleApp_ScanningCallback (gapScanningEvent_t* pScanningEvent)
{
	switch (pScanningEvent->eventType)
	{
		case gDeviceScanned_c: // if device is scanned
		{
			gapScannedDevice_t device = pScanningEvent->eventData.scannedDevice; // do something with this, whatever it is

			int test = 0;
			bleDeviceAddress_t beaconAddress[] = {0x0E, 0x21, 0x00, 0x2F, 0x62, 0x00};
			if (device.aAddress == beaconAddress){
				test = 1;
			}
			//LED_StartFlashWithPeriod(LED2, 500); // flash LED2

			//TODO: filter out all BLE addresses apart from the one defined in the beacon
			//TODO: work out what the data means

			LED_ToggleLed(LED3);
		}
		case gScanStateChanged_c: // received when scanning is enabled or disabled
		{
			//LED_StartFlashWithPeriod(LED3, 100); // flash LED2
		}
	}
}

/*! *********************************************************************************
* @}
********************************************************************************** */
