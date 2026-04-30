<p align="center">
  <br>
  <b><font size="7">UniQ Embedded Library</font></b><br>
  <b><font size="6">Technical Reference Manual</font></b><br>
  <br>
  <font size="4"><i>— Cross-platform Decoupling & High-efficiency Embedded Framework —</i></font><br>
  <br>
  <img src="https://img.shields.io/badge/UniQ-Framework-blue?style=for-the-badge" />
  <br><br>
  <b>Version:</b> v1.0.0 <br>
  <b>Author:</b> Li Jiaqi <br>
  <b>Date:</b> May, 2024 <br>
  <br>
  <br>
  <b>[ Philosophy ]</b><br>
  Write once, run on any MCU. Liberate logic from vendor-specific HAL. <br>
  <br>
</p>

---
#### **1. Introduction**
*   **1.1 Background**: Why UniQ? (Breaking the vendor lock-in).
*   **1.2 Core Values**: Reusability, Portability, and Professional Coding Standards.
*   **1.3 Key Features**: 
    *   Platform-independent Delay System.
    *   Universal Peripheral Abstraction (GPIO, UART, I2C, etc.).
    *   Device Driver Library for various ICs.
    *   High-performance Embedded Algorithms (PID, Filter, etc.).

#### **2. Library Structure**
*   **2.1 Directory Overview**: Detailed explanation of the folder tree.
*   **2.2 Architectural Design**:
    *   **Application Layer**: User-defined business logic.
    *   **Core Logic Layer**: Pure C implementation without vendor headers.
    *   **Hardware Abstraction Interface**: Pointer-based abstract interfaces.
    *   **Porting Layer (Adapter)**: Connecting UniQ to vendor-specific HAL.
*   **2.3 Dependency**: Standard C99 environment, zero external dependencies.

#### **3. How to Use**
*   **3.1 Setup**: How to import UniQ into Keil/STM32CubeIDE projects.
*   **3.2 The 3-Step Porting Guide**:
    1. Define low-level drivers (based on your HAL).
    2. Fill the Interface Structure.
    3. Call the initialization function (Dependency Injection).
*   **3.3 Examples**:
    *   *Example 1*: Cross-platform LED blinking.
    *   *Example 2*: Porting an I2C sensor driver.
*   **3.4 FAQ**: Troubleshooting common porting issues.

---
