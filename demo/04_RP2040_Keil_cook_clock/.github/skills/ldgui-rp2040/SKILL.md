---
name: ldgui-rp2040
description: "用于开发、扩展、调试或快速理解基于 LingDongGUI（LDGUI）、GuiEasyEditor 和 Arm-2D 的 RP2040 Keil 烹饪时钟开发板，包括显示场景、控件、定时器、页面跳转和板载驱动。"
argument-hint: "描述要实现的界面、控件、板载功能或待修复的问题"
user-invocable: true
---

# RP2040 烹饪时钟上的 LDGUI

使用本技能可在保持 LDGUI 生成页面结构和 Arm-2D 场景规范的前提下，快速为该开发板添加显示功能。

## 系统结构

- `main.c` 初始化 RP2040 平台，注册初始页面函数组，然后持续调用 `ldGuiLoop()`。
- `platform/pi_platform.c` 启动时钟、性能计数器、stdio 和 ST7789 显示屏。必须先调用 `platform_init()`，再调用 `ldGuiInit()`。
- `project/mdk/eeProject/GuiUser/<page>.c` 是生成的页面/UI 配置代码，负责创建控件、静态文本、样式、定时器和生命周期转发。
- `project/mdk/eeProject/GuiUser/<page>Logic.c` 是手写业务行为层，负责轮询驱动、更新控件、定时器回调、页面跳转和可选的 Arm-2D 绘制。
- `deivers/` 包含芯片级驱动。板级引脚配置和任务策略应放在生成页面文件之外。该目录的拼写是刻意保留的，因为 Keil 工程已引用它。
- `project/mdk/template.uvprojx` 是 Keil MDK 构建工程；工程指南指定使用 `D:\keil538\UV4\UV4.exe`。

## 快速理解工程

1. 阅读 `main.c`、`platform/pi_platform.c`，以及目标页面的 `<page>.c`、`<page>.h` 和 `<page>Logic.c`。
2. 编码前先确认本地 `ldGui.h` 中的确切 API。上游 LingDongGUI 版本可能不同，不能未经核对就复制在线示例中的宏签名。
3. 从 `home.c` 和 `homeLogic.c` 了解可运行模式：生成的 UI 负责构造控件，`homeLogic.c` 在定时器回调中按 ID 获取控件并调用其 setter 更新。
4. 参考 `page1Logic.c`，了解最小化的页面跳转和控件动画示例。

## 实现连接板载硬件的 UI 功能

1. 先确定功能需要新增页面，还是只需在已有页面增加一个控件。
2. 处理布局时，使用 GuiEasyEditor 编辑对应的 `.ui` 文件，并重新生成关联的 `GuiUser/<page>.c/.h`。除非无法重新生成且修改只是临时方案，否则不要手改生成的控件构造代码。
3. 将业务行为放在 `GuiUser/<page>Logic.c`：
   - 在 `<page>LogicInit()` 或应用封装层初始化板载驱动。
   - 在 `<page>LogicLoop()` 或周期性的生成定时器回调中轮询轻量状态。
   - 按控件 ID 更新生成控件，例如先调用 `ldBaseGetWidgetById(ID_PROGRESSBAR_0)`，再调用相应的 `ld*Set...` API。
   - 使用 `ldGuiJumpPage(...)` 进行页面跳转，并以本地 `ldGui.h` 的实际声明为准。
4. 保持循环非阻塞。较长的传感器传输、延时、声音播放或动画应表示为状态，并在多次 `ldGuiLoop()` 调用中逐步推进。
5. 像生成页面一样，使用 `ldTimeOut(period_ms, repeat, &timer)` 执行周期工作；在页面初始化时重置该页面的定时器变量。
6. 一致地使用页面生命周期：
   - `LogicInit`：获取或初始化功能状态。
   - `LogicFrameStart`：按需准备一帧。
   - `LogicLoop`：轮询并更新状态。
   - `LogicDraw`：仅当标准 LDGUI 控件不足时，执行自定义 Arm-2D 绘制。
   - `LogicQuit`：释放资源或停止页面专属活动。

## Arm-2D 的使用边界

- 标签、文本、进度条、二维码、菜单和普通控件优先使用 LDGUI 控件。
- 需要自定义动画或渲染时，在 `<page>LogicDraw(ld_scene_t *, arm_2d_tile_t *, bool)` 中使用传入的 tile 绘制。不要在应用逻辑中直接向显示屏绘制，从而绕过 LDGUI/Arm-2D 的帧流程。
- 新建原生 Arm-2D 场景时，遵循本仓库的规定命名和结构：`arm_2d_scene_<name>.c/.h`、`user_scene_<name>_t`、`__arm_2d_scene_<name>_init`、`__on_scene_<name>_*` 和 `__pfb_draw_scene_<name>_handler`。以 `project/mdk/RTE/Acceleration/` 下现有的 gas-gauge 场景为模板。

## 可用硬件功能入口

- RTC：`deivers/bm8563.c/.h`；使用带读写回调的 `bm8563_t` 实例。
- IMU：`deivers/drv_QMI8658.c/.h`；`QMI8658A_Get_G_DPS(float data[6])` 依次返回加速度和角速度。
- 手势：`deivers/drv_paj7620.c/.h`；使用 `paj7620Init()` 初始化，手势标志位定义在 `GES_enum` 中。
- 光照：`deivers/drv_light.c/.h`；使用 `drv_light_init()` 初始化，并通过 `drv_light_read()` 采样。
- 蜂鸣器：`deivers/drv_buzzer.c/.h`；优先使用其非阻塞的音调、乐谱或 PCM API。
- 红外：`deivers/drv_ir.c/.h`；调用 `drv_ir_send_byte_start()` 后，发射过程为非阻塞。

## 构建与烧录

1. 先验证工程指南指定的 MDK 可执行文件：

   ```powershell
   Test-Path 'D:\keil538\UV4\UV4.exe'
   ```

   若该文件不存在，请定位实际安装的 `UV4.exe`，或在 Keil IDE 中构建。在真正的 MDK 构建完成前，不要将固件构建报告为已验证。

2. 在 Keil 中构建活动工程，或运行：

   ```powershell
   & 'D:\keil538\UV4\UV4.exe' -b '.\project\mdk\template.uvprojx' -j0
   ```

3. 优先解决生成页面头文件或 `ldGui.h` 中的编译错误，再考虑修改无关显示代码。
4. 使用已配置的 Keil 调试/下载设置烧录 RP2040，然后在硬件上验证功能。

## 完成检查

声明 GUI 功能完成前，确认以下项目：

- `platform_init()` 位于 `ldGuiInit()` 之前，且初始页面函数组有效。
- 页面回调中未加入阻塞延时或无限工作循环。
- 生成的页面构造代码与手写 `Logic` 业务行为保持分离。
- 控件 ID 存在于当前生成的页面头文件中。
- Keil 工程构建成功，并已在开发板上实际验证目标行为。

## 上游说明

LingDongGUI 是对 Arm-2D 的封装，支持与原生 Arm-2D API 混合使用。其上游仓库和本模板的版本可能不同，因此 API 签名和配置应以本地头文件与生成代码为准。