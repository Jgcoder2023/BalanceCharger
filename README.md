## 物联网平衡充电器&数控电源

#### 功能说明
1. 1-4节锂电池的物联网充电器
2. 可远程查看充电电流 电压
3. 可远程启动和停止充电
4. 可远程提醒充电结束
5. 本地也可调节1-4节的电池充电
6. 没有电池充电的时候是一个数控电压源

#### 实现方式
1. 先使用CH224K进行usb诱导出9-20V的电压（需要根据使用的充电器来决定）
2. 然后在使用DAC控制CN3765的FB引脚来调节电压进行1-4节电池的充电
3. 以上控制均是airf103cbt6来进行控制，同时采集输出电压电流进行反馈调整
5. 将充电的各项参数通过ESP8266进行WIFI传上服务器，然后反馈在App上进行数据展示，远程操作等功能。
6. 为了在本地也方便操作，也放置了按键和显示屏。
![整体框图.png](//image.lceda.cn/pullimage/Lhc2REHgwBn6cU9AuP0ynefK7IzBoKl9frwCYwYz.png)
![模块图.png](//image.lceda.cn/pullimage/PRQggLGNu2GYHrXxqXVnqJXF0uA56sxEvH5aobX6.png)

#### 实物图片
![物联网平衡充电器&数控电源](//image.lceda.cn/pullimage/Md1qNA0ObODXrdJOGCkB4bA5DwOfM2iFI4HWXhlf.jpeg)

#### 演示视频
[演示视频链接（B站） https://www.bilibili.com/video/BV1Ph411L72o/?share_source=copy_web&vd_source=25322cfc0891122acccc6a068b3e26e](https://www.bilibili.com/video/BV1Ph411L72o/?share_source=copy_web&vd_source=25322cfc0891122acccc6a068b3e26e0)

#### 开源链接
[代码开源地址\(https://github.com/Jgcoder2023/BalanceCharger\)](https://github.com/Jgcoder2023/BalanceCharger)