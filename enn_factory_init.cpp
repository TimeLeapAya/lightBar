#include "enn_gen2_lightcontrol.h"
#include "enn_user_define.h"
#include <thread>
#include <chrono>

int main(void)
{
    std::unique_ptr<Enn_Light_Controller> p_light_controller = std::make_unique<Enn_Light_Controller>(ENN_DEFAULT_SERIAL_NAME, ENN_DEFAULT_SERIAL_BAUDRATE);
    p_light_controller->Enn_light_controller_set_save_all_item(ENN_POWERING_ITEM_STORE_NUM, ENN_NROMAL_ITEM_STORE_NUM, ENN_WARNING_ITEM_STORE_NUM, ENN_ERROR_ITEM_STORE_NUM);
    p_light_controller->Enn_light_device_poweron_play_item(ENN_POWERING_ITEM_STORE_NUM);
    p_light_controller->Enn_light_device_poweron_mode(CONTROLLER_PARA_POWERON_RUN);
    return 0;
}
