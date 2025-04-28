#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__

#include <iostream>
#include <modbus/modbus.h>
#include <string>
#include <cstring>

class ModbusRTU{
public:
    ModbusRTU(const std::string& device, int baud_rate);

    ~ModbusRTU();

    bool connect();

    bool readCoils(int slave_address, int address, int nb, uint8_t* dest);                            // Read coils

    bool readDiscreteInputs(int slave_address, int address, int nb, uint8_t* dest);                   // Eead Discrete Inputs

    bool readHoldingRegisters(int slave_address, int address, int nb, uint16_t* dest);                // Read holding Registers

    bool readInputRegisters(int slave_address, int address, int nb, uint16_t* dest);                  // Read Input Registers

    bool writeSingleCoil(int slave_address, int address, uint8_t value);                              // Write single coil

    bool writeMultiCoils(int slave_addr, int address, int nb, const uint8_t* src);                    // Write multi coils

    bool writeSingleRegister(int slave_address, int address, uint16_t value);                         // Write single holding Register

    bool writeMultipleRegister(int slave_address, int address, int nb, const uint16_t* src);          // Write Multi holding Register

private:
    modbus_t* ctx_;
    std::string device_;
    int baud_rate_;
};

#endif


