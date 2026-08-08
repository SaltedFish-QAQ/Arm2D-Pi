# RP2040 Arm-2D 工程配置说明

## 使用范围与读取顺序

本文覆盖本工程中会改变硬件连接、编译特性、内存分配、资源加载、图形渲染、调试或演示行为的 `.h` 配置文件。按 [project/mdk/template.uvprojx](project/mdk/template.uvprojx) 中的 Keil 编组分类，并给出实际文件位置。

当前构建目标为 `AC6-flash`。`project/mdk/app_cfg.h` 会通过 C/C++ 的 `-include "app_cfg.h"` 强制包含，因此其中的宏优先于场景头文件中的 `#ifndef` 默认值。

| 标记 | 含义 |
| --- | --- |
| **直接使用** | 当前 `AC6-flash` 目标直接编译、强制包含或由 RTE 选中的配置。 |
| **条件使用** | 源文件已在工程内；具体行为还取决于 `main.c` 宏、RTE 组件或场景是否初始化。 |
| **派生值** | 由其他配置计算得出，不建议单独改动。 |

> 修改 Keil 工程已打开时，请重新加载工程再编译。带 `Configuration Wizard` 注释的文件可在 Keil 中右键使用 **Configuration Wizard** 修改；其余文件建议直接编辑宏定义。

## Keil `Application` 编组

### `project/mdk/app_cfg.h` - Arm-2D 演示全局开关

**直接使用。** 此文件覆盖多个场景头的默认值。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `SPACE_BADGE_SHOW_NEBULA` | `1` | 启用 Space Badge 星云背景。 |
| `SPACE_BADGE_NEBULA_FLYING_SPEED` | `2.0f` | 星云飞行速度倍率。 |
| `SPACE_BADGE_90TH_BATTLE_STYLE` | `0` | 关闭 90 年代战斗视觉风格。 |
| `SPACE_BADGE_EXPLOSION_SPEED` | `16` | 爆炸动画速度。 |
| `ARM_2D_DEMO_USE_ZJPGD` | `1` | 允许演示使用 ZJpgD JPEG 解码器。 |
| `ARM_2D_SCENE_HISTOGRAM_USE_QOI/JPG/ZJPGD` | `1/1/0` | 直方图场景的资源解码器选择；若 QOI 可用，场景头会关闭 JPG 路径。 |
| `ARM_2D_SCENE_METER_USE_QOI/JPG/ZJPGD` | `1/1/1` | 仪表场景的资源解码器选择；同样受 RTE 组件可用性约束。 |
| `ARM_2D_SCENE_METER_SHOW_FPS` | `1` | 在仪表场景显示 FPS。 |
| `ARM_2D_DEMO_RADAR_USE_JPG_FOR_ANIMATION` | `0` | 雷达动画不使用 JPG。 |
| `ARM_2D_DEMO_RADAR_USE_QOI_FOR_ANIMATION` | `0` | 雷达动画不使用 QOI。 |
| `ARM_2D_DEMO_RADAR_USE_ZHRGB565_FOR_ANIMATION` | `1` | 雷达动画使用 ZhRGB565。 |
| `ARM_2D_DEMO_RADAR_SHOW_ANIMATION` | `1` | 显示雷达动画。 |
| `ARM_2D_DEMO_RADAR_DEBUG` | `1` | 启用雷达演示调试行为/输出。 |

### `project/mdk/tusb_config.h` - TinyUSB 设备栈

**直接使用。** 该文件决定 TinyUSB 为 RP2040 构建哪些 USB 类。

| 配置项 | 当前值 | 作用 |
| --- | --- | --- |
| `CFG_TUSB_MCU` | `OPT_MCU_RP2040` | 指定 RP2040 后端。 |
| `CFG_TUSB_OS` | `OPT_OS_NONE` | 无 RTOS 模式。 |
| `CFG_TUSB_DEBUG` | `0` | 关闭 TinyUSB 调试日志。 |
| `CFG_TUSB_RHPORT0_MODE` | `OPT_MODE_DEVICE` | USB0 工作在设备模式。 |
| `CFG_TUD_ENABLED` | `1` | 启用设备栈。 |
| `CFG_TUD_MAX_SPEED` | `OPT_MODE_FULL_SPEED` | 使用 USB Full Speed。 |
| `CFG_TUD_ENDPOINT0_SIZE` | `64` | EP0 控制端点大小。 |
| `CFG_TUD_CDC/MSC/HID/MIDI/VENDOR` | `0/1/1/0/0` | 分别关闭 CDC、启用 MSC 与 HID、关闭 MIDI 与厂商类。 |
| `CFG_TUD_HID_EP_BUFSIZE` | `16` | HID 端点缓冲大小。 |
| `CFG_TUD_MSC_EP_BUFSIZE` | `32768` | MSC 端点缓冲大小，消耗 32 KiB RAM。 |
| `CFG_TUSB_MEM_SECTION` | 空 | TinyUSB 缓冲段属性；当前不放入专用段。 |
| `CFG_TUSB_MEM_ALIGN` | `aligned(4)` | TinyUSB 缓冲按 4 字节对齐。 |

