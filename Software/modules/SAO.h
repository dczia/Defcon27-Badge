//
// Created by hamster on 8/4/19.
//

#ifndef SOFTWARE_SAO_H
#define SOFTWARE_SAO_H

#include "common.h"

class SAO {

    public:

        SAO(uint8_t address);

        bool isPresent();
        uint8_t read(uint8_t *p_data, uint8_t bytes);
        uint8_t write(uint8_t const * pdata, size_t size);

    private:

        uint8_t address;
};


#endif //SOFTWARE_SAO_H
