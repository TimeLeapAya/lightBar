#ifndef __ENN_USER_DEFINE__
#define __ENN_USER_DEFINE_

#include "light_modbus_define.h"

#define ENN_LIGHT_CONFIGURATION_JSON_FILE      "/var/user/lightcontrol/lightconf.json"

// Default definitions
#define ENN_DEFAULT_SERIAL_NAME                "/dev/ttyUSB0"
#define ENN_DEFAULT_SERIAL_BAUDRATE            1
#define ENN_DEFAULT_DEVICE_ADDR                1
#define ENN_DEFAULT_IC_TYPE                    LIGHT_PARA_IC_TYPE_WS2815
#define ENN_DEFAULT_LIGHT_IC_NUMBER            38
#define ENN_DEFAULT_MATTRIC_NUMBER             1
#define ENN_DEFAULT_RGB_ORDER                  LIGHT_PARA_COLOR_ORDER_GRB

#define ENN_DEMO1_LIGHT_IC_NUMBER              38

// For Powering on display
#define ENN_POWERING_ITEM_STORE_NUM            2001
#define ENN_POWERING_MODE_TEMPLATE_NUM         MODE_CONF_MODEL_1012_BUF_INOUT
#define ENN_POWERING_LIGHT_MATTRIC_NUMBER      1
#define ENN_POWERING_LIGHT_IC_NUMBER           ENN_DEMO1_LIGHT_IC_NUMBER
#define ENN_POWERING_FRAME_DELAY               30                          // 30MS
#define ENN_POWERING_RUNTIME_DELAY             0
#define ENN_POWERING_MATTRIC_MODE              MODE_CONF_MATRIC_MODE_GENERAL
#define ENN_POWERING_RUNTIME_END_MODE          MODE_CONF_END_METHOD_RUN_AGAIN
#define ENN_POWERING_INIT_LIGHT_MODE           MODE_CONF_INIT_LIGHT_TURNOFF
#define ENN_POWERING_BEGIN_IC_NUMBER           1
#define ENN_POWERING_BACKGROUND_RCOLOR         0
#define ENN_POWERING_BACKGROUND_GCOLOR         0
#define ENN_POWERING_BACKGROUND_BCOLOR         0
#define ENN_POWERING_BUFFER_BSI                1
#define ENN_POWERING_BUFFER_BIN                9
#define ENN_POWERING_RUNING_DIRCETION          WATERLIGHT_1012_MOVE_DIRECTION_FORWARD
#define ENN_POWERING_STEP_NUMBER               1
#define ENN_POWERING_BUF_PICK_DIRCTION         WATERLIGHT_1012_BUF_PICK_DIRECTION_FROM_LOW


// For Normal(Blue) Display
#define ENN_NROMAL_ITEM_STORE_NUM              2002
#define ENN_NORMAL_MODE_TEMPLATE_NUM           MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR
#define ENN_NORMAL_LIGHT_IC_NUMBER             ENN_DEMO1_LIGHT_IC_NUMBER
#define ENN_NORMAL_RUNTIME_DELAY               3                  // 300MS
#define ENN_NORMAL_RUNTIME_END_MODE            MODE_CONF_END_METHOD_RUN_AGAIN
#define ENN_NORMAL_INIT_LIGHT_MODE             MODE_CONF_INIT_LIGHT_TURNOFF
#define ENN_NORMAL_BEGIN_IC_NUMBER             1
#define ENN_NORMAL_BREATH_RCOLOR               0
#define ENN_NORMAL_BREATH_GCOLOR               0
#define ENN_NORMAL_BREATH_BCOLOR               255
#define ENN_NORMAL_BREATH_WIRE                 0
#define ENN_NORMAL_BREATH_TO_LIGHT_STEPS       100
#define ENN_NORMAL_BREATH_TO_DARK_STEPS        80
#define ENN_NORMAL_BREATH_TO_LIGHT_SPEED       25
#define ENN_NORMAL_BREATH_TO_DARK_SPEED        25

// For Warning(Yello) Display
#define ENN_WARNING_ITEM_STORE_NUM             2004
#define ENN_WARNING_MODE_TEMPLATE_NUM          MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR
#define ENN_WARNING_LIGHT_IC_NUMBER            ENN_DEMO1_LIGHT_IC_NUMBER
#define ENN_WARNING_RUNTIME_DELAY              5                  // 500MS
#define ENN_WARNING_RUNTIME_END_MODE           MODE_CONF_END_METHOD_RUN_AGAIN
#define ENN_WARNING_INIT_LIGHT_MODE            MODE_CONF_INIT_LIGHT_TURNOFF
#define ENN_WARNING_BEGIN_IC_NUMBER            1
#define ENN_WARNING_BREATH_RCOLOR              255
#define ENN_WARNING_BREATH_GCOLOR              215
#define ENN_WARNING_BREATH_BCOLOR              0
#define ENN_WARNING_BREATH_WIRE                1
#define ENN_WARNING_BREATH_TO_LIGHT_STEPS      80
#define ENN_WARNING_BREATH_TO_DARK_STEPS       80
#define ENN_WARNING_BREATH_TO_LIGHT_SPEED      10
#define ENN_WARNING_BREATH_TO_DARK_SPEED       10

// For Error(Red) Display
#define ENN_ERROR_ITEM_STORE_NUM               2003
#define ENN_ERROR_MODE_TEMPLATE_NUM            MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR
#define ENN_ERROR_LIGHT_IC_NUMBER              ENN_DEMO1_LIGHT_IC_NUMBER
#define ENN_ERROR_RUNTIME_DELAY                5                  // 500MS
#define ENN_ERROR_RUNTIME_END_MODE             MODE_CONF_END_METHOD_RUN_AGAIN
#define ENN_ERROR_INIT_LIGHT_MODE              MODE_CONF_INIT_LIGHT_TURNOFF
#define ENN_ERROR_BEGIN_IC_NUMBER              1
#define ENN_ERROR_BREATH_RCOLOR                240
#define ENN_ERROR_BREATH_GCOLOR                0
#define ENN_ERROR_BREATH_BCOLOR                0
#define ENN_ERROR_BREATH_WIRE                  1
#define ENN_ERROR_BREATH_TO_LIGHT_STEPS        80
#define ENN_ERROR_BREATH_TO_DARK_STEPS         80
#define ENN_ERROR_BREATH_TO_LIGHT_SPEED        10
#define ENN_ERROR_BREATH_TO_DARK_SPEED         10


// For MQTT INFO
#define ENN_MQTT_ID                            "light_mqtt"
#define ENN_MQTT_SERVER                        "localhost"
#define ENN_MQTT_PORT                          1883
#define ENN_MQTT_USERNAME                      "admin"
#define ENN_MQTT_PASSWD                        "admin"
#define ENN_SUBTOPIC_NUM                       2
#define ENN_SUBTOPIC_1                         "/gatway/appcall/lightconf"
#define ENN_SUBTOPIC_2                         "/gatway/appcall/lightcontrol"

#endif