### `application/bsp_cfg.h` - 公共 I2C 引脚

**直接使用。** QMI8658 与 RTC 板级封装使用本配置。

| 配置项 | 当前值 | 作用 |
| --- | --- | --- |
| `I2C_PORT` | `i2c0` | 选择 I2C 控制器。 |
| `I2C_SDA` | `0` | SDA 使用 GPIO0。 |
| `I2C_SCL` | `1` | SCL 使用 GPIO1。 |

### `application/qmi8658_motion.h` - 姿态映射与手势方向

**直接使用。** 带 Configuration Wizard；请根据实际安装方向调整轴号和符号。

| 配置项 | 当前值 | 作用 |
| --- | --- | --- |
| `QMI8658_MOTION_SAMPLE_PERIOD_MS` | `20` | 姿态更新周期，范围 5-100 ms。 |
| `QMI8658_MOTION_USE_PRISM` | `0` | `0` 使用当前板装方向的屏幕 X/Y 映射；`1` 使用棱镜安装映射。 |
| `QMI8658_FIRE_USE_PRISM` | 跟随上项 | 火焰/FLIP 运动映射是否使用棱镜模式；不可与上项冲突。 |
| `QMI8658_FIRE_GRAVITY_MAPPING` | `SCREEN_Y` | 火焰重力映射；`SCREEN_Y=1`、`PRISM_Z=0`。 |
| `QMI8658_MOTION_ACCEL_[XYZ]_AXIS` | `0/1/2` | 屏幕加速度 X/Y/Z 对应芯片原始轴。 |
| `QMI8658_MOTION_ACCEL_[XYZ]_SIGN` | `1/1/1` | 各加速度轴方向，填 `1` 或 `-1`。 |
| `QMI8658_MOTION_GYRO_[XYZ]_AXIS` | `0/1/2` | 屏幕陀螺仪轴对应芯片原始轴。 |
| `QMI8658_MOTION_GYRO_[XYZ]_SIGN` | `1/1/1` | 各陀螺仪轴方向。 |
| `QMI8658_MOTION_GYRO_OFFSET_MAX_[XYZ]` | `24/24/24` | 允许的静止陀螺零偏上限；超出时不接受校准。 |
| `QMI8658_MOTION_GYRO_DEADZONE_Z_DPS100` | `100` | Z 轴死区，单位 $0.01\,dps$，即 1 dps。 |

### `application/arm_loader_io_fatfs.h` - Arm-2D 文件读取缓存

**条件使用。** 用于 SD 卡 QOI/LMSK 等资源的 FatFs 流式读取。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `ARM_LOADER_IO_FATFS_CACHE_SIZE` | `2*1024` | 每一路缓存窗口大小，单位字节。 |
| `ARM_LOADER_IO_FATFS_CACHE_WAYS` | `2` | 组相联缓存路数。总静态缓存约为 $2\,KiB \times 2$。 |
| `ARM_LOADER_IO_FATFS_USE_CACHE` | 派生为 `1` | 只要前两项都非零则启用；任一设为 `0` 则改为直接 `f_lseek()`/`f_read()`。 |

### `application/fal_cfg.h` 与 `application/rp2040_flash_layout.h` - Flash 分区

**直接使用。** 修改两者后须确保链接脚本、芯片实际 Flash 容量及资源写入工具一致。

| 文件 | 配置项 | 当前值 | 作用 |
| --- | --- | --- | --- |
| `rp2040_flash_layout.h` | `RP2040_FLASH_XIP_BASE` | `0x10000000` | RP2040 XIP 映射基址。 |
| 同上 | `RP2040_FLASH_TOTAL_SIZE` | `32 MiB` | 外接 Flash 总容量假设。 |
| 同上 | `RP2040_FLASH_CODE_OFFSET/SIZE` | `0 / 2 MiB` | 程序代码区起点和大小。 |
| 同上 | `RP2040_FLASH_PIC_OFFSET/SIZE` | `2 MiB / 30 MiB` | 图片资源区范围。 |
| 同上 | `RP2040_FLASH_PIC_XIP_BASE` | 派生 | 图片区 XIP 地址。 |
| `fal_cfg.h` | `FAL_FLASH_DEV_NAME` | `"rp2040_onchip"` | FAL 设备名。 |
| 同上 | `FAL_PART_TABLE` 的 `app` | 代码区，只读 | 固件分区。 |
| 同上 | `FAL_PART_TABLE` 的 `pic` | 图片区，可读写 | 图片/资源分区。 |

