#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <mosquitto.h>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "mqtt_controller.h"
#include "enn_user_define.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

extern std::queue<std::unique_ptr<struct enn_light_message>> messageQueue;
extern std::mutex mtx;
extern std::condition_variable cv;


void Enn_MQTT_Controller::on_connect(int rc) 
{
    if(rc == 0){
        std::cout << "Connect to MQTT broker success." << std::endl;
        subscribe(nullptr, ENN_SUBTOPIC_1, 0);
        subscribe(nullptr, ENN_SUBTOPIC_2, 0);
    }
    else{
        std::cout << "Conenction failed with code(" << mosquitto_strerror(rc) << "), Retring..." << std::endl;
    }
}

/*
void Enn_MQTT_Controller::on_disconnect(int rc)
{
    std::cout << "Disconnect from server with code(" << mosquitto_strerror(rc) << "), Retring..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    connect_async(mqtt_address.c_str(), mqtt_port, 60);
}
*/


void Enn_MQTT_Controller::on_message(const mosquitto_message *msg)
{
    std::string _topic(msg->topic);
    std::string _message(reinterpret_cast<const char *>(msg->payload), msg->payloadlen);

    auto new_msg = std::make_unique<struct enn_light_message>();
    new_msg->topic = std::move(_topic);
    new_msg->message = std::move(_message);
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        messageQueue.push(std::move(new_msg));
    }
    cv.notify_one();
}


void Enn_MQTT_Controller::on_subscribe(int mid, int qos_count, const int *grandted_qos)
{
    std::cout << "Subscribed to topic with QoS: ";
    for(int i = 0; i < qos_count; ++i){
        std::cout << grandted_qos[i] << " ";
    }
    std::cout << std::endl;
}

Enn_MQTT_Controller::Enn_MQTT_Controller(const char *id):mosqpp::mosquittopp(id)
{
    std::cout << "Enn_MQTT_Controller: Initial..." << std::endl;
    mqtt_port = ENN_MQTT_PORT;
    mqtt_address = ENN_MQTT_SERVER;
    mqtt_username = ENN_MQTT_USERNAME;
    mqtt_passwd = ENN_MQTT_PASSWD;

    Enn_mqtt_get_config_from_file(ENN_LIGHT_CONFIGURATION_JSON_FILE);

    mosqpp::lib_init();
 //   std::cout << mqtt_username << "   " << mqtt_passwd << std::endl;
    username_pw_set(mqtt_username.c_str(), mqtt_passwd.c_str());
    int keepalive = 60;
    std::cout << "Enn_MQTT_Controller: Try connect..." << std::endl;
//    std::cout << mqtt_address << "   " << mqtt_port << std::endl;
    connect_async(mqtt_address.c_str(), mqtt_port, keepalive);

    std::cout << "Enn_MQTT_Controller: loop start..." << std::endl;

    m_loopThread = std::thread(&Enn_MQTT_Controller::loop_go, this);
}


Enn_MQTT_Controller::Enn_MQTT_Controller(const char *id, const char *host, int port, const char *username, const char *passwd):mosqpp::mosquittopp(id)
{
    std::cout << "Enn_MQTT_Controller: Initial..." << std::endl;
    mosqpp::lib_init();
    username_pw_set(username, passwd);
    int keepalive = 60;
    std::cout << "Enn_MQTT_Controller: Try connect..." << std::endl;
    connect_async(host, port, keepalive);
    std::cout << "Enn_MQTT_Controller: loop start..." << std::endl;

    m_loopThread = std::thread(&Enn_MQTT_Controller::loop_go, this);
}


Enn_MQTT_Controller::~Enn_MQTT_Controller()
{
    m_running = false;
    if(m_loopThread.joinable()){
        m_loopThread.join();
    }
    disconnect();
    mosqpp::lib_cleanup();
}


bool Enn_MQTT_Controller::Enn_mqtt_get_config_from_file(std::string filename)
{
    std::ifstream conf_file(filename);
    std::cout << "Enn light Try get MQTT config from file." << std::endl;   
    if(conf_file.good()){
        if(conf_file.is_open()){
            std::stringstream buffer;
            buffer << conf_file.rdbuf();
            std::string content = buffer.str();
            conf_file.close();
            if(Enn_mqtt_get_config_from_string(content)){
                std::cout << "Enn light get MQTT config from file success." << std::endl;
            }
            else
                std::cout << "Enn light get MQTT config from file open failed." << std::endl;
        }
        else
            std::cout << "Enn light get MQTT config from file failed, file invalid." << std::endl;
    }
    else
        std::cout << "Enn light get MQTT config from file failed, file invalid." << std::endl;
    return false;
}


bool Enn_MQTT_Controller::Enn_mqtt_get_config_from_string(std::string content)
{
    bool ret_val = false;
    rapidjson::Document document;
    if(!document.Parse(content.c_str()).HasParseError()){
        if(document.HasMember("mqtt_addr") && document["mqtt_addr"].IsString()){
            mqtt_address = document["mqtt_addr"].GetString();
            ret_val = true;
        }
        if(document.HasMember("mqtt_port") && document["mqtt_port"].IsInt()){
            mqtt_port = document["mqtt_port"].GetInt();
            ret_val = true;
        }
        if(document.HasMember("mqtt_username") && document["mqtt_username"].IsString()){
            mqtt_username = document["mqtt_username"].GetString();
            ret_val = true;
        }
        if(document.HasMember("mqtt_password") && document["mqtt_password"].IsString()){
            mqtt_passwd = document["mqtt_password"].GetString();
            ret_val = true;
        }
    }
    else{
        std::cout << "Enn light get MQTT config from string failed, Parse failed." << std::endl;
    }
    return ret_val;
}


void Enn_MQTT_Controller::loop_go(){
    while(m_running){
        int rc = loop(100);
        if(rc != MOSQ_ERR_SUCCESS){
            std::cerr << "MQTT Loop error: " << rc << std::endl;
            if(rc == MOSQ_ERR_CONN_LOST){
                std::cerr << "Connection lost, Reconnecting ..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                reconnect();
            }
            else if(rc == MOSQ_ERR_NO_CONN){
                std::cerr << "Connection failed, check server ..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));
                connect_async(mqtt_address.c_str(), mqtt_port, 60);
            }
        }
    }
}
