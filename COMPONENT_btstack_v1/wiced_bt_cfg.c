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
 * Runtime Bluetooth stack configuration parameters
 *
 */

/* This file is applicable for all devices with BTSTACK version lower than 3.0, i.e. 20xxx and 43012C0 */

#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_transport.h"
#include "wifi_onboarding.h"

#ifndef DEV_NAME
#define DEV_NAME "bleProv"
#endif

/*****************************************************************************
 * wiced_bt core stack configuration
 ****************************************************************************/

const wiced_bt_cfg_settings_t wiced_bt_cfg_settings =
{
    .device_name                         = (uint8_t*)DEV_NAME,                                         /**< Local device name (NULL terminated) */
    .device_class                        = {0x00, 0x00, 0x00},                                         /**< Local device class */
    .security_requirement_mask           = BTM_SEC_NONE,                                               /**< Security requirements mask (BTM_SEC_NONE, or combinination of BTM_SEC_IN_AUTHENTICATE, BTM_SEC_OUT_AUTHENTICATE, BTM_SEC_ENCRYPT (see #wiced_bt_sec_level_e)) */

    .max_simultaneous_links              = 3,                                                          /**< Maximum number simultaneous links to different devices */

    .br_edr_scan_cfg =                                              /* BR/EDR scan config */
    {
        .inquiry_scan_type               = BTM_SCAN_TYPE_STANDARD,                                     /**< Inquiry scan type (BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED) */
        .inquiry_scan_interval           = WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_INTERVAL,                 /**< Inquiry scan interval  (0 to use default) */
        .inquiry_scan_window             = WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_WINDOW,                   /**< Inquiry scan window (0 to use default) */

        .page_scan_type                  = BTM_SCAN_TYPE_STANDARD,                                     /**< Page scan type (BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED) */
        .page_scan_interval              = WICED_BT_CFG_DEFAULT_PAGE_SCAN_INTERVAL,                    /**< Page scan interval  (0 to use default) */
        .page_scan_window                = WICED_BT_CFG_DEFAULT_PAGE_SCAN_WINDOW                       /**< Page scan window (0 to use default) */
    },

    .ble_scan_cfg =                                                 /* LE scan settings  */
    {
        .scan_mode                       = BTM_BLE_SCAN_MODE_PASSIVE,                                  /**< LE scan mode (BTM_BLE_SCAN_MODE_PASSIVE, BTM_BLE_SCAN_MODE_ACTIVE, or BTM_BLE_SCAN_MODE_NONE) */

        /* Advertisement scan configuration */
        .high_duty_scan_interval         = WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_INTERVAL,               /**< High duty scan interval */
        .high_duty_scan_window           = WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_WINDOW,                 /**< High duty scan window */
        .high_duty_scan_duration         = 5,                                                          /**< High duty scan duration in seconds (0 for infinite) */

        .low_duty_scan_interval          = WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_INTERVAL,                /**< Low duty scan interval  */
        .low_duty_scan_window            = WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_WINDOW,                  /**< Low duty scan window */
        .low_duty_scan_duration          = 5,                                                          /**< Low duty scan duration in seconds (0 for infinite) */

        /* Connection scan configuration */
        .high_duty_conn_scan_interval    = WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_INTERVAL,          /**< High duty cycle connection scan interval */
        .high_duty_conn_scan_window      = WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_WINDOW,            /**< High duty cycle connection scan window */
        .high_duty_conn_duration         = 30,                                                         /**< High duty cycle connection duration in seconds (0 for infinite) */

        .low_duty_conn_scan_interval     = WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_INTERVAL,           /**< Low duty cycle connection scan interval */
        .low_duty_conn_scan_window       = WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_WINDOW,             /**< Low duty cycle connection scan window */
        .low_duty_conn_duration          = 30,                                                         /**< Low duty cycle connection duration in seconds (0 for infinite) */

        /* Connection configuration */
        .conn_min_interval               = WICED_BT_CFG_DEFAULT_CONN_MIN_INTERVAL,                     /**< Minimum connection interval */
        .conn_max_interval               = WICED_BT_CFG_DEFAULT_CONN_MAX_INTERVAL,                     /**< Maximum connection interval */
        .conn_latency                    = WICED_BT_CFG_DEFAULT_CONN_LATENCY,                          /**< Connection latency */
        .conn_supervision_timeout        = WICED_BT_CFG_DEFAULT_CONN_SUPERVISION_TIMEOUT,              /**< Connection link supervision timeout */
    },

    .ble_advert_cfg =                                               /* LE advertisement settings */
    {
        .channel_map                     = BTM_BLE_ADVERT_CHNL_37 |                                    /**< Advertising channel map (mask of BTM_BLE_ADVERT_CHNL_37, BTM_BLE_ADVERT_CHNL_38, BTM_BLE_ADVERT_CHNL_39) */
                                           BTM_BLE_ADVERT_CHNL_38 |
                                           BTM_BLE_ADVERT_CHNL_39,

        .high_duty_min_interval          = WICED_BT_CFG_DEFAULT_HIGH_DUTY_ADV_MIN_INTERVAL,            /**< High duty undirected connectable minimum advertising interval */
        .high_duty_max_interval          = WICED_BT_CFG_DEFAULT_HIGH_DUTY_ADV_MAX_INTERVAL,            /**< High duty undirected connectable maximum advertising interval */
        .high_duty_duration              = 30,                                                         /**< High duty undirected connectable advertising duration in seconds (0 for infinite) */

        .low_duty_min_interval           = 1024,                                                       /**< Low duty undirected connectable minimum advertising interval */
        .low_duty_max_interval           = 1024,                                                       /**< Low duty undirected connectable maximum advertising interval */
        .low_duty_duration               = 60,                                                         /**< Low duty undirected connectable advertising duration in seconds (0 for infinite) */

        .high_duty_directed_min_interval = WICED_BT_CFG_DEFAULT_HIGH_DUTY_DIRECTED_ADV_MIN_INTERVAL,   /**< High duty directed connectable minimum advertising interval */
        .high_duty_directed_max_interval = WICED_BT_CFG_DEFAULT_HIGH_DUTY_DIRECTED_ADV_MAX_INTERVAL,   /**< High duty directed connectable maximum advertising interval */

        .low_duty_directed_min_interval  = WICED_BT_CFG_DEFAULT_LOW_DUTY_DIRECTED_ADV_MIN_INTERVAL,    /**< Low duty directed connectable minimum advertising interval */
        .low_duty_directed_max_interval  = WICED_BT_CFG_DEFAULT_LOW_DUTY_DIRECTED_ADV_MAX_INTERVAL,    /**< Low duty directed connectable maximum advertising interval */
        .low_duty_directed_duration      = 30,                                                         /**< Low duty directed connectable advertising duration in seconds (0 for infinite) */

        .high_duty_nonconn_min_interval  = WICED_BT_CFG_DEFAULT_HIGH_DUTY_NONCONN_ADV_MIN_INTERVAL,    /**< High duty non-connectable minimum advertising interval */
        .high_duty_nonconn_max_interval  = WICED_BT_CFG_DEFAULT_HIGH_DUTY_NONCONN_ADV_MAX_INTERVAL,    /**< High duty non-connectable maximum advertising interval */
        .high_duty_nonconn_duration      = 30,                                                         /**< High duty non-connectable advertising duration in seconds (0 for infinite) */

        .low_duty_nonconn_min_interval   = WICED_BT_CFG_DEFAULT_LOW_DUTY_NONCONN_ADV_MIN_INTERVAL,     /**< Low duty non-connectable minimum advertising interval */
        .low_duty_nonconn_max_interval   = WICED_BT_CFG_DEFAULT_LOW_DUTY_NONCONN_ADV_MAX_INTERVAL,     /**< Low duty non-connectable maximum advertising interval */
        .low_duty_nonconn_duration       = 0                                                           /**< Low duty non-connectable advertising duration in seconds (0 for infinite) */
    },

    .gatt_cfg =                                                     /* GATT configuration */
    {
        .appearance                     = APPEARANCE_GENERIC_TAG,                                      /**< GATT appearance (see gatt_appearance_e) */
        .client_max_links               = 0,                                                           /**< Client config: maximum number of servers that local client can connect to  */
        .server_max_links               = 1,                                                           /**< Server config: maximum number of remote clients connections allowed by the local */
        .max_attr_len                   = 512,                                                         /**< Maximum attribute length; gki_cfg must have a corresponding buffer pool that can hold this length */
#if !defined(CYW20706A2)
        .max_mtu_size                   = 515                                                          /**< Maximum MTU size for GATT connections, should be between 23 and (max_attr_len + 5) */
#endif
    },

    .rfcomm_cfg =                                                   /* RFCOMM configuration */
    {
        .max_links                      = 0,                                                           /**< Maximum number of simultaneous connected remote devices*/
        .max_ports                      = 0                                                            /**< Maximum number of simultaneous RFCOMM ports */
    },

    .l2cap_application =                                            /* Application managed l2cap protocol configuration */
    {
        .max_links                      = 0,                                                           /**< Maximum number of application-managed l2cap links (BR/EDR and LE) */

        /* BR EDR l2cap configuration */
        .max_psm                        = 0,                                                           /**< Maximum number of application-managed BR/EDR PSMs */
        .max_channels                   = 0,                                                           /**< Maximum number of application-managed BR/EDR channels  */

        /* LE L2cap connection-oriented channels configuration */
        .max_le_psm                     = 0,                                                           /**< Maximum number of application-managed LE PSMs */
        .max_le_channels                = 0,                                                           /**< Maximum number of application-managed LE channels */
#if !defined(CYW20706A2)
        /* LE L2cap fixed channel configuration */
        .max_le_l2cap_fixed_channels    = 0                                                            /**< Maximum number of application managed fixed channels supported (in addition to mandatory channels 4, 5 and 6). > */
#endif
    },

    .avdt_cfg =
    /* Audio/Video Distribution configuration */
    {
        .max_links                      = 0,                                                           /**< Maximum simultaneous audio/video links */
#if !defined(CYW20706A2)
        .max_seps                       = 0                                                            /**< Maximum number of stream end points */
#endif
    },

    .avrc_cfg =                                                     /* Audio/Video Remote Control configuration */
    {
        .roles                          = 0,                                                           /**< Mask of local roles supported (AVRC_CONN_INITIATOR|AVRC_CONN_ACCEPTOR) */
        .max_links                      = 0                                                            /**< Maximum simultaneous remote control links */
    },

    /* LE Address Resolution DB size  */
    .addr_resolution_db_size            = 5,                                                           /**< LE Address Resolution DB settings - effective only for pre 4.2 controller*/

#ifdef CYW20706A2
    .max_mtu_size                       = 360,                                                         /**< Maximum MTU size for GATT connections, should be between 23 and (max_attr_len + 5) */
    .max_pwr_db_val                     = 12                                                           /**< Max. power level of the device */
#else
    /* Maximum number of buffer pools */
#ifdef COMPONENT_nvram_emulation
    /* increase to allow nvram emulation to create more buffer pools */
    .max_number_of_buffer_pools         = 4 + 4,                                                       /**< Maximum number of buffer pools in p_btm_cfg_buf_pools and by wiced_create_pool */
#else
    .max_number_of_buffer_pools         = 4,                                                           /**< Maximum number of buffer pools in p_btm_cfg_buf_pools and by wiced_create_pool */
#endif

    /* Interval of  random address refreshing */
#if defined(COMPONENT_nvram_emulation)
    .rpa_refresh_timeout                = WICED_BT_CFG_DEFAULT_RANDOM_ADDRESS_NEVER_CHANGE,            /**< Never refresh random address */
#else
    .rpa_refresh_timeout                = WICED_BT_CFG_DEFAULT_RANDOM_ADDRESS_CHANGE_TIMEOUT,          /**< Interval of  random address refreshing - secs */
#endif
    /* LE Filter Accept List size */
    .ble_filter_accept_list_size                = 0,                                                           /**< Maximum number of Filter Accept List devices allowed. Cannot be more than 128 */
#endif

#if defined(CYW20719B2) || defined(CYW20721B2) || defined(CYW20819A1) || defined (CYW20820A1) || defined (CYW30739A0)
    .default_ble_power_level            = 12                                                           /**< Default LE power level, Refer lm_TxPwrTable table for the power range */
#endif
};

