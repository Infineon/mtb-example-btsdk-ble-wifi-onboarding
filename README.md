# AIROC™ BTSTACK: Bluetooth® Wi-Fi Onboarding Embedded

This code example demonstrates the Bluetooth® Wi-Fi Onboarding embedded using the AIROC™ Wi-Fi & Bluetooth® combo chip with Arm® Cortex®-A Class processors.

[View this README on GitHub.](https://github.com/Infineon/mtb-example-btsdk-ble-wifi-onboarding)

[Provide feedback on this code example.](replace_code_example_voc_survey_url)

# Requirements

- Programming language: C
- Embedded Linux Host platforms (Arm® Cortex®-A Class processors). Supported Embedded Linux host platforms:
   1. WiFi enabled host. i.e) RPI CM4-lite with custom HW base board from Infineon
- ModusToolbox 3.2 or later


## Supported AIROC™ Wi-Fi & Bluetooth® combo chip

- [AIROC™ CYW5557x Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-6-6e-802.11ax/)
- [AIROC™ CYW43012 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-5-802.11ac/cyw43022/)

## Hardware setup

Set up the hardware according to the following diagram:

**Figure 1. Block diagram: Hardware setup**

![](images/blockdiagram.png)

## Software setup

1. Instal Modus Toolbox 3.2 or later

2. For the Bluetooth&reg; LE GATT Client, download and install **nRF Connect** on mobile phone using the following:
   - https://apps.apple.com/tw/app/nrf-connect-for-mobile/id1054362403
   - https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=zh_TW&gl=US.

   **Or**

   - Scan the following QR codes from your mobile phone to download the app.

   **Figure 2. Application QR code**

   - App store [iOS]:

   ![](images/ios_nrf_connect.png)

   - Play store [Android]:

   ![](images/nrf_connect.png)

   ## Additional settings on Android
   - Enlarge MTU size for receiving data with bigger size. eg: Notification status: "Connected:\<ip address>"

      1. Open the menu on the top left corner.

         <img src="./images/mtu_open_menu.png" width="40%" height="40%"/>

      2. Press "Settings".

         <img src="./images/mtu_settings.png" width="40%" height="40%"/>

      3. Press "Connectivity".

         <img src="./images/mtu_connectivity.png" width="40%" height="40%"/>

      4. Enable "Auto request maximum MTU".

         <img src="./images/mtu_request_maximum.png" width="40%" height="40%"/>

      5. Now you should be able to receive longer data such as ip address etc.

## Using the code example

Do the following on Modus Toolbox to compile the code example:

1. Create mtb-example-btsdk-ble-wifi-onboarding app and built.

2. Copy build/APP_CYWxxx/Debug/LE_WiFi-Onboarding_download.hcd to host


# Operation

### Operation procedure

1. Run runh2.sh or run43022.sh in hostexample
   ```bash
   For 5557x
   sudo ./runh2.sh LE_WiFi-Onboarding_download.hcd
   For 43022
   sudo ./run43022.sh LE_WiFi-Onboarding_download.hcd
   ```

