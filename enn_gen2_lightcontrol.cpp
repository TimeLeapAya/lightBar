#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include "enn_gen2_lightcontrol.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "enn_user_define.h"

uint16_t powering_buffer_content[ENN_POWERING_BUFFER_BIN * 4 ] = {0, 255, 0, 0, 0, 223, 0, 0, 0, 191, 0, 0, 0, 159, 0, 0, 0, 127, 0, 0, 0, 95, 0, 0, 0, 63, 0, 0, 0, 31, 0, 0, 0, 10, 0, 0};

Enn_Light_Controller::Enn_Light_Controller(const std::string& device, int baud_rate): serial_name(device), serial_baud_rate(baud_rate)
{
    serial_name = device;
    serial_baud_rate = baud_rate;
    device_addr = ENN_DEFAULT_DEVICE_ADDR;
    light_ic_type = ENN_DEFAULT_IC_TYPE;
    light_channel_num = LIGHT_PARA_CHANNEL_NUMBER_3;
    light_rgb_turn = ENN_DEFAULT_RGB_ORDER;
    light_ic_number = ENN_DEFAULT_LIGHT_IC_NUMBER;
    light_mattric_number = ENN_DEFAULT_MATTRIC_NUMBER;
    light_current_item = ENN_POWERING_ITEM_STORE_NUM;
    Enn_light_get_base_config_from_file(ENN_LIGHT_CONFIGURATION_JSON_FILE);
    Enn_light_controller_init();
}

Enn_Light_Controller::Enn_Light_Controller(const std::string& device, int baud_rate, int dev_addr, uint16_t ic_type, uint16_t channel_num, uint16_t light_turn, uint16_t ic_num, uint16_t mattric_num): 
    serial_name(device), serial_baud_rate(baud_rate), device_addr(dev_addr), light_ic_type(ic_type), light_channel_num(channel_num), light_rgb_turn(light_turn), light_ic_number(ic_num), light_mattric_number(mattric_num)
{
    Enn_light_controller_init();
}

Enn_Light_Controller::~Enn_Light_Controller()
{
    Enn_light_controller_play_suspend();
    Enn_light_controller_all_light_turnoff();
}

bool Enn_Light_Controller::Enn_light_get_base_config_from_file(const std::string& filename)
{
    std::ifstream conf_file(filename);
    if(conf_file.good()){
        if(conf_file.is_open()){
            std::stringstream buffer;
            buffer << conf_file.rdbuf();
            std::string content = buffer.str();
            conf_file.close();
            if(Enn_light_get_base_config_from_string(content)){
                std::cout << "Enn light get Base config from file success." << std::endl;
                return true;
            }
            else
                std::cout << "Enn light get Base config from file failed." << std::endl;
        }
    }
    return false;
}


bool Enn_Light_Controller::Enn_light_get_spec_config_from_file(const std::string& filename)
{
    std::ifstream conf_file(filename);
    if(conf_file.good()){
        if(conf_file.is_open()){
            std::stringstream buffer;
            buffer << conf_file.rdbuf();
            std::string content = buffer.str();
            conf_file.close();
            if(Enn_light_get_spec_config_from_string(content)){
                return true;
            }
        }
    }
    return false;
}


