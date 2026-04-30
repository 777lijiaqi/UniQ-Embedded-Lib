# 🚀 UniQ (Universal Qi) Embedded Library

<p align="center">
  <b>让单片机开发不再“随波逐流”，打造真正的跨平台驱动之魂！</b>
</p>

<p align="center">
  <!-- 1. License 静态图标-->
  <a href="https://github.com/777lijiaqi/UniQ-Embedded-Lib/blob/main/LICENSE">
    <img src="https://img.shields.io/badge/license-GPLv3%20%2B%20Commercial-blue?style=flat-square" alt="license">
  </a>

  <!-- 版本号图标：点击跳转到 Releases 发布页面 -->
  <a href="https://github.com/777lijiaqi/UniQ-Embedded-Lib/releases">
    <img src="https://img.shields.io/github/v/release/777lijiaqi/UniQ-Embedded-Lib?style=flat-square" alt="release">
  </a>

  <!-- Star 图标：点击查看谁给你点了赞 -->
  <a href="https://github.com/777lijiaqi/UniQ-Embedded-Lib/stargazers">
    <img src="https://img.shields.io/github/stars/777lijiaqi/UniQ-Embedded-Lib?style=flat-square" alt="stars">
  </a>
</p>

---

## 😫 Are you suffering from "Vendor Lock-in"?

Imagine this: You've spent weeks writing drivers for STM32. Suddenly, your boss says: "xx, the chip price just doubled. Switch to GD32 (or some obscure MCU) by tomorrow!"

You look at your screen full of `HAL_GPIO_WritePin` and `__HAL_TIM_SET_COUNTER`, and you feel the pain:
*   **Deep Coupling:** Business logic is tangled with vendor HALs like spaghetti.
*   **Migration Nightmare:** Changing chips means rewriting half of your project.
*   **Zero Reusability:** That PID algorithm you wrote last year? Good luck digging it out.

**This is why UniQ (Universal Qi) was born.**

---

## 💎 What is UniQ?

**UniQ** is a deeply decoupled embedded functional library initiated and maintained by **Li Jiaqi**. 

Using advanced **Dependency Injection** and **Interface-Oriented Programming**, UniQ completely "tears apart" business logic from hardware. Whether you use HAL, LL, or direct register access, UniQ runs flawlessly across different chips through a minimal adapter layer.

### ✨ Core Features:
*   **Total Decoupling:** No `stm32xxx.h` or vendor-specific types in the source code.
*   **All-in-One:** Built-in sensor drivers, universal delay libraries, smooth filtering algorithms, and more.
*   **Ultra-Lightweight:** Written in pure C, no OS required, built for speed and stability.
*   **Porting in Seconds:** Just fill in a few function pointers, and your new chip is ready to go!

---

## 💻 Quick Example

Here is how you implement cross-platform delay using UniQ's universal interface:

```c

```

---

## ⚖️ License

This project is licensed under **GPL v3**.

*   **Personal / Educational / Open Source:** Fully FREE! Just keep your project open-source as per GPL terms.
*   **Closed-source / Commercial:** 
    *   If you don't want to open-source your code or want to use UniQ in a money-making product:
    *   **Commercial Waiver:** A simple "Coffee Sponsorship" ($5 USD) unlocks the commercial achievement!

Check the [LICENSE](./LICENSE) for details.

---

<a name="sponsorship"></a>
## ☕ Sponsorship & Support

If UniQ saved your day, consider supporting Jiaqi's hard work! 

**[Sponsor via PayPal ($5 USD)](https://www.paypal.me/JiaqiLi)**

*(Note: Please include "UniQ Commercial Use + Your Email" in the payment note for record-keeping.)*

---

<p align="center">
  Made with ❤️ by <b>Li Jiaqi (李家琦)</b>. <br>
  <i>"Hard work pays off, and time never betrays the dedicated."</i>
</p>
```

---
