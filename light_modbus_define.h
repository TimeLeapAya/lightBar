#ifndef __LIGHT_CONTROL_MODBUS_DEFINE_H__
#define __LIGHT_CONTROL_MODBUS_DEFINE_H__

// 10.1 Device parameter
#define CONTROLLER_PARA_FUNCTION_CODE_READ                         0x03
#define CONTROLLER_PARA_FUNCTION_CODE_WRITE                        0x06

#define CONTROLLER_PARA_ADDR_DEVICE_ADDR                           0x03E8      // Range(1-247), default: 1
#define CONTROLLER_PARA_ADDR_BAUDRATE_ADDR                         0x03E9      // RS485 Baudrate(0-7), , default:1, Refer to (10.1.1 Device supported baudrate)
#define CONTROLLER_PARA_ADDR_STOPBIT_ADDR                          0x03EA      // RS485 Stopbit(1), default: 1, cannot modified
#define CONTROLLER_PARA_ADDR_PARITYCK_ADDR                         0x03EB      // RS485 Parity check(0-2), default: 0, Refer to (10.1.2 Device supported Parity check)
#define CONTROLLER_PARA_ADDR_POWERON_PLAY_ADDR                     0x03EC      // Power On Display(1000-1023 for inner default mode, 2000-2127 for created&stored mode, 3000-3069 for created&store mode list), default: 1000
#define CONTROLLER_PARA_ADDR_POWERON_MODE_ADDR                     0x03EF      // Power On Mode, default: 0, Refer to (10.1.3 Device poweron mode)
#define CONTROLLER_PARA_ADDR_FACTORY_ADDR                          0x03F5      // Device Factory set, default: 0, Refer to(10.1.4 Device factory set)

// 10.1.1 Device supported baudrate
#define CONTROLLER_PARA_BAUDRATE_4800                              0x00
#define CONTROLLER_PARA_BAUDRATE_9600                              0x01
#define CONTROLLER_PARA_BAUDRATE_19200                             0x02
#define CONTROLLER_PARA_BAUDRATE_38400                             0x03
#define CONTROLLER_PARA_BAUDRATE_57600                             0x04
#define CONTROLLER_PARA_BAUDRATE_115200                            0x05
#define CONTROLLER_PARA_BAUDRATE_230400                            0x06
#define CONTROLLER_PARA_BAUDRATE_460800                            0x07

// 10.1.2 Device supported Parity check
#define CONTROLLER_PARA_PARITYCK_NONE                              0x00
#define CONTROLLER_PARA_PARITYCK_ODD                               0x01
#define CONTROLLER_PARA_PARITYCK_EVEN                              0x02

// 10.1.3 Device poweron mode
#define CONTROLLER_PARA_POWERON_RUN                                0x00
#define CONTROLLER_PARA_POWERON_STOP                               0x01

// 10.1.4 Device factory set
#define CONTROLLER_PARA_FACTORY_PARA1                              0x04D2
#define CONTROLLER_PARA_FACTORY_PARA2                              0x10E1

// 10.2 Light Bar parameter configuration
#define LIGHT_PARA_FUNCTION_CODE_READ                              0x03
#define LIGHT_PARA_FUNCTION_CODE_WRITE                             0x06

#define LIGHT_PARA_ADDR_IC_TYPE_ADDR                               0x07D0      // Light IC type Addr, Default: 0, Refer t0(10.2.1 Light IC types)
#define LIGHT_PARA_ADDR_CHANNEL_NUM_ADDR                           0x07D1      // Light IC Channel number, default: 3, Refer to(10.2.2 Light IC channel number)
#define LIGHT_PARA_ADDR_COLOR_ORDER_ADDR                           0x07D2      // Light IC Color order, default: 0, Refer to (10.2.3 Light Color order)
#define LIGHT_PARA_ADDR_IC_RESETTIME_ADDR                          0x07D6      // Light IC Reset time(us), default: 280, cannot modified
#define LIGHT_PARA_ADDR_IC_BITTIME_ADDR                            0x07D7      // Light IC Bit-time(ns), range(400~3000), default: 1250, used only when use LIGHT_PARA_IC_TYPE_SELF_DEFINED
#define LIGHT_PARA_ADDR_IC_BITTIME_0H_ADDR                         0x07d8      // Light IC Bit-time(ns) bit 0 high-level time, range(1/8 ~ 1/2 Bit-time), default: 350, used only when use LIGHT_PARA_IC_TYPE_SELF_DEFINED
#define LIGHT_PARA_ADDR_IC_BITTIME_1H_ADDR                         0x07d9      // Light IC Bit-time(ns) bit 1 high-level time, range(1/2 ~ 1/8 Bit-time), default: 800, used only when use LIGHT_PARA_IC_TYPE_SELF_DEFINED

