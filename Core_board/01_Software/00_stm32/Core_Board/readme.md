# 软件分层

## BSP层: 

### bsp_delay: 

使用TIM9实现**阻塞式** ms 级与 us 级延时，支持输入 16位宽 的延时时间

### bsp_key:

使用PB10作为输入端口, 使用rtos实现 轮询 检测, 0有效

### bsp_uart:

使用UART1作为串口, 使用 DMA+环形缓冲区 管理, 并解码协议

### bsp_oled:

使用PC14(SDA), PC15(SCL)作为 软件IIC1 模拟端口, 使用阻塞式延时实现时序控制, 使用rtos提供的临界区实现线程安全

### bsp_motor:

使用PA0(X_STEP), PA1(X_DIR)作为 X轴 步进电机控制端口(速度与方向控制)

使用PB6(Y_STEP), PB7(Y_DIR)作为 Y轴 步进电机控制端口(速度与方向控制)



## MID层

### mid_pid: 

为APP层提供PID对象及其对应方法

### RTT: 

使用jlink_RTT作为log输出

### mid_circular_buffer: 

为bsp_uart提供环形缓冲区的管理



## APP层

### app_control: 

系统集成, 实现整体项目的控制




# 串口自定义通信协议

### 帧头：0xaa

### 数据帧: x坐标, y坐标(共两个无符号8位)

### 校验码: 有效数据之和(无符号8位)

### 帧尾: 0xdd

