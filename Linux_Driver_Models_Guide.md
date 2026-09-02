# Linux 驱动模型与子系统全景指南

在 Linux 内核中，驱动开发远不止传统的“字符设备”或“块设备”。Linux 具有高度模块化的架构，针对不同类型的硬件抽象出了众多**设备模型（Device Models）**和**子系统框架（Subsystems / Frameworks）**。

当你挂载一个 I2C 或 SPI 设备时，你可以选择将它包装成最原始的字符设备（提供 `/dev/xxx` 供应用层 ioctl/read/write），但这往往是最笨拙的方式。更优雅的做法是**将其对接到内核已有的专用子系统中**（例如：SPI 屏幕对接到 Framebuffer 框架，SPI 传感器对接到 IIO 框架）。

以下是 Linux 驱动模型的全景解析。

---

## 一、 基础三大设备类型
这是 Linux 最古老、最基础的分类，决定了设备在用户空间的展现形式。

### 1. 字符设备 (Character Device)
* **概述**：按字节流顺序访问的设备，不支持随机读取。绝大多数非存储外设（串口、按键、自定义SPI/I2C外设）如果没有合适的子系统，通常会写成字符设备。
* **头文件**：`<linux/fs.h>`, `<linux/cdev.h>`
* **核心 API**：
  * `alloc_chrdev_region()`, `register_chrdev_region()`: 申请设备号。
  * `cdev_init()`, `cdev_add()`: 初始化并向系统添加字符设备。
  * `struct file_operations`: 定义 `open`, `read`, `write`, `ioctl`。

### 2. 块设备 (Block Device)
* **概述**：支持随机访问，以数据块（如512字节）为单位进行缓存和读写的设备。主要用于存储介质（硬盘、eMMC、SD卡）。
* **头文件**：`<linux/blkdev.h>`, `<linux/genhd.h>`
* **核心 API**：
  * `register_blkdev()`: 注册块设备。
  * `alloc_disk()`, `add_disk()`: 分配和添加通用磁盘结构 (gendisk)。
  * `blk_mq_init_sq_queue()`: 初始化多队列块层请求队列。

### 3. 网络设备 (Network Device)
* **概述**：面向报文（数据包）的设备，没有对应的 `/dev/xxx` 节点，而是通过 `eth0`, `wlan0` 等接口和 Socket 进行交互。
* **头文件**：`<linux/netdevice.h>`, `<linux/etherdevice.h>`
* **核心 API**：
  * `alloc_etherdev()`: 分配网络设备结构体 `net_device`。
  * `register_netdev()`: 注册网络设备。
  * `struct net_device_ops`: 定义 `ndo_open`, `ndo_start_xmit` (发送数据包)。

---

## 二、 Linux 设备驱动模型 (LDM - Linux Device Model)
为了解决设备电源管理、即插即用和设备树（Device Tree）匹配问题，Linux 引入了 LDM，其核心思想是**分离设备（Device）与驱动（Driver）**，通过**总线（Bus）**进行匹配。

### 1. 平台设备模型 (Platform Device Model)
* **概述**：针对 CPU 内部集成的外设（如 I2C 控制器、SPI 控制器、内部 UART）发明的虚拟总线模型。
* **头文件**：`<linux/platform_device.h>`
* **核心 API**：
  * `module_platform_driver()`: 注册平台驱动宏。
  * `platform_get_resource()`: 从设备树获取寄存器物理地址或中断号。

### 2. I2C / SPI 核心模型
* **概述**：外接在 I2C/SPI 控制器上的芯片（如 EEPROM、传感器、屏幕）。你的驱动注册为 i2c_driver/spi_driver，当设备树中有对应的节点时，内核会自动调用你的 `probe` 函数。
* **头文件**：`<linux/i2c.h>`, `<linux/spi/spi.h>`
* **核心 API**：
  * `i2c_add_driver()`, `spi_register_driver()`: 注册总线驱动。
  * `spi_sync()`, `spi_write()`, `i2c_transfer()`: 总线数据收发。

---

## 三、 高级子系统与框架 (针对外设的 IO 模型)
当你在 SPI 上接了一个 ADC 传感器时，你不应该写一个字符驱动让用户空间去解析 SPI 协议。你应该写一个 **IIO 驱动**，内核会帮你把数据标准化，暴露在 `/sys/bus/iio/` 目录下。这就是“针对性 IO 模型”。