// 10.2.1 Light IC types
#define LIGHT_PARA_IC_TYPE_WS2812B                                 0x00
#define LIGHT_PARA_IC_TYPE_WS2811                                  0x01
#define LIGHT_PARA_IC_TYPE_WS2813                                  0x02
#define LIGHT_PARA_IC_TYPE_WS2815                                  0x03
#define LIGHT_PARA_IC_TYPE_WS2818                                  0x04
#define LIGHT_PARA_IC_TYPE_SK6812_1                                0x05
#define LIGHT_PARA_IC_TYPE_SK6812_2                                0x06
#define LIGHT_PARA_IC_TYPE_UCS1903                                 0x07
#define LIGHT_PARA_IC_TYPE_TM1903                                  0x08
#define LIGHT_PARA_IC_TYPE_FW1903                                  0x09
#define LIGHT_PARA_IC_TYPE_FW1935                                  0x0A
#define LIGHT_PARA_IC_TYPE_TM1934                                  0x0B
#define LIGHT_PARA_IC_TYPE_SM16703                                 0x0C
#define LIGHT_PARA_IC_TYPE_MT1815                                  0x0D
#define LIGHT_PARA_IC_TYPE_SELF_DEFINED                            0xF9

// 10.2.2 Light IC channel number
#define LIGHT_PARA_CHANNEL_NUMBER_3                                0x03
#define LIGHT_PARA_CHANNEL_NUMBER_4                                0x04

// 10.2.3 Light Color order
   // For LIGHT_PARA_CHANNEL_NUMBER_3: 
#define LIGHT_PARA_COLOR_ORDER_RGB                                 0x00
#define LIGHT_PARA_COLOR_ORDER_RBG                                 0x01
#define LIGHT_PARA_COLOR_ORDER_GRB                                 0x02
#define LIGHT_PARA_COLOR_ORDER_GBR                                 0x03
#define LIGHT_PARA_COLOR_ORDER_BRG                                 0x04
#define LIGHT_PARA_COLOR_ORDER_BGR                                 0x05
   // For LIGHT_PARA_CHANNEL_NUMBER_4:
#define LIGHT_PARA_COLOR_ORDER_RGBW                                0x00
#define LIGHT_PARA_COLOR_ORDER_RBGW                                0x01
#define LIGHT_PARA_COLOR_ORDER_GRBW                                0x02
#define LIGHT_PARA_COLOR_ORDER_GBRW                                0x03
#define LIGHT_PARA_COLOR_ORDER_BRGW                                0x04
#define LIGHT_PARA_COLOR_ORDER_BGRW                                0x05
#define LIGHT_PARA_COLOR_ORDER_WRGB                                0x06
#define LIGHT_PARA_COLOR_ORDER_WRBG                                0x07
#define LIGHT_PARA_COLOR_ORDER_WGRB                                0x08
#define LIGHT_PARA_COLOR_ORDER_WGBR                                0x09
#define LIGHT_PARA_COLOR_ORDER_WBRG                                0x0A
#define LIGHT_PARA_COLOR_ORDER_WBGR                                0x0B


// 10.3 Coils action
#define COIL_PARA_FUNCTION_CODE_READ                               0x01
#define COIL_PARA_FUNCTION_CODE_WRITE                              0x05

#define COIL_PARA_RESET_FAULT_ADDR                                 0x03E8             // Reset input-register(1004~1008,1009) fault code, default: 0, Trigger value: 1
#define COIL_PARA_RESET_MODE_ADDR                                  0x03E9             // Reset current play(item or list) with default mode, default 0, Trigger value: 1
#define COIL_PARA_PLAY_SUSPEND_ADDR                                0x03EA             // Suspend current play, default 0, Trigger value: 1
#define COIL_PARA_PLAY_STOP_ADDR                                   0x03EB             // Stop current play, default 0, Trigger value: 1
#define COIL_PARA_PLAY_START_ADDR                                  0x03EC             // Start current play, default 0, Trigger value: 1
#define COIL_PARA_RELOAD_PLAY_ADDR                                 0x03ED             // Reload current play, default 0, Trigger value: 1
#define COIL_PARA_RESTART_PLAY_ADDR                                0x03EE             // Restart current play, default 0, Trigger value: 1
#define COIL_PARA_QUICK_TURNOFF_ADDR                               0x03F0             // Quick turn-off cmd, default 0, Trigger value: 1

#define COIL_PARA_CMD_TRIGGER                                      0x01               // Trigger corresponding cmd

