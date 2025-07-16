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
* WiFi Onboarding
*
*/
#include <string.h>
#include "sparcommon.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_hal_gpio.h"
#include "wiced_bt_uuid.h"
#include "wiced_result.h"
#include "wiced_bt_stack.h"
#include "wiced_transport.h"
#include "wiced_hal_puart.h"
#include "wiced_platform.h"

#ifdef  WICED_BT_TRACE_ENABLE
#include "wiced_bt_trace.h"
#endif
#include "wiced_timer.h"
#include "wifi_onboarding.h"

#ifdef COMPONENT_nvram_emulation
    #include "nvram_emulation_mem.h"
#endif

#include "hci_control_api.h"

// #define ENABLE_HCI_TRACE 1 // configures HCI traces to be routed to the WICED HCI interface
/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define WIFI_ONBOARDING_GATTS_MAX_CONN     1

#define APP_ADV_NAME wiced_bt_cfg_settings.device_name

#ifndef DEV_NAME
#define DEV_NAME "bleProv"
#endif

extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;

#if defined(CYW43022C1)
#define LEGATTDB_PERM_VARIABLE_LENGTH        GATTDB_PERM_VARIABLE_LENGTH
#define LEGATTDB_PERM_WRITE_CMD              GATTDB_PERM_WRITE_CMD
#define LEGATTDB_PERM_AUTH_READABLE          GATTDB_PERM_AUTH_READABLE
#define LEGATTDB_PERM_AUTH_WRITABLE          GATTDB_PERM_AUTH_WRITABLE
#define LEGATTDB_PERM_RELIABLE_WRITE         GATTDB_PERM_RELIABLE_WRITE
#define LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE GATTDB_CHAR_PROP_WRITE_NO_RESPONSE
#endif

/******************************************************************************
 *                                Variables Definitions
 ******************************************************************************/
/*
 * This is the GATT database for the WiFi Onboarding application.  It defines
 * services, characteristics and descriptors supported by the sensor.  Each
 * attribute in the database has a handle, (characteristic has two, one for
 * characteristic itself, another for the value).  The handles are used by
 * the peer to access attributes, and can be used locally by application for
 * example to retrieve data written by the peer.  Definition of characteristics
 * and descriptors has GATT Properties (read, write, notify...) but also has
 * permissions which identify if and how peer is allowed to read or write
 * into it.  All handles do not need to be sequential, but need to be in
 * ascending order.
 */
