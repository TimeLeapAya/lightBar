#ifndef __ENN_GEN2_LIGHTCONTROL_
#define __ENN_GEN2_LIGHTCONTROL_

#include <memory>
#include "light_modbus_define.h"
#include "modbus_RTU.h"


#define ENN_STORED_ITEM_FIRST                  2000
#define ENN_STORED_ITEM_TOTAL_NUMBER           128

class Enn_Light_Controller{
public:
    Enn_Light_Controller(const std::string& device, int baud_rate);
    Enn_Light_Controller(const std::string& device, int baud_rate, int dev_addr, uint16_t ic_type, uint16_t channel_num, uint16_t light_turn, uint16_t ic_num, uint16_t mattric_num);
    ~Enn_Light_Controller();

    //common function
    bool Enn_light_get_base_config_from_file(const std::string& filename);
    bool Enn_light_get_spec_config_from_file(const std::string& filename);
    bool Enn_light_get_base_config_from_string(const std::string& content);
    bool Enn_light_get_spec_config_from_string(const std::string& content);
    bool Enn_light_save_base_config_to_file(const std::string& filename, const std::string& content_str);
    bool Enn_light_serial_check(const std::string& filename);
    bool Enn_light_serial_baudrate_check(int baud_rate);
    bool Enn_light_controller_addr_check(int device_addr);
    bool Enn_light_controller_init(void);

    // 10.1
    bool Enn_light_device_addr_config(uint16_t device_addr);
    bool Enn_light_device_serial_config(uint16_t baud_rate, uint16_t parity_check);
    bool Enn_light_device_poweron_play_item(uint16_t booton_item);
    bool Enn_light_device_poweron_mode(uint16_t mode);
    bool Enn_light_device_controller_factory_recovery(void);

    // 10.2
    bool Enn_light_controller_select_ic_type(uint16_t type);
    bool Enn_light_controller_set_channel_num(uint16_t channel_num);
    bool Enn_light_controller_set_rgb_turn(uint ic_turn);
    bool Enn_light_controller_set_code_time(uint16_t code_time);
    bool Enn_light_controller_set_code0_htime(uint16_t code_htime);
    bool Enn_light_controller_set_code1_htime(uint16_t code_htime);

    // 10.3
    bool Enn_light_controller_reset_fault_flags(void);
    bool Enn_light_controller_reset_all_configuration(void);
    bool Enn_light_controller_play_suspend(void);
    bool Enn_light_controller_play_stop(void);
    bool Enn_light_controller_play_start(void);
    bool Enn_light_controller_reload_play_item(void);
    bool Enn_light_controller_play_restart(void);
    bool Enn_light_controller_all_light_turnoff(void);

    // 10.4 
    bool Enn_light_controller_play_is_suspend(void);
    bool Enn_light_controller_play_is_stop(void);
    bool Enn_light_controller_play_is_running(void);
    uint8_t Enn_light_controller_get_FIN0_state(void);
    uint8_t Enn_light_controller_get_IN0_state(void);
    uint8_t Enn_light_controller_get_IN1_state(void);
    uint8_t Enn_light_controller_get_IN2_state(void);
    uint8_t Enn_light_controller_get_IN3_state(void);
    uint8_t Enn_light_controller_get_IN4_state(void);
    uint8_t Enn_light_controller_get_IN5_state(void);

    // 10.5
    bool Enn_light_controller_get_hw_ver(uint16_t *ver);
    bool Enn_light_controller_get_sw_ver(uint16_t *ver);
    bool Enn_light_controller_get_newest_item(uint16_t *item);
    bool Enn_light_controller_get_current_item(uint16_t *item);
    bool Enn_light_controller_get_faultcode(uint16_t *faultcode);
    bool Enn_light_controller_get_play_isend(uint16_t *endstate);

    // 10.6 - Ignored

    // 10.7 2100 - 2118
    bool Enn_light_controller_select_mode(uint16_t mode);
    bool Enn_light_controller_backgroud_edit_mode(uint16_t mode);
    bool Enn_light_controller_trigger_edit_mode(void);
    bool Enn_light_controller_set_light_number(uint16_t number);
    bool Enn_light_controller_set_mattric_number(uint16_t number);
    bool Enn_light_controller_set_run_times(uint16_t times);
    bool Enn_light_contorller_set_delay_between_frames(uint16_t delay);
    bool Enn_light_controller_set_delay_between_runtimes(uint16_t delay);
    bool Enn_light_controller_set_mattric_mode(uint16_t mode);
    bool Enn_light_controller_set_endways(uint16_t ways);
    bool Enn_light_controller_set_begin_style(uint16_t style);
    bool Enn_light_controller_set_begin_ic(uint16_t begin_ic);
    bool Enn_light_controller_set_left_color(uint16_t color);