/*****************************************************************************
 * wiced_bt_stack buffer pool configuration
 *
 * Configure buffer pools used by the stack
 *
 * Pools must be ordered in increasing buf_size.
 * If a pool runs out of buffers, the next  pool will be used
 *****************************************************************************/
const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[WICED_BT_CFG_NUM_BUF_POOLS] =
{
/*  { buf_size, buf_count } */
    { 64,       12  },      /* Small Buffer Pool */
    { 360,      6   },      /* Medium Buffer Pool (used for HCI & RFCOMM control messages, min recommended size is 360) */
    { 1056,     6   },      /* Large Buffer Pool  (used for HCI ACL messages) */
    { 1056,     0   },      /* Extra Large Buffer Pool - Used for avdt media packets and miscellaneous (if not needed, set buf_count to 0) */
};

/* transport configuration */
#if defined(COMPONENT_nvram_emulation) && NVRAM_EMULATION_HCI
/* configure for HCI backup to Host MCU with NVRAM_EMULATION_HCI */

/* application callback handlers for HCI transport */
void wifi_onboarding_transport_status( wiced_transport_type_t type );
uint32_t wifi_onboarding_process_rx( uint8_t *p_buffer, uint32_t length );
void wifi_onboarding_tx_complete(wiced_transport_buffer_pool_t* p_pool);

