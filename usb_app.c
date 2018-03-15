#include <stdint.h>

#include "usbh_def.h"

#include "usb_app.h"

#define REPORT_ID_JOYSTICK      4
#define REPORT_ID_JOYSTICK_FFB  5
#define X_MIN                   (-511)
#define X_MAX                   (511)

static const uint8_t hidReportDescriptor0[] = {
//    0x05, 0x01, // Usage Page (Generic Desktop)
    0x05, 0x02, // Usage Page (Generic Desktop)
    0x09, 0x04, // Usage (Joystick)
    0xA1, 0x01, // Collection (Application)
    0x85, REPORT_ID_JOYSTICK, //ID
    0x05, 0x01, // Usage Page (Generic Desktop)
    0x09, 0x01, /*   Usage (Pointer), */
    0xA1, 0x00,     // Collection (Phy.)
//    0x09, 0x38,     // USAGE (Wheel)
//    0x09, 0x33,     // USAGE (Rx)
//    0x09, 0x34,     // USAGE (Ry)
    0x09, 0x30,     // USAGE (X)
    0x16, LOBYTE(X_MIN), HIBYTE(X_MIN), /* Logical Minimum (X_MIN), */
    0x26, LOBYTE(X_MAX), HIBYTE(X_MAX), /* Logical Maximum (X_MAX), */
    0x75, 0x10,     // REPORT_SIZE (16)
    0x95, 0x01,     // REPORT_COUNT (1)
    0x81, 0x02,     // INPUT (Data,Var,Abs)
    0x09, 0x31,     // USAGE (Y)
    0x09, 0x32,     // USAGE (Z)
    0x15, 0x00,     /* Logical Minimum (0), */
    0x25, 0x7F,     /* Logical Maximum (127), */
    0x75, 0x08,     // REPORT_SIZE (8)
    0x95, 0x02,     // REPORT_COUNT (2)
    0x81, 0x02,     // INPUT (Data,Var,Abs)
    0xC0,           // End Collection
    0x05, 0x09,     // USAGE_PAGE (Button)
    0x19, 0x01,     // USAGE_MINIMUM (Button 1)
    0x29, 0x08,     // USAGE_MAXIMUM (Button 8)
    0x15, 0x00,     // LOGICAL_MINIMUM (0)
    0x25, 0x01,     // LOGICAL_MAXIMUM (1)
    0x75, 0x01,     // REPORT_SIZE (1)
    0x95, 0x08,     // REPORT_COUNT (8)
    0x55, 0x00,     // UNIT_EXPONENT (0)
    0x65, 0x00,     // UNIT (None)
    0x81, 0x02,     // INPUT (Data,Var,Abs)
    0xC0,       // End Collection

    0x09,0x96,    //    Usage DC Disable Actuators
       0xA1,0x02,    //    Collection Datalink
          0x85,REPORT_ID_JOYSTICK_FFB,    //    Report ID Ch (12d)
          0x09,0x97,    //    Usage DC Stop All Effects
          0x09,0x98,    //    Usage DC Device Reset
          0x09,0x99,    //    Usage DC Device Pause
          0x09,0x9A,    //    Usage DC Device Continue
          0x09,0x9B,    //    Usage PID Device State
          0x09,0x9C,    //    Usage DS Actuators Enabled
          0x15,0x01,    //    Logical Minimum 1
          0x25,0x06,    //    Logical Maximum 6
          0x75,0x08,    //    Report Size 8
          0x95,0x01,    //    Report Count 1
          0x91,0x00,    //    Output
       0xC0,         //    End Collection
};

//#define USBATTR_BUSPOWER        0x80  // USB 1.1 does not define this value any more
#define USBATTR_BUSPOWER        0
#define USBATTR_SELFPOWER       0x40
#define USBATTR_REMOTEWAKE      0x20

static const uint8_t configDescriptor[]  =
{    /* USB configuration descriptor */
    9,          /* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USB_DESC_TYPE_CONFIGURATION,    /* descriptor type */
    9 + 1 * (9 + 9 + 7), 0,
    /* total length of data returned (including inlined descriptors) */
    1,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
    USBATTR_BUSPOWER,   /* attributes */
    100 / 2,  /* max USB current in 2mA units */

/* --- interface 0 --- */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USB_DESC_TYPE_INTERFACE, /* descriptor type */
    0,          /* index of this interface */
    0,          /* alternate setting for this interface */
    1,          /* endpoints excl 0: number of endpoint descriptors to follow */
    3,          /* 3=HID class */
    0,          /* subclass: 0=none, 1=boot */
    0,          /* protocol: 0=none, 1=keyboard, 2=mouse */
    0,          /* string index for interface */

    /* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USB_DESC_TYPE_HID,       /* descriptor type: HID */
    0x01, 0x01,     /* BCD representation of HID version */
    0x00,           /* target country code */
    0x01,           /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,           /* descriptor type: report */
    LOBYTE(sizeof(hidReportDescriptor0)), HIBYTE(sizeof(hidReportDescriptor0)), /* total length of report descriptor */

    /* endpoint descriptor for endpoint 1 */
    7,          /* sizeof(usbDescrEndpoint) */
    USB_DESC_TYPE_ENDPOINT,  /* descriptor type = endpoint */
    0x81,           /* IN endpoint number 1 */
    0x03,           /* attrib: Interrupt endpoint */
    8, 0,           /* maximum packet size */
    10, /* USB_CFG_INTR_POLL_INTERVAL in ms */
};
