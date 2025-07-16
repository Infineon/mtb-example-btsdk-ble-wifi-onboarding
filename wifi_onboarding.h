/*
 * Copyright 2016-2025, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/** @file
*
* LE Vendor Specific Device
*
* This file provides definitions and function prototypes for Hello Sensor
* device
*
*/
#ifndef _WIFI_ONBOARDING_H_
#define _WIFI_ONBOARDING_H_

#include "wiced_hal_nvram.h"
#include "hci_control_api.h"

#if BTSTACK_VER >= 0x03000001
#include "wiced_memory.h"
#include "bt_types.h"
#define BT_STACK_HEAP_SIZE          1024 * 6
#define wiced_bt_gatt_send_notification(id, type, len, ptr) wiced_bt_gatt_server_send_notification(id, type, len, ptr, NULL)
#define wiced_bt_gatt_send_indication(id, type, len, ptr)   wiced_bt_gatt_server_send_indication(id, type, len, ptr, NULL)
uint8_t *wifi_onboarding_alloc_buffer(uint16_t len);
void wifi_onboarding_free_buffer(uint8_t *p_data);
typedef void (*pfn_free_buffer_t)(uint8_t *);

#else // BTSTACK_VER

#include "wiced_gki.h"
#if !defined(CYW20835B1) && !defined(CYW20719B1) && !defined(CYW20721B1) && !defined(CYW20819A1) && !defined(CYW20719B2) && !defined(CYW20721B2) && !defined(CYW30739A0)
#include "wiced_bt_app_common.h"
#include "wiced_bt_app_hal_common.h"
#endif

extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];
#endif // BTSTACK_VER

extern const wiced_transport_cfg_t transport_cfg;
void wifi_onboarding_gatt_init();
extern const uint8_t wifi_onboarding_gatt_database[];
extern size_t wifi_onboarding_gatt_database_size;
extern uint32_t wifi_onboarding_proc_rx_cmd(uint8_t *p_buffer, uint32_t length);
extern void wifi_onboarding_build_and_send_Onboarding_event(uint16_t code, uint8_t* val, uint16_t len, uint16_t handle);
extern uint8_t wifi_onboarding_gatts_send_notification(uint8_t *p_buffer,uint32_t len, uint16_t conn_id);
extern void wifi_onboarding_transport_status( wiced_transport_type_t type );

#ifndef PACKED
#define PACKED
#endif

/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define WIFI_ONBOARDING_MAX_NUM_CLIENTS 1

/* Hello Sensor App Timer Timeout in seconds  */
#define WIFI_ONBOARDING_APP_TIMEOUT_IN_SECONDS                 1

/* Hello Sensor App Fine Timer Timeout in milli seconds  */
#define WIFI_ONBOARDING_APP_FINE_TIMEOUT_IN_MS                 1

/* Hello Sensor Connection Idle  Timeout in milli seconds  */
#define WIFI_ONBOARDING_CONN_IDLE_TIMEOUT_IN_SECONDS           3

#define WIFI_ONBOARDING_VS_ID                      WICED_NVRAM_VSID_START
#define WIFI_ONBOARDING_LOCAL_KEYS_VS_ID          ( WICED_NVRAM_VSID_START + 1 )
#define WIFI_ONBOARDING_PAIRED_KEYS_VS_ID          ( WICED_NVRAM_VSID_START + 2 )

#ifdef CYW20706A2
#define WIFI_ONBOARDING_GPIO_BUTTON_SETTINGS       WICED_GPIO_BUTTON_SETTINGS( GPIO_EN_INT_RISING_EDGE )
#define WIFI_ONBOARDING_GPIO_BUTTON_PRESSED_VALUE  WICED_GPIO_BUTTON_DEFAULT_STATE
#endif

/* RX PDU size */
#define CY_BT_RX_PDU_SIZE                                     517

#define __UUID_SERVICE_GENERIC_ACCESS                                   0x1800
#define __UUID_CHARACTERISTIC_DEVICE_NAME                               0x2A00
#define __UUID_CHARACTERISTIC_APPEARANCE                                0x2A01
#define __UUID_SERVICE_GENERIC_ATTRIBUTE                                0x1801
#define __UUID_SERVICE_CUSTOM_SERVICE                                   0x5B, 0x19, 0xBA, 0xE4, 0xE4, 0x52, 0xA9, 0x96, 0xF1, 0x4A, 0x84, 0xC8, 0x09, 0x4D, 0xC0, 0x21
#define __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_SSID                  0x63, 0x97, 0xCA, 0x92, 0x5F, 0x02, 0x1E, 0xB9, 0x3D, 0x4A, 0x31, 0x6B, 0x43, 0x00, 0x50, 0x1E
#define __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION               0x2901
#define __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_PASSWORD              0x64, 0x97, 0xCA, 0x92, 0x5F, 0x02, 0x1E, 0xB9, 0x3D, 0x4A, 0x31, 0x6B, 0x43, 0x00, 0x50, 0x1E
#define __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD         0x65, 0x97, 0xCA, 0x92, 0x5F, 0x02, 0x1E, 0xB9, 0x3D, 0x4A, 0x31, 0x6B, 0x43, 0x00, 0x50, 0x1E
#define __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_NETWORKS              0x66, 0x97, 0xCA, 0x92, 0x5F, 0x02, 0x1E, 0xB9, 0x3D, 0x4A, 0x31, 0x6B, 0x43, 0x00, 0x50, 0x1E
#define __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION           0x2902
#define __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_CONTROL               0x67, 0x97, 0xCA, 0x92, 0x5F, 0x02, 0x1E, 0xB9, 0x3D, 0x4A, 0x31, 0x6B, 0x43, 0x00, 0x50, 0x1E

