//
// Created by hamster on 5/23/19.
//

#ifndef I2C_H
#define I2C_H

void twi_master_init(void);
ret_code_t i2cMasterTransmit(uint8_t addr, uint8_t const * pdata, size_t size);
ret_code_t i2cMasterRead(uint8_t addr, uint16_t register_addr, uint8_t *p_data, uint8_t bytes, bool no_stop);

/* Master Configuration */
#define MASTER_TWI_INST     0       //!< TWI interface used as a master accessing EEPROM memory.


#endif //I2C_H