### 任务与调试头

| 文件 | 配置项 | 当前值 | 作用 |
| --- | --- | ---: | --- |
| `application/buzzer_task.h` | `BUZZER_TASK_REPEAT_PAUSE_MS` | `900 ms` | PCM 播放完成后的重复间隔。 |
| 同上 | `BUZZER_TASK_ENABLE_PRINTF` | `1` | 任务输出调试信息。 |
| `application/ir_task.h` | `IR_TASK_SEND_INTERVAL_MS` | `1000 ms` | 红外环回发送周期。 |
| 同上 | `IR_TASK_RESULT_DELAY_MS` | `60 ms` | 发送后等待接收结果的延迟。 |
| `application/light_task.h` | `LIGHT_TASK_INTERVAL_MS` | `500 ms` | 光敏任务输出/采样周期。 |
| `application/usb_debug.h` | `USB_DEBUG_PRINTF_ENABLED` | `0` | `1` 时将 USB 调试宏重定向到 `printf`。 |

### `project/mdk/arm_2d_scene_*.h` - 工程局部场景资源

这些文件受 `app_cfg.h` 覆盖，且对应 RTE Loader 组件必须已选中。

| 文件 | 配置项 | 默认值 | 作用 |
| --- | --- | --- | --- |
| `arm_2d_scene_histogram.h` | `ARM_2D_SCENE_HISTOGRAM_USE_QOI/JPG/ZJPGD` | `0/0/0` | 选择直方图背景解码器。QOI 启用时 JPG 路径会被关闭。 |
| 同上 | `ARM_2D_SCENE_HISTOGRAM_DATA_WINDOW_SIZE` | `14` | 直方图滑动数据窗口/柱数量。 |
| `arm_2d_scene_meter.h` | `ARM_2D_SCENE_METER_USE_QOI/JPG/ZJPGD` | `0/0/1` | 选择仪表背景解码器。 |
| 同上 | `ARM_2D_SCENE_METER_SHOW_FPS` | `0` | 显示仪表场景 FPS。 |
| `arm_2d_scene_qoi.h` | `ARM_2D_SCENE_QOI_IMAGE_PATH` | `"/image.qoi"` | QOI 场景从 FatFs 打开的资源路径。 |

## Keil `platform` 与 `drivers` 编组

### `platform/st7789_simple.h` - 显示尺寸常量

**直接使用。** `ST7789_WIDTH=320`、`ST7789_HEIGHT=240` 为驱动的固定画面尺寸。它们必须与 `arm_2d_disp_adapter_0.h` 的 `__DISP0_CFG_SCEEN_WIDTH__/HEIGHT__` 一致；改变 LCD 型号时需同步修改显示驱动和 PIO 时序，而不仅改这两个常量。

### `deivers/drv_QMI8658.h` - QMI8658 底层 IMU

**直接使用。** `I2C_PORT` 的默认值会被 `bsp_cfg.h` 的同名宏先行定义覆盖。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `Device_Address` / `QMI8658_ADDRESS_ALT` | `0x6B / 0x6A` | 主/备用 7 位 I2C 地址，取决于 AD0 接法。 |
| `QMI8658_I2C_BAUD_HZ` | `40000` | I2C 时钟频率。 |
| `QMI8658_CTRL2_VALUE` / `ACCRANGE` | `0x33 / 16 g` | 加速度寄存器值与换算满量程，必须匹配。 |
| `QMI8658_CTRL3_VALUE` / `GYRRANGE` | `0x73 / 2048 dps` | 陀螺仪寄存器值与换算满量程，必须匹配。 |
| `SAMPLERATE` | `1000 Hz` | 名义采样率，用于注释和调参参考。 |
| `QMI8658_STARTUP_SELF_TEST/COD/STILL_CALIBRATION` | `0/0/0` | 启动自检、按需校准、静止校准开关；启用会增加启动时间。 |
| `QMI8658_TAP_ENABLE` | `1` | 启用点击识别。 |
| `QMI8658_TAP_POLL_INTERVAL_MS` | `50 ms` | 点击状态轮询周期。 |
| `QMI8658_TAP_DEBUG_PRINTF` | `1` | 输出点击调试信息。 |
| `QMI8658_TAP_REPORT_SINGLE` | `0` | 是否上报单击。 |
| `QMI8658_TAP_EVENT_COOLDOWN_MS` | `300 ms` | 点击事件冷却时间。 |
| `QMI8658_TAP_DEBUG_INVALID_PRINTF` | `0` | 输出无效点击的诊断。 |
| `QMI8658_TAP_DEFAULT_PEAK_WINDOW/PRIORITY/TAP_WINDOW/DTAP_WINDOW` | `16/0/50/220` | 点击引擎的峰值窗口、优先级、单击和双击时间窗。 |
| `QMI8658_TAP_DEFAULT_ALPHA/GAMMA` | `0x02/0x08` | 点击算法滤波/灵敏度参数。 |
| `QMI8658_TAP_DEFAULT_PEAK_MAG_THR/UDM_THR` | `0x0180/0x00C0` | 点击峰值与未定义运动阈值。 |