/* Service Generic Access */
#define HDLS_GAP                                                        0x0001
/* Characteristic Device Name */
#define HDLC_GAP_DEVICE_NAME                                            0x0002
#define HDLC_GAP_DEVICE_NAME_VALUE                                      0x0003
#define MAX_LEN_GAP_DEVICE_NAME                                         0x0007
/* Characteristic Appearance */
#define HDLC_GAP_APPEARANCE                                             0x0004
#define HDLC_GAP_APPEARANCE_VALUE                                       0x0005
#define MAX_LEN_GAP_APPEARANCE                                          0x0002

/* Service Generic Attribute */
#define HDLS_GATT                                                       0x0006

/* Service Custom Service */
#define HDLS_CUSTOM_SERVICE                                             0x0007
/* Characteristic WIFI SSID */
#define HDLC_CUSTOM_SERVICE_WIFI_SSID                                   0x0008
#define HDLC_CUSTOM_SERVICE_WIFI_SSID_VALUE                             0x0009
#define MAX_LEN_CUSTOM_SERVICE_WIFI_SSID                                0x0020
/* Descriptor Characteristic User Description */
#define HDLD_CUSTOM_SERVICE_WIFI_SSID_CHAR_USER_DESCRIPTION             0x000A
#define MAX_LEN_CUSTOM_SERVICE_WIFI_SSID_CHAR_USER_DESCRIPTION          0x0009
/* Characteristic WIFI PASSWORD */
#define HDLC_CUSTOM_SERVICE_WIFI_PASSWORD                               0x000B
#define HDLC_CUSTOM_SERVICE_WIFI_PASSWORD_VALUE                         0x000C
#define MAX_LEN_CUSTOM_SERVICE_WIFI_PASSWORD                            0x003F
/* Descriptor Characteristic User Description */
#define HDLD_CUSTOM_SERVICE_WIFI_PASSWORD_CHAR_USER_DESCRIPTION         0x000D
#define MAX_LEN_CUSTOM_SERVICE_WIFI_PASSWORD_CHAR_USER_DESCRIPTION      0x000D
/* Characteristic WIFI SSID PASSWORD */
#define HDLC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD                          0x000E
#define HDLC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_VALUE                    0x000F
#define MAX_LEN_CUSTOM_SERVICE_WIFI_SSID_PASSWORD                       0x0000
/* Descriptor Characteristic User Description */
#define HDLD_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_CHAR_USER_DESCRIPTION    0x0010
#define MAX_LEN_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_CHAR_USER_DESCRIPTION    0x0016
/* Characteristic WIFI NETWORKS */
#define HDLC_CUSTOM_SERVICE_WIFI_NETWORKS                               0x0011
#define HDLC_CUSTOM_SERVICE_WIFI_NETWORKS_VALUE                         0x0012
#define MAX_LEN_CUSTOM_SERVICE_WIFI_NETWORKS                            0x0000
/* Descriptor Characteristic User Description */
#define HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CHAR_USER_DESCRIPTION         0x0013
#define MAX_LEN_CUSTOM_SERVICE_WIFI_NETWORKS_CHAR_USER_DESCRIPTION      0x000D
/* Descriptor Client Characteristic Configuration */
#define HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CLIENT_CHAR_CONFIG            0x0014
#define MAX_LEN_CUSTOM_SERVICE_WIFI_NETWORKS_CLIENT_CHAR_CONFIG         0x0002
/* Characteristic WIFI CONTROL */
#define HDLC_CUSTOM_SERVICE_WIFI_CONTROL                                0x0015
#define HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE                          0x0016
#define MAX_LEN_CUSTOM_SERVICE_WIFI_CONTROL                             0x0001
/* Descriptor Client Characteristic Configuration */
#define HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CLIENT_CHAR_CONFIG             0x0017
#define MAX_LEN_CUSTOM_SERVICE_WIFI_CONTROL_CLIENT_CHAR_CONFIG          0x0002
/* Descriptor Characteristic User Description */
#define HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CHAR_USER_DESCRIPTION          0x0018
#define MAX_LEN_CUSTOM_SERVICE_WIFI_CONTROL_CHAR_USER_DESCRIPTION       0x000C


/******************************************************************************
 *                                Structures
 ******************************************************************************/
typedef struct
{
    BD_ADDR   remote_addr;              // remote peer device address
    uint16_t  conn_id;                  // connection ID referenced by the stack
    uint16_t  peer_mtu;                 // peer MTU
} wifi_onboarding_state_t;


#pragma pack(1)
/* Host information saved in  NVRAM */
typedef PACKED struct
{
    BD_ADDR  bdaddr;                                /* BD address of the bonded host */
    uint16_t  characteristic_client_configuration;  /* Current value of the client configuration descriptor */
} host_info_t;
#pragma pack()

typedef struct
{
    uint16_t handle;
    uint16_t attr_len;
    void     *p_attr;
} attribute_t;

#endif // _WIFI_ONBOARDING_H_