const uint8_t wifi_onboarding_gatt_database[]=
{
    /* Primary Service: Generic Access */
    PRIMARY_SERVICE_UUID16 (
            HDLS_GAP,
            __UUID_SERVICE_GENERIC_ACCESS),
        /* Characteristic: Device Name */
        CHARACTERISTIC_UUID16 (
                HDLC_GAP_DEVICE_NAME,
                HDLC_GAP_DEVICE_NAME_VALUE,
                __UUID_CHARACTERISTIC_DEVICE_NAME,
                LEGATTDB_CHAR_PROP_READ,
                LEGATTDB_PERM_READABLE),
        /* Characteristic: Appearance */
        CHARACTERISTIC_UUID16 (
                HDLC_GAP_APPEARANCE,
                HDLC_GAP_APPEARANCE_VALUE,
                __UUID_CHARACTERISTIC_APPEARANCE,
                LEGATTDB_CHAR_PROP_READ,
                LEGATTDB_PERM_READABLE),

    /* Primary Service: Generic Attribute */
    PRIMARY_SERVICE_UUID16 (
            HDLS_GATT,
            __UUID_SERVICE_GENERIC_ATTRIBUTE),

    /* Primary Service: Custom Service */
    PRIMARY_SERVICE_UUID128 (
            HDLS_CUSTOM_SERVICE,
            __UUID_SERVICE_CUSTOM_SERVICE),
        /* Characteristic: WIFI SSID */
        CHARACTERISTIC_UUID128_WRITABLE (
                HDLC_CUSTOM_SERVICE_WIFI_SSID,
                HDLC_CUSTOM_SERVICE_WIFI_SSID_VALUE,
                __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_SSID,
                LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_WRITE,
                LEGATTDB_PERM_VARIABLE_LENGTH | LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_AUTH_READABLE | LEGATTDB_PERM_AUTH_WRITABLE | LEGATTDB_PERM_RELIABLE_WRITE),
            /* Descriptor: Characteristic User Description */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_CUSTOM_SERVICE_WIFI_SSID_CHAR_USER_DESCRIPTION,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE),
        /* Characteristic: WIFI PASSWORD */
        CHARACTERISTIC_UUID128_WRITABLE (
                HDLC_CUSTOM_SERVICE_WIFI_PASSWORD,
                HDLC_CUSTOM_SERVICE_WIFI_PASSWORD_VALUE,
                __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_PASSWORD,
                LEGATTDB_CHAR_PROP_WRITE | LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE,
                LEGATTDB_PERM_VARIABLE_LENGTH | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_AUTH_WRITABLE),
            /* Descriptor: Characteristic User Description */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_CUSTOM_SERVICE_WIFI_PASSWORD_CHAR_USER_DESCRIPTION,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE),
        /* Characteristic: WIFI SSID PASSWORD */
        CHARACTERISTIC_UUID128_WRITABLE (
                HDLC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD,
                HDLC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_VALUE,
                __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD,
                LEGATTDB_CHAR_PROP_WRITE | LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE,
                LEGATTDB_PERM_VARIABLE_LENGTH | LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_AUTH_WRITABLE),
            /* Descriptor: Characteristic User Description */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_CHAR_USER_DESCRIPTION,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE),
        /* Characteristic: WIFI NETWORKS */
        CHARACTERISTIC_UUID128 (
                HDLC_CUSTOM_SERVICE_WIFI_NETWORKS,
                HDLC_CUSTOM_SERVICE_WIFI_NETWORKS_VALUE,
                __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_NETWORKS,
                LEGATTDB_CHAR_PROP_NOTIFY,
                LEGATTDB_PERM_NONE),
            /* Descriptor: Characteristic User Description */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CHAR_USER_DESCRIPTION,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE),
            /* Descriptor: Client Characteristic Configuration */
            CHAR_DESCRIPTOR_UUID16_WRITABLE (
                    HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CLIENT_CHAR_CONFIG,
                    __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION,
                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ),
        /* Characteristic: WIFI CONTROL */
        CHARACTERISTIC_UUID128_WRITABLE (
                HDLC_CUSTOM_SERVICE_WIFI_CONTROL,
                HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE,
                __UUID_CHARACTERISTIC_CUSTOM_SERVICE_WIFI_CONTROL,
                LEGATTDB_CHAR_PROP_WRITE | LEGATTDB_CHAR_PROP_WRITE_NO_RESPONSE | LEGATTDB_CHAR_PROP_NOTIFY,
                LEGATTDB_PERM_WRITE_REQ | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_AUTH_WRITABLE),
            /* Descriptor: Client Characteristic Configuration */
            CHAR_DESCRIPTOR_UUID16_WRITABLE (
                    HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CLIENT_CHAR_CONFIG,
                    __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION,
                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ),
            /* Descriptor: Characteristic User Description */
            CHAR_DESCRIPTOR_UUID16 (
                    HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CHAR_USER_DESCRIPTION,
                    __UUID_DESCRIPTOR_CHARACTERISTIC_USER_DESCRIPTION,
                    LEGATTDB_PERM_READABLE),
};

size_t wifi_onboarding_gatt_database_size = sizeof(wifi_onboarding_gatt_database);

