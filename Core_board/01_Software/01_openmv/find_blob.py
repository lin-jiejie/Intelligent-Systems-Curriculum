import sensor,image,time
from pyb import UART

uart = UART(3,115200)                          #定义串口3变量P4，P5
uart.init(115200, bits=8, parity=None, stop=1) # init with given parameters
# 8位数据位 无奇偶校验 1位停止位

sensor.reset()                         # 复位和初始化传感器。6
sensor.set_pixformat(sensor.RGB565) # 设置像素格式为RGB565 or GRAYSCALE    (RGB565为彩图  GRAYSCALE为灰度图)
sensor.set_framesize(sensor.QVGA)
#QQQVGA：分辨率为80x60     QQVGA：分辨率为160x120    CIF：分辨率为352x288      VGA：分辨率为640x480
#SXGA：分辨率为1280x1024  UXGA：分辨率为1600x1200  XGA：分辨率为1024x768像素  SVGA：分辨率为800x600
sensor.skip_frames(time = 2000) # 等待设置生效。
sensor.set_auto_gain(False)     #自动白平衡，默认开启，开启参数为 True ，关闭为 False 若想追踪颜色，则需关闭白平衡。
sensor.set_auto_exposure(False) #自动曝光，默认开启，开启参数为 True ，关闭为 False

clock = time.clock()            # 创建一个clock对象来跟踪FPS。

threshold = [(36, 55, 32, 88, -15, 52)]#阈值列表，以元组为单位
cnt_find_fail = 0

#发送      请根据实际情况更改发送的数据
def send_data(data1,data2):
    uart.write(bytes([0xaa]))
    uart.write(bytes([data1]))  # 将整数转换为字节类型并发送
    uart.write(bytes([data2]))  # 将整数转换为字节类型并发送
    uart.write(bytes([ ( (data1 + data2) & 0xFF) ]) )
    uart.write(bytes([0xdd]))
def rece_data():
    if uart.any():
        a = int(uart.read(1))
        print("已接收到数据{0}".format(a))#打印接收到的数字

def find_max(blobs):#寻找最大色块辅助函数
    max_size = 0
    max_blob = 0
    for blob in blobs:
        if blob[2]*blob[3] > max_size:
            max_blob=blob
            max_size = blob[2]*blob[3]
    return max_blob

def find_maxblobs(img,threshold):#寻找最大色块函数
    global cnt_find_fail
#    print(threshold)
    blobs = img.find_blobs(threshold,roi = (40,0,240,240))
    if blobs:
    #如果找到了目标颜色
        cnt_find_fail = 0
        max_blob = find_max(blobs)
        img.draw_rectangle(max_blob.rect(), color = (255,255,255))
#        img.draw_cross(max_blob.cx(),max_blob.cy(), color = (255,255,255))      
        circles = img.find_circles(roi = max_blob.rect(), threshold = 10000)
#        if circles :    #如果能找到圆就发送圆心坐标，找不到就发送最大色块的坐标
#            max_circle = max(circles,key=lambda x: x[3])
#            img.draw_circle(max_circle.x(),max_circle.y(),max_circle.r(),color=(255,0,0))
##            print(max_circle)
#            img.draw_cross(max_circle.x(),max_circle.y(),color=(255,0,0),size=5, thickness=3)
##            send_data(max_circle.cx() - 40,max_circle.cy())
#            print("send circle")
#            return
        send_data(max_blob.cx() - 40,max_blob.cy())
        print("send blob")
        print(max_blob)
    else:
        cnt_find_fail = cnt_find_fail+1
        if cnt_find_fail >= 10:
            print("find_out")
            send_data(255,255)
while(True):
    clock.tick()
    img = sensor.snapshot()#.binary([(86, 254)])二值化操作
    find_maxblobs(img,threshold)#匹配最大色块
#    send_data(255,255)
    print("FPS %f" % clock.fps())
