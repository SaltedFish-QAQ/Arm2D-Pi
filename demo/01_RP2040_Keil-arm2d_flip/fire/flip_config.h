#ifndef __FLIP_CONFIG_H__
#define __FLIP_CONFIG_H__

#define FLIP_CONFIG_Q16_FROM_X100(VALUE)       \
    (((VALUE) * 65536) / 100)
#define FLIP_CONFIG_Q16_FROM_X100000(VALUE)    \
    (((VALUE) * 65536) / 100000)
#define FLIP_CONFIG_Q15_FROM_X100000(VALUE)    \
    (((VALUE) * 32768) / 100000)

//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

// <h>FLIP Resource Limits

// <o>Simulation grid width <8-64>
// <i>Higher values improve detail but increase SRAM and CPU usage.
#ifndef FLIP_GRID_W
#   define FLIP_GRID_W                              36
#endif

// <o>Simulation grid height <8-64>
// <i>Higher values improve detail but increase SRAM and CPU usage.
#ifndef FLIP_GRID_H
#   define FLIP_GRID_H                              36
#endif

// <o>Maximum particle count <1-2000>
#ifndef FLIP_MAX_PARTICLES
#   define FLIP_MAX_PARTICLES                       768
#endif

// <o>FLIP memory pool size in bytes <16384-131072:1024>
// <i>This buffer is allocated statically from SRAM.
#ifndef FLIP_MEM_SIZE
#   define FLIP_MEM_SIZE                            65536u
#endif

// <o>Maximum render width <1-480>
// <i>Sets the size of static render lookup buffers.
#ifndef FLIP_RENDER_MAX_WIDTH
#   define FLIP_RENDER_MAX_WIDTH                    240
#endif

// <o>Maximum render height <1-480>
// <i>Sets the size of static render lookup buffers.
#ifndef FLIP_RENDER_MAX_HEIGHT
#   define FLIP_RENDER_MAX_HEIGHT                   240
#endif

// </h>

// <h>FLIP Solver

// <o>Particle cell scale (x0.01) <100-400>
// <i>220 means 2.20 particle diameters.
#ifndef FLIP_PARTICLE_CELL_SCALE_X100
#   define FLIP_PARTICLE_CELL_SCALE_X100           220
#endif

// <o>Pressure solver iterations <1-64>
#ifndef FLIP_PRESSURE_ITERS
#   define FLIP_PRESSURE_ITERS                      24
#endif

// <o>Particle separation iterations <0-8>
#ifndef FLIP_PARTICLE_ITERS
#   define FLIP_PARTICLE_ITERS                      1
#endif

// <o>FLIP ratio in percent <0-100>
// <i>Lower values add PIC damping and stability.
#ifndef FLIP_DEFAULT_FLIP_RATIO_PERCENT
#   define FLIP_DEFAULT_FLIP_RATIO_PERCENT          95
#endif

// <o>Pressure over-relaxation (x0.01) <100-200>
// <i>190 means 1.90.
#ifndef FLIP_OVER_RELAXATION_X100
#   define FLIP_OVER_RELAXATION_X100                190
#endif

// <o>Simulation update rate in Hz <15-240>
#ifndef FLIP_SIMULATION_RATE_HZ
#   define FLIP_SIMULATION_RATE_HZ                  60
#endif

// <o>Global velocity limit (x0.01) <100-4000>
// <i>1000 means 10.00.
#ifndef FLIP_VELOCITY_LIMIT_X100
#   define FLIP_VELOCITY_LIMIT_X100                 1000
#endif

// </h>

// <h>Gravity and Motion Mapping

// <o>Downward fixed gravity magnitude (x0.01 m/s^2) <0-4000>
// <i>981 means 9.81 m/s^2 downward on the simulation Y axis.
#ifndef FLIP_GRAVITY_MAGNITUDE_X100
#   define FLIP_GRAVITY_MAGNITUDE_X100               981
#endif

// <o>Motion gravity scale (x0.01) <0-4000>
// <i>981 means 9.81.
#ifndef FLIP_MOTION_GRAVITY_SCALE_X100
#   define FLIP_MOTION_GRAVITY_SCALE_X100           981
#endif