uint8_t wifi_onboarding_device_name[]          = DEV_NAME;                                          //GAP Service characteristic Device Name
uint8_t wifi_onboarding_appearance_name[2]     = { BIT16_TO_8(APPEARANCE_GENERIC_TAG) };
uint8_t wifi_onboarding_service_wifi_ssid[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0',};
uint8_t wifi_onboarding_service_wifi_ssid_char_user_description[] = {'W', 'i', 'F', 'i', ' ', 'S',
    'S', 'I', 'D',};
uint8_t wifi_onboarding_service_wifi_password[] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0',};
uint8_t wifi_onboarding_service_wifi_password_char_user_description[] = {'W', 'i', 'F', 'i', ' ',
    'P', 'a', 's', 's', 'w', 'o', 'r', 'd',};
uint8_t wifi_onboarding_service_wifi_ssid_password[] = {0x00,};
uint8_t wifi_onboarding_service_wifi_ssid_password_char_user_description[] = {'W', 'i', 'F', 'i',
    ' ', 'S', 'S', 'I', 'D', ' ', 'a', 'n', 'd', ' ', 'P', 'a', 's', 's', 'w', 'o', 'r', 'd',};
uint8_t wifi_onboarding_service_wifi_networks[] = {0x00,};
uint8_t wifi_onboarding_service_wifi_networks_char_user_description[] = {'W', 'i', 'F', 'i', ' ',
    'N', 'e', 't', 'w', 'o', 'r', 'k', 's',};
uint8_t wifi_onboarding_service_wifi_networks_client_char_config[] = {0x00, 0x00,};
uint8_t wifi_onboarding_service_wifi_control[] = {0x00,};
uint8_t wifi_onboarding_service_wifi_control_client_char_config[] = {0x00, 0x00,};
uint8_t wifi_onboarding_service_wifi_control_char_user_description[] = {'W', 'i', 'F', 'i', ' ', 'C',
    'o', 'n', 't', 'r', 'o', 'l',};


/* Holds the global state of the wifi onboarding application */
wifi_onboarding_state_t wifi_onboarding_state;

/* Holds the host info saved in the NVRAM */
host_info_t wifi_onboarding_hostinfo;

/* Attribute list of the WiFi Onboarding */
attribute_t gauAttributes[] =
{
    { HDLC_GAP_DEVICE_NAME_VALUE, sizeof(wifi_onboarding_device_name ), wifi_onboarding_device_name },
    { HDLC_GAP_APPEARANCE_VALUE, sizeof(wifi_onboarding_appearance_name), wifi_onboarding_appearance_name },
    { HDLC_CUSTOM_SERVICE_WIFI_SSID_VALUE, sizeof(wifi_onboarding_service_wifi_ssid), wifi_onboarding_service_wifi_ssid },
    { HDLD_CUSTOM_SERVICE_WIFI_SSID_CHAR_USER_DESCRIPTION, 9, wifi_onboarding_service_wifi_ssid_char_user_description },
    { HDLC_CUSTOM_SERVICE_WIFI_PASSWORD_VALUE, 63, wifi_onboarding_service_wifi_password },
    { HDLD_CUSTOM_SERVICE_WIFI_PASSWORD_CHAR_USER_DESCRIPTION,  13, wifi_onboarding_service_wifi_password_char_user_description },
    { HDLC_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_VALUE, 0, wifi_onboarding_service_wifi_ssid_password },
    { HDLD_CUSTOM_SERVICE_WIFI_SSID_PASSWORD_CHAR_USER_DESCRIPTION, 22, wifi_onboarding_service_wifi_ssid_password_char_user_description },
    { HDLC_CUSTOM_SERVICE_WIFI_NETWORKS_VALUE, 0, wifi_onboarding_service_wifi_networks },
    { HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CHAR_USER_DESCRIPTION, 13, wifi_onboarding_service_wifi_networks_char_user_description },
    { HDLD_CUSTOM_SERVICE_WIFI_NETWORKS_CLIENT_CHAR_CONFIG, 2, wifi_onboarding_service_wifi_networks_client_char_config },
    { HDLC_CUSTOM_SERVICE_WIFI_CONTROL_VALUE, 1, wifi_onboarding_service_wifi_control },
    { HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CLIENT_CHAR_CONFIG, 2, wifi_onboarding_service_wifi_control_client_char_config },
    { HDLD_CUSTOM_SERVICE_WIFI_CONTROL_CHAR_USER_DESCRIPTION, 12, wifi_onboarding_service_wifi_control_char_user_description },
};

static wiced_result_t           wifi_onboarding_management_cback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );
wiced_bt_gatt_status_t          wifi_onboarding_gatts_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data);
static void                     wifi_onboarding_set_advertisement_data(void);
static void                     wifi_onboarding_smp_bond_result( uint8_t result );
static void                     wifi_onboarding_encryption_changed( wiced_result_t result, uint8_t* bd_addr );
static void                     wifi_onboarding_application_init( void );
#ifdef ENABLE_HCI_TRACE
static void                     wifi_onboarding_hci_trace_cback( wiced_bt_hci_trace_type_t type, uint16_t length, uint8_t* p_data );
#endif
static void                     wifi_onboarding_load_keys_for_address_resolution( void );

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that Bluetooth device is ready.
 */
