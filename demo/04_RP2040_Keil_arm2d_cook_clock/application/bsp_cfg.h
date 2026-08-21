#ifndef BSP_CFG_H
#define BSP_CFG_H

#include "hardware/i2c.h"

/* Shared sensor bus, matching the wiring used by the sibling projects. */
#ifndef I2C_PORT
#   define I2C_PORT                     i2c0
#endif

#ifndef I2C_SDA
#   define I2C_SDA                      0u
#endif

#ifndef I2C_SCL
#   define I2C_SCL                      1u
#endif

#endif