bool Enn_Light_Controller::Enn_light_get_base_config_from_string(const std::string& content)
{
    bool ret_val = false;
    rapidjson::Document document;
    if(!document.Parse(content.c_str()).HasParseError()){
        if(document.HasMember("serailCode") && document["serailCode"].IsString()){
            serial_name = document["serailCode"].GetString();
            ret_val = true;
        }
        if(document.HasMember("baudRate") && document["baudRate"].IsInt()){
            serial_baud_rate = document["baudRate"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("checkBit") && document["checkBit"].IsString()){
            serial_checkbit = document["checkBit"].GetString();
            ret_val = true;
        }
        if(document.HasMember("modbusAddressType") && document["modbusAddressType"].IsInt()){
            serial_modbus_address_type = document["modbusAddressType"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("flowCtl") && document["flowCtl"].IsString()){
            serial_flowctl = document["flowCtl"].GetString();
            ret_val = true;
        }
        if(document.HasMember("protocol") && document["protocol"].IsString()){
            protocal_used = document["protocol"].GetString();
            ret_val = true;
        }
        if(document.HasMember("lightaddr") && document["lightaddr"].IsInt()){
            device_addr = document["lightaddr"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("powerOnItem") && document["powerOnItem"].IsInt()){
            light_poweron_item = document["powerOnItem"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("powerOnRun") && document["powerOnRun"].IsInt()){
            light_poweron_run = document["powerOnRun"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("ic_type") && document["ic_type"].IsInt()){
            light_ic_type = document["ic_type"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("ic_channel_num") && document["ic_channel_num"].IsInt()){
            light_channel_num = document["ic_channel_num"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("ic_rgb_turn") && document["ic_rgb_turn"].IsInt()){
            light_rgb_turn = document["ic_rgb_turn"].GetInt();
            ret_val = true;
        }
        // if(document.HasMember("ic_number") && document["ic_number"].IsInt()){
        //     light_ic_number = document["ic_number"].GetInt();
        // }
        // if(document.HasMember("seg_number") && document["seg_number"].IsInt()){
        //     light_mattric_number = document["seg_number"].GetInt();
        // }
    }
    return ret_val;
}

bool Enn_Light_Controller::Enn_light_get_spec_config_from_string(const std::string& content)
{
    bool ret_val = true;
    rapidjson::Document document;
    if(!document.Parse(content.c_str()).HasParseError()){
        if(document.HasMember("items") && document["items"].IsArray()){
            const rapidjson::Value& ligth_items = document["items"];
            for(uint8_t i = 0; i < ligth_items.Size(); i++){
                const rapidjson::Value& _item = ligth_items[i];
                if(_item.IsObject()){
                    if(_item.HasMember("saveItemNum") && _item["saveItemNum"].IsInt()){
                        if(_item.HasMember("modelNum") && _item["modelNum"].IsInt()){
                            if((_item["modelNum"].GetInt() == MODE_CONF_MODEL_1012_BUF_INOUT) || (_item["modelNum"].GetInt() == MODE_CONF_MODEL_1013_BUF_FLOW)){
//                                Enn_light_controller_delete_item_from_store(_item["saveItemNum"].GetInt());
                                if(Enn_light_controller_callfor_backedit_item(_item["saveItemNum"].GetInt())){
                                    if(!Enn_light_controller_select_mode(_item["modelNum"].GetInt())){
                                        std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: select mode failed..." << std::endl;
                                        return false;
                                    }
                                    if(_item.HasMember("ic_number") && _item["ic_number"].IsInt()){
                                        if(!Enn_light_controller_set_light_number(_item["ic_number"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set light number failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("seg_number") && _item["seg_number"].IsInt()){
                                        if(!Enn_light_controller_set_mattric_number(_item["seg_number"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set mattric number failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("runtimes") && _item["runtimes"].IsInt()){
                                        if(!Enn_light_controller_set_run_times(_item["runtimes"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set runtime failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("frame_delay") && _item["frame_delay"].IsInt()){
                                        if(!Enn_light_contorller_set_delay_between_frames(_item["frame_delay"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set frame delay failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("runtime_delay") && _item["runtime_delay"].IsInt()){
                                        if(!Enn_light_controller_set_delay_between_runtimes(_item["runtime_delay"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set runtime delay failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("seg_mode") && _item["seg_mode"].IsInt()){
                                        if(!Enn_light_controller_set_mattric_mode(_item["seg_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set mattric mode failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("end_mode") && _item["end_mode"].IsInt()){
                                        if(!Enn_light_controller_set_endways(_item["end_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set runtime end mode failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("start_mode") && _item["start_mode"].IsInt()){
                                        if(!Enn_light_controller_set_begin_style(_item["start_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set runtime beging style failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("start_pos") && _item["start_pos"].IsInt()){
                                        if(!Enn_light_controller_set_begin_ic(_item["start_pos"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set begin ic number failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("left_mode") && _item["left_mode"].IsInt()){
                                        if(!Enn_light_controller_set_left_color(_item["left_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set left mode failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("bg_rcolor") && _item["bg_rcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_RColor_set(_item["bg_rcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set background red color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("bg_gcolor") && _item["bg_gcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_GColor_set(_item["bg_gcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set background green color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("bg_bcolor") && _item["bg_bcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_BColor_set(_item["bg_bcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set background blue color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("buffer_bsi") && _item["buffer_bsi"].IsInt()){
                                        if(!Enn_light_controller_1012_1013_buf_start_index(_item["buffer_bsi"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set buffer BSI failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("buffer_bin") && _item["buffer_bin"].IsInt()){
                                        if(!Enn_light_controller_1012_1013_buf_inuse_number(_item["buffer_bin"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set buffer BIN failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("run_direction") && _item["run_direction"].IsInt()){
                                        if(!Enn_light_controller_1012_1013_run_direction(_item["run_direction"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set run direction failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("run_step_num") && _item["run_step_num"].IsInt()){
                                        if(!Enn_light_controller_1012_1013_step_number(_item["run_step_num"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set run step failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("buffer_pick_direction") && _item["buffer_pick_direction"].IsInt()){
                                        if(!Enn_light_controller_1012_1013_buffer_pick_direction(_item["buffer_pick_direction"].GetInt())){
                                            std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set buffer pick dirction failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("buffers") && _item["buffers"].IsArray()){
                                        const rapidjson::Value& _bufffers_array = _item["buffers"];
                                        for(uint8_t j = 0; j < _bufffers_array.Size(); j++){
                                            const rapidjson::Value& _bufffer_item = _bufffers_array[j];
                                            if(_bufffer_item.IsObject()){
                                                if(_bufffer_item.HasMember("index") && _bufffer_item["index"].IsInt()){
                                                    uint16_t buffer_content[4] = {0};
                                                    if(_bufffer_item.HasMember("rcolor") && _bufffer_item["rcolor"].IsInt()){
                                                        buffer_content[0] = _bufffer_item["rcolor"].GetInt();
                                                    }
                                                    if(_bufffer_item.HasMember("gcolor") && _bufffer_item["gcolor"].IsInt()){
                                                        buffer_content[1] = _bufffer_item["gcolor"].GetInt();
                                                    }
                                                    if(_bufffer_item.HasMember("bcolor") && _bufffer_item["bcolor"].IsInt()){
                                                        buffer_content[2] = _bufffer_item["bcolor"].GetInt();
                                                    }
                                                    if(_bufffer_item.HasMember("wcolor") && _bufffer_item["wcolor"].IsInt()){
                                                        buffer_content[3] = _bufffer_item["wcolor"].GetInt();
                                                    }
                                                    if(!Enn_light_controller_set_buffers(_bufffer_item["index"].GetInt(), 1, buffer_content, 4)){
                                                        std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: set buffer content failed..." << std::endl;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    if(!Enn_light_controller_trigger_save_item_to_store()){
                                        std::cout << "Config item(MODEL_1012 | MODEL_1013) failed: save to store failed..." << std::endl;
                                        ret_val = false;
//                                        return false;
                                    }
                                }
                            }
                            else if(_item["modelNum"].GetInt() == MODE_CONF_MODEL_1017_ALL_BREATH_1COLOR){
                                // Enn_light_controller_delete_item_from_store(_item["saveItemNum"].GetInt());
                                if(Enn_light_controller_callfor_backedit_item(_item["saveItemNum"].GetInt())){
                                    if(!Enn_light_controller_select_mode(_item["modelNum"].GetInt())){
                                        std::cout << "Config item(MODEL_1017) failed: select mode failed..." << std::endl;
                                        return false;
                                    }
                                    if(_item.HasMember("ic_number") && _item["ic_number"].IsInt()){
                                        if(!Enn_light_controller_set_light_number(_item["ic_number"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set light number failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("runtimes") && _item["runtimes"].IsInt()){
                                        if(!Enn_light_controller_set_run_times(_item["runtimes"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set runtime failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("runtime_delay") && _item["runtime_delay"].IsInt()){
                                        if(!Enn_light_controller_set_delay_between_runtimes(_item["runtime_delay"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set runtime delay failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("end_mode") && _item["end_mode"].IsInt()){
                                        if(!Enn_light_controller_set_endways(_item["end_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set runtime end mode failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("start_mode") && _item["start_mode"].IsInt()){
                                        if(!Enn_light_controller_set_begin_style(_item["start_mode"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set runtime beging style failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("start_pos") && _item["start_pos"].IsInt()){
                                        if(!Enn_light_controller_set_begin_ic(_item["start_pos"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set begin ic number failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_rcolor") && _item["breath_rcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_RColor_set(_item["breath_rcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath red color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_gcolor") && _item["breath_gcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_GColor_set(_item["breath_gcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath green color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_bcolor") && _item["breath_bcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_BColor_set(_item["breath_bcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath blue color failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_wcolor") && _item["breath_wcolor"].IsInt()){
                                        if(!Enn_light_controller_breath_WColor_set(_item["breath_wcolor"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath  Wcolor failed..." << std::endl;
                                        }
                                    }
                                    // Specific configuration
                                    if(_item.HasMember("breath_wire") && _item["breath_wire"].IsInt()){
                                        if(!Enn_light_controller_breath_Wire_set(_item["breath_wire"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath wire failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_ligth_steps") && _item["breath_ligth_steps"].IsInt()){
                                        if(!Enn_light_controller_breath_light_step_set(_item["breath_ligth_steps"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath to light steps failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_dark_steps") && _item["breath_dark_steps"].IsInt()){
                                        if(!Enn_light_controller_breath_dark_step_set(_item["breath_dark_steps"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath to dark steps failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_light_spe") && _item["breath_light_spe"].IsInt()){
                                        if(!Enn_light_controller_breath_light_speed_set(_item["breath_light_spe"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath to light speed failed..." << std::endl;
                                        }
                                    }
                                    if(_item.HasMember("breath_dark_spe") && _item["breath_dark_spe"].IsInt()){
                                        if(!Enn_light_controller_breath_dark_speed_set(_item["breath_dark_spe"].GetInt())){
                                            std::cout << "Config item(MODEL_1017) failed: set breath to dark speed failed..." << std::endl;
                                        }
                                    }
                                    // Save configuration
                                    if(!Enn_light_controller_trigger_save_item_to_store()){
                                        std::cout << "Config item(MODEL_1017) failed: save to store failed..." << std::endl;
                                        ret_val = false;
                                    }
                                }
                            }
                            else{
                                std::cout << "Enn light spec config JSON string has a unsupported item..." << std::endl;
                            }
                        }
                    }
                }
                else{
                    std::cout << "Enn light spec config JSON string invalid item object..." << std::endl;
                }
            }
        }
        else{
            std::cout << "Enn light spec config JSON string has no valid item information..." << std::endl;
            ret_val = false;
        }
    }
    else{
        std::cout << "Enn light spec config JSON string parse failed..." << std::endl;
        ret_val = false;
    }
    return ret_val;
}


bool Enn_Light_Controller::Enn_light_save_base_config_to_file(const std::string& filename, const std::string& content_str)
{
    bool ret_val = false;
    std::ifstream conf_file(filename);
    if(conf_file.good()){
        if(conf_file.is_open()){
            std::stringstream buffer;
            buffer << conf_file.rdbuf();
            std::string content = buffer.str();
            conf_file.close();
            if(content.compare(content_str)){
                rapidjson::Document document;
                if(!document.Parse(content.c_str()).HasParseError()){
                    if(document.HasMember("serailCode") && document["serailCode"].IsString()){
                        if(serial_name.compare(document["serailCode"].GetString()) && !serial_name.empty()){
                            document["serailCode"].SetString(serial_name.c_str(), document.GetAllocator());
                        }
                    }
                    else{
                        if(!serial_name.empty()){
                            rapidjson::Value key("serailCode", document.GetAllocator());
                            rapidjson::Value value(serial_name.c_str(), document.GetAllocator());
                            document.AddMember(key, value, document.GetAllocator());
                        }
                    }
                    if(document.HasMember("baudRate") && document["baudRate"].IsInt()){
                        if((serial_baud_rate != document["baudRate"].GetInt())){
                            document["baudRate"].SetInt(serial_baud_rate);
                        }
                    }
                    else{
                            rapidjson::Value key("baudRate", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(serial_baud_rate);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("checkBit") && document["checkBit"].IsString()){
                        if(serial_checkbit.compare(document["checkBit"].GetString()) && !serial_checkbit.empty()){
                            document["checkBit"].SetString(serial_checkbit.c_str(), document.GetAllocator());
                        }
                    }
                    else{
                        if(!serial_checkbit.empty()){
                            rapidjson::Value key("checkBit", document.GetAllocator());
                            rapidjson::Value value(serial_checkbit.c_str(), document.GetAllocator());
                            document.AddMember(key, value, document.GetAllocator());
                        }
                    }
                    if(document.HasMember("modbusAddressType") && document["modbusAddressType"].IsInt()){
                        if((serial_modbus_address_type != document["modbusAddressType"].GetInt())){
                            document["modbusAddressType"].SetInt(serial_modbus_address_type);
                        }
                    }
                    else{
                            rapidjson::Value key("modbusAddressType", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(serial_modbus_address_type);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("flowCtl") && document["flowCtl"].IsString()){
                        if(serial_flowctl.compare(document["flowCtl"].GetString()) && !serial_flowctl.empty()){
                            document["flowCtl"].SetString(serial_flowctl.c_str(), document.GetAllocator());
                        }
                    }
                    else{
                        if(!serial_flowctl.empty()){
                            rapidjson::Value key("flowCtl", document.GetAllocator());
                            rapidjson::Value value(serial_flowctl.c_str(), document.GetAllocator());
                            document.AddMember(key, value, document.GetAllocator());
                        }
                    }
                    if(document.HasMember("protocol") && document["protocol"].IsString()){
                        if(protocal_used.compare(document["protocol"].GetString()) && !protocal_used.empty()){
                            document["protocol"].SetString(protocal_used.c_str(), document.GetAllocator());
                        }
                    }
                    else{
                        if(!protocal_used.empty()){
                            rapidjson::Value key("protocol", document.GetAllocator());
                            rapidjson::Value value(protocal_used.c_str(), document.GetAllocator());
                            document.AddMember(key, value, document.GetAllocator());
                        }
                    }
                    if(document.HasMember("lightaddr") && document["lightaddr"].IsInt()){
                        if((device_addr != document["lightaddr"].GetInt())){
                            document["lightaddr"].SetInt(device_addr);
                        }
                    }
                    else{
                            rapidjson::Value key("lightaddr", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(device_addr);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("powerOnItem") && document["powerOnItem"].IsInt()){
                        if((light_poweron_item != document["powerOnItem"].GetInt())){
                            document["powerOnItem"].SetInt(light_poweron_item);
                        }
                    }
                    else{
                            rapidjson::Value key("powerOnItem", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(light_poweron_item);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("powerOnRun") && document["powerOnRun"].IsInt()){
                        if((light_poweron_run != document["powerOnRun"].GetInt())){
                            document["powerOnRun"].SetInt(light_poweron_run);
                        }
                    }
                    else{
                            rapidjson::Value key("powerOnRun", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(light_poweron_run);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("ic_type") && document["ic_type"].IsInt()){
                        if((light_ic_type != document["ic_type"].GetInt())){
                            document["ic_type"].SetInt(light_ic_type);
                        }
                    }
                    else{
                            rapidjson::Value key("ic_type", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(light_ic_type);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("ic_channel_num") && document["ic_channel_num"].IsInt()){
                        if((light_channel_num != document["ic_channel_num"].GetInt())){
                            document["ic_channel_num"].SetInt(light_channel_num);
                        }
                    }
                    else{
                            rapidjson::Value key("ic_channel_num", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(light_channel_num);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    if(document.HasMember("ic_rgb_turn") && document["ic_rgb_turn"].IsInt()){
                        if((light_rgb_turn != document["ic_rgb_turn"].GetInt())){
                            document["ic_rgb_turn"].SetInt(light_rgb_turn);
                        }
                    }
                    else{
                            rapidjson::Value key("ic_rgb_turn", document.GetAllocator());
                            rapidjson::Value value;
                            value.SetInt(light_rgb_turn);
                            document.AddMember(key, value, document.GetAllocator());
                    }
                    rapidjson::StringBuffer w_buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> w_writer(w_buffer);
                    document.Accept(w_writer);
                    std::string write_back_content = w_buffer.GetString();
                    std::ofstream conf_w_file(filename, std::ios::trunc);
                    if (!conf_w_file.is_open()) {
                        std::cerr << "Failed to open file: " << filename << std::endl;
                        return false;
                    }
                    conf_w_file << write_back_content;
                    conf_w_file.close();
                    return true;
                }
            }
            else{
                std::cout << "Enn light configuration content same with content save in file..." << std::endl;
                return true;
            }
            conf_file.close();
        }
    }
    return false;
}


bool Enn_Light_Controller::Enn_light_serial_check(const std::string& filename)
{
    std::ifstream infile(filename);
    return infile.good();
}

bool Enn_Light_Controller::Enn_light_serial_baudrate_check(int baud_rate)
{
    const std::vector<int> validBaudRates = {0, 1, 2, 3, 4, 5, 6, 7};
    if(std::find(validBaudRates.begin(), validBaudRates.end(), baud_rate) != validBaudRates.end()){
        return true;
    }
    else
        return false;
}

bool Enn_Light_Controller::Enn_light_controller_addr_check(int dev_addr)
{
    if((dev_addr > 0) && (dev_addr < 248)){
        return true;
    }
    else
        return false;
}

bool Enn_Light_Controller::Enn_light_controller_init(void)
{
    connection_flag = false;
    if(!Enn_light_serial_check(serial_name)){
        std::cerr << "Serial Port: " << serial_name << " invalid, please check." << std::endl;
        return false;
    }
    if(!Enn_light_serial_baudrate_check(serial_baud_rate)){
        std::cerr << "Serial Port baudrate: " << serial_baud_rate << " invalid, use default Serial port baudrate: " << ENN_DEFAULT_SERIAL_BAUDRATE << std::endl;
        serial_baud_rate = ENN_DEFAULT_SERIAL_BAUDRATE;
    }
    if(!Enn_light_controller_addr_check(device_addr)){
        std::cerr << "Light Controller addr: " << device_addr << " invalid, use default device addr: " << ENN_DEFAULT_DEVICE_ADDR << std::endl;
        device_addr = ENN_DEFAULT_DEVICE_ADDR;
    }
    
    p_modbus_operator = std::make_unique<ModbusRTU>(serial_name, serial_baud_rate);

    if(!p_modbus_operator->connect()){
        std::cerr << "Enn Modbus connect failed." << std::endl;
        return false;
//        throw std::runtime_error("Enn Modbus connect failed.");
    }
    else{
        std::cout << "Enn Modbus connect success." << std::endl;
        connection_flag = true;
        uint16_t ver;
        Enn_light_controller_get_hw_ver(&ver);
        std::cout << "Hardware version: "<< ver << std::endl;
        Enn_light_controller_get_sw_ver(&ver);
        std::cout << "Software version: "<< ver << std::endl;
        Enn_light_controller_select_ic_type(light_ic_type);
        Enn_light_controller_set_channel_num(light_channel_num);
        Enn_light_controller_set_light_number(light_ic_number);
        Enn_light_controller_set_rgb_turn(light_rgb_turn);

        if(Enn_light_controller_get_current_item(&light_current_mode)){
            std::cout << "Get current running mode(" << light_current_mode << ") success." << std::endl;
        }
        else
            std::cout << "Get current running mode failed." << std::endl; 
    }
    return true;
}


// 10.1 1000
bool Enn_Light_Controller::Enn_light_device_addr_config(uint16_t dev_addr)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_DEVICE_ADDR - 1, dev_addr)){
        std::cout << "Config Light controller device address(" << dev_addr << ") success." << std::endl;
        device_addr = dev_addr;
        return true;
    }
    else{
        std::cout << "Config Light controller device address(" << dev_addr << ")  failed." << std::endl;
        return false;
    }
}


// 10.1 1001 1003
bool Enn_Light_Controller::Enn_light_device_serial_config(uint16_t baud_rate, uint16_t parity_check)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_BAUDRATE_ADDR - 1, baud_rate)){
        std::cout << "Config Light controller serial baudrate(" << baud_rate << ") success." << std::endl;
        serial_baud_rate = baud_rate;
    }
    else{
        std::cout << "Config Light controller serial baudrate(" << baud_rate << ") failed." << std::endl;
        return false;
    }

    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_PARITYCK_ADDR - 1, parity_check)){
        std::cout << "Config Light controller serial parity check(" << parity_check << ") success." << std::endl;
        serial_baud_rate = baud_rate;
    }
    else{
        std::cout << "Config Light controller serial parity check(" << parity_check << ") failed." << std::endl;
        return false;
    }
    return true;
}


// 10.1 1004
bool Enn_Light_Controller::Enn_light_device_poweron_play_item(uint16_t booton_item)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_POWERON_PLAY_ADDR - 1, booton_item)){
        std::cout << "Config Light controller poweron play item(" << booton_item << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Config Light controller poweron play item(" << booton_item << ") failed." << std::endl;
        return false;
    }
}


// 10.1 1007
bool Enn_Light_Controller::Enn_light_device_poweron_mode(uint16_t mode)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_POWERON_MODE_ADDR - 1, mode)){
        std::cout << "Config Light controller poweron mode(" << mode << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Config Light controller poweron mode(" << mode << ") failed." << std::endl;
        return false;
    }
}

// 10.1 1013
bool Enn_Light_Controller::Enn_light_device_controller_factory_recovery(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_FACTORY_ADDR - 1, CONTROLLER_PARA_FACTORY_PARA1)){
        if(p_modbus_operator->writeSingleRegister(device_addr, CONTROLLER_PARA_ADDR_FACTORY_ADDR - 1, CONTROLLER_PARA_FACTORY_PARA2)){
            std::cout << "Config Light controller execute factory recovery success, please donot cut power within 1 minite..." << std::endl;
            return true;
        }
    }
    return false;
}


// 10.2 2000
bool Enn_Light_Controller::Enn_light_controller_select_ic_type(uint16_t type)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, LIGHT_PARA_ADDR_IC_TYPE_ADDR - 1, type)){
        std::cout << "Config Light controller IC type(" << type << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Config Light controller IC type(" << type << ") failed." << std::endl;
        return false;
    }
}


// 10.2 2001
bool Enn_Light_Controller::Enn_Light_Controller::Enn_light_controller_set_channel_num(uint16_t channel_num)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, LIGHT_PARA_ADDR_CHANNEL_NUM_ADDR - 1, channel_num)){
        std::cout << "Config Light controller channel number(" << channel_num << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Config Light controller channel number(" << channel_num << ") failed." << std::endl;
        return false;
    }
}


// 10.2 2002
bool Enn_Light_Controller::Enn_light_controller_set_rgb_turn(uint rgb_turn)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, LIGHT_PARA_ADDR_COLOR_ORDER_ADDR - 1, rgb_turn)){
        std::cout << "Config Light controller light RGB turn(" << rgb_turn << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Config Light controller light RGB turn(" << rgb_turn << ") failed." << std::endl;
        return false;
    }
}


// 10.2 2007
bool Enn_Light_Controller::Enn_light_controller_set_code_time(uint16_t code_time)
{
    return true;
}


// 10.2 2008
bool Enn_Light_Controller::Enn_light_controller_set_code0_htime(uint16_t code_htime)
{
    return true;
}


// 10.2 2009
bool Enn_Light_Controller::Enn_light_controller_set_code1_htime(uint16_t code_htime)
{
    return true;
}


// 10.3 1000
bool Enn_Light_Controller::Enn_light_controller_reset_fault_flags(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_RESET_FAULT_ADDR - 1, 1)){
        std::cout << "Light controller reset fault flags success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller reset fault flags failed." << std::endl;
        return false;
    }
}

// 10.3 1001
bool Enn_Light_Controller::Enn_light_controller_reset_all_configuration(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_RESET_MODE_ADDR - 1, 1)){
        std::cout << "Light controller reset all configuration success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller reset all configuration failed." << std::endl;
        return false;
    }
}


// 10.3 1002
bool Enn_Light_Controller::Enn_light_controller_play_suspend(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_PLAY_SUSPEND_ADDR - 1, 1)){
        std::cout << "Light controller play suspend success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller play suspend failed." << std::endl;
        return false;
    }
}

// 10.3 1003
bool Enn_Light_Controller::Enn_light_controller_play_stop(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_PLAY_STOP_ADDR - 1, 1)){
        std::cout << "Light controller play stop success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller play stop failed." << std::endl;
        return false;
    }
}

// 10.3 1004
bool Enn_Light_Controller::Enn_light_controller_play_start(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_PLAY_START_ADDR - 1, 1)){
        std::cout << "Light controller play start success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller play start failed." << std::endl;
        return false;
    }
}


// 10.3 1005
bool Enn_Light_Controller::Enn_light_controller_reload_play_item(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_RELOAD_PLAY_ADDR - 1, 1)){
        std::cout << "Light controller play item reload success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller play item reload failed." << std::endl;
        return false;
    }
}

// 10.3 1006
bool Enn_Light_Controller::Enn_light_controller_play_restart(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_RESTART_PLAY_ADDR - 1, 1)){
        std::cout << "Light controller play item restart success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller play item restart failed." << std::endl;
        return false;
    }
}

// 10.3 1008
bool Enn_Light_Controller::Enn_light_controller_all_light_turnoff(void)
{
    if(p_modbus_operator->writeSingleCoil(device_addr, COIL_PARA_QUICK_TURNOFF_ADDR - 1, 1)){
        std::cout << "Light controller light all turn-off success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller light all turn-off failed." << std::endl;
        return false;
    }
}


// 10.4 1002
bool Enn_Light_Controller::Enn_light_controller_play_is_suspend(void)
{
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_SUSPEND_ADDR - 1, 1, &state)){
        std::cout << "Light controller suspend-state check success." << std::endl;
        if(state){
            return true;
        }
        else
            return false;
    }
    else{
        std::cout << "Light controller suspend-state check failed." << std::endl;
        return false;
    }
}

// 10.4 1003
bool Enn_Light_Controller::Enn_light_controller_play_is_stop(void)
{
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_STOP_ADDR - 1, 1, &state)){
        std::cout << "Light controller stop-state check success." << std::endl;
        if(state){
            return true;
        }
        else
            return false;
    }
    else{
        std::cout << "Light controller stop-state check failed." << std::endl;
        return false;
    }
}

// 10.4 1004
bool Enn_Light_Controller::Enn_light_controller_play_is_running(void)
{
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_RUN_ADDR - 1, 1, &state)){
        std::cout << "Light controller running-state check success." << std::endl;
        if(state){
            return true;
        }
        else
            return false;
    }
    else{
        std::cout << "Light controller running-state check failed." << std::endl;
        return false;
    }
}

// 10.4 1007
uint8_t Enn_Light_Controller::Enn_light_controller_get_FIN0_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_FIN0_ADDR - 1, 1, &state)){
        std::cout << "Light controller get FIN0 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get FIN0 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1008
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN0_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN0_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN0 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN0 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1009
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN1_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN1_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN1 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN1 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1010
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN2_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN2_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN2 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN2 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1011
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN3_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN3_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN3 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN3 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1012
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN4_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN4_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN4 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN4 state failed." << std::endl;
    }
    return input_state;
}

// 10.4 1013
uint8_t Enn_Light_Controller::Enn_light_controller_get_IN5_state(void)
{
    uint8_t input_state = INPUT_PARA_UNKNOW;
    uint8_t state = 0;
    if(p_modbus_operator->readDiscreteInputs(device_addr, DISCRETE_INPUT_PARA_IN5_ADDR - 1, 1, &state)){
        std::cout << "Light controller get IN5 state success." << std::endl;
        input_state = state;
    }
    else{
        std::cout << "Light controller get IN5 state failed." << std::endl;
    }
    return input_state;
}


// 10.5 1000
bool Enn_Light_Controller::Enn_light_controller_get_hw_ver(uint16_t *ver)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_HW_VER_ADDR - 1 , 1, ver)){
        std::cout << "Light controller get hardware version success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get hardware version failed." << std::endl;
        return false;
    }
}


// 10.5 1001
bool Enn_Light_Controller::Enn_light_controller_get_sw_ver(uint16_t *ver)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_SW_VER_ADDR - 1, 1, ver)){
        std::cout << "Light controller get software version success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get software version failed." << std::endl;
        return false;
    }
}

// 10.5 1002
bool Enn_Light_Controller::Enn_light_controller_get_newest_item(uint16_t *item)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_NEWEST_PLAY_ADDR - 1, 1, item)){
        std::cout << "Light controller get newest play item success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get newest play item failed." << std::endl;
        return false;
    }
}

// 10.5 1003
bool Enn_Light_Controller::Enn_light_controller_get_current_item(uint16_t *item)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_CURRENT_PLAY_ADDR - 1, 1, item)){
        std::cout << "Light controller get current play item success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get current play item failed." << std::endl;
        return false;
    }
}

// 10.5 1009
bool Enn_Light_Controller::Enn_light_controller_get_faultcode(uint16_t *faultcode)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_FAULTCODE_ADDR - 1, 1, faultcode)){
        std::cout << "Light controller get fault code success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get fault code failed." << std::endl;
        return false;
    }
}

// 10.5 1011
bool Enn_Light_Controller::Enn_light_controller_get_play_isend(uint16_t *endstate)
{
    if(p_modbus_operator->readInputRegisters(device_addr, INPUT_REGISTER_PARA_PLAY_END_ADDR - 1, 1, endstate)){
        std::cout << "Light controller get end state success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get end state failed." << std::endl;
        return false;
    }
}


// 10.7 2100
bool Enn_Light_Controller::Enn_light_controller_select_mode(uint16_t mode)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_SEL_ADDR - 1, mode)){
        std::cout << "Light controller select mode(: "<< mode << ") success." << std::endl;
        light_current_mode = mode;
        return true;
    }
    else{
        std::cout << "Light controller select mode(: "<< mode << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2102
bool Enn_Light_Controller::Enn_light_controller_backgroud_edit_mode(uint16_t mode)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_BACKGROUD_EDIT_ADDR - 1, mode)){
        std::cout << "Light controller select backgroud edit mode(: "<< mode << ") success." << std::endl;
        light_backedit_mode = mode;
        return true;
    }
    else{
        std::cout << "Light controller select backgroud edit mode(: "<< mode << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2103
bool Enn_Light_Controller::Enn_light_controller_trigger_edit_mode(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_BACK_EDIT_TRIG_ADDR - 1, 1)){
        std::cout << "Light controller trig backgroud edit mode:("<< light_backedit_mode << ") success." << std::endl;
        light_current_mode = light_backedit_mode;
        light_backedit_mode = 0;
        return true;
    }
    else{
        std::cout << "Light controller trig backgroud edit mode:("<< light_backedit_mode << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2104
bool Enn_Light_Controller::Enn_light_controller_set_light_number(uint16_t number)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_LIGHT_IC_NUM_ADDR - 1, number)){
        std::cout << "Light controller set IC number("<< number << ") success." << std::endl;
        light_ic_number = number;
        return true;
    }
    else{
        std::cout << "Light controller set IC number("<< number << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2105
bool Enn_Light_Controller::Enn_light_controller_set_mattric_number(uint16_t number)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_MATRIC_SEG_NUM_ADDR - 1, number)){
        std::cout << "Light controller set mattric number("<< number << ") success." << std::endl;
        light_mattric_number = number;
        return true;
    }
    else{
        std::cout << "Light controller set mattric number("<< number << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2106
bool Enn_Light_Controller::Enn_light_controller_set_run_times(uint16_t times)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_RUN_TIMES_ADDR - 1, times)){
        std::cout << "Light controller set run times("<< times << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set run times("<< times << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2107
bool Enn_Light_Controller::Enn_light_contorller_set_delay_between_frames(uint16_t delay)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_FRAMES_DELAY_ADDR - 1, delay)){
        std::cout << "Light controller set delay between frames("<< delay << "ms) success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set delay between frames("<< delay << "ms) failed." << std::endl;
        return false;
    }
}

// 10.7 2108
bool Enn_Light_Controller::Enn_light_controller_set_delay_between_runtimes(uint16_t delay)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_RUNTIME_DELAY_ADDR - 1, delay)){
        std::cout << "Light controller set delay between runtimes("<< delay << "*100ms) success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set delay between runtimes("<< delay << "*100ms) failed." << std::endl;
        return false;
    }
}

// 10.7 2109
bool Enn_Light_Controller::Enn_light_controller_set_mattric_mode(uint16_t mode)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_MATRIC_MOE_ADDR - 1, mode)){
        std::cout << "Light controller set mattric mode("<< mode << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set mattric mode("<< mode << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2111
bool Enn_Light_Controller::Enn_light_controller_set_endways(uint16_t ways)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_END_METHOD_ADDR - 1, ways)){
        std::cout << "Light controller set End method("<< ways << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set End method("<< ways << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2112
bool Enn_Light_Controller::Enn_light_controller_set_begin_style(uint16_t style)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_INIT_LIGHT_ADDR - 1, style)){
        std::cout << "Light controller set Begin style("<< style << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set Begin style("<< style << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2117
bool Enn_Light_Controller::Enn_light_controller_set_begin_ic(uint16_t begin_ic)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_MODE_BEGIN_POSTION_ADDR - 1, begin_ic)){
        std::cout << "Light controller set Begin IC position("<< begin_ic << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set Begin IC position("<< begin_ic << ") failed." << std::endl;
        return false;
    }
}

// 10.7 2118
bool Enn_Light_Controller::Enn_light_controller_set_left_color(uint16_t color)
{
    return true;
}


// 10.7.13 MODE 1012 specific configuration
// 10.7.13 2129
bool Enn_Light_Controller::Enn_light_controller_1012_1013_buf_start_index(uint16_t index)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_WATERLIGHT_BUFFER_INOUT_BSI_ADDR - 1, index)){
        std::cout << "Light controller 1012/1013 MODE Set start buffer index("<< index << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller 1012/1013 MODE Set start buffer index("<< index << ") failed." << std::endl;
        return false;
    }
}

// 10.7.13 2130
bool Enn_Light_Controller::Enn_light_controller_1012_1013_buf_inuse_number(uint16_t number)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_WATERLIGHT_BUFFER_INOUT_BIN_ADDR - 1, number)){
        std::cout << "Light controller 1012/1013 MODE Set buffer number("<< number << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller 1012/1013 MODE Set buffer number("<< number << ") failed." << std::endl;
        return false;
    }
}

// 10.7.13 2131
bool Enn_Light_Controller::Enn_light_controller_1012_1013_run_direction(uint16_t direction)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_WATERLIGHT_BUFFER_INOUT_MOVE_DIRECTION_ADDR - 1, direction)){
        std::cout << "Light controller 1012/1013 MODE Set Run direction("<< direction << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller 1012/1013 MODE Set Run direction("<< direction << ") failed." << std::endl;
        return false;
    }
}

// 10.7.13 2132
bool Enn_Light_Controller::Enn_light_controller_1012_1013_step_number(uint16_t step)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_WATERLIGHT_BUFFER_INOUT_MOVE_STEP_ADDR - 1, step)){
        std::cout << "Light controller 1012/1013 MODE Set step("<< step << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller 1012/1013 MODE Set step("<< step << ") failed." << std::endl;
        return false;
    }
}

// 10.7.13 2133
bool Enn_Light_Controller::Enn_light_controller_1012_1013_buffer_pick_direction(uint16_t dirction)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_WATERLIGHT_BUFFER_INOUT_BUFF_PICK_ADDR - 1, dirction)){
        std::cout << "Light controller 1012/1013 MODE Set pick direction("<< dirction << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller 1012/1013 MODE Set pick direction("<< dirction << ") failed." << std::endl;
        return false;
    }
}


// 10.7.18 Breath light mode specific configuration
// 10.7.18 2125
bool Enn_Light_Controller::Enn_light_controller_breath_RColor_set(uint16_t color)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_RCOLOR_SET_ADDR - 1, color)){
        std::cout << "Light controller set Red color("<< color << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set Red color("<< color << ") failed." << std::endl;
        return false;
    }
}

// 10.7.19 2126
bool Enn_Light_Controller::Enn_light_controller_breath_GColor_set(uint16_t color)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_GCOLOR_SET_ADDR - 1, color)){
        std::cout << "Light controller set Green color("<< color << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set Green color("<< color << ") failed." << std::endl;
        return false;
    }
}

// 10.7.20 2127
bool Enn_Light_Controller::Enn_light_controller_breath_BColor_set(uint16_t color)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_BCOLOR_SET_ADDR - 1, color)){
        std::cout << "Light controller set Blue color("<< color << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set Blue color("<< color << ") failed." << std::endl;
        return false;
    }
}

// 10.7.20 2128
bool Enn_Light_Controller::Enn_light_controller_breath_WColor_set(uint16_t color)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_WCOLOR_SET_ADDR - 1, color)){
        std::cout << "Light controller set W color("<< color << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set W color("<< color << ") failed." << std::endl;
        return false;
    }
}