APPLICATION_START( )
{
    wiced_transport_init( &transport_cfg );

#ifdef WICED_BT_TRACE_ENABLE
    // Set the debug uart as WICED_ROUTE_DEBUG_NONE to get rid of prints
    // wiced_set_debug_uart(WICED_ROUTE_DEBUG_NONE);

#ifdef NO_PUART_SUPPORT
    // Set to HCI to see traces on HCI uart - default if no call to wiced_set_debug_uart()
    wiced_set_debug_uart( WICED_ROUTE_DEBUG_TO_WICED_UART );
#else
#if defined(CYW43022C1)
    // 43022 transport clock rate is 24Mhz for baud rates <= 1.5 Mbit/sec, and 48Mhz for baud rates > 1.5 Mbit/sec.
    // HCI UART and Debug UART both use the Transport clock, so if the HCI UART rate is <= 1.5 Mbps, please do not set the Debug UART > 1.5 Mbps.
    // The default Debug UART baud rate is 115200, and default HCI UART baud rate is 3Mbps
    debug_uart_set_baudrate(115200);

    // CYW943022M2BTBGA doesn't have GPIO pin for PUART.
    // CYW943022M2BTBGA Debug UART Tx (WICED_GPIO_02) is connected to UART2_RX (ARD_D1) on PUART Level Translators of CYW9BTM2BASE1.
    // We need to set to WICED_ROUTE_DEBUG_TO_DBG_UART to see traces on PUART of CYW9BTM2BASE1.
    wiced_set_debug_uart( WICED_ROUTE_DEBUG_TO_DBG_UART );
#else
    // Set to PUART to see traces on peripheral uart(puart)
    wiced_set_debug_uart( WICED_ROUTE_DEBUG_TO_PUART );
#if defined(CYW20706A2)
    wiced_hal_puart_select_uart_pads( WICED_PUART_RXD, WICED_PUART_TXD, 0, 0);
#endif
#endif
#endif

#ifdef ENABLE_HCI_TRACE
    // Use WICED_ROUTE_DEBUG_TO_WICED_UART to send formatted debug strings over the WICED
    // HCI debug interface to be parsed by ClientControl/BtSpy.
    wiced_set_debug_uart(WICED_ROUTE_DEBUG_TO_WICED_UART);
#endif

#endif //WICED_BT_TRACE_ENABLE

    WICED_BT_TRACE( "WiFi Onboarding Start\n" );

    // Register call back and configuration with stack
#if BTSTACK_VER >= 0x03000001
    /* Create default heap */
    if (wiced_bt_create_heap("default_heap", NULL, BT_STACK_HEAP_SIZE, NULL, WICED_TRUE) == NULL)
    {
        WICED_BT_TRACE("create default heap error: size %d\n", BT_STACK_HEAP_SIZE);
        return;
    }
    wiced_bt_stack_init(wifi_onboarding_management_cback, &wiced_bt_cfg_settings);
#else
    wiced_bt_stack_init(wifi_onboarding_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
#endif
#ifdef COMPONENT_nvram_emulation
    nvram_emulation_mem_init();
#endif
}

/*
 * This function is executed in the BTM_ENABLED_EVT management callback.
 */
void wifi_onboarding_application_init( void )
{
    wiced_result_t         result;

    WICED_BT_TRACE( "wifi_onboarding_application_init\n" );

    // init gatt
    wifi_onboarding_gatt_init();

#ifdef ENABLE_HCI_TRACE
    wiced_bt_dev_register_hci_trace( wifi_onboarding_hci_trace_cback );
#endif

    /* Load previous paired keys for address resolution */
    wifi_onboarding_load_keys_for_address_resolution();

#if defined(CYW20706A2)
    /* Enable privacy to advertise with RPA */
    wiced_bt_ble_enable_privacy ( WICED_TRUE );
#endif
    /* Allow peer to pair */
    wiced_bt_set_pairable_mode(WICED_TRUE, 0);

    /* Set the advertising params and make the device discoverable */
    wifi_onboarding_set_advertisement_data();

    result =  wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL );
    WICED_BT_TRACE( "wiced_bt_start_advertisements %d\n", result );

}

/*
 *  Pass protocol traces up through the UART
 */
#ifdef ENABLE_HCI_TRACE
void wifi_onboarding_hci_trace_cback( wiced_bt_hci_trace_type_t type, uint16_t length, uint8_t* p_data )
{
 // Can write HCI event data to PUART for debug purpose
 // WICED_BT_TRACE( "HCI event type %d len %d\n", type,length );
 // WICED_BT_TRACE_ARRAY( p_data, length, "" );

    //send the trace
 #ifdef NEW_DYNAMIC_MEMORY_INCLUDED
    wiced_transport_send_hci_trace( type, p_data, length );
 #else
    wiced_transport_send_hci_trace( NULL, type, length, p_data );
 #endif
}
#endif

/*
 * Setup advertisement data with 16 byte UUID and device name
 */
void wifi_onboarding_set_advertisement_data(void)
{
    wiced_bt_ble_advert_elem_t adv_elem[3];
    uint8_t num_elem = 0;

    const uint8_t cy_bt_adv_packet_elem_0[1] = { 0x06 };
    const uint8_t cy_bt_adv_packet_elem_1[7] = { 0x62, 0x6C, 0x65, 0x50, 0x72, 0x6F, 0x76 };
    const uint8_t cy_bt_adv_packet_elem_2[16] = { 0x5B, 0x19, 0xBA, 0xE4, 0xE4, 0x52, 0xA9, 0x96, 0xF1, 0x4A, 0x84, 0xC8, 0x09, 0x4D, 0xC0, 0x21 };

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len          = 1;
    adv_elem[num_elem].p_data       = (uint8_t*)cy_bt_adv_packet_elem_0;
    num_elem++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len          = 7;
    adv_elem[num_elem].p_data       = (uint8_t*)cy_bt_adv_packet_elem_1;
    num_elem++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE;
    adv_elem[num_elem].len          = 16;
    adv_elem[num_elem].p_data       = (uint8_t*)cy_bt_adv_packet_elem_2,
    num_elem++;

    wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);
}

/*
 * This function is invoked when advertisements stop.  If we are configured to stay connected,
 * disconnection was caused by the peer, start low advertisements, so that peer can connect
 * when it wakes up
 */