2. To test, using the **nRF Connect** mobile app:
   **Note:** Refer to [GATT interface service and chanracteristics detailes](#gatt-interface-and-features) for more details on choosing proper service and chanracteristics.

   [For example, the following pictures use the nRF Connect on iOS]
   1. Turn ON Bluetooth® on your phone.
   2. Launch the nRF Connect app.
   3. Swipe down the nRF Connect app home screen to start scanning for Bluetooth® LE Peripherals. Your device (“bleProv”) appears on nRF Connect app home screen. Select your device to establish a Bluetooth® LE connection.

       **Figure 4. nRF Connect app flow**

      <img src="./images/bleprov.png" width="40%" height="40%"/>

   4. Select service UUID:21C04D09-xxxx, then:
      1. Select characteristic **1E500043-6B31-4A3D-B91E-025F92CA9763**.
         1. Click upward arrow.
         2. Choose **UTF8** format.
         3. Enter your **Wi-Fi SSID** in a human readable string (for example, "YOURSSID").

            **Figure 5. nRF Connect app flow**

            <img src="./images/ssid.png" width="40%" height="40%"/> <img src="./images/enter_ssid.png" width="40%" height="40%"/>

      2. Select characteristic **1E500043-6B31-4A3D-B91E-025F92CA9764**.
         1. Click upward arrow.
         2. Choose **UTF8** format.
         3. Enter your **Wi-Fi PASSWORD** in a human readable string.

            **Figure 6. nRF Connect app flow**

            <img src="./images/password.png" width="40%" height="40%"/> <img src="./images/enter_password.png" width="40%" height="40%"/>

      3. [Optional] Select characteristic **1E500043-6B31-4A3D-B91E-025F92CA9765**.

         1. Click upward arrow.
         2. Choose **ByteArray** format.
         3. Enter your **Wi-Fi SSID and PASSWORD** in **TLV** format.
         For example:
            - Enter following bytes in hex: 0x01 08 57 49 46 49 53 53 49 44 02 08 50 41 53 53 57 4F 52 44
               - **0x01**: The following data is SSID
               - **0x08**: There are eight bytes following
               - 0x57 49 46 49 53 53 49 44: data value, in this case is "WIFISSID"
               - **0x02**: The following data is PASSWORD
               - **0x08**: There are eight bytes following
               - 0x50 41 53 53 57 4F 52 44: Data value, in this case is "PASSWORD"

            **Figure 7. nRF Connect app flow (0x44 is in the end of ByteArray)**

            <img src="./images/ssid_and_password.png" width="30%" height="30%"/> <img src="./images/enter_ssid_and_password.png" width="30%" height="30%"/>

            You will see the following on your console.
            ```
            Current Wi-Fi SSID: WIFISSID
            Current Wi-Fi Password: PASSWORD
            ```

      4. [Optional] Subscribe characteristic **1E500043-6B31-4A3D-B91E-025F92CA9766** for starting the Wi-Fi status monitoring and enabling the notification of both Wi-Fi status and Wi-Fi scan results.

         1. Click downward triple arrows to enable the notification and start Wi-Fi monitoring.

            **Figure 8. nRF Connect app flow**

            <img src="./images/subscibe_notification.png" width="40%" height="40%"/>


      5. Select characteristic **1E500043-6B31-4A3D-B91E-025F92CA9767**.

         1. Click upward arrow.
         2. Choose **ByteArray** format.
         3. Enter Wi-Fi control command.

            <img src="./images/wifi_control.png" width="40%" height="40%"/>

            - **0x00**: Disconnect from Wi-Fi access point
            <img src="./images/wifi_disconnect.png" width="40%" height="40%"/>
            <img src="./images/console_disconnected.png" alt="drawing" style="width:100%;"/>

            - **0x01**: Connect to Wi-Fi access point using the saved SSID and PASSWORD
            <img src="./images/wifi_connect.png" width="40%" height="40%"/>
            <img src="./images/console_connected.png" alt="drawing" style="width:60%;"/>

            - **0x02**: [Optional] Wi-Fi scan to get Wi-Fi SSIDs around the target device
            <img src="./images/wifi_scan.png" width="40%" height="40%"/>
            <img src="./images/console_scan_results.png" alt="drawing" style="width:100%;"/>

            - **0x03**: [Optional] Establish Wi-Fi connection with particular scanned index from scan results,notice that the index has already been displayed in hex format.
            For example:
            **0x03**: connect command
            **0x24**: [**hex**] particular index corresponding to SSID that we want to extablish Wi-Fi connection with.
            <img src="./images/console_connect_to_scanned_index.png" alt="drawing" style="width:100%;"/>
            <img src="./images/wifi_connect_scanned_index.png" width="40%" height="40%"/>

      6. Showcase the Infineon home page accessed through Wi-Fi connection established by scanned index: 0x42.
            <img src="./images/infineon_web_page.jpg" alt="drawing" style="width:100%;"/>


## Design and implementation

**Roles implemented:**

- **CE code example (implementation under test):** GATT Server and GAP Peripheral role

- **Testing mobile device with LightBlue app/ nRF Connect app:** GATT Client and GAP Central role


## GATT interface and features

This code example demonstrates how to establish a connection with a **WPA2** or **OPEN** Wi-Fi access point.
* Service UUID: **21C04D09-xxxx**
   - characteristic **1E500043-6B31-4A3D-B91E-025F92CA9763**
      - It is used to input **SSID**
   - characteristic **1E500043-6B31-4A3D-B91E-025F92CA9764**
      - It is used to input **PASSWORD**
   - characteristic **1E500043-6B31-4A3D-B91E-025F92CA9765**
      - input SSID and PASSWORD in TLV format
         - TLV stands for "Type" "Length" "Value"
            - Type: The type of following data
               - **0x01**: SSID
               - **0x02**: PASSWORD
            - Length: The length of following data
            - Value: Data in bytes
         - Example:
            - input: **0x01** **08** 57 49 46 49 53 53 49 44 **02** **08** 50 41 53 53 57 4F 52 44
            - **0x01**: The following data is SSID
            - **0x08**: There are eight bytes following
            - 0x57 49 46 49 53 53 49 44: Data value, in this case is "WIFISSID"
            - **0x02**: The following data is PASSWORD
            - **0x08**: There are eight bytes following
            - 0x50 41 53 53 57 4F 52 44: Data value, in this case is "PASSWORD"

   - characteristic **1E500043-6B31-4A3D-B91E-025F92CA9766**
      - Subscribe for monitoring Wi-Fi status and reveiving notification
         - Wi-Fi status will be one of the following:
            - **connected**: Provides ipv4 if it has connected to Wi-Fi access point
            - **disconnected**
         - Wi-Fi scan results in TLV format:
            - Type: The type of following data
               - **0x01**: SSID
               - **0x02**: Security **tag**
                  - **tag** is one of the following:
                     - **WPA2**: 0x00 0x00 0x40 0x00
                     - **Default**: 0xFF 0xFF 0xFF 0xFF
            - Length: The length of following data
            - Value: Data in bytes
            - Example:
               - Receive: **0x01** **08** 57 49 46 49 53 53 49 44 **02** **04** 00 00 40 00
               - **0x01**: The following data is SSID
               - **0x08**: There are eight bytes following
               - 0x57 49 46 49 53 53 49 44: data value, in this case is "WIFISSID"
               - **0x02**: The following data is security tag
               - **0x04**: There are four bytes following
               - 0x00 00 40 00: means it has WPA2
   - characteristic **1E500043-6B31-4A3D-B91E-025F92CA9767**
      - input control command which is one of following
         - **0x00**: Disconnect from Wi-Fi access point
         - **0x01**: Connect to Wi-Fi access point using the saved SSID and PASSWORD
         - **0x02**: Wi-Fi scan to get Wi-Fi SSIDs around the target device
            - Example:
               index / tag / **SSID**
               1.  [WPA2]   **CityFree**
               2.  [WPA2]   **friends54377**
               3.  [others]   **ssidking**
         - **0x03**: connect to a corresponding scanned index displayed in the command 0x02's log
            - Example: Send **0x03** **0x03** to establish a connection with index **0x03** which SSID is "ssidking" by previously saved password.


## Resources and settings

This section explains the Bluetooth® Configurator software resources and their configuration as used in this code example. Note that all the configuration explained in this section has already been done in the code example.

**Bluetooth® Configurator:** The Bluetooth® peripheral has an additional configurator called the “Bluetooth® Configurator” that is used to generate the Bluetooth® LE GATT database and various Bluetooth® settings for the application.

<br />

## Related resources

Resources  | Links
-----------|----------------------------------
Device documentation | [AIROC™ CYW5557x Wi-Fi 6E tri-band Wi-Fi and Bluetooth® 5.2 SoC](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/cyw5557x/) <br> [AIROC™ CYW54591 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-5-802.11ac/cyw54591/) <br> [AIROC™ CYW43439 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-4-802.11n/cyw43439/) <br> [AIROC™ CYW43012 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-4-802.11n/cyw43012/) <br> [AIROC™ CYW4373 Wi-Fi & Bluetooth® combo chip](https://www.infineon.com/cms/en/product/wireless-connectivity/airoc-wi-fi-plus-bluetooth-combos/wi-fi-5-802.11ac/cyw4373/)
Libraries and middleware | [AIROC™ BTSTACK library](https://github.com/Infineon/btstack/tree/release-v3.8.2) <br> [BT Audio profile layer](https://github.com/Infineon/bt-audio-profiles/tree/release-v1.0.0) <br> [Linux porting layer source code](https://github.com/Infineon/bluetooth-linux)

## Other resources

Infineon provides a wealth of data at www.infineon.com to help you select the right device, and quickly and effectively integrate it into your design.

## Document history

Document title: *CE236728* – *AIROC™ BTSTACK: Bluetooth® Wi-Fi Onboarding for Linux host*

 Version | Description of change
 ------- | ---------------------
 1.0.0   | New code example

<br />

---------------------------------------------------------

© Cypress Semiconductor Corporation, 2023. This document is the property of Cypress Semiconductor Corporation, an Infineon Technologies company, and its affiliates ("Cypress").  This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide.  Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights.  If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress’s patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products.  Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.
<br />
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  No computing device can be absolutely secure.  Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach").  Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach.  In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes.  It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product.  "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage.  Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices.  "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness.  Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, including its affiliates, and its directors, officers, employees, agents, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress’s published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.
<br />
Cypress, the Cypress logo, and combinations thereof, WICED, ModusToolbox, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress or a subsidiary of Cypress in the United States or in other countries. For a more complete list of Cypress trademarks, visit www.infineon.com. Other names and brands may be claimed as property of their respective owners.

## BTSTACK version

BTSDK AIROC&#8482; chips contain the embedded AIROC&#8482; Bluetooth&#174; stack, BTSTACK. Different chips use different versions of BTSTACK, so some assets may contain variant sets of files targeting the different versions in COMPONENT\_btstack\_vX (where X is the stack version). Applications automatically include the appropriate folder using the COMPONENTS make variable mechanism, and all BSPs declare which stack version should be used in the BSP .mk file, with a declaration such as:<br>
> COMPONENTS+=btstack\_v1<br>
or:<br>
> COMPONENTS+=btstack\_v3

## Common application settings

Application settings below are common for all BTSDK applications and can be configured via the makefile of the application or passed in via the command line.

##### BT\_DEVICE\_ADDRESS
> Set the BDA (Bluetooth&#174; Device Address) for your device. The address is 6 bytes, for example, 20819A10FFEE. By default, the SDK will set a BDA for your device by combining the 7 hex digit device ID with the last 5 hex digits of the host PC MAC address.

##### UART
> Set to the UART port you want to use to download the application. For example 'COM6' on Windows or '/dev/ttyWICED\_HCI\_UART0' on Linux or '/dev/tty.usbserial-000154' on macOS. By default, the SDK will auto-detect the port.

##### ENABLE_DEBUG
> For HW debugging, configure ENABLE\_DEBUG=1. See the document [AIROC&#8482;-Hardware-Debugging](https://infineon.github.io/btsdk-docs/BT-SDK/AIROC-Hardware-Debugging.pdf) for more information. This setting configures GPIO for SWD.<br>
>
   - CYW920819EVB-02/CYW920820EVB-02: SWD signals are shared with D4 and D5, see SW9 in schematics.
   - CYBT-213043-EVAL/CYBT-253059-EVAL: SWD signals are routed to P12=SWDCK and P13=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-223058-EVAL/CYW920835M2EVB-01/CYBT-243053-EVAL/CYBLE-343072-EVAL-M2B/CYBLE-333074-EVAL-M2B/CYBLE-343072-MESH/Vela-IF820-INT-ANT-DVK/Vela-IF820-EXT-ANT-DVK: SWD signals are routed to P02=SWDCK and P03=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-263065-EVAL/CYBT-273063-EVAL: SWD signals are routed to P02=SWDCK and P04=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-343026-EVAL/CYBT-353027-EVAL/CYBT-333047-EVAL: SWD signals are routed to P11=SWDCK and P15=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYBT-413055-EVAL/CYBT-413061-EVAL: SWD signals are routed to P16=SWDCK and P17=SWDIO. Use expansion connectors to connect VDD, GND, SWDCK, and SWDIO to your SWD Debugger probe.
   - CYW989820EVB-01: SWDCK (P02) is routed to the J13 DEBUG connector, but not SWDIO. Add a wire from J10 pin 3 (PUART CTS) to J13 pin 2 to connect GPIO P10 to SWDIO.
   - CYW920719B2Q40EVB-01: PUART RX/TX signals are shared with SWDCK and SWDIO. Remove RX and TX jumpers on J10 when using SWD. PUART and SWD cannot be used simultaneously on this board unless these pins are changed from the default configuration.
   - CYW920721M2EVK-02/CYW920721M2EVB-03: The default setup uses P03 for SWDIO and P05 for SWDCK. Check the position of SW15 if using JLink with the DEBUG connector.
   - CYW920706WCDEVAL: SWD debugging requires fly-wire connections. The default setup P15 (J22 pin 3 or J24 pin 1) for SWDIO and P11 (J23 pin 5
    or J22 pin 4) for SWDCK.
   - CYW920736M2EVB-01: SWD hardware debugging requires fly-wire connections. The only option is using P14 for SWDCK and P15 for SWDIO. These route to Arduino header J2, A1 and A0. These can be fly-wired to Arduino header J4, D4 and D5. From there the signals connect to the KitProg3 SWD bridge. In addition, the debug macros (SETUP\_APP\_FOR\_DEBUG\_IF\_DEBUG\_ENABLED and BUSY\_WAIT\_TILL\_MANUAL\_CONTINUE\_IF\_DEBUG\_ENABLED) are placed in sparinit.c in code common to all applications for this device. Most applications for this device call bleprofile\_GPIOInit() in subsequent code, overwriting the SWD pin configuration. To use hardware debugging after the call to bleprofile\_GPIOInit(), place the debug macros in code after that call.
   - CYW943012B2EVK-01: SWD signals are shared with D4 and D5.
   - CYW920820M2EVB-01 & CYW920819M2EVB-01: The default setup uses P03 for SWDIO and P02 for SWDCK. Check the position of SW15 if using JLink with the DEBUG connector.
   - CYW989820M2EVB-01: SWD hardware debugging requires a fly-wire connection to use P14 for SWDIO. P2 is connected directly to SWDCK / ARD_D4. Fly-wire P14 / ARD_D8 on J3.10 to J4.3 / ARD_D5 to connect SWDIO.

   - SWD hardware debugging is not supported on the following:
      - CYW920721M2EVK-01
      - CYW920835REF-RCU-01
      - CYW9M2BASE-43012BT
      - CYBT-423054-EVAL
      - CYBT-423060-EVAL
      - CYBT-483056-EVAL
      - CYBT-483062-EVAL
      - CYW955572BTEVK-01
      - CYW943022BTEVK-01

##### DIRECT_LOAD
> BTSDK chips support downloading applications either to FLASH storage or to RAM storage. Some chips support only one or the other, and some chips support both.

> If a chip only supports one or the other, this variable is not applicable, applications will be downloaded to the appropriate storage supported by the device.

> If a chip supports both FLASH and RAM downloads, the default is to download to FLASH, and the DIRECT_LOAD make variable may be set to 1 in the application makefile (or in the command line make command) to override the default and download to RAM.

> Currently, the following chips support both FLASH and RAM download and can set DIRECT_LOAD=1 if desired:
>
   - CYW20835
   - CYW20706

## Building and downloading code examples

**Using the ModusToolbox&#8482; Eclipse IDE**

1. Install ModusToolbox&#8482; 2.4.1 (or higher).
2. In the ModusToolbox&#8482; Eclipse IDE, click the **New Application** link in the Quick Panel (or, use **File > New > ModusToolbox IDE Application**).
3. Pick your board for BTSDK under AIROC&#8482; Bluetooth&#174; BSPs.
4. Select the application in the IDE.
5. In the Quick Panel, select **Build** to build the application.
6. To program the board (download the application), select **Program** in the Launches section of the Quick Panel.

**Using command line**

1. Install ModusToolbox&#8482; 2.4.1 (or higher).
2. On Windows, use Cygwin from \ModusToolbox\tools_2.x\modus-shell\Cygwin.bat to build apps.
3. Use the tool 'project-creator-cli' under \ModusToolbox\tools_2.x\project-creator\ to create your application.<br/>
   > project-creator-cli --board-id (BSP) --app-id (appid) -d (dir) <br/>
   See 'project-creator-cli --help' for useful options to list all available BSPs, and all available apps per BSP.<br/>
   For example:<br/>
   > project-creator-cli --app-id mtb-example-btsdk-empty --board-id CYW920706WCDEVAL -d .<br/>
4. To build the app call make build. For example:<br/>
   > cd mtb-examples-btsdk-empty<br/>
   > make build<br/>
5. To program (download to) the board, call:<br/>
   > make qprogram<br/>
6. To build and program (download to) the board, call:<br/>
   > make program<br/><br>
   Note: make program = make build + make qprogram

If you have issues downloading to the board, follow the steps below:

- Press and hold the 'Recover' button on the board.
- Press and hold the 'Reset' button on the board.
- Release the 'Reset' button.
- After one second, release the 'Recover' button.

Note: this is only applicable to boards that download application images to FLASH storage. Boards that only support RAM download (DIRECT_LOAD) such as CYW9M2BASE-43012BT or CYW943022BTEVK-01 can be power cycled to boot from ROM.

## Over The Air (OTA) Firmware Upgrade
Applications that support OTA upgrade can be updated via the peer OTA app in:<br>
>\<Workspace Dir>\mtb\_shared\wiced\_btsdk\tools\btsdk-peer-apps-ota<br>

See the readme.txt file located in the above folder for instructions.<br>
To generate the OTA image for the app, configure OTA\_FW\_UPGRADE=1 in the app
makefile, or append OTA\_FW\_UPGRADE=1 to a build command line, for example:
> make PLATFORM=CYW920706WCDEVAL OTA\_FW\_UPGRADE=1 build<br>

This will the generate \<app>.bin file in the 'build' folder.

## SDK software features

- Dual-mode Bluetooth&#174; stack included in the ROM (BR/EDR and LE)
- Bluetooth&#174; stack and profile level APIs for embedded Bluetooth&#174; application development
- AIROC&#8482; HCI protocol to simplify host/MCU application development
- APIs and drivers to access on-board peripherals
- Bluetooth&#174; protocols include GAP, GATT, SMP, RFCOMM, SDP, AVDT/AVCT, LE Mesh
- LE and BR/EDR profile APIs, libraries, and sample apps
- Support for Over-The-Air (OTA) upgrade
- Device Configurator for creating custom pin mapping
- Bluetooth&#174; Configurator for creating LE GATT Database
- Peer apps based on Android, iOS, Windows, etc. for testing and reference
- Utilities for protocol tracing, manufacturing testing, etc.
- Documentation for APIs, datasheets, profiles, and features
- BR/EDR profiles: A2DP, AVRCP, HFP, HSP, HID, SPP, MAP, PBAP, OPP
- LE profiles: Mesh profiles, HOGP, ANP, BAP, HRP, FMP, IAS, ESP, LE COC
- Apple support: Apple Media Service (AMS), Apple Notification Center Service (ANCS), iBeacon, Homekit, iAP2
- Google support: Google Fast Pair Service (GFPS), Eddystone
- Amazon support: Alexa Mobile Accessories (AMA)

Note: this is a list of all features and profiles supported in BTSDK, but some AIROC&#8482; devices may only support a subset of this list.

## List of boards available for use with BTSDK

- [CYW20819A1 chip](https://github.com/Infineon/20819A1)
    - [CYW920819EVB-02](https://github.com/Infineon/TARGET_CYW920819EVB-02), [CYW920819M2EVB-01](https://github.com/Infineon/TARGET_CYW920819M2EVB-01), [CYBT-213043-EVAL](https://github.com/Infineon/TARGET_CYBT-213043-EVAL), [CYBT-223058-EVAL](https://github.com/Infineon/TARGET_CYBT-223058-EVAL), [CYBT-263065-EVAL](https://github.com/Infineon/TARGET_CYBT-263065-EVAL), [CYBT-273063-EVAL](https://github.com/Infineon/TARGET_CYBT-273063-EVAL)
- [CYW20820A1 chip](https://github.com/Infineon/20820A1)
    - [CYW920820EVB-02](https://github.com/Infineon/TARGET_CYW920820EVB-02), [CYW989820M2EVB-01](https://github.com/Infineon/TARGET_CYW989820M2EVB-01), [CYW989820EVB-01](https://github.com/Infineon/TARGET_CYW989820EVB-01), [CYBT-243053-EVAL](https://github.com/Infineon/TARGET_CYBT-243053-EVAL), [CYBT-253059-EVAL](https://github.com/Infineon/TARGET_CYBT-253059-EVAL), [CYW920820M2EVB-01](https://github.com/Infineon/TARGET_CYW920820M2EVB-01), [Vela-IF820-INT-ANT-DVK](https://github.com/Infineon/TARGET_Vela-IF820-INT-ANT-DVK), [Vela-IF820-EXT-ANT-DVK](https://github.com/Infineon/TARGET_Vela-IF820-EXT-ANT-DVK)
- [CYW20721B2 chip](https://github.com/Infineon/20721B2)
    - [CYW920721M2EVK-01](https://github.com/Infineon/TARGET_CYW920721M2EVK-01), [CYW920721M2EVK-02](https://github.com/Infineon/TARGET_CYW920721M2EVK-02), [CYW920721M2EVB-03](https://github.com/Infineon/TARGET_CYW920721M2EVB-03), [CYBT-423060-EVAL](https://github.com/Infineon/TARGET_CYBT-423060-EVAL), [CYBT-483062-EVAL](https://github.com/Infineon/TARGET_CYBT-483062-EVAL), [CYBT-413061-EVAL](https://github.com/Infineon/TARGET_CYBT-413061-EVAL)
- [CYW20719B2 chip](https://github.com/Infineon/20719B2)
    - [CYW920719B2Q40EVB-01](https://github.com/Infineon/TARGET_CYW920719B2Q40EVB-01), [CYBT-423054-EVAL](https://github.com/Infineon/TARGET_CYBT-423054-EVAL), [CYBT-413055-EVAL](https://github.com/Infineon/TARGET_CYBT-413055-EVAL), [CYBT-483056-EVAL](https://github.com/Infineon/TARGET_CYBT-483056-EVAL)
- [CYW20706A2 chip](https://github.com/Infineon/20706A2)
    - [CYW920706WCDEVAL](https://github.com/Infineon/TARGET_CYW920706WCDEVAL), [CYBT-353027-EVAL](https://github.com/Infineon/TARGET_CYBT-353027-EVAL), [CYBT-343026-EVAL](https://github.com/Infineon/TARGET_CYBT-343026-EVAL), [CYBT-333047-EVAL](https://github.com/Infineon/TARGET_CYBT-333047-EVAL)
- [CYW20835B1 chip](https://github.com/Infineon/20835B1)
    - [CYW920835REF-RCU-01](https://github.com/Infineon/TARGET_CYW920835REF-RCU-01), [CYW920835M2EVB-01](https://github.com/Infineon/TARGET_CYW920835M2EVB-01), [CYBLE-343072-EVAL-M2B](https://github.com/Infineon/TARGET_CYBLE-343072-EVAL-M2B), [CYBLE-333074-EVAL-M2B](https://github.com/Infineon/TARGET_CYBLE-333074-EVAL-M2B), [CYBLE-343072-MESH](https://github.com/Infineon/TARGET_CYBLE-343072-MESH)
- [CYW43012C0 chip](https://github.com/Infineon/43012C0)
    - [CYW9M2BASE-43012BT](https://github.com/Infineon/TARGET_CYW9M2BASE-43012BT), [CYW943012BTEVK-01](https://github.com/Infineon/TARGET_CYW943012BTEVK-01)
- [CYW43022C1 chip](https://github.com/Infineon/43022C1)
    - [CYW943022BTEVK-01](https://github.com/Infineon/TARGET_CYW943022BTEVK-01)
- [CYW20736A1 chip](https://github.com/Infineon/20736A1)
    - [CYW920736M2EVB-01](https://github.com/Infineon/TARGET_CYW920736M2EVB-01)
- [CYW30739A0 chip](https://github.com/Infineon/30739A0)
    - [CYW930739M2EVB-01](https://github.com/Infineon/TARGET_CYW930739M2EVB-01)
- [CYW55572A1 chip](https://github.com/Infineon/55572A1)
    - [CYW955572BTEVK-01](https://github.com/Infineon/TARGET_CYW955572BTEVK-01)


## Folder structure

All BTSDK code examples need the 'mtb\_shared\wiced\_btsdk' folder to build and test the apps. 'wiced\_btsdk' includes the 'dev-kit' and 'tools' folders. The contents of the 'wiced\_btsdk' folder will be automatically populated incrementally as needed by the application being used.

**dev-kit**

This folder contains the files that are needed to build the embedded Bluetooth&#174; apps.

* baselib: Files for chips supported by BTSDK. For example CYW20819, CYW20719, CYW20706, etc.

* bsp: Files for BSPs (platforms) supported by BTSDK. For example CYW920819EVB-02, CYW920706WCDEVAL etc.

* btsdk-include: Common header files needed by all apps and libraries.

* btsdk-tools: Build tools needed by BTSDK.

* libraries: Profile libraries used by BTSDK apps such as audio, LE, HID, etc.

**tools**

This folder contains tools and utilities need to test the embedded Bluetooth&#174; apps.

* btsdk-host-apps-bt-ble: Host apps (Client Control) for LE and BR/EDR embedded apps, demonstrates the use of AIROC&#8482; HCI protocol to control embedded apps.

* btsdk-host-peer-apps-mesh: Host apps (Client Control) and Peer apps for embedded Mesh apps, demonstrates the use of AIROC&#8482; HCI protocol to control embedded apps, and configuration and provisioning from peer devices.

* btsdk-peer-apps-ble: Peer apps for embedded LE apps.

* btsdk-peer-apps-ota: Peer apps for embedded apps that support Over The Air Firmware Upgrade.

* btsdk-utils: Utilities used in BTSDK such as BTSpy, wmbt, and ecdsa256.

See README.md in the sub-folders for more information.

## Software Tools
The following tool applications are installed on your computer either with ModusToolbox&#8482;, or by creating an application in the workspace that can use the tool.

**BTSpy:**<br>
>   BTSpy is a trace viewer utility that can be used with AIROC&#8482; Bluetooth&#174; platforms to
    view protocol and application trace messages from the embedded device. The
    utility is located in the folder below. For more information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel, or by right-clicking
    a project in the Project Explorer pane and selecting the ModusToolbox&#8482;
    context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Location:  \<Workspace Dir>\wiced_btsdk\tools\btsdk-utils\BTSpy

**Bluetooth&#174; Classic and LE Profile Client Control:**<br>
>   This application emulates host MCU applications for LE and BR/EDR profiles.
    It demonstrates AIROC&#8482; Bluetooth&#174; APIs. The application communicates with embedded
    apps over the "WICED HCI UART" interface. The application is located in the folder
    below. For more information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel, or by right-clicking
    a project in the Project Explorer pane and selecting the ModusToolbox&#8482;
    context menu.<br>
    It is supported on Windows, Linux, and macOS.<br>
    Location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-host-apps-bt-ble\client_control

**LE Mesh Client Control:**<br>
>   Similar to the above app, this application emulates host MCU applications
    for LE Mesh models. It can configure and provision mesh devices and create
    mesh networks. The application is located in the folder below. For more
    information, see readme.txt in the same folder.<br>
    This utility can be run directly from the filesystem, or it can be run from
    the Tools section of the ModusToolbox&#8482; QuickPanel (if a mesh-capable
    project is selected in the Project Explorer pane), or by right-clicking
    a mesh-capable project in the Project Explorer pane and selecting the
    ModusToolbox&#8482; context menu.<br>
    The full version is provided for Windows (VS\_ClientControl) supporting all
    Mesh models.<br>
    A limited version supporting only the Lighting model (QT\_ClientControl) is
    provided for Windows, Linux, and macOS.<br>
    Location:  \<Workspace Dir>\wiced_btsdk\tools\btsdk-host-peer-apps-mesh\host

**Peer apps:**<br>
>   Applications that run on Windows, iOS or Android and act as peer Bluetooth&#174; apps to
    demonstrate specific profiles or features, communicating with embedded apps
    over the air.<br>
    LE apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-peer-apps-ble<br>
    LE Mesh apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-host-peer-apps-mesh\peer<br>
    OTA apps location:  \<Workspace Dir>\wiced\_btsdk\tools\btsdk-peer-apps-ota

**Device Configurator:**<br>
>   Use this GUI tool to create source code for a custom pin mapping for your device. Run this tool
    from the Tools section of the ModusToolbox&#8482; QuickPanel, or by
    right-clicking a project in the Project Explorer pane and selecting the
    ModusToolbox&#8482; context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Note: The pin mapping is based on wiced\_platform.h for your board.<br>
    Location:  \<Install Dir>\tools_2.x\device-configurator

Note: Not all BTSDK chips support Device Configurator. BSPs using the following devices do not currently support Device Configurator: CYW20706, CYW20736

**Bluetooth&#174; Configurator:**<br>
>   Use this GUI tool to create and configure the LE GATT Database and the BR/EDR SDP Database, generated as source code for your
    application.<br>
    Run this tool from the Tools section of the ModusToolbox&#8482; QuickPanel, or
    by right-clicking a project in the Project Explorer pane and selecting
    the ModusToolbox&#8482; context menu.<br>
    It is supported on Windows, Linux and macOS.<br>
    Location:  \<Install Dir>\tools_2.x\bt-configurator

## Tracing
To view application traces, there are 2 methods available. Note that the
application needs to configure the tracing options.<br>

1. "WICED Peripheral UART" - Open this port on your computer using a serial port
utility such as TeraTerm or PuTTY (usually using 115200 baud rate for non-Mesh apps, and 921600 for Mesh apps).<br>
2. "WICED HCI UART" - Open this port on your computer using the Client Control
application mentioned above (usually using 3M baud rate). Then run the BTSpy
utility mentioned above.

## Using BSPs (platforms)

BTSDK BSPs are located in the \mtb\_shared\wiced\_btsdk\dev-kit\bsp\ folder by default.

#### a. Selecting an alternative BSP

The application makefile has a default BSP. See "TARGET". The makefile also has a list of other BSPs supported by the application. See "SUPPORTED_TARGETS". To select an alternative BSP, use Library Manager from the Quick Panel to deselect the current BSP and select an alternate BSP. Then right-click the newly selected BSP and choose 'Set Active'.  This will automatically update TARGET in the application makefile.

#### b. Custom BSP

To create a custom BSP from a BSP template for BTSDK devices, see the following KBA article: [KBA238530](https://community.infineon.com/t5/Knowledge-Base-Articles/Create-custom-BTSDK-BSP-using-ModusToolbox-version-3-x-KBA238530/ta-p/479355)

## Using libraries

The libraries needed by the app can be found in in the mtb\_shared\wiced\_btsdk\dev-kit\libraries folder. To add an additional library to your application, launch the Library Manager from the Quick Panel to add a library. Then update the makefile variable "COMPONENTS" of your application to include the library. For example:<br/>
   COMPONENTS += fw\_upgrade\_lib


## Documentation

BTSDK API documentation is available [online](https://infineon.github.io/btsdk-docs/BT-SDK/index.html)

Note: For offline viewing, git clone the [documentation repo](https://github.com/Infineon/btsdk-docs)

BTSDK Technical Brief and Release Notes are available [online](https://community.infineon.com/t5/Bluetooth-SDK/bd-p/ModusToolboxBluetoothSDK)

<br>
<sup>The Bluetooth&#174; word mark and logos are registered trademarks owned by Bluetooth SIG, Inc., and any use of such marks by Infineon is under license.</sup>