// 10.7.21 2129
bool Enn_Light_Controller::Enn_light_controller_breath_Wire_set(uint16_t type)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_WIRE_MODE_SET_ADDR - 1, type)){
        std::cout << "Light controller set wire type("<< type << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set wire type("<< type << ") failed." << std::endl;
        return false;
    }
}

// 10.7.22 2130
bool Enn_Light_Controller::Enn_light_controller_breath_light_step_set(uint16_t step)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_STEP_LIGHT_SET_ADDR - 1, step)){
        std::cout << "Light controller set breath to light step("<< step << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set breath to light step("<< step << ") failed." << std::endl;
        return false;
    }
}

// 10.7.23 2131
bool Enn_Light_Controller::Enn_light_controller_breath_dark_step_set(uint16_t step)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_STEP_DARK_SET_ADDR - 1, step)){
        std::cout << "Light controller set breath to dark step("<< step << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set breath to dark step("<< step << ") failed." << std::endl;
        return false;
    }
}

// 10.7.24 2132
bool Enn_Light_Controller::Enn_light_controller_breath_light_speed_set(uint16_t speed)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_SPEED_LIGHT_SET_ADDR - 1, speed)){
        std::cout << "Light controller set breath to light speed("<< speed << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set breath to light step("<< speed << ") failed." << std::endl;
        return false;
    }
}