const wiced_transport_cfg_t transport_cfg =
{
    .type = WICED_TRANSPORT_UART,
    .cfg =
    {
        .uart_cfg =
        {
            .mode = WICED_TRANSPORT_UART_HCI_MODE,
            .baud_rate =  HCI_UART_DEFAULT_BAUD
        },
    },
    /* buffers used to receive HCI packets */
    .rx_buff_pool_cfg =
    {
        .buffer_size = 1024,
        .buffer_count = 2
    },
    /* transport callback handlers that can call corresponding nvram emulation handlers */
    .p_status_handler = wifi_onboarding_transport_status,
    .p_data_handler = wifi_onboarding_process_rx,
    .p_tx_complete_cback = wifi_onboarding_tx_complete
};

#else
void wifi_onboarding_transport_status( wiced_transport_type_t type );
const wiced_transport_cfg_t transport_cfg =
{
    .type = WICED_TRANSPORT_UART,
    .cfg =
    {
        .uart_cfg =
        {
            .mode = WICED_TRANSPORT_UART_HCI_MODE,
            .baud_rate =  HCI_UART_DEFAULT_BAUD
        },
    },
    .rx_buff_pool_cfg =
    {
        .buffer_size = 0,
        .buffer_count = 0
    },
    .p_status_handler = wifi_onboarding_transport_status,
    .p_data_handler = wifi_onboarding_proc_rx_cmd,
    .p_tx_complete_cback = NULL
};
#endif