// <o>Motion acceleration scale (x0.01) <0-4000>
// <i>600 means 6.00.
#ifndef FLIP_MOTION_ACCEL_SCALE_X100
#   define FLIP_MOTION_ACCEL_SCALE_X100             600
#endif

// <o>Motion acceleration deadzone (x0.00001 g) <0-10000>
// <i>3125 means 0.03125 g.
#ifndef FLIP_MOTION_ACCEL_DEADZONE_X100000
#   define FLIP_MOTION_ACCEL_DEADZONE_X100000       3125
#endif

// <o>Motion force limit (x0.01) <0-6400>
// <i>4000 means 40.00.
#ifndef FLIP_MOTION_FORCE_LIMIT_X100
#   define FLIP_MOTION_FORCE_LIMIT_X100             4000
#endif

// </h>

// <h>Container Geometry and Flow

// <e>Use hourglass container
// <i>Disabled: use a regular square boundary without a neck flow limiter.
#ifndef FLIP_USE_HOURGLASS_CONTAINER
#   define FLIP_USE_HOURGLASS_CONTAINER             1
#endif

// <o>Hourglass waist half-width <1-100>
// <i>Percentage of the maximum half-width. Smaller values narrow the neck.
#ifndef FLIP_HOURGLASS_WAIST_PERCENT
#   define FLIP_HOURGLASS_WAIST_PERCENT             1
#endif

// <o>Neck limiter half-height in grid cells <1-16>
// <i>The limiter covers this many cells above and below the waist.
#ifndef FLIP_HOURGLASS_NECK_HALF_HEIGHT_CELLS
#   define FLIP_HOURGLASS_NECK_HALF_HEIGHT_CELLS    2
#endif

// <o>Neck speed limit (x0.00001) <1-1000000>
// <i>1526 means 0.01526; 10000 means 0.10; 18000 means 0.18.
#ifndef FLIP_HOURGLASS_NECK_SPEED_X100000
#   define FLIP_HOURGLASS_NECK_SPEED_X100000        1526
#endif

// </e>

// </h>

// <h>FLIP Rendering

// <q>Keep the density field as the solver colour source
#ifndef FLIP_RENDER_DENSITY_FIELD
#   define FLIP_RENDER_DENSITY_FIELD                1
#endif

// <o>Default render mode
//     <0=> Particle
//     <1=> Continuous surface
#ifndef FLIP_SIM_DEFAULT_RENDER_MODE
#   define FLIP_SIM_DEFAULT_RENDER_MODE             0
#endif

// <o>Particle red-speed threshold (x0.01) <1-1000>
// <i>Particles reach the red end of the palette at this speed.
// <i>200 means 2.00.
#ifndef FLIP_SIM_PARTICLE_RED_SPEED_X100
#   define FLIP_SIM_PARTICLE_RED_SPEED_X100         200
#endif

// <o>Hourglass container scale <20-100>
// <i>Percentage of the available render area.
#ifndef FLIP_CONTAINER_SCALE_PERCENT
#   define FLIP_CONTAINER_SCALE_PERCENT             82
#endif

// <o>Surface visibility threshold shift <0-15>
// <i>Larger values reveal lower-density surface pixels.
#ifndef FLIP_SURFACE_THRESHOLD_SHIFT
#   define FLIP_SURFACE_THRESHOLD_SHIFT             4u
#endif

// <o>Container border RGB565 colour <0x0-0xFFFF>
// <i>0x0000 keeps the border invisible on the black background.
#ifndef FLIP_CONTAINER_BORDER_COLOUR
#   define FLIP_CONTAINER_BORDER_COLOUR             0x0000u
#endif

// </h>

//------------- <<< end of configuration section >>> -----------------------

#ifndef FLIP_PARTICLE_CELL_SCALE_Q16
#   define FLIP_PARTICLE_CELL_SCALE_Q16             \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_PARTICLE_CELL_SCALE_X100)
#endif

#ifndef FLIP_DEFAULT_FLIP_RATIO_Q16
#   define FLIP_DEFAULT_FLIP_RATIO_Q16               \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_DEFAULT_FLIP_RATIO_PERCENT)
#endif

