# 应用层外设接口

`board_peripherals` 在芯片驱动之上提供适合主循环调用的蜂鸣器、QMI8658 和 GPIO
按键接口。I2C 默认使用 `i2c0` 的 `GPIO0`（SDA）和 `GPIO1`（SCL）；可在
`bsp_cfg.h` 中覆盖。

在 `main()` 的 `platform_init()` 之后初始化，并在主循环中调用任务函数：

```c
#include "application/board_peripherals.h"

static app_button_t s_tStartButton;

int main(void)
{
    platform_init();
    app_peripherals_init();
    app_button_init(&s_tStartButton, 15u, true, 20u);

    while (true) {
        uint32_t now_ms = to_ms_since_boot(get_absolute_time());

        app_peripherals_task(now_ms);
        app_button_poll(&s_tStartButton, now_ms);

        if (app_button_was_pressed(&s_tStartButton)) {
            app_buzzer_beep(1000u, 80u);
        }
    }
}
```

按键引脚未预设，因为父工程没有定义此开发板的普通 GPIO 按键连接。示例使用
`GPIO15`，按键另一端接地，因此参数 `true` 表示低电平有效；应按实际硬件修改。

使用 `app_imu_read(float data[6])` 可取得 `data[0..2]` 的加速度（g）和
`data[3..5]` 的角速度（dps）。QMI8658 未连接或初始化失败时，该函数返回 `false`。

蜂鸣器使用 `GPIO23`。`app_buzzer_beep()` 是非阻塞的，需保持调用
`app_peripherals_task()` 才会在指定时间停止。

调用 `app_buzzer_play_happy_birthday()` 可播放内置生日快乐旋律；该 API 同样是非阻塞的，
由 `app_peripherals_task()` 推进音符。

## 提示音策略

使用 `cook_clock_set_countdown_finished_effect()` 可绑定倒计时结束时的显示与声音：

- `COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_DEFAULT`：上行确认音与 `00:00`。
- `COOK_CLOCK_COUNTDOWN_FINISHED_EFFECT_BIRTHDAY`：生日快乐旋律与 `happy birthday`。

播放过程非阻塞，由 `app_peripherals_task()` 推进。调用
`app_alert_set_handler()` 可替换应用自己的音符表、PCM 或视觉提示策略：

```c
static void app_custom_alert(app_alert_t alert, void *target)
{
    (void)target;

    if (APP_ALERT_COUNTDOWN_FINISHED == alert) {
        app_buzzer_beep(1000u, 120u);
    }
}

app_alert_set_handler(app_custom_alert, NULL);
```