// 10.4 Seperate input
#define DISCRTE_INPUT_FUNCTION_CODE_READ                           0x02                

#define DISCRETE_INPUT_PARA_SUSPEND_ADDR                           0x03EA              // Play suspend state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_STOP_ADDR                              0x03EB              // Play stop state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_RUN_ADDR                               0x03EC              // Play run state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_FIN0_ADDR                              0x03EF              // Frequence signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN0_ADDR                               0x03F0              // I/O Input0 signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN1_ADDR                               0x03F1              // I/O Input1 signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN2_ADDR                               0x03F2              // I/O Input2 signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN3_ADDR                               0x03F3              // I/O Input3 signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN4_ADDR                               0x03F4              // I/O Input4 signal state, range(0~1), default: 0
#define DISCRETE_INPUT_PARA_IN5_ADDR                               0x03F5              // I/O Input5 signal state, range(0~1), default: 0

#define INPUT_PARA_LOW                                             0
#define INPUT_PARA_HIGH                                            1
#define INPUT_PARA_UNKNOW                                          3

// 10.5 Input register
#define INPUT_REGISTER_FUNCTION_CODE_READ                          0x04

#define INPUT_REGISTER_PARA_HW_VER_ADDR                            0x03E8              // Controller hardware version, range(0~0xFFFF), default: 100(1.00)
#define INPUT_REGISTER_PARA_SW_VER_ADDR                            0x03E9              // Controller software version, range(0~0xFFFF), default: 100(1.00)
#define INPUT_REGISTER_PARA_NEWEST_PLAY_ADDR                       0x03EA              // Controller most recent play mode or list, range(0~0xFFFF), default: 0
#define INPUT_REGISTER_PARA_CURRENT_PLAY_ADDR                      0x03EB              // Controller most current play mode or list, range(1000-1023 for inner default mode, 2000-2127 for created&stored mode, 3000-3069 for created&store mode list), default: 1000
#define INPUT_REGISTER_PARA_FAULTCODE_ADDR                         0x03F1              // Controller fault-code, default: 0, Refer to  (10.5.1 Controller fault-code)
#define INPUT_REGISTER_PARA_PLAY_END_ADDR                          0x03F3              // Controller play end flag, range(0~0xFFFF), default:0

// 10.5.1 Controller fault-code
#define INPUT_REGISTER_PARA_FAULTCODE_NONE                         0x00                // No fault, Running normally
#define INPUT_REGISTER_PARA_FAULTCODE_MODE                         0x09                // Mode number error, exceed range
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_DEV_PARA               0x0A                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_DEV_PARA                0x0B                // 
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_LIGHT_PARA             0x0C                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_LIGHT_PARA              0x0D                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_MODE                    0x0E                // 
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_ITEM                   0x0F                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_ITEM                    0x10                // 
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_LIST                   0x11                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_LIST                    0x12                // 
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_FLASH                  0x13                // 
#define INPUT_REGISTER_PARA_FAULTCODE_READ_FLASH                   0x14
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_BUF                    0x15
#define INPUT_REGISTER_PARA_FAULTCODE_READ_BUF                     0x16
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_LIGHT                  0x17
#define INPUT_REGISTER_PARA_FAULTCODE_READ_LIGHT                   0x18
#define INPUT_REGISTER_PARA_FAULTCODE_STORE_IO                     0x19
#define INPUT_REGISTER_PARA_FAULTCODE_READ_IO                      0x1A


// 10.7 Mode configuration 
#define MODE_CONF_MODE_SEL_ADDR                                    0x0834               // Select wanted mode, default:0, Refer to (10.7.1 Light mode support)
#define MODE_CONF_MODE_BACKGROUD_EDIT_ADDR                         0x0836               // Back mode edit
#define MODE_CONF_MODE_BACK_EDIT_TRIG_ADDR                         0x0837               // Back mode trigger, Refer to (10.7.2 Back mode trigger)
#define MODE_CONF_MODE_LIGHT_IC_NUM_ADDR                           0x0838               // 5V: 1 for 1 Light, 12V: 1 for 3 Light, 24V: 1 for 6 Light
#define MODE_CONF_MODE_MATRIC_SEG_NUM_ADDR                         0x0839               
#define MODE_CONF_MODE_RUN_TIMES_ADDR                              0x083A               // Current mode run times, Range(0~65535), default:0, Refer to(10.7.3 Mode for run times)
#define MODE_CONF_MODE_FRAMES_DELAY_ADDR                           0x083B               // 
#define MODE_CONF_MODE_RUNTIME_DELAY_ADDR                          0x083C 
#define MODE_CONF_MODE_MATRIC_MOE_ADDR                             0x083D               // Mattric mode, Range(0~3), default: 0, Refer to (10.7.4 Mode for matric mode)
#define MODE_CONF_MODE_END_METHOD_ADDR                             0x083F               // End method, Range(0 ~ 3), default: 0, Refer to (10.7.5 Mode for end method)
#define MODE_CONF_MODE_INIT_LIGHT_ADDR                             0x0840               // Initial light state, Range(0~1), default: 0, Refer to (10.7.6 Mode for Init light)
#define MODE_CONF_MODE_BEGIN_POSTION_ADDR                          0x0845               // Start current mode from witch IC, range(1~1200), default: 1