### `deivers/drv_ir.h` - 红外收发

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `DRV_IR_TX_PIN` / `DRV_IR_RX_PIN` | `28 / 22` | 红外发射 PWM 和解调接收 GPIO。 |
| `DRV_IR_CARRIER_HZ` | `38000 Hz` | 典型红外接收头载波。 |
| `DRV_IR_DUTY_PERMILLE` | `2` | 发射 PWM 占空比（千分比），影响 LED 电流与距离。 |
| `DRV_IR_TX_ACTIVE_LOW` | `0` | `1` 适配低端 NPN 反相驱动。 |

### `deivers/drv_light.h` - GL5528 光敏电阻

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `DRV_LIGHT_ADC_GPIO/INPUT` | `26 / 0` | ADC 引脚与通道。 |
| `DRV_LIGHT_VREF_MV` | `3300 mV` | ADC 参考电压。 |
| `DRV_LIGHT_PULLUP_OHM` | `10000 Ω` | 分压上拉电阻。 |
| `DRV_LIGHT_DEFAULT_SAMPLE_COUNT` | `32` | 单次测量的平均样本数；增大可降噪但变慢。 |
| `DRV_LIGHT_GL5528_R10_OHM` | `15000 Ω` | 10 lux 参考阻值；应按实物校准。 |
| `DRV_LIGHT_GL5528_GAMMA` | `0.70` | 光阻-照度幂律指数；应按传感器曲线校准。 |

### `deivers/drv_buzzer.h` - 无源蜂鸣器

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `DRV_BUZZER_PIN` | `23` | 蜂鸣器输出 GPIO。 |
| `DRV_BUZZER_DUTY_PERMILLE` | `500` | 默认 PWM 占空比，约 50%。 |
| `DRV_BUZZER_PCM_CARRIER_HZ` | `20000 Hz` | PCM 调制 PWM 载波；提高可减小可闻啸叫。 |
| `DRV_BUZZER_PCM_CLOCK_SLICE` | `7` | PCM 采样中断使用的 PWM slice；勿与蜂鸣器 GPIO 对应 slice 冲突。 |

## Keil `FatFs_SDIO`、`FAL`、`TinyUSB` 编组

### `middleware/fatfs_sd/port/rp2040_sdcard_config.h` - SDIO 接线

**直接编译。** PIO SDIO 对引脚有严格关系：`CLK=(D0-2) mod 32`，`D1/D2/D3=D0+1/+2/+3`。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `RP2040_SDIO_CMD_GPIO` | `4` | SD 卡 CMD 引脚。 |
| `RP2040_SDIO_D0_GPIO` | `5` | SD 卡 D0；其余数据和时钟由 PIO 规则推导。 |
| `RP2040_SDIO_BAUD_RATE` | `125 MHz / 6` | SDIO 工作时钟，约 20.83 MHz。 |

`application/usb_msc_sd.h`、`application/rp2040_sdcard.h` 和 `middleware` 内其余头文件为 API、寄存器定义或数据结构，不含本工程可调的功能配置。

## Keil `::Acceleration` RTE 编组

### `project/mdk/RTE/Acceleration/arm_2d_cfg.h` - Arm-2D 全局特性

**直接使用。** 带 Configuration Wizard。