void wifi_onboarding_advertisement_stopped( void )
{
    wiced_result_t result;

    if ( !wifi_onboarding_state.conn_id )
    {
        result =  wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL );
        WICED_BT_TRACE( "wiced_bt_start_advertisements: %d\n", result );
    }
    else
    {
        WICED_BT_TRACE( "ADV stop\n");
    }

    UNUSED_VARIABLE(result);
}

/*
 * Process SMP bonding result. If we successfully paired with the
 * central device, save its BDADDR in the NVRAM and initialize
 * associated data
 */

void wifi_onboarding_smp_bond_result( uint8_t result )
{
    wiced_result_t status;
    uint8_t written_byte = 0;
    WICED_BT_TRACE( "wifi_onboarding, bond result: %d\n", result );

    /* Bonding success */
    if ( result == WICED_BT_SUCCESS )
    {
        /* Pack the data to be stored into the hostinfo structure */
        memcpy( wifi_onboarding_hostinfo.bdaddr, wifi_onboarding_state.remote_addr, sizeof( BD_ADDR ) );

        /* Write to NVRAM */
        /* using single VS_ID to support storing only one paired device */
        /* see README for multiple concurrent pairing support */
        written_byte = wiced_hal_write_nvram( WIFI_ONBOARDING_VS_ID, sizeof(wifi_onboarding_hostinfo), (uint8_t*)&wifi_onboarding_hostinfo, &status );
        WICED_BT_TRACE("NVRAM write: %d\n", written_byte);
    }

    UNUSED_VARIABLE(written_byte);
}


/*
 * Process notification from stack that encryption has been set. If connected
 * client is registered for notification or indication, it is a good time to
 * send it out
 */
void wifi_onboarding_encryption_changed( wiced_result_t result, uint8_t* bd_addr )
{
    WICED_BT_TRACE( "encryp change bd ( %B ) res: %d ", wifi_onboarding_hostinfo.bdaddr,  result);

    /* Connection has been encrypted meaning that we have correct/paired device
     * restore values in the database
     */
    /* using single VS_ID to support storing only one paired device */
    /* see README for multiple concurrent pairing support */
    wiced_hal_read_nvram( WIFI_ONBOARDING_VS_ID, sizeof(wifi_onboarding_hostinfo), (uint8_t*)&wifi_onboarding_hostinfo, &result );
}

/*
 * wifi_onboarding device and link management callback
 */