// 10.7.1 Light mode support
#define MODE_CONF_MODEL_1000_TEST                                  0x03E8
#define MODE_CONF_MODEL_1001_SIGNLE_STATIC                         0x03E9
#define MODE_CONF_MODEL_1002_SINGLE_TBLINK                         0x03EA
#define MODE_CONF_MODEL_1003_SINGLE_FBLINK                         0x03EB
#define MODE_CONF_MODEL_1004_ALL_STATIC                            0x03EC
#define MODE_CONF_MODEL_1005_SEG_STATIC                            0x03ED
#define MODE_CONF_MODEL_1006_ALL_FLOW1                             0x03EE
#define MODE_CONF_MODEL_1007_ALL_FLOW2                             0x03EF
#define MODE_CONF_MODEL_1008_ALL_FLOW3                             0x03F0
#define MODE_CONF_MODEL_1009_ALL_DOUBLE_CYCLE                      0x03F1
#define MODE_CONF_MODEL_1010_PART1_STATIC                          0x03F2
#define MODE_CONF_MODEL_1011_PART2_STATIC                          0x03F3
#define MODE_CONF_MODEL_1012_BUF_INOUT                             0x03F4               // 1012 mode specific config Refer to (10.7.13 Waterlight buffer INOUT mode specific config)
#define MODE_CONF_MODEL_1013_BUF_FLOW                              0x03F5
#define MODE_CONF_MODEL_1014_ALL_2COLOR_CHANGE                     0x03F6
#define MODE_CONF_MODEL_1015_ALL_3COLOR_CHANGE                     0x03F7
#define MODE_CONF_MODEL_1016_90DEGREE                              0x03F8
#define MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR                     0x03F9               // 1017 mode specific config Refer to (10.7.18 Breath light mode specific config)
#define MODE_CONF_MODEL_1018_ALL_2COLOR_BLINK                      0x03FA
#define MODE_CONF_MODEL_1019_COLOR_BLOCK_MOVE                      0x03FB
#define MODE_CONF_MODEL_1020_WINDOW_MOVE                           0x03FC
#define MODE_CONF_MODEL_1021_BUF_WINDOW                            0x03FD
#define MODE_CONF_MODEL_1022_BUF_MULTI_FRAME                       0x03FE
#define MODE_CONF_MODEL_1023_FLASH_MULTI_FRAME                     0x03FF
#define MODE_CONF_MODEL_TOTAL_NUMBER                               24

// 10.7.2 Back mode trigger
#define MODE_CONF_TRIGGER_BACK_MODE                                0x01

// 10.7.3 Mode for run times
#define MODE_CONF_RUN_TIME_ALWAYS                                  0x00

// 10.7.4 Mode for matric mode
#define MODE_CONF_MATRIC_MODE_GENERAL                              0x00
#define MODE_CONF_MATRIC_MODE_MIRROR                               0x01

// 10.7.5 Mode for end method
#define MODE_CONF_END_METHOD_RUN_AGAIN                             0x00
#define MODE_CONF_END_METHOD_END_STOP                              0x01
#define MODE_CONF_END_METHOD_INIT_STOP                             0x02
#define MODE_CONF_END_METHOD_END_TURNOFF                           0x03

// 10.7.6 Mode for Init light
#define MODE_CONF_INIT_LIGHT_TURNOFF                               0x00
#define MODE_CONG_INIT_LIGHT_TURNON                                0x01


// 10.7.13 Waterlight buffer INOUT mode specific config
#define MODE_WATERLIGHT_BUFFER_INOUT_BSI_ADDR                      0x0851             //  Range(1 ~ 1200), default: 1
#define MODE_WATERLIGHT_BUFFER_INOUT_BIN_ADDR                      0x0852             //  Range(1 ~ 1200), default: 8
#define MODE_WATERLIGHT_BUFFER_INOUT_MOVE_DIRECTION_ADDR           0x0853             //  Range(0 ~ 1), default: 0, Refer to(10.7.13.1 WATERLIGHT_1012_MOVE_DIRECTION)
#define MODE_WATERLIGHT_BUFFER_INOUT_MOVE_STEP_ADDR                0x0854             //  Range(1 ~ ICs/8), default：1
#define MODE_WATERLIGHT_BUFFER_INOUT_BUFF_PICK_ADDR                0x0855             //  Range(0 ~ 1), default: 0, Refer to(10.7.13.2 WATERLIGHT_1012_BUF_PICK_DIRECTION)

