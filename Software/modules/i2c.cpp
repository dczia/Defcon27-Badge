//
// Created by hamster on 5/23/19.
//

#include "common.h"
#include "i2c.h"


const nrf_drv_twi_t m_twi_master = NRF_DRV_TWI_INSTANCE(MASTER_TWI_INST);

void twi_master_init(void){
    const nrf_drv_twi_config_t config = {
            .scl                = I2C_SCL_PIN,
            .sda                = I2C_SDA_PIN,
            .frequency          = NRF_DRV_TWI_FREQ_400K,
            .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
            .clear_bus_init     = false
    };

    if (NRF_SUCCESS == nrf_drv_twi_init(&m_twi_master, &config, NULL, NULL)){
        nrf_drv_twi_enable(&m_twi_master);
    }

}


ret_code_t i2cMasterTransmit(uint8_t addr, uint8_t const * pdata, size_t size){

    return nrf_drv_twi_tx(&m_twi_master, addr, pdata, size, false);

}

ret_code_t i2cMasterRead(uint8_t addr, uint16_t register_addr, uint8_t *p_data, uint8_t bytes, bool no_stop){
    ret_code_t err_code;

    uint8_t reg[2];
    reg[0] = (register_addr >> 8) & 0xFF;
    reg[1] = register_addr & 0xFF;
    err_code = nrf_drv_twi_tx(&m_twi_master, addr, reg, 2, no_stop);
//    APP_ERROR_CHECK(err_code);
//
//    if(err_code != NRF_SUCCESS) {
//        return err_code;
//    }

    err_code = nrf_drv_twi_rx(&m_twi_master, addr, p_data, bytes);
    return err_code;
}