wiced_result_t wifi_onboarding_management_cback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
    wiced_bt_dev_encryption_status_t    *p_status;
    wiced_bt_dev_ble_pairing_info_t     *p_info;
    wiced_bt_ble_advert_mode_t          *p_mode;
    wiced_bt_device_address_t           id_addr;
    uint8_t                             *p_keys;
    wiced_result_t                      result = WICED_BT_SUCCESS;

    WICED_BT_TRACE("wifi_onboarding_management_cback: 0x%x\n", event );

    switch( event )
    {
        /* Bluetooth  stack enabled */
        case BTM_ENABLED_EVT:
            wifi_onboarding_application_init();
            break;

        case BTM_DISABLED_EVT:
            break;

        case BTM_USER_CONFIRMATION_REQUEST_EVT:
            WICED_BT_TRACE("Numeric_value: %d \n", p_event_data->user_confirmation_request.numeric_value);
            wiced_bt_dev_confirm_req_reply( WICED_BT_SUCCESS , p_event_data->user_confirmation_request.bd_addr);
            break;

        case BTM_PASSKEY_NOTIFICATION_EVT:
            WICED_BT_TRACE("PassKey Notification. BDA %B, Key %d \n", p_event_data->user_passkey_notification.bd_addr, p_event_data->user_passkey_notification.passkey );
            wiced_bt_dev_confirm_req_reply(WICED_BT_SUCCESS, p_event_data->user_passkey_notification.bd_addr );
            break;

        case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
            p_event_data->pairing_io_capabilities_ble_request.local_io_cap      = BTM_IO_CAPABILITIES_NONE;
            p_event_data->pairing_io_capabilities_ble_request.oob_data          = BTM_OOB_NONE;
            p_event_data->pairing_io_capabilities_ble_request.auth_req          = BTM_LE_AUTH_REQ_SC_BOND;
            p_event_data->pairing_io_capabilities_ble_request.max_key_size      = 0x10;
            p_event_data->pairing_io_capabilities_ble_request.init_keys         = BTM_LE_KEY_PENC|BTM_LE_KEY_PID|BTM_LE_KEY_PCSRK|BTM_LE_KEY_LENC;
            p_event_data->pairing_io_capabilities_ble_request.resp_keys         = BTM_LE_KEY_PENC|BTM_LE_KEY_PID|BTM_LE_KEY_PCSRK|BTM_LE_KEY_LENC;
            break;

        case BTM_PAIRING_COMPLETE_EVT:
            p_info =  &p_event_data->pairing_complete.pairing_complete_info.ble;
            WICED_BT_TRACE( "Pairing Complete: %d ", p_info->reason);
            wifi_onboarding_smp_bond_result( p_info->reason );
            break;

        case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
            /* save keys to NVRAM */
            /* using single VS_ID to support storing only one paired device */
            /* see README for multiple concurrent pairing support */
            p_keys = (uint8_t*)&p_event_data->paired_device_link_keys_update;
            wiced_hal_write_nvram ( WIFI_ONBOARDING_PAIRED_KEYS_VS_ID, sizeof( wiced_bt_device_link_keys_t ), p_keys ,&result );
            WICED_BT_TRACE("keys save to NVRAM %B result: %d \n", p_keys, result);
            break;

        case  BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
            {
                wiced_bt_device_link_keys_t link_keys;
                p_keys = (uint8_t *)&p_event_data->paired_device_link_keys_request;
                /* using single VS_ID to support storing only one paired device */
                /* see README for multiple concurrent pairing support */
                wiced_hal_read_nvram( WIFI_ONBOARDING_PAIRED_KEYS_VS_ID,
                                      sizeof(wiced_bt_device_link_keys_t),
                                      (uint8_t *)&link_keys,
                                      &result );

                WICED_BT_TRACE("keys read from NVRAM %B req BDA %B result: %d \n", link_keys.bd_addr, p_event_data->paired_device_link_keys_request.bd_addr, result);

                // Break if link key retrival is failed or link key is not available.
                if (result != WICED_BT_SUCCESS)
                    break;

                // Compare the BDA
                if( memcmp(&(link_keys.bd_addr), &(p_event_data->paired_device_link_keys_request.bd_addr), sizeof(wiced_bt_device_address_t) ) == 0 )
                {
                    memcpy(p_keys, (uint8_t *)&link_keys, sizeof(wiced_bt_device_link_keys_t));
                }
                else
                {
                    result = WICED_BT_ERROR;
                }
            }
            break;

        case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
            /* save keys to NVRAM */
            /* using single VS_ID to support storing only one paired device */
            /* see README for multiple concurrent pairing support */
            p_keys = (uint8_t*)&p_event_data->local_identity_keys_update;
            wiced_hal_write_nvram ( WIFI_ONBOARDING_LOCAL_KEYS_VS_ID, sizeof( wiced_bt_local_identity_keys_t ), p_keys ,&result );
            WICED_BT_TRACE("local keys save to NVRAM result: %d \n", result);
            break;

        case  BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
            /* read keys from NVRAM */
            /* using single VS_ID to support storing only one paired device */
            /* see README for multiple concurrent pairing support */
            p_keys = (uint8_t *)&p_event_data->local_identity_keys_request;
            wiced_hal_read_nvram( WIFI_ONBOARDING_LOCAL_KEYS_VS_ID, sizeof(wiced_bt_local_identity_keys_t), p_keys, &result );
            WICED_BT_TRACE("local keys read from NVRAM result: %d \n",  result);
            break;

        case BTM_ENCRYPTION_STATUS_EVT:
            p_status = &p_event_data->encryption_status;
            WICED_BT_TRACE( "Encryption Status Event: bd ( %B ) res %d", p_status->bd_addr, p_status->result);
            wifi_onboarding_encryption_changed( p_status->result, p_status->bd_addr );
        break;

        case BTM_SECURITY_REQUEST_EVT:
            wiced_bt_ble_security_grant( p_event_data->security_request.bd_addr, WICED_BT_SUCCESS );
            break;

        case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
            p_mode = &p_event_data->ble_advert_state_changed;
            WICED_BT_TRACE( "Advertisement State Change: %d\n", *p_mode);
            if ( *p_mode == BTM_BLE_ADVERT_OFF )
            {
                wifi_onboarding_advertisement_stopped();
            }
            break;

    default:
            break;
    }

    return result;
}

/*
 * Find attribute description by handle
 */
attribute_t * wifi_onboarding_get_attribute( uint16_t handle )
{
    uint32_t i;
    for ( i = 0; i <  sizeof( gauAttributes ) / sizeof( gauAttributes[0] ); i++ )
    {
        if ( gauAttributes[i].handle == handle )
        {
            return ( &gauAttributes[i] );
        }
    }
    WICED_BT_TRACE( "attr not found:%x\n", handle );
    return NULL;
}