// 10.7.13.1 WATERLIGHT_1012_MOVE_DIRECTION
#define WATERLIGHT_1012_MOVE_DIRECTION_FORWARD                     0x00
#define WATERLIGHT_1012_MOVE_DIRECTION_BACKWARD                    0x01

//10.7.13.2 WATERLIGHT_1012_BUF_PICK_DIRECTION
#define WATERLIGHT_1012_BUF_PICK_DIRECTION_FROM_LOW                0x00
#define WATERLIGHT_1012_BUF_PICK_DIRECTION_FROM_HIGH               0x01

// 10.7.18 Breath light mode specific config
#define MODE_CONF_BREATH_RCOLOR_SET_ADDR                           0x084D              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, define Red color value, default 0, Range(0 ~ 255)
#define MODE_CONF_BREATH_GCOLOR_SET_ADDR                           0x084E              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, define Green color value, default 0, Range(0 ~ 255)
#define MODE_CONF_BREATH_BCOLOR_SET_ADDR                           0x084F              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, define Blue color value, default 0, Range(0 ~ 255)
#define MODE_CONF_BREATH_WCOLOR_SET_ADDR                           0x0850              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, define W color value, default 0, Range(0 ~ 255)
#define MODE_CONF_BREATH_WIRE_MODE_SET_ADDR                        0x0851              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, Breath wire set, default 0, Range(0 ~ 1)
#define MODE_CONF_BREATH_STEP_LIGHT_SET_ADDR                       0x0852              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, Breath to light step, default 120, Range(10 ~ 1000)
#define MODE_CONF_BREATH_STEP_DARK_SET_ADDR                        0x0853              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, Breath to dark step, default 100, Range(10 ~ 1000)
#define MODE_CONF_BREATH_SPEED_LIGHT_SET_ADDR                      0x0854              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, Breath to light speed, default 25, Range(10 ~ 200)
#define MODE_CONF_BREATH_SPEED_DARK_SET_ADDR                       0x0855              // For MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR mode, Breath to dark speed, default 25, Range(10 ~ 200)

// 10.8 ITEM information config
#define ITEM_CALL_EXEC_FROM_STORE_ADDR                             0x0898              // Call and execute item from store, Range(2000~2127), default: 2000
#define ITEM_CALLED_FROM_STORE_MODE_ADDR                           0x0899              // The mode number of current called item from store, Read only, Range(1000~1023), default: 0
#define ITEM_CALLED_HAS_VALID_MODE_ADDR                            0x089A              // If the called item has valid mode info, Range(0~1), default: 0, Refer to (10.8.1 State of called Store item)
#define ITEM_CALLED_BACKEDIT_ITEM_ADDR                             0x089C              // Called and modify the item display information backgroud, Trigger with 2229, save with 2228
#define ITEM_CALLED_DELETE_FROM_STORE_ADDR                         0x089D              // Trigger with 1 to clear the item in the stores
#define ITEM_IS_IN_BACKEDIT_MODE_ADDR                              0x089E              // State & Trigger
#define ITEM_GET_MODEINFO_IN_ITEM_ADDR                             0x089F              
#define ITEM_CALLED_RUNTIME_SET_ADDR                               0x08A0              // Set item call from store runtime, Range(0 ~ 50000), defalut: 0, Refer to (10.8.2 Runtime of called Store item)
#define ITEM_CALLED_SAVE_TO_STORE_ADDR                             0x08B4              // Save the item to store Trigger
#define ITEM_CALLED_EXEC_ADDR                                      0x08B5              // Execute the called item Trigger


// 10.8.2 Runtime of called Store item
#define ITEM_CALLED_RUN_FOREVER                                    0

// 10.8.1 State of called Store item
#define ITEM_CALLED_FROM_STORE_INVALID                             0
#define ITEM_CALLED_FROM_STORE_VALID                               1

// 10.9 Item list Configuration 


// 10.10 BUFFER Configeration
#define BUFFER_AREA_START_ADDR                                     0x1389
#define BUFFER_AREA_SINGLE_IC_CHANNELS                             0x04
#define BUFFER_AREA_MAX_ICS                                        1200

#endif