// 10.7.25 2133
bool Enn_Light_Controller::Enn_light_controller_breath_dark_speed_set(uint16_t speed)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, MODE_CONF_BREATH_SPEED_DARK_SET_ADDR - 1, speed)){
        std::cout << "Light controller set breath to dark speed("<< speed << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set breath to dark step("<< speed << ") failed." << std::endl;
        return false;
    }
}


// 10.8.1 2200
bool Enn_Light_Controller::Enn_light_controller_call_execute_stored_item(uint16_t item)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALL_EXEC_FROM_STORE_ADDR - 1, item)){
        std::cout << "Light controller call && execute stored item("<< item << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller call && execute stored item("<< item << ") failed." << std::endl;
        return false;
    }
}

// 10.8.2 2201
bool Enn_Light_Controller::Enn_light_controller_get_called_item_mode_number(uint16_t *item)
{
    if(p_modbus_operator->readHoldingRegisters(device_addr, ITEM_CALLED_FROM_STORE_MODE_ADDR - 1, 1, item)){
        std::cout << "Light controller get stored item mode number("<< *item << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get stored item mode number failed." << std::endl;
        return false;
    }
}

// 10.8.3 2202
bool Enn_Light_Controller::Enn_light_controller_get_called_item_mode_state(uint16_t *state)
{
    if(p_modbus_operator->readHoldingRegisters(device_addr, ITEM_CALLED_HAS_VALID_MODE_ADDR - 1, 1, state)){
        std::cout << "Light controller get stored item mode state("<< *state << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get stored item mode state failed." << std::endl;
        return false;
    }
}