/* This function is invoked when connection is established */
wiced_bt_gatt_status_t wifi_onboarding_gatts_connection_up( wiced_bt_gatt_connection_status_t *p_status )
{
    wiced_result_t result;

    WICED_BT_TRACE( "wifi_onboarding_conn_up %B id:%d\n:", p_status->bd_addr, p_status->conn_id);

    /* Update the connection handler.  Save address of the connected device. */
    wifi_onboarding_state.conn_id = p_status->conn_id;
    memcpy(wifi_onboarding_state.remote_addr, p_status->bd_addr, sizeof(BD_ADDR));

#if BTSTACK_VER >= 0x03000001
    /*Configure MTU to be able to send notification packets larger than 23 bytes*/
    wiced_bt_gatt_client_configure_mtu(wifi_onboarding_state.conn_id, CY_BT_RX_PDU_SIZE);
#endif

    /* Saving host info in NVRAM  */
    memcpy( wifi_onboarding_hostinfo.bdaddr, p_status->bd_addr, sizeof( BD_ADDR ) );
    wifi_onboarding_hostinfo.characteristic_client_configuration = 0;

    {
        uint8_t bytes_written = 0;
        /* using single VS_ID to support storing only one paired device */
        /* see README for multiple concurrent pairing support */
        bytes_written = wiced_hal_write_nvram( WIFI_ONBOARDING_VS_ID, sizeof(wifi_onboarding_hostinfo), (uint8_t*)&wifi_onboarding_hostinfo, &result );

        WICED_BT_TRACE("NVRAM write %d\n", bytes_written);
        UNUSED_VARIABLE(bytes_written);
    }

    return WICED_BT_GATT_SUCCESS;
}

/*
 * This function is invoked when connection is lost
 */
wiced_bt_gatt_status_t wifi_onboarding_gatts_connection_down( wiced_bt_gatt_connection_status_t *p_status )
{
    wiced_result_t result;

    WICED_BT_TRACE( "connection_down %B conn_id:%d reason:%d\n", wifi_onboarding_state.remote_addr, p_status->conn_id, p_status->reason );

    /* Resetting the device info */
    memset( wifi_onboarding_state.remote_addr, 0, 6 );
    wifi_onboarding_state.conn_id = 0;

    /* Set the advertising params and make the device discoverable */
    wifi_onboarding_set_advertisement_data();

    result =  wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL );
    WICED_BT_TRACE( "wiced_bt_start_advertisements %d\n", result );

    return WICED_BT_SUCCESS;
}

/*
 * Connection up/down event
 */
wiced_bt_gatt_status_t wifi_onboarding_gatts_conn_status_cb( wiced_bt_gatt_connection_status_t *p_status )
{
    if ( p_status->connected )
    {
        return wifi_onboarding_gatts_connection_up( p_status );
    }

    return wifi_onboarding_gatts_connection_down( p_status );
}

static void wifi_onboarding_load_keys_for_address_resolution( void )
{
    wiced_bt_device_link_keys_t link_keys;
    wiced_result_t              result;
    uint8_t                     *p;

    memset( &link_keys, 0, sizeof(wiced_bt_device_link_keys_t));
    p = (uint8_t*)&link_keys;
    /* using single VS_ID to support storing only one paired device */
    /* see README for multiple concurrent pairing support */
    wiced_hal_read_nvram( WIFI_ONBOARDING_PAIRED_KEYS_VS_ID, sizeof(wiced_bt_device_link_keys_t), p, &result);

    if(result == WICED_BT_SUCCESS)
    {
#ifdef CYW20706A2
        result = wiced_bt_dev_add_device_to_address_resolution_db ( &link_keys, link_keys.key_data.ble_addr_type );
#else
        result = wiced_bt_dev_add_device_to_address_resolution_db ( &link_keys );
#endif
    }
    WICED_BT_TRACE("wifi_onboarding_load_keys_for_address_resolution %B result:%d \n", p, result );
}

/*
 * hci_control_transport_status.
 * Called when Transport is ready to tell host that device is started
 */
#if defined(COMPONENT_nvram_emulation) && NVRAM_EMULATION_HCI

void  wifi_onboarding_transport_status( wiced_transport_type_t type )
{
    WICED_BT_TRACE( "[%s] transport %x \n", __FUNCTION__, type );
    nvram_emulation_transport_status_handler(WICED_TRANSPORT_UART);
}


/*
 * hci_control_tx_complete
 * This function is called when a Transport Buffer has been sent to the MCU
 */
void  wifi_onboarding_tx_complete(wiced_transport_buffer_pool_t* p_pool)
{
    WICED_BT_TRACE( "[%s] pool 0x%x \n", __FUNCTION__, p_pool );
    /* no call to nvram emulation needed */
}