### 1. IIO 子系统 (Industrial I/O)
* **概述**：专门用于处理模数转换器 (ADC)、数模转换器 (DAC)、加速度计、陀螺仪等传感器的框架。
* **头文件**：`<linux/iio/iio.h>`, `<linux/iio/sysfs.h>`
* **核心 API**：
  * `iio_device_alloc()`: 分配 IIO 设备。
  * `iio_device_register()`: 注册设备。提供标准化的 sysfs 接口读取电压或加速度值。

### 2. Input 子系统
* **概述**：用于处理人机交互设备（按键、鼠标、触摸屏、摇杆）。你的 SPI 触摸屏驱动读到坐标后，通过 Input 框架上报，应用层的 Qt/Android 就能直接识别，而无需关心底层是 SPI 还是 I2C。
* **头文件**：`<linux/input.h>`
* **核心 API**：
  * `input_allocate_device()`: 分配 input 设备。
  * `input_report_key()`, `input_report_abs()`: 上报按键事件或绝对坐标。
  * `input_register_device()`: 注册后在 `/dev/input/eventX` 生成标准事件节点。

### 3. Framebuffer / DRM 子系统 (显示设备)
* **概述**：针对屏幕显示设备。Framebuffer 是较老的简单模型，DRM (Direct Rendering Manager) 是现代图形架构。
* **头文件**：`<linux/fb.h>` 或 `<drm/drm_drv.h>`
* **核心 API (FB)**：
  * `framebuffer_alloc()`: 分配 `fb_info` 结构。
  * `register_framebuffer()`: 注册后生成 `/dev/fb0`，内核控制台和图形界面可以直接向其映射的内存写入像素。

### 4. Regmap 子系统
* **概述**：极度强悍的寄存器抽象层。无论你的芯片是挂在 I2C 还是 SPI 上，只要它是基于“寄存器地址-值”交互的，你都可以用 Regmap。内核帮你处理锁机制、缓存 (Cache)、端序转换。
* **头文件**：`<linux/regmap.h>`
* **核心 API**：
  * `devm_regmap_init_i2c()`, `devm_regmap_init_spi()`: 初始化。
  * `regmap_read()`, `regmap_write()`: 极其简单的读写，完全不用再手搓 spi_transfer。

### 5. 其他常见子系统
* **LED 子系统** (`<linux/leds.h>`): 控制指示灯（支持心跳、定时闪烁等触发器），生成 `/sys/class/leds/`。
* **RTC 子系统** (`<linux/rtc.h>`): 实时时钟，生成 `/dev/rtc0`，支持 `hwclock` 命令。
* **V4L2 子系统** (`<linux/videodev2.h>`): 视频采集设备（摄像头）。

---

## 四、 开发文档与 API 释义去哪里找？

Linux 内核 API 极其庞大且随版本变动，因此查找资料是有固定套路的：

### 1. 官方源码文档 (The Source of Truth)
绝大多数内核 API 的参数解释，直接写在**内核源码的函数定义处（注释中）**。
* **推荐工具**：[Bootlin Elixir Cross Referencer](https://elixir.bootlin.com/linux/latest/source)
* **使用方法**：如果你想知道 `spi_sync` 怎么用，在网站右上角搜索 `spi_sync`，跳转到其定义的 C 文件，它的上方会有标准的 `kernel-doc` 格式注释，详细说明了每个参数的作用、返回值和上下文限制。

### 2. 官方在线文档 (Sphinx 文档)
内核源码目录下的 `Documentation/` 文件夹已经被整理成漂亮的在线网页：
* **网址**：[https://www.kernel.org/doc/html/latest/](https://www.kernel.org/doc/html/latest/)
* **特点**：包含各大子系统（IIO, Input, DRM, SPI等）的架构设计思想、如何编写驱动的 Step-by-Step 教程（如：*How to write an I2C client driver*）。

### 3. 内核源码中的样例代码
内核源码里有一个庞大的宝库，叫 `drivers/` 目录。
* **技巧**：你想写一个 SPI 屏幕驱动，与其查 API，不如直接看别人怎么写的。
* `drivers/staging/fbtft/`：里面包含了上百种 SPI 屏幕的驱动源码（包括 ili9341, st7789 等）。
* `drivers/iio/adc/`：各种 I2C/SPI ADC 的标准写法。

### 4. 经典书籍
* **《Linux Device Drivers 3rd Edition (LDD3)》**：虽然基于 2.6 内核，有点老，但对字符设备、并发、中断的底层原理讲解无可替代。
* **《Linux Driver Development for Embedded Processors》** (Alberto Alvarez)：非常现代，详细讲解了设备树、Regmap、IIO 等新特性。
