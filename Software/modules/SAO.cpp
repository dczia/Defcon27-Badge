//
// Created by hamster on 8/4/19.
//

#include "SAO.h"
#include "common.h"
#include "i2c.h"

SAO::SAO(uint8_t address){
    this->address = address;
}


bool SAO::isPresent(){

    uint8_t buf = 0;
    ret_code_t code = read(&buf, 1);

    printf("Retcode is %d\n", code);

    if(code == NRF_ERROR_DRV_TWI_ERR_ANACK || code == NRF_ERROR_DRV_TWI_ERR_DNACK){
        return false;
    }

    return true;

}


uint8_t SAO::read(uint8_t *p_data, uint8_t bytes){

    return i2cMasterRead(this->address, 0, p_data, bytes, false);

}


uint8_t SAO::write(uint8_t const * pdata, size_t size){

    return i2cMasterTransmit(this->address, pdata, size);

}