// 10.8.4 2204
bool Enn_Light_Controller::Enn_light_controller_callfor_backedit_item(uint16_t item)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALLED_BACKEDIT_ITEM_ADDR - 1, item)){
        std::cout << "Light controller call backedit item("<< item << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller call backedit item("<< item << ") failed." << std::endl;
        return false;
    }
}

// 10.8.5 2205
bool Enn_Light_Controller::Enn_Light_controller_clear_called_item(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALLED_DELETE_FROM_STORE_ADDR - 1, 1)){
        std::cout << "Light controller clear called item from stored success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller clear called item from stored failed." << std::endl;
        return false;
    }
}

// 10.8.6 2206 
bool Enn_Light_Controller::Enn_light_controller_backedit_state_check(uint16_t *state)
{
    if(p_modbus_operator->readHoldingRegisters(device_addr, ITEM_IS_IN_BACKEDIT_MODE_ADDR - 1, 1, state)){
        std::cout << "Light controller get backedit state("<< *state << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get backedit state failed." << std::endl;
        return false;
    }
}

// 10.8.7 2206
bool Enn_Light_Controller::Enn_light_controller_backedit_giveup(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_IS_IN_BACKEDIT_MODE_ADDR - 1, 0)){
        std::cout << "Light controller give up backedit success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller give up backedit failed." << std::endl;
        return false;
    }
}