#ifndef FLIP_OVER_RELAXATION_Q16
#   define FLIP_OVER_RELAXATION_Q16                  \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_OVER_RELAXATION_X100)
#endif

#ifndef FLIP_DT_Q16
#   define FLIP_DT_Q16                              (65536 / FLIP_SIMULATION_RATE_HZ)
#endif

#ifndef FLIP_VELOCITY_LIMIT_Q16
#   define FLIP_VELOCITY_LIMIT_Q16                   \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_VELOCITY_LIMIT_X100)
#endif

#ifndef FLIP_GRAVITY_X100
#   define FLIP_GRAVITY_X100                        (-FLIP_GRAVITY_MAGNITUDE_X100)
#endif

#ifndef FLIP_GRAVITY_Q16
#   define FLIP_GRAVITY_Q16                          \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_GRAVITY_X100)
#endif

#ifndef FLIP_MOTION_GRAVITY_SCALE_Q16
#   define FLIP_MOTION_GRAVITY_SCALE_Q16             \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_MOTION_GRAVITY_SCALE_X100)
#endif

#ifndef FLIP_MOTION_ACCEL_SCALE_Q16
#   define FLIP_MOTION_ACCEL_SCALE_Q16               \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_MOTION_ACCEL_SCALE_X100)
#endif

#ifndef FLIP_MOTION_ACCEL_DEADZONE_Q15
#   define FLIP_MOTION_ACCEL_DEADZONE_Q15            \
        FLIP_CONFIG_Q15_FROM_X100000(FLIP_MOTION_ACCEL_DEADZONE_X100000)
#endif

#ifndef FLIP_MOTION_FORCE_LIMIT_Q16
#   define FLIP_MOTION_FORCE_LIMIT_Q16               \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_MOTION_FORCE_LIMIT_X100)
#endif

#ifndef FLIP_HOURGLASS_NECK_SPEED_LIMIT_Q16
#   define FLIP_HOURGLASS_NECK_SPEED_LIMIT_Q16       \
        FLIP_CONFIG_Q16_FROM_X100000(FLIP_HOURGLASS_NECK_SPEED_X100000)
#endif

#ifndef FLIP_SIM_PARTICLE_RED_SPEED_Q16
#   define FLIP_SIM_PARTICLE_RED_SPEED_Q16           \
        FLIP_CONFIG_Q16_FROM_X100(FLIP_SIM_PARTICLE_RED_SPEED_X100)
#endif

#if (FLIP_GRAVITY_MAGNITUDE_X100 < 0) \
 || (FLIP_GRAVITY_MAGNITUDE_X100 > 4000)
#   error FLIP_GRAVITY_MAGNITUDE_X100 must be in the range 0..4000
#endif

#if (FLIP_HOURGLASS_WAIST_PERCENT < 1) || (FLIP_HOURGLASS_WAIST_PERCENT > 100)
#   error FLIP_HOURGLASS_WAIST_PERCENT must be in the range 1..100
#endif

#if FLIP_HOURGLASS_NECK_HALF_HEIGHT_CELLS < 1
#   error FLIP_HOURGLASS_NECK_HALF_HEIGHT_CELLS must be greater than zero
#endif

#if (FLIP_USE_HOURGLASS_CONTAINER != 0) \
 && (FLIP_USE_HOURGLASS_CONTAINER != 1)
#   error FLIP_USE_HOURGLASS_CONTAINER must be 0 or 1
#endif

#if (FLIP_SIM_DEFAULT_RENDER_MODE < 0) || (FLIP_SIM_DEFAULT_RENDER_MODE > 1)
#   error FLIP_SIM_DEFAULT_RENDER_MODE must be 0 or 1
#endif

#if FLIP_SIM_PARTICLE_RED_SPEED_Q16 == 0
#   error FLIP_SIM_PARTICLE_RED_SPEED_Q16 must be greater than zero
#endif

#if FLIP_SIMULATION_RATE_HZ < 1
#   error FLIP_SIMULATION_RATE_HZ must be greater than zero
#endif

#endif
