#ifndef __MQTT_CONTROLLER_H__
#define __MQTT_CONTROLLER_H__

#include <memory>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <mosquittopp.h>

enum light_control_action{
    ITEM_SUSPEND = 1,
    ITEM_STOP,
    ITEM_START,
    ITEM_RELOAD,
    ITEM_RESTART,
    ITEM_TURNOFF,
    ITEM_CHANGE
};

struct enn_light_message{
    std::string topic;
    std::string message;
};

class Enn_MQTT_Controller : public mosqpp::mosquittopp{
public:
    Enn_MQTT_Controller(const char *id);
    Enn_MQTT_Controller(const char *id, const char *host, int port, const char *username, const char *passwd);
    ~Enn_MQTT_Controller();

    void on_connect(int rc) override;
    void on_message(const mosquitto_message *msg) override;
    void on_subscribe(int mid, int qos_count, const int *grandted_qos) override;

    bool Enn_mqtt_get_config_from_file(std::string filename);
    bool Enn_mqtt_get_config_from_string(std::string filename);


    bool Enn_MQTT_Init(void);

    void loop_go();

private:
    int mqtt_port;
    std::string mqtt_address;
    std::string mqtt_username;
    std::string mqtt_passwd;
    mosquitto  *mosq;
    bool m_running = true;
    std::thread m_loopThread;
};

#endif

