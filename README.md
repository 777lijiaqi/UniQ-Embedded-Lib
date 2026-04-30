# 🚀 UniQ (Universal Qi) Embedded Library

<p align="center">
  <b>让单片机开发不再“随波逐流”，打造真正的跨平台驱动之魂！</b>
</p>

<p align="center">
  <!-- 许可协议图标：点击跳转到 LICENSE 文件 -->
  <a href="https://github.com/777lijiaqi/UniQ-Embedded-Lib/blob/main/LICENSE">
    <img src="https://img.shields.io/github/license/777lijiaqi/UniQ-Embedded-Lib?style=flat-square" alt="license">
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

## 😫 你是否也曾有过这种“痛”？

当你辛辛苦苦写好了 STM32 的驱动，老板突然说：“xx，芯片涨价了，明天咱们换成 GD32（或者国产某不知名单片机）！”

你看着满屏的 `HAL_GPIO_WritePin`、`__HAL_TIM_SET_COUNTER`，心中万马奔腾：
*   **深度绑定：** 逻辑代码和厂家库混在一起，分不清谁是谁。
*   **移植噩梦：** 换个芯片就得重写一半代码。
*   **复用性差：** 去年写的 PID 算法，今年想用还得再抠一遍。

**这就是为什么要诞生 UniQ (Universal Qi)！**

---

## 💎 什么是 UniQ？

**UniQ** 是由 **李家琦** 发起并维护的一套深度解耦的嵌入式功能库。它的核心思想是：**“逻辑向上，硬件向下”**。

通过高阶的 **依赖注入 (Dependency Injection)** 和 **面向接口编程**，UniQ 将业务逻辑与底层硬件彻底撕裂！无论你底层用的是 HAL 库、LL 库，还是寄存器操作，UniQ 都能通过极简的适配器，让同一套驱动在不同芯片上完美跑起来。

### ✨ 核心特性：
*   **彻底解耦：** 源码中绝不出现任何 `stm32xxx.h` 或厂商特定类型。
*   **全能外设：** 内置多种常用传感器驱动、通用延时库、平滑滤波算法等。
*   **极致轻量：** C 语言编写，不依赖 OS，主打一个快、准、稳。
*   **移植简单：** 填好几个函数指针，你的新芯片就“满血复活”了。

---

## 🏗️ 库架构预览

```text
  [ 你的应用层代码 ]
         |
  [ UniQ 核心库 (Logic) ]  <-- 跨平台、不改一行代码
         |
  [ 适配层 (Adapter) ]     <-- 只需要在这里写一点点“翻译”代码
         |
  [ 芯片 HAL 库 / 寄存器 ]  <-- STM32, GD32, ESP32, 51...
```

---

## 💻 快速开始 (Example)

这里展示如何利用 UniQ 的通用接口实现一个跨平台的延时。

```c

```
---

## ⚖️ 许可证 (License)

本项目采用 **GPL v3** 协议授权。

*   **个人/教育/开源项目：** 随便用，完全免费！请遵守 GPL 协议开源你的项目。
*   **闭源商业用途：** 
    *   如果您不想开源您的代码，或者想把 UniQ 用在您的发财项目里，作者提供“商业豁免授权”。
    *   **建议：** 请作者喝杯咖啡（赞助 5 美元 / 35 RMB），即可解锁商用成就！

详细协议内容请参考：[LICENSE](./LICENSE)

---

<a name="sponsorship"></a>
## ☕ 赞助与支持 (Sponsorship)

如果您觉得 UniQ 让您感到便捷、兴奋，欢迎通过以下方式支持家琦的持续开发！您的名字将有机会出现在贡献者名单中。

| 支付宝 (Alipay) | 微信支付 (WeChat) |
| :---: | :---: |
| <img src="./assets/alipay_qr.png" width="180" /> | <img src="./assets/wechat_qr.png" width="180" /> |

> **赞助备注：** UniQ 商用授权 + 你的联系方式/邮箱。

---

<p align="center">
  Made with ❤️ by <b>李家琦 (Li Jiaqi)</b>. <br>
  星光不问赶路人，时光不负有心人。
</p>

---