| 配置组 | 配置项（当前值） | 作用 |
| --- | --- | --- |
| 异步与变换 | `__ARM_2D_HAS_ASYNC__=0` | 关闭异步 programmer 模型，节省代码。 |
| 同上 | `__ARM_2D_HAS_ANTI_ALIAS_TRANSFORM__=1` | 启用变换 4x SSAA 抗锯齿，增加计算量。 |
| 同上 | `__ARM_2D_CFG_SUPPORT_COLOUR_CHANNEL_ACCESS__=0` | 关闭 `ARM_2D_CHANNEL_8in32` 通道访问。 |
| 同上 | `__ARM_2D_CFG_SUPPORT_CCCA8888_IMPLICIT_CONVERSION__=0` | 关闭 ARGB8888 隐式转换。 |
| 模糊/字体 | `__ARM_2D_CFG_USE_IIR_BLUR_REVERSE_PATH__=0` | 关闭高质量 IIR 模糊反向路径。 |
| 同上 | `__ARM_2D_CFG_SUPPORT_TRANSFORM_FOR_NON_A8_FONTS__=1` | 允许 A1/A2/A4 字体缩放。 |
| 日志 | `__ARM_2D_CFG_ENABLE_LOG__=0` | 关闭 Arm-2D 日志。 |
| 同上 | `__ARM_2D_CFG_LOG_OUTPUT_SUPPORT_COLOUR__=0` | 日志终端不使用颜色。 |
| 同上 | `__ARM_2D_LOG_MAX_STRING_LEN__=256` | 单条日志申请的最大缓冲。 |
| 同上 | `__ARM_2D_LOG_CHANNEL_MASK_FILTER__` | 当前包含全部日志通道；仅在启用日志时有意义。 |
| 调试 | `__ARM_2D_HELPER_CFG_LAYOUT_DEBUG_MODE__=0` | 关闭布局区域标记。 |
| 优化 | `__ARM_2D_CFG_CALL_NON_OPACITY_VERSION_IMPLICITILY_FOR_255__=0` | 不自动把 alpha=255 调用切换为非透明 API。 |
| 同上 | `__ARM_2D_CFG_OPTIMIZE_FOR_POINTER_LIKE_SHAPES_IN_TRANSFORM__=1` | 启用指针形资源变换优化。 |
| 同上 | `__ARM_2D_CFG_OPTIMIZE_FOR_HOLLOW_OUT_MASK_IN_TRANSFORM__=0` | 关闭镂空掩码特化优化。 |
| 同上 | `__ARM_2D_CFG_OPTIMIZE_FOR_PFB_IN_LAYOUT_ASSISTANT__=0` | 关闭 Layout Assistant PFB 优化。 |
| 同上 | `__ARM_2D_CFG_DISABLE_ANTI_ALIAS_IN_FILL_COLOUR_WITH_TRANSFORMED_MASK_AND_TARGET_MASK__=0` | 保留该操作的抗锯齿。 |
| 显示/基准 | `__GLCD_CFG_COLOUR_DEPTH__=16` | 全局 LCD 色深，决定 RGB565 资源格式。 |
| 同上 | `__LCD_PRINTF_CFG_TEXT_BUFFER_SIZE__=256` | LCD printf 缓冲。 |
| 同上 | `__GLCD_CFG_SCEEN_WIDTH__/HEIGHT__=320/240` | 基准演示画面尺寸。 |
| 同上 | `ITERATION_CNT=1000` | Arm-2D 基准运行次数。 |
| 同上 | `__ARM_2D_CFG_BENCHMARK_TINY_MODE__=0` | 关闭低内存基准模式。 |
| 同上 | `__ARM_2D_CFG_WATCH_PANEL_STOPWATCH_MODE__=0` | 关闭表盘秒针模式。 |
| 同上 | `__ARM_2D_CFG_BENCHMARK_WATCH_PANEL_USE_NEBULA__=0` | 关闭 Tiny Mode 星云表盘。 |
| 同上 | `__ARM_2D_CFG_BENCHMARK_EXIT_WHEN_FINISH__=0` | 基准完成后不自动退出。 |
| 同上 | `__ARM_2D_CFG_CONTROL_TEXT_BOX_USE_CONTEXT__=0` | 关闭文本框上下文缓存。 |

文件中被注释的 `__ARM_2D_CFG_PFB_RETURN_SKIPPED__` 与各 `__ARM_2D_CFG_UNSAFE_*` 宏是可选性能/画质折中项；默认不定义。它们可能改变行为或降低精度，除非已完成显示回归验证，不应启用。

### `project/mdk/RTE/Acceleration/arm_2d_disp_adapter_0.h` - 显示与 PFB