    // 10.7.1 MODE 1000 specific configuration
    // 10.7.2 MODE 1001 specific configuration
    // 10.7.3 MODE 1002 specific configuration
    // 10.7.4 MODE 1003 specific configuration
    // 10.7.5 MODE 1004 specific configuration
    // 10.7.6 MODE 1005 specific configuration
    // 10.7.7 MODE 1006 specific configuration
    // 10.7.8 MODE 1007 specific configuration
    // 10.7.9 MODE 1008 specific configuration
    // 10.7.10 MODE 1009 specific configuration
    // 10.7.11 MODE 1010 specific configuration
    // 10.7.12 MODE 1011 specific configuration
    // 10.7.13 MODE 1012 specific configuration & 10.7.14 MODE 1013 specific configuration
    bool Enn_light_controller_1012_1013_buf_start_index(uint16_t index);
    bool Enn_light_controller_1012_1013_buf_inuse_number(uint16_t number);
    bool Enn_light_controller_1012_1013_run_direction(uint16_t direction);
    bool Enn_light_controller_1012_1013_step_number(uint16_t step);
    bool Enn_light_controller_1012_1013_buffer_pick_direction(uint16_t dirction);
    // 10.7.15 MODE 1014 specific configuration
    // 10.7.16 MODE 1015 specific configuration
    // 10.7.17 MODE 1016 specific configuration
    // 10.7.18 MODE 1017 specific configuration - Breath light mode
    bool Enn_light_controller_breath_RColor_set(uint16_t color);
    bool Enn_light_controller_breath_GColor_set(uint16_t color);
    bool Enn_light_controller_breath_BColor_set(uint16_t color);
    bool Enn_light_controller_breath_WColor_set(uint16_t color);
    bool Enn_light_controller_breath_Wire_set(uint16_t type);
    bool Enn_light_controller_breath_light_step_set(uint16_t step);
    bool Enn_light_controller_breath_dark_step_set(uint16_t step);
    bool Enn_light_controller_breath_light_speed_set(uint16_t speed);
    bool Enn_light_controller_breath_dark_speed_set(uint16_t speed);
    // 10.7.19 MODE 1018 specific configuration
    // 10.7.20 MODE 1019 specific configuration

    // 10.8 2200 - 2229
    bool Enn_light_controller_call_execute_stored_item(uint16_t item);
    bool Enn_light_controller_get_called_item_mode_number(uint16_t *item);
    bool Enn_light_controller_get_called_item_mode_state(uint16_t *state);
    bool Enn_light_controller_callfor_backedit_item(uint16_t item);
    bool Enn_Light_controller_clear_called_item(void);
    bool Enn_light_controller_backedit_state_check(uint16_t *state);
    bool Enn_light_controller_backedit_giveup(void);
    bool Enn_light_controller_get_modeinfo_in_item(void);
    bool Enn_light_controller_set_runtime_for_called_item(uint16_t runtime);
    bool Enn_light_controller_trigger_save_item_to_store(void);
    bool Enn_light_controller_trigger_exec_item_play(void);

    // 10.10 5001 - 9800
    bool Enn_light_controller_clear_buffers(uint16_t start_buffer, uint16_t buffer_numbers);
    bool Enn_light_controller_set_buffers(uint16_t start_buffer, uint16_t buffer_numbers, uint16_t *values, uint16_t values_len);

    bool Enn_light_controller_clear_all_stored_item(void);
    bool Enn_light_controller_delete_item_from_store(uint16_t item);
    bool Enn_light_controller_set_save_powering_item(uint16_t item);
    bool Enn_light_controller_set_save_normal_item(uint16_t item);
    bool Enn_light_controller_set_save_warning_item(uint16_t item);
    bool Enn_light_controller_set_save_error_item(uint16_t item);
    void Enn_light_controller_set_save_all_item(uint16_t poweon_item, uint16_t normal_item, uint16_t warning_item, uint16_t error_item);

    void Enn_light_controller_set_current_item(uint16_t item);
    uint16_t Enn_light_controller_get_current_item(void);

    void Enn_light_controller_demo_base0_display(void);
    void Enn_light_controller_demo_base1_display(void);

private:
    std::unique_ptr<ModbusRTU> p_modbus_operator;
    std::string serial_name;
    int serial_baud_rate;
    std::string serial_checkbit;
    std::string serial_flowctl;
    std::string protocal_used;
    uint16_t serial_modbus_address_type;
    uint16_t device_addr;
    uint16_t light_poweron_item;
    uint16_t light_poweron_run;

    uint16_t light_ic_type;
    uint16_t light_channel_num;
    uint16_t light_rgb_turn;
    uint16_t light_code_time;
    uint16_t light_0code_htime;
    uint16_t light_1code_htime;
    uint16_t light_current_mode;
    uint16_t light_current_item;
    uint16_t light_backedit_mode;
    uint16_t light_ic_number;
    uint16_t light_mattric_number;

    bool connection_flag;
};



#endif 