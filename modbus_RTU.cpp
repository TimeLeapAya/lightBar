#include "modbus_RTU.h"

#include <iostream>
#include <modbus/modbus.h>
#include <string>
#include <cstring>


ModbusRTU::ModbusRTU(const std::string& device, int baud_rate): device_(device), baud_rate_(baud_rate){
    ctx_ = modbus_new_rtu(device.c_str(), baud_rate, 'N', 8, 1);
    if(!ctx_){
        std::cerr << "Failed to create Modbus context: " << modbus_strerror(errno) << std::endl;
        throw std::runtime_error("Failed to create Modbus context.");
    }
}

ModbusRTU::~ModbusRTU(){
    if(ctx_){
        modbus_close(ctx_);
        modbus_free(ctx_);
    }
}

bool ModbusRTU::connect(){
    if(modbus_connect(ctx_) == -1){
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    else{
        std::cout << "Connection success." << std::endl;
    }
    return true;
}

bool ModbusRTU::readCoils(int slave_address, int address, int nb, uint8_t* dest){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_read_bits(ctx_, address, nb, dest);
    if(rc == -1){
        std::cerr << "Read coils failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool ModbusRTU::readDiscreteInputs(int slave_address, int address, int nb, uint8_t* dest){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_read_input_bits(ctx_, address, nb, dest);
    if(rc == -1){
        std::cerr << "Read discrete inputs failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool ModbusRTU::readHoldingRegisters(int slave_address, int address, int nb, uint16_t* dest){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_read_registers(ctx_, address, nb, dest);
    if(rc == -1){
        std::cerr << "Read holding register failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}


bool ModbusRTU::readInputRegisters(int slave_address, int address, int nb, uint16_t* dest)
{
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_read_input_registers(ctx_, address, nb, dest);
    if(rc == -1){
        std::cerr << "Read input register failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}


bool ModbusRTU::writeSingleCoil(int slave_address, int address, uint8_t value){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_write_bit(ctx_, address, value);
    if(rc == -1){
        std::cerr << "Write single coil failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool ModbusRTU::writeMultiCoils(int slave_addr, int address, int nb, const uint8_t* src){
    modbus_set_slave(ctx_, slave_addr);
    int rc = modbus_write_bits(ctx_, address, nb, src);
    if(rc == -1){
        std::cerr << "Write multiple coils failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool ModbusRTU::writeSingleRegister(int slave_address, int address, uint16_t value){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_write_register(ctx_, address, value);
    if(rc ==  -1){
        std::cerr << "Write single register failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}

bool ModbusRTU::writeMultipleRegister(int slave_address, int address, int nb, const uint16_t* src){
    modbus_set_slave(ctx_, slave_address);
    int rc = modbus_write_registers(ctx_, address, nb, src);
    if(rc == -1){
        std::cerr << "Write multiple registers failed: " << modbus_strerror(errno) << std::endl;
        return false;
    }
    return true;
}