**直接使用。** `PFB` 是局部帧缓冲，内存量大致受 `宽 × 高 × 色深 × 块数` 影响。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `__DISP0_CFG_NANO_ONLY__` | `0` | 使用完整场景播放器，不限于 Nano 模式。 |
| `__DISP0_CFG_COLOR_SOLUTION__` | `0` | 无预设单色方案。 |
| `__DISP0_CFG_COLOUR_DEPTH__` | `16` | RGB565；单色方案为 `1` 时会强制改为 8 位。 |
| `__DISP0_CFG_SCEEN_WIDTH__/HEIGHT__` | `320/240` | 屏幕分辨率。 |
| `__DISP0_CFG_ROTATE_SCREEN__` | `0` | 不旋转；软件旋转很慢。 |
| `__DISP0_CFG_PFB_BLOCK_WIDTH/HEIGHT` | `320/60` | 每块 PFB 尺寸。 |
| `__DISP0_CFG_PFB_PIXEL_ALIGN_WIDTH/HEIGHT` | `1/0` | PFB X 方向按 $2^1$ 像素对齐，Y 方向按 1 像素对齐。 |
| `__DISP0_CFG_PFB_HEAP_SIZE__` | `2` | PFB 块池数量。 |
| `__DISP0_CFG_DISABLE_DYNAMIC_PFB__` | `0` | 保留动态 PFB 优化。 |
| `__DISP0_CFG_NAVIGATION_LAYER_MODE__` | `0` | 关闭导航层/FPS 导航显示。 |
| `__DISP0_CFG_ITERATION_CNT__` | `30` | FPS 统计帧数。 |
| `__DISP0_CFG_FPS_CACULATION_MODE__` | `1` | 使用真实 FPS，而非仅渲染 FPS。 |
| `__DISP0_CFG_USE_CONSOLE__` | `0` | 关闭浮动控制台。 |
| `__DISP0_CFG_CONSOLE_INPUT_BUFFER` | `255` | 控制台输入缓冲；仅启用控制台时有效。 |
| `__DISP0_CFG_CONSOLE_DISPALY_TIME__` | `3000 ms` | 控制台内容显示时长。 |
| `__DISP0_CFG_DEBUG_DIRTY_REGIONS__` | `0` | 关闭脏矩形可视化。 |
| `__DISP0_CFG_OPTIMIZE_DIRTY_REGIONS__` | `1` | 启用脏矩形优化。 |
| `__DISP0_CFG_DIRTY_REGION_POOL_SIZE__` | `8` | 脏矩形对象池大小。 |
| `__DISP0_CFG_SWAP_RGB16_HIGH_AND_LOW_BYTES__` | `1` | 交换 RGB565 高低字节，需与 LCD 总线字节序匹配。 |
| `__DISP0_CFG_ENABLE_ASYNC_FLUSHING__` | `1` | 启用异步刷屏；必须由平台实现完成回调。 |
| `__DISP0_CFG_ENABLE_3FB_HELPER_SERVICE__` | `0` | 关闭 LCD Direct/3FB 服务。 |
| `__DISP0_CFG_DISABLE_DEFAULT_SCENE__` | `1` | 不加载默认场景。 |
| `__DISP0_CFG_VIRTUAL_RESOURCE_HELPER__` | `2` | 每 API 可使用 1 个虚拟资源。 |
| `__DISP0_CFG_USE_HEAP_FOR_VIRTUAL_RESOURCE_HELPER__` | `0` | 虚拟资源辅助器使用静态 PFB 缓冲，而非 heap。 |
| `__DISP0_CFG_PFB_ANS_WIDTH/HEIGHT` | `8/8` | 抗噪扫描块大小。 |

### 图像/字体解码器配置

| 文件 | 配置项（当前值） | 作用 |
| --- | --- | --- |
| `arm_qoi_cfg.h` | `ARM_QOI_IO_BUFF_SIZE=8192` | QOI Loader I/O 缓冲，单位字节。 |
| 同上 | `__ARM_QOI_USE_LOADER_IO__=1` | 通过 Loader I/O 流式读取；设 `0` 后缓冲自动为 `0`。 |
| `arm_lmsk_cfg.h` | `__ARM_LMSK_USE_LOADER_IO__=1` | LMSK 从 Loader I/O 读取。 |
| 同上 | `__ARM_LMSK_LSB_COMPENSATION_POLICY__=0` | alpha 不做 LSB 补偿；`1` 仅补最大 alpha，`2` 线性插值。 |
| `arm_tjpgd_cfg.h` | `JD_SZBUF=1024` | JPEG 输入流缓冲，单位字节。 |
| 同上 | `JD_USE_SCALE=0` | 关闭 JPEG 输出缩放。 |
| 同上 | `JD_TBLCLIP=1` | 启用饱和运算查表，约增加 1 KiB 代码换取速度。 |
| 同上 | `JD_FASTDECODE=1` | 32 位 MCU 优化级别；`2` 更快但需要更多 RAM。 |
| 同上 | `JD_FORMAT` | **派生**于全局色深；当前为 `1`（RGB565）。 |
| 同上 | `JD_SWAP_RED_AND_BLUE=1` | 交换 JPEG 解码结果的 R/B 通道。 |
| `arm_zhrgb565_cfg.h` | `__ARM_2D_ZHRGB565_USE_LOADER_IO__=1` | ZhRGB565 使用 Loader I/O。 |
| 同上 | `__ARM_2D_ZHRGB565_PIXEL_CACHE_SIZE__=32` | 像素缓存数量。 |
| `arm_zjpgd_cfg.h` | `ZJD_MEM_POOL_SZ=3072` | ZJpgD 内存池，单位字节。 |
| 同上 | `ZJD_DEBUG=0` | 关闭 ZJpgD 调试日志。 |
| `arm_freetype_cfg.h` | `ARM_FREETYPE_CFG_SUPPORT_CFF=1` | 支持 CFF/OTF 字体，增加代码体积。 |