// 10.8.8 2207
bool Enn_Light_Controller::Enn_light_controller_get_modeinfo_in_item(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_GET_MODEINFO_IN_ITEM_ADDR - 1, 1)){
        std::cout << "Light controller get mode info in item success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller get mode info in item failed." << std::endl;
        return false;
    }
}

// 10.8.9 2208
bool Enn_Light_Controller::Enn_light_controller_set_runtime_for_called_item(uint16_t runtime)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALLED_RUNTIME_SET_ADDR - 1, runtime)){
        std::cout << "Light controller set runtime("<< runtime << ") success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller set runtime("<< runtime << ") failed." << std::endl;
        return false;
    }
}

// 10.8.10 2228
bool Enn_Light_Controller::Enn_light_controller_trigger_save_item_to_store(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALLED_SAVE_TO_STORE_ADDR - 1, 1)){
        std::cout << "Light controller trigger save item to store success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller trigger save item to store failed." << std::endl;
        return false;
    }
}

// 10.8.11 2229
bool Enn_Light_Controller::Enn_light_controller_trigger_exec_item_play(void)
{
    if(p_modbus_operator->writeSingleRegister(device_addr, ITEM_CALLED_EXEC_ADDR - 1, 1)){
        std::cout << "Light controller trigger exec item play success." << std::endl;
        return true;
    }
    else{
        std::cout << "Light controller trigger exec item play failed." << std::endl;
        return false;
    }
}


