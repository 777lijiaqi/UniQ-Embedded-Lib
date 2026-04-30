<p align="center">
  <br>
  <b><font size="7">UniQ 嵌入式通用库</font></b><br>
  <b><font size="6">技术参考手册</font></b><br>
  <br>
  <font size="4"><i>—— 跨平台驱动解耦与高效嵌入式开发框架 ——</i></font><br>
  <br>
  <img src="https://img.shields.io/badge/UniQ-Framework-blue?style=for-the-badge" />
  <br><br>
  <b>版本：</b> v1.0.0 <br>
  <b>作者：</b> 李家琦 (Li Jiaqi) <br>
  <b>日期：</b> 2024年5月 <br>
  <br>
  <br>
  <b>[ 核心理念 ]</b><br>
  一次编写，多芯片运行。将业务逻辑从厂商 HAL 库中彻底解放。<br>
  <br>
</p>

---
<div style="page-break-after: always;"></div>

#### **1. 介绍 (Introduction)**
*   **1.1 项目背景**：解决嵌入式开发中代码与硬件强耦合的痛点。
*   **1.2 核心价值**：通用性、可移植性、工业级代码规范。
*   **1.3 主要功能**：
    *   跨平台延时接口
    *   通用外设抽象层 (GPIO, UART, I2C等)
    *   常用 IC 芯片驱动库
    *   高性能嵌入式算法 (PID, 滤波等)

#### **2. 库结构 (Library Structure)**
*   **2.1 目录组织**：模块化文件夹结构详解。
*   **2.2 架构设计**：
    *   **应用层 (App Layer)**：用户业务逻辑。
    *   **核心逻辑层 (Core Logic)**：纯 C 语言编写，不包含任何硬件头文件。
    *   **硬件接口层 (Hardware Interface)**：通过函数指针定义的抽象接口。
    *   **适配层 (Porting Layer)**：连接厂商 HAL 库与 UniQ 的桥梁。
*   **2.3 依赖说明**：标准 C99 环境，零外部库依赖。

#### **3. 如何使用 (How to Use)**
*   **3.1 环境准备**：如何将 UniQ 导入您的 Keil/STM32CubeIDE 工程。
*   **3.2 移植三部曲**：
    1. 定义底层驱动函数 (基于您的 HAL 库)。
    2. 填充接口结构体 (Interface Structure)。
    3. 调用初始化函数进行“依赖注入”。
*   **3.3 示例代码 (Examples)**：
    *   *示例 1*：实现跨平台 LED 闪烁。
    *   *示例 2*：移植一个 I2C 传感器驱动。
*   **3.4 常见问题 (FAQ)**：解决初次移植时的报错。

---
