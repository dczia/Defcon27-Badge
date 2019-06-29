/*
 * @file common.h
 *
 * @date June 20 2019
 * @author hamster, rehr
 *
 * Common header file included by all files in the project
 *
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


// System headers
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdbool.h>

#include "config/app/sdk_config.h"
#include "config/custom_board.h"

// Nordic headers 
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "ble.h"
#include "ble_err.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "bsp.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "boards.h"
#include "app_timer.h"
#include "app_button.h"
#include "ble_lbs.h"
#include "nrf_ble_gatt.h"
#include "nrf_ble_qwr.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf52840_peripherals.h"
#include "nrf_spim.h"
#include "nrf_drv_spi.h"
#include "nrf_drv_twi.h"
#include "ff.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_block_dev.h"
#include "diskio.h"
#include "diskio_blkdev.h"
#include "legacy/nrf_drv_rng.h"
#include "nrf_drv_saadc.h"
#include "nrf_saadc.h"
#include "nrf_drv_ppi.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_pwm.h"
#include "nrfx_pwm.h"
#include "app_uart.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#define printf NRF_LOG_RAW_INFO



#endif /* SYSTEM_H_ */

