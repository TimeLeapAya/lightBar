#include "enn_gen2_lightcontrol.h"
#include "enn_user_define.h"
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <mosquittopp.h>

#include "mqtt_controller.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#define LIGHT_BLUE_WATER_NUMBER      0
#define LIGHT_BLUE_BREATH_NUMBER     1
#define LIGHT_YELLO_BREATH_NUMBER    2
#define LIGHT_RED_BREATH_NUMBER      3
#define LIGHT_COLOR_ITEM_NUM         4


std::queue<std::unique_ptr<struct enn_light_message>> messageQueue;
std::mutex mtx;
std::condition_variable cv;


static const int enn_light_item_array[LIGHT_COLOR_ITEM_NUM] = {ENN_POWERING_ITEM_STORE_NUM, ENN_NROMAL_ITEM_STORE_NUM, ENN_WARNING_ITEM_STORE_NUM, ENN_ERROR_ITEM_STORE_NUM};


void Enn_light_controller_routine(Enn_Light_Controller& light_routine)
{
    rapidjson::Document document;
    while(true){
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, []{return !messageQueue.empty();});
        auto msg = std::move(messageQueue.front());
        messageQueue.pop();
        lock.unlock();
        std::cout << "Received MQTT message on topic: " << msg->topic << ", Payload" << msg->message << std::endl;
        if(document.Parse(msg->message.c_str()).HasParseError()){
            std::cerr << "Failed to parse MQTT message as JSON: " << msg->message << std::endl;
        }
        else
            std::cout << "Succeed to parse MQTT message as Json." << std::endl;
        if(msg->topic == ENN_SUBTOPIC_1){
            if(light_routine.Enn_light_get_base_config_from_string(msg->message)){
                light_routine.Enn_light_save_base_config_to_file(ENN_LIGHT_CONFIGURATION_JSON_FILE, msg->message);
                light_routine.Enn_light_controller_init();
            }
            light_routine.Enn_light_get_spec_config_from_string(msg->message);
        }
        else if(msg->topic == ENN_SUBTOPIC_2){
            if(document.HasMember("action") && document["action"].IsNumber()){
                switch(document["action"].GetInt()){
                    case ITEM_SUSPEND:
                        light_routine.Enn_light_controller_play_suspend();
                        break;
                    case ITEM_STOP:
                        light_routine.Enn_light_controller_play_stop();
                        break;
                    case ITEM_START:
                        light_routine.Enn_light_controller_play_start();
                        break;
                    case ITEM_RELOAD:
                        light_routine.Enn_light_controller_reload_play_item();
                        break;
                    case ITEM_RESTART:
                        light_routine.Enn_light_controller_play_restart();
                        break;
                    case ITEM_TURNOFF:
                        light_routine.Enn_light_controller_all_light_turnoff();
                        break;
                    case ITEM_CHANGE:
                        if(document.HasMember("param") && document["param"].IsObject()){
                            const rapidjson::Value& parameter = document["param"];
                            if(parameter.HasMember("num") && parameter["num"].IsInt()){
                                if(parameter["num"].GetInt() < LIGHT_COLOR_ITEM_NUM){
                                    if(parameter["num"].GetInt() != light_routine.Enn_light_controller_get_current_item()){
                                        if(light_routine.Enn_light_controller_call_execute_stored_item(enn_light_item_array[parameter["num"].GetInt()])){
                                            light_routine.Enn_light_controller_set_current_item(parameter["num"].GetInt());
                                        }
                                    }
                                }
                                else{
                                    std::cerr << "Invalid Json struct for ITEM_CHANGE, num exceed MAX Value(" << LIGHT_COLOR_ITEM_NUM << ")..." << std::endl;
                                }
                            }
                            else{
                                std::cerr << "Invalid Json struct for ITEM_CHANGE, num not a int value..." << std::endl;
                            }
                        }
                        else{
                            std::cerr << "Invalid Json struct for ITEM_CHANGE, param not a object..." << std::endl;
                        }
                        break;
                    default:
                        std::cerr << "Invalid action Number(" << document["action"].GetInt() << ")..." << std::endl;
                        break;
                }
            }
        }
    }
}


int main(void)
{
    Enn_Light_Controller light_controller(ENN_DEFAULT_SERIAL_NAME, ENN_DEFAULT_SERIAL_BAUDRATE);
    Enn_MQTT_Controller  mqtt_controller(ENN_MQTT_ID);
//    Enn_MQTT_Controller  mqtt_controller(ENN_MQTT_ID, ENN_MQTT_SERVER, ENN_MQTT_PORT, ENN_MQTT_USERNAME, ENN_MQTT_PASSWD);

    std::thread enn_light_controller(Enn_light_controller_routine, std::ref(light_controller));


    enn_light_controller.join();

    return 0;
}


/*
int main(void)
{
    std::unique_ptr<Enn_Light_Controller> p_light_controller = std::make_unique<Enn_Light_Controller>(ENN_DEFAULT_SERIAL_NAME, ENN_DEFAULT_SERIAL_BAUDRATE);
//    p_light_controller->Enn_light_controller_set_save_all_item(ENN_POWERING_ITEM_STORE_NUM, ENN_NROMAL_ITEM_STORE_NUM, ENN_WARNING_ITEM_STORE_NUM, ENN_ERROR_ITEM_STORE_NUM);
//    p_light_controller->Enn_light_device_poweron_play_item(ENN_POWERING_ITEM_STORE_NUM);
//    p_light_controller->Enn_light_device_poweron_mode(CONTROLLER_PARA_POWERON_RUN);
    while(true){
        p_light_controller->Enn_light_controller_demo_base1_display();
    }
    return 0;
}
*/