// 10.10 5001 - 9800
bool Enn_Light_Controller::Enn_light_controller_clear_buffers(uint16_t start_buffer, uint16_t buffer_numbers)
{
    for(uint16_t i = start_buffer; i < buffer_numbers; i++){
        for(uint8_t j = 0; j < 4; j++)
        {
            if(p_modbus_operator->writeSingleRegister(device_addr, (BUFFER_AREA_START_ADDR + (i -1)*4 + j - 1), 0)){
                std::cout << "Light controller clear buffer(addr:" <<  (BUFFER_AREA_START_ADDR + (i -1)*4 + j) << "Success." << std::endl;
            }
            else{
                std::cout << "Light controller clear buffer(addr:" <<  (BUFFER_AREA_START_ADDR + (i -1)*4 + j) << "failed." << std::endl;
            }
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_set_buffers(uint16_t start_buffer, uint16_t buffer_numbers, uint16_t *values, uint16_t values_len)
{
    uint16_t *p_value = values;
    for(uint16_t i = 0; i < buffer_numbers; i++){
        for(uint8_t j = 0; j < values_len; j++)
        {
            if(p_modbus_operator->writeSingleRegister(device_addr, (BUFFER_AREA_START_ADDR + (start_buffer + i - 1)*4 + j - 1), *(p_value + j))){
                std::cout << "Light controller write buffer(addr:" <<  (BUFFER_AREA_START_ADDR + (start_buffer + i -1)*4 + j) << ") Value: " << *(p_value + j) << " Success." << std::endl;
            }
            else{
                std::cout << "Light controller write buffer(addr:" <<  (BUFFER_AREA_START_ADDR + (start_buffer + i -1)*4 + j) << ") failed." << std::endl;
            }
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_clear_all_stored_item(void)
{
    uint16_t state;
    for(int i = ENN_STORED_ITEM_FIRST; i < ENN_STORED_ITEM_TOTAL_NUMBER; i++){
        if(Enn_light_controller_callfor_backedit_item(i)){
            if(Enn_light_controller_get_called_item_mode_state(&state)){
                if(state){
                    Enn_Light_controller_clear_called_item();
                }
            }
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_delete_item_from_store(uint16_t item)
{
    uint16_t state;
    if(Enn_light_controller_callfor_backedit_item(item)){
        if(Enn_light_controller_get_called_item_mode_state(&state)){
            if(state){
                Enn_Light_controller_clear_called_item();
            }
            return true;
        }
    }
    return false;
}


bool Enn_Light_Controller::Enn_light_controller_set_save_powering_item(uint16_t item)
{
    Enn_light_controller_delete_item_from_store(item);
    if(Enn_light_controller_callfor_backedit_item(item)){
        if(!Enn_light_controller_select_mode(ENN_POWERING_MODE_TEMPLATE_NUM)){
            std::cout << "Set & Save powering item failed: select mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_light_number(ENN_POWERING_LIGHT_IC_NUMBER)){
            std::cout << "Set & Save powering item failed: set light number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_mattric_number(ENN_POWERING_LIGHT_MATTRIC_NUMBER)){
            std::cout << "Set & Save powering item failed: set mattric number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_run_times(MODE_CONF_RUN_TIME_ALWAYS)){
            std::cout << "Set & Save powering item failed: set runtime failed..." << std::endl;
            return false;
        }
        if(!Enn_light_contorller_set_delay_between_frames(ENN_POWERING_FRAME_DELAY)){
            std::cout << "Set & Save powering item failed: set frame delay failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_delay_between_runtimes(ENN_POWERING_RUNTIME_DELAY)){
            std::cout << "Set & Save powering item failed: set runtime delay failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_mattric_mode(ENN_POWERING_MATTRIC_MODE)){
            std::cout << "Set & Save powering item failed: set mattric mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_endways(ENN_POWERING_RUNTIME_END_MODE)){
            std::cout << "Set & Save powering item failed: set runtime end mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_style(ENN_POWERING_INIT_LIGHT_MODE)){
            std::cout << "Set & Save powering item failed: set runtime beging style failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_ic(ENN_POWERING_BEGIN_IC_NUMBER)){
            std::cout << "Set & Save powering item failed: set begin ic number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_RColor_set(ENN_POWERING_BACKGROUND_RCOLOR)){
            std::cout << "Set & Save powering item failed: set background red color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_GColor_set(ENN_POWERING_BACKGROUND_GCOLOR)){
            std::cout << "Set & Save powering item failed: set background green color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_BColor_set(ENN_POWERING_BACKGROUND_BCOLOR)){
            std::cout << "Set & Save powering item failed: set background blue color failed..." << std::endl;
            return false;
        }
        // Specific configuration
        if(!Enn_light_controller_1012_1013_buf_start_index(ENN_POWERING_BUFFER_BSI)){
            std::cout << "Set & Save powering item failed: set buffer BSI failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_1012_1013_buf_inuse_number(ENN_POWERING_BUFFER_BIN)){
            std::cout << "Set & Save powering item failed: set buffer BIN failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_1012_1013_run_direction(ENN_POWERING_RUNING_DIRCETION)){
            std::cout << "Set & Save powering item failed: set run direction failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_1012_1013_step_number(ENN_POWERING_STEP_NUMBER)){
            std::cout << "Set & Save powering item failed: set run step failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_1012_1013_buffer_pick_direction(ENN_POWERING_BUF_PICK_DIRCTION)){
            std::cout << "Set & Save powering item failed: set buffer pick dirction failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_buffers(ENN_POWERING_BUFFER_BSI, ENN_POWERING_BUFFER_BIN, powering_buffer_content, (ENN_POWERING_BUFFER_BIN * 4))){
            std::cout << "Set & Save powering item failed: set buffer content failed..." << std::endl;
            return false;
        }
        // Save configuration
        if(!Enn_light_controller_trigger_save_item_to_store()){
            std::cout << "Set & Save powering item failed: save to store failed..." << std::endl;
            return false;
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_set_save_normal_item(uint16_t item)
{
    Enn_light_controller_delete_item_from_store(item);
    if(Enn_light_controller_callfor_backedit_item(item)){
        if(!Enn_light_controller_select_mode(ENN_NORMAL_MODE_TEMPLATE_NUM)){
            std::cout << "Set & Save NORMAL item failed: select mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_light_number(ENN_NORMAL_LIGHT_IC_NUMBER)){
            std::cout << "Set & Save NORMAL item failed: set light number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_run_times(MODE_CONF_RUN_TIME_ALWAYS)){
            std::cout << "Set & Save NORMAL item failed: set runtime failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_delay_between_runtimes(ENN_NORMAL_RUNTIME_DELAY)){
            std::cout << "Set & Save NORMAL item failed: set runtime delay failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_endways(ENN_NORMAL_RUNTIME_END_MODE)){
            std::cout << "Set & Save NORMAL item failed: set runtime end mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_style(ENN_NORMAL_INIT_LIGHT_MODE)){
            std::cout << "Set & Save NORMAL item failed: set begin style failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_ic(ENN_NORMAL_BEGIN_IC_NUMBER)){
            std::cout << "Set & Save NORMAL item failed: set begin ic number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_RColor_set(ENN_NORMAL_BREATH_RCOLOR)){
            std::cout << "Set & Save NORMAL item failed: set Red color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_GColor_set(ENN_NORMAL_BREATH_GCOLOR)){
            std::cout << "Set & Save NORMAL item failed: set green color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_BColor_set(ENN_NORMAL_BREATH_BCOLOR)){
            std::cout << "Set & Save NORMAL item failed: set blue color failed..." << std::endl;
            return false;
        }
        // Specific configuration
        if(!Enn_light_controller_breath_Wire_set(ENN_NORMAL_BREATH_WIRE)){
            std::cout << "Set & Save NORMAL item failed: set breath wire failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_step_set(ENN_NORMAL_BREATH_TO_LIGHT_STEPS)){
            std::cout << "Set & Save NORMAL item failed: set breath to light steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_step_set(ENN_NORMAL_BREATH_TO_DARK_STEPS)){
            std::cout << "Set & Save NORMAL item failed: set breath to dark steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_speed_set(ENN_NORMAL_BREATH_TO_LIGHT_SPEED)){
            std::cout << "Set & Save NORMAL item failed: set breath to light speed failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_speed_set(ENN_NORMAL_BREATH_TO_DARK_SPEED)){
            std::cout << "Set & Save NORMAL item failed: set breath to dark speed failed..." << std::endl;
            return false;
        }
        // Save configuration
        if(!Enn_light_controller_trigger_save_item_to_store()){
            std::cout << "Set & Save NORMAL item failed: save to store failed..." << std::endl;
            return false;
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_set_save_warning_item(uint16_t item)
{
    Enn_light_controller_delete_item_from_store(item);
    if(Enn_light_controller_callfor_backedit_item(item)){
        if(!Enn_light_controller_select_mode(ENN_WARNING_MODE_TEMPLATE_NUM)){
            std::cout << "Set & Save WARNING item failed: select mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_light_number(ENN_WARNING_LIGHT_IC_NUMBER)){
            std::cout << "Set & Save WARNING item failed: set light number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_run_times(MODE_CONF_RUN_TIME_ALWAYS)){
            std::cout << "Set & Save WARNING item failed: set runtime failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_delay_between_runtimes(ENN_WARNING_RUNTIME_DELAY)){
            std::cout << "Set & Save WARNING item failed: set runtime delay failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_endways(ENN_WARNING_RUNTIME_END_MODE)){
            std::cout << "Set & Save WARNING item failed: set runtime end mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_style(ENN_WARNING_INIT_LIGHT_MODE)){
            std::cout << "Set & Save WARNING item failed: set begin style failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_ic(ENN_WARNING_BEGIN_IC_NUMBER)){
            std::cout << "Set & Save WARNING item failed: set begin ic number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_RColor_set(ENN_WARNING_BREATH_RCOLOR)){
            std::cout << "Set & Save WARNING item failed: set Red color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_GColor_set(ENN_WARNING_BREATH_GCOLOR)){
            std::cout << "Set & Save WARNING item failed: set green color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_BColor_set(ENN_WARNING_BREATH_BCOLOR)){
            std::cout << "Set & Save WARNING item failed: set blue color failed..." << std::endl;
            return false;
        }
        // Specific configuration
        if(!Enn_light_controller_breath_Wire_set(ENN_WARNING_BREATH_WIRE)){
            std::cout << "Set & Save WARNING item failed: set breath wire failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_step_set(ENN_WARNING_BREATH_TO_LIGHT_STEPS)){
            std::cout << "Set & Save WARNING item failed: set breath to light steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_step_set(ENN_WARNING_BREATH_TO_DARK_STEPS)){
            std::cout << "Set & Save WARNING item failed: set breath to dark steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_speed_set(ENN_WARNING_BREATH_TO_LIGHT_SPEED)){
            std::cout << "Set & Save WARNING item failed: set breath to light speed failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_speed_set(ENN_WARNING_BREATH_TO_DARK_SPEED)){
            std::cout << "Set & Save WARNING item failed: set breath to dark speed failed..." << std::endl;
            return false;
        }
        // Save configuration
        if(!Enn_light_controller_trigger_save_item_to_store()){
            std::cout << "Set & Save WARNING item failed: save to store failed..." << std::endl;
            return false;
        }
    }
    return true;
}


bool Enn_Light_Controller::Enn_light_controller_set_save_error_item(uint16_t item)
{
    Enn_light_controller_delete_item_from_store(item);
    if(Enn_light_controller_callfor_backedit_item(item)){
        if(!Enn_light_controller_select_mode(ENN_ERROR_MODE_TEMPLATE_NUM)){
            std::cout << "Set & Save ERROR item failed: select mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_light_number(ENN_ERROR_LIGHT_IC_NUMBER)){
            std::cout << "Set & Save ERROR item failed: set light number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_run_times(MODE_CONF_RUN_TIME_ALWAYS)){
            std::cout << "Set & Save ERROR item failed: set runtime failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_delay_between_runtimes(ENN_ERROR_RUNTIME_DELAY)){
            std::cout << "Set & Save ERROR item failed: set runtime delay failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_endways(ENN_ERROR_RUNTIME_END_MODE)){
            std::cout << "Set & Save ERROR item failed: set runtime end mode failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_style(ENN_ERROR_INIT_LIGHT_MODE)){
            std::cout << "Set & Save ERROR item failed: set begin style failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_set_begin_ic(ENN_ERROR_BEGIN_IC_NUMBER)){
            std::cout << "Set & Save ERROR item failed: set begin ic number failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_RColor_set(ENN_ERROR_BREATH_RCOLOR)){
            std::cout << "Set & Save ERROR item failed: set Red color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_GColor_set(ENN_ERROR_BREATH_GCOLOR)){
            std::cout << "Set & Save ERROR item failed: set green color failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_BColor_set(ENN_ERROR_BREATH_BCOLOR)){
            std::cout << "Set & Save ERROR item failed: set blue color failed..." << std::endl;
            return false;
        }
        // Specific configuration
        if(!Enn_light_controller_breath_Wire_set(ENN_ERROR_BREATH_WIRE)){
            std::cout << "Set & Save ERROR item failed: set breath wire failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_step_set(ENN_ERROR_BREATH_TO_LIGHT_STEPS)){
            std::cout << "Set & Save ERROR item failed: set breath to light steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_step_set(ENN_ERROR_BREATH_TO_DARK_STEPS)){
            std::cout << "Set & Save ERROR item failed: set breath to dark steps failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_light_speed_set(ENN_ERROR_BREATH_TO_LIGHT_SPEED)){
            std::cout << "Set & Save ERROR item failed: set breath to light speed failed..." << std::endl;
            return false;
        }
        if(!Enn_light_controller_breath_dark_speed_set(ENN_ERROR_BREATH_TO_DARK_SPEED)){
            std::cout << "Set & Save ERROR item failed: set breath to dark speed failed..." << std::endl;
            return false;
        }
        // Save configuration
        if(!Enn_light_controller_trigger_save_item_to_store()){
            std::cout << "Set & Save ERROR item failed: save to store failed..." << std::endl;
            return false;
        }
    }
    return true;
}

void Enn_Light_Controller::Enn_light_controller_set_save_all_item(uint16_t poweon_item, uint16_t normal_item, uint16_t warning_item, uint16_t error_item)
{
    Enn_light_controller_set_save_powering_item(poweon_item);
    Enn_light_controller_set_save_normal_item(normal_item);
    Enn_light_controller_set_save_warning_item(warning_item);
    Enn_light_controller_set_save_error_item(error_item);
}


void Enn_Light_Controller::Enn_light_controller_set_current_item(uint16_t item)
{
    light_current_item = item;
}


uint16_t Enn_Light_Controller::Enn_light_controller_get_current_item(void)
{
    return light_current_item;
}


void Enn_Light_Controller::Enn_light_controller_demo_base0_display(void)
{
    Enn_light_controller_select_mode(MODE_CONF_MODEL_1000_TEST);

    for(int i = 1; i < MODE_CONF_MODEL_TOTAL_NUMBER; i++){
        Enn_light_controller_backgroud_edit_mode(MODE_CONF_MODEL_1000_TEST + i);
        Enn_light_controller_set_light_number(ENN_DEMO1_LIGHT_IC_NUMBER);
        Enn_light_controller_set_mattric_number(1);
        Enn_light_controller_set_run_times(MODE_CONF_RUN_TIME_ALWAYS);
        Enn_light_contorller_set_delay_between_frames(100);     // 100ms
        Enn_light_controller_backgroud_edit_mode(5);            // 500ms
        Enn_light_controller_set_mattric_mode(0);               // in turn
        Enn_light_controller_set_endways(0);                    // restart
        Enn_light_controller_set_begin_style(0);                // trunoff light before this running 
        Enn_light_controller_set_begin_ic(1);                   // Begin from first IC
        std::this_thread::sleep_for(std::chrono::seconds(10));
        Enn_light_controller_trigger_edit_mode();

    }
}


void Enn_Light_Controller::Enn_light_controller_demo_base1_display(void)
{
    // Power-ON To Ready to Run light state
//    Enn_light_controller_call_execute_stored_item(ENN_POWERING_ITEM_STORE_NUM);
//    std::this_thread::sleep_for(std::chrono::seconds(30));
    // 20s Green Breath light
    Enn_light_controller_call_execute_stored_item(ENN_NROMAL_ITEM_STORE_NUM);
    std::this_thread::sleep_for(std::chrono::seconds(120));
    // 20s Blue Breath light
    Enn_light_controller_call_execute_stored_item(ENN_WARNING_ITEM_STORE_NUM);
    std::this_thread::sleep_for(std::chrono::seconds(20));
    // 20s Red Breath Ligth
    Enn_light_controller_call_execute_stored_item(ENN_ERROR_ITEM_STORE_NUM);
    std::this_thread::sleep_for(std::chrono::seconds(20));
}