/*
 * Handle received command over UART.
 * Please refer to https://infineon.github.io/btsdk-docs/BT-SDK/AIROC-HCI-Control-Protocol.pdf
 * for details on the HCI UART control protocol.
*/
uint32_t wifi_onboarding_process_rx( uint8_t *p_buffer, uint32_t length )
{
    uint16_t opcode;
    uint32_t i, result;
    uint8_t  tx_buf[15];
    uint8_t  cmd = 0;
    uint32_t chip = CHIP;

    WICED_BT_TRACE( "[%s] %d bytes: ", __FUNCTION__, length);
    for(i = 0; i < length; i++)
    {
        WICED_BT_TRACE( "%02x ", p_buffer[i]);
    }
    WICED_BT_TRACE( "\n");

    /* pass received packets to nvram emulation for handling */
    result = nvram_emulation_transport_rx_data_handler(p_buffer, length);

    /* packets processed by nvram_emulation are freed and return value is HCI_CONTROL_STATUS_SUCCESS */
    /* otherwise, more processing is needed */
    if(result != HCI_CONTROL_STATUS_SUCCESS)
    {
        if(length >= 4)
        {
            opcode = p_buffer[0] + ( p_buffer[1] << 8 );
            if(opcode == HCI_CONTROL_MISC_COMMAND_GET_VERSION)
            {
                tx_buf[cmd++] = WICED_SDK_MAJOR_VER;
                tx_buf[cmd++] = WICED_SDK_MINOR_VER;
                tx_buf[cmd++] = WICED_SDK_REV_NUMBER;
                tx_buf[cmd++] = WICED_SDK_BUILD_NUMBER & 0xFF;
                tx_buf[cmd++] = (WICED_SDK_BUILD_NUMBER>>8) & 0xFF;
                tx_buf[cmd++] = chip & 0xFF;
                tx_buf[cmd++] = (chip>>8) & 0xFF;
                tx_buf[cmd++] = (chip>>24) & 0xFF;
                tx_buf[cmd++] = 0; // not used

                /* Send MCU app the supported features */
                tx_buf[cmd++] = HCI_CONTROL_GROUP_DEVICE;

                wiced_transport_send_data( HCI_CONTROL_MISC_EVENT_VERSION, tx_buf, cmd );
            }
        }
        // Freeing the buffer in which data is received
        wiced_transport_free_buffer(p_buffer);
    }
    return result;
}
#endif

void wifi_onboarding_transport_status( wiced_transport_type_t type )
{
     WICED_BT_TRACE("onboarding_transport_status\n");
}

void wifi_onboarding_send_command_status_evt( uint16_t code, uint8_t status )
{
    wiced_transport_send_data( code, &status, 1 );
}

void wifi_onboarding_build_and_send_Onboarding_event(uint16_t code, uint8_t* val, uint16_t len, uint16_t handle)
{
    uint8_t buf[1024]={0,};
    uint16_t buf_len = 0;

    WICED_BT_TRACE("hci send event code=%x\n",code);
    if(code == HCI_CONTROL_WIFI_ONBOARDING_EVENT_WRITE_REQUEST)
    {
        buf[0] = (uint8_t) handle;
        buf[1] = (uint8_t) handle >> 8;
        memcpy(&buf[2], val, len);
        buf_len = len + 2;
    }
    else
    {
        memcpy(buf, val, len);
        buf_len = len;
    }

    wiced_transport_send_data( code, buf, buf_len );
}

uint32_t wifi_onboarding_proc_rx_cmd(uint8_t *p_buffer, uint32_t length)
{
    uint16_t                opcode;
    uint8_t*                p_data = p_buffer;
    uint16_t                payload_len;
    uint8_t                 status = HCI_CONTROL_STATUS_SUCCESS;

    WICED_BT_TRACE("wifi_onboarding_proc_rx_cmd:%d\n", length);

    if (!p_data)
    {
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    //Expected minimum 4 byte as the header
    if (length < 4)
    {
	WICED_BT_TRACE("invalid params\n");
        wiced_transport_free_buffer(p_data);
        return HCI_CONTROL_STATUS_INVALID_ARGS;
    }

    STREAM_TO_UINT16(opcode, p_data);       // Get OpCode
    STREAM_TO_UINT16(payload_len, p_data);  // Gen Payload Length

    switch (opcode)
    {
	case HCI_CONTROL_WIFI_ONBOARDING_COMMAND_SEND_NOTIFICATION:
	    wifi_onboarding_gatts_send_notification(p_data, payload_len, wifi_onboarding_state.conn_id);
		break;

	case HCI_CONTROL_WIFI_ONBOARDING_COMMAND_SET_SSID:
		memset(wifi_onboarding_service_wifi_ssid, 0, sizeof(wifi_onboarding_service_wifi_ssid));
		memcpy(wifi_onboarding_service_wifi_ssid, p_data, payload_len);
		WICED_BT_TRACE("Set SSID %s\n",wifi_onboarding_service_wifi_ssid);
		break;

        default:
		WICED_BT_TRACE("ignored opcode:0x%04x\n", opcode);
            status = HCI_CONTROL_STATUS_UNKNOWN_COMMAND;
            break;
    }

    return status;
}