### RTE 场景头中的额外选项

| 文件 | 配置项 | 当前默认值 | 作用 |
| --- | --- | ---: | --- |
| `arm_2d_scene_0.h` | `__USER_SCENE0_ENABLE_3D__` | `0` | 场景 0 是否引入 3D loader。 |
| 同上 | `__USER_SCENE0_USE_LMSK__` | `0` | 场景 0 是否使用 LMSK 资源。 |

其他 `arm_2d_scene_*.h`、`arm_2d_user_opcode_*.h` 主要定义场景 OOC 类型、回调和接口，没有独立的用户配置宏。

## Keil `::CMSIS-Compiler`、`::CMSIS-View`、`::Device`、`::Utilities` 编组

### `RTE/Compiler/EventRecorderConf.h` 与 `RTE/CMSIS-View/EventRecorderConf.h`

两个文件的配置语义相同；当前值不同后缀但数值一致。避免同时以不同配置替换同一组件。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `EVENT_RECORD_COUNT` | `64` | Event Recorder 环形缓冲记录数；每条 16 字节。必须是 $2^n$，范围 8-65536。 |
| `EVENT_TIMESTAMP_SOURCE` | `3` | 时间戳来源：`0` DWT、`1` SysTick、`2` RTOS 定时器、`3` 用户定时器普通复位、`4` 用户定时器上电复位。RP2040 Cortex-M0+ 不适用 DWT。 |
| `EVENT_TIMESTAMP_FREQ` | `0` | 初始时间戳频率；未使用时为 0。 |

### `RTE/Device/RP2040_Core0/boot2_config.h` - boot2 Flash 启动

**直接使用。** 此文件由汇编器 `-include` 强制包含。

| 配置项 | 当前值 | 作用 |
| --- | --- | --- |
| `PICO_BOOT_STAGE2_CHOOSE_W25Q080` | `0` | 不使用 W25Q080 专用 QE 处理。 |
| `PICO_BOOT_STAGE2_CHOOSE_GENERIC_03H` | `1` | 使用通用 0x03 读命令 boot2。 |
| `PICO_FLASH_SPI_CLKDIV` | `6` | boot2 SPI 时钟分频。修改前必须验证外接 Flash 型号、时钟裕量及上电稳定性。 |

### `RTE/Utilities/core_portme.h` - CoreMark 移植配置

**条件使用。** 仅当 RTE 选择 Benchmark/CoreMark 且应用调用 `coremark_main()` 时生效。

| 配置项 | 当前值 | 作用 |
| --- | ---: | --- |
| `HAS_FLOAT/HAS_TIME_H/USE_CLOCK/HAS_STDIO/HAS_PRINTF` | `1/1/1/1/1` | 声明平台具备这些运行库能力。 |
| `COMPILER_VERSION` | 自动识别 | 生成报告中的编译器名称。 |
| `COMPILER_FLAGS` | `"Unspecified"` | 生成报告中的编译标志文字。 |
| `SEED_METHOD` | `SEED_VOLATILE` | CoreMark 随机种子获取方式。 |
| `MEM_METHOD` | `MEM_STACK` | CoreMark 数据放在栈；可选 `MEM_MALLOC`、`MEM_STATIC`。 |
| `MULTITHREAD` | `1` | 单上下文运行。 |
| `MAIN_HAS_NOARGC/MAIN_HAS_NORETURN` | `0/0` | `main` 支持参数且可返回。 |
| `ITERATIONS` | `6000` | CoreMark 迭代次数；需保证运行至少 10 秒才可获得有效成绩。 |

## Keil `flip` 与 `Infinite Corridor` 编组

### `fire/flip_config.h` - FLIP 流体模拟

**条件使用。** 带完整 Configuration Wizard。除下表外的 `*_Q16` 宏为派生定点数值，不应单独改动。

| 分类 | 配置项（当前值） | 作用 |
| --- | --- | --- |
| 资源限制 | `FLIP_GRID_W/H=36/36` | 模拟网格尺寸；增大可提高细节，也提高 CPU/RAM。 |
| 同上 | `FLIP_MAX_PARTICLES=768` | 最大粒子数。 |
| 同上 | `FLIP_MEM_SIZE=65536` | 静态内存池，单位字节。 |
| 同上 | `FLIP_RENDER_MAX_WIDTH/HEIGHT=240/240` | 渲染查表缓冲的最大尺寸。 |
| 求解器 | `FLIP_PARTICLE_CELL_SCALE_X100=220` | 粒子直径相对单元边长为 2.20。 |
| 同上 | `FLIP_PRESSURE_ITERS=24` | 压力求解迭代；增大通常更稳定但更慢。 |
| 同上 | `FLIP_PARTICLE_ITERS=1` | 粒子分离迭代数。 |
| 同上 | `FLIP_DEFAULT_FLIP_RATIO_PERCENT=95` | FLIP/PIC 混合比例；降低会增加阻尼和稳定性。 |
| 同上 | `FLIP_OVER_RELAXATION_X100=190` | 压力松弛系数 1.90。 |
| 同上 | `FLIP_SIMULATION_RATE_HZ=60` | 模拟更新率。 |
| 同上 | `FLIP_VELOCITY_LIMIT_X100=1000` | 全局速度上限 10.00。 |
| 重力/运动 | `FLIP_GRAVITY_MAGNITUDE_X100=981` | 向下固定重力 $9.81\,m/s^2$。 |
| 同上 | `FLIP_MOTION_GRAVITY_SCALE_X100=981` | IMU 重力映射比例。 |
| 同上 | `FLIP_MOTION_ACCEL_SCALE_X100=600` | IMU 加速度映射比例 6.00。 |
| 同上 | `FLIP_MOTION_ACCEL_DEADZONE_X100000=3125` | IMU 加速度死区 $0.03125\,g$。 |
| 同上 | `FLIP_MOTION_FORCE_LIMIT_X100=4000` | 外力上限 40.00。 |
| 沙漏容器 | `FLIP_USE_HOURGLASS_CONTAINER=1` | 使用沙漏形边界；设 `0` 使用普通方形边界。 |
| 同上 | `FLIP_HOURGLASS_WAIST_PERCENT=1` | 沙漏腰部半宽占最大半宽的百分比。 |
| 同上 | `FLIP_HOURGLASS_NECK_HALF_HEIGHT_CELLS=2` | 腰部限流区上下半高，单位格。 |
| 同上 | `FLIP_HOURGLASS_NECK_SPEED_X100000=1526` | 腰部速度限制 0.01526。 |
| 渲染 | `FLIP_RENDER_DENSITY_FIELD=1` | 密度场作为求解器颜色来源。 |
| 同上 | `FLIP_SIM_DEFAULT_RENDER_MODE=0` | `0` 粒子模式，`1` 连续表面。 |
| 同上 | `FLIP_SIM_PARTICLE_RED_SPEED_X100=200` | 粒子转红速度阈值 2.00。 |
| 同上 | `FLIP_CONTAINER_SCALE_PERCENT=82` | 容器占可用渲染区域的比例。 |
| 同上 | `FLIP_SURFACE_THRESHOLD_SHIFT=4` | 连续表面可见阈值偏移。 |
| 同上 | `FLIP_CONTAINER_BORDER_COLOUR=0x0000` | RGB565 边框颜色；黑色相当于不可见。 |

`fire/user_generic_loader_fire.h`、`fire/user_generic_loader_flip.h`、`corridor/user_generic_loader_infinite_corridor.h` 只提供 Arm-2D loader 类型与接口，不含用户配置项。

## 不应作为配置入口的头文件

以下头文件虽然存在宏，但宏是 include guard、寄存器地址、数据模型、自动生成代码或第三方库内部实现，并非本工程的常规配置入口：

- `3d/*.h`：模型声明、定点数学和 loader API；模型内容由 `3d/tools/` 生成。
- `platform/st77xx_parallel_*.pio.h`：由 `.pio` 通过 `pioasm` 生成，修改会在下次构建覆盖。
- `platform/platform.h`、`application/bm8563_task.h`、`application/qmi8658c_task.h`、`application/usb_mouse.h`、`application/usb_msc_sd.h`：接口声明。
- `deivers/bm8563.h`、`deivers/drv_paj7620.h`：设备寄存器/协议定义；除非迁移芯片或重写驱动，不要当作项目配置修改。
- `middleware/fatfs_sd` 下的 `ff.h`、`diskio.h`、`sd_regs.h` 与 SPI/SDIO API 头：第三方接口或协议常量。
- `lib/tinyusb/` 内的头文件：上游 TinyUSB 源码和测试配置；工程可调的 USB 配置集中在 `project/mdk/tusb_config.h`。

## 修改前检查清单

1. 改变引脚、I2C 地址、Flash 容量或 SDIO 接线前，先与原理图核对。
2. 改变屏幕尺寸、色深或字节序时，同步检查 ST7789 驱动、PFB 尺寸和图像资源格式。
3. 增大 PFB、USB MSC、FatFs 缓存或 FLIP 粒子/网格前，评估 RP2040 可用 SRAM；这些配置可能同时占用大量静态内存。
4. 修改 IMU 轴映射后，分别验证显示方向、姿态响应、点击检测和 FLIP 重力方向。
5. 修改 Flash 分区后，重新检查资源烧录地址，避免覆盖应用代码。
6. 修改 `boot2_config.h` 或任何 RTE 设备启动文件前，保留可恢复的下载方式并实际验证冷启动。