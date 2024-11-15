import cv2
import numpy as np
import serial


# 红色的HSV范围
lower_red1 = np.array([0, 43, 50])
upper_red1 = np.array([10, 255, 255])
lower_red2 = np.array([160, 43, 50])
upper_red2 = np.array([180, 255, 255])

# 蓝色的HSV范围
lower_blue = np.array([100, 150, 50])
upper_blue = np.array([140, 255, 255])


def decimal_to_hex(decimal_number):
    # 将十进制数转换为十六进制字符串，并去掉 '0x' 前缀
    hex_str = hex(decimal_number)[2:]

    # 如果长度为奇数，在前面加一个零
    if len(hex_str) % 2 != 0:
        hex_str = '0' + hex_str

    return hex_str


def xunji_rcg(frame_xunji, hsv_xunji, area_threshold_xunjiuse):
    """
    主要识别函数，用于识别引导线和门框，通过 biaoshi 变量区分
    :param frame_xunji: 传入摄像头获取的一帧图片
    :param hsv_xunji: 摄像头获取的图片转化为hsv空间
    :param area_threshold_xunjiuse: 面积阈值，面积小于阈值的引导玻璃不会被识别
    :return: center 近似引导线中心（最小外接圆圆心坐标）， area 引导线粗略大小（最小外接圆面积）
    """
    # 创建掩码
    mask1 = cv2.inRange(hsv_xunji, lower_red1, upper_red1)
    mask2 = cv2.inRange(hsv_xunji, lower_red2, upper_red2)
    mask = mask1 | mask2

    # 轮廓检测
    contours, hierarchy = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    if contours:
        # 找到最大轮廓
        max_contour = max(contours, key=cv2.contourArea)
        max_contour_area = cv2.contourArea(max_contour)

        # 获取最大轮廓的索引
        max_contour_index = None
        for i, contour in enumerate(contours):
            if np.array_equal(contour, max_contour):
                max_contour_index = i
                break

        # 检查是否找到最大轮廓的索引
        if max_contour_index is None:
            raise ValueError("未找到最大轮廓的索引。")

        # 初始化子轮廓面积
        max_child_contour_area = 0

        # 查找最大轮廓的子轮廓
        for i in range(len(contours)):
            # 如果当前轮廓的父轮廓是最大轮廓
            if hierarchy[0][i][3] == max_contour_index:
                # 计算该子轮廓的面积
                child_contour_area = cv2.contourArea(contours[i])
                # 更新最大子轮廓面积
                if child_contour_area > max_child_contour_area:
                    max_child_contour_area = child_contour_area

        # 判断最大子轮廓面积是否大于最大轮廓面积的80%
        if max_child_contour_area <= 0.8 * max_contour_area:
            biaoshi = 1  # 判定为识别到引导线
        else:
            biaoshi = 2  # 判定为识别到门框

        # 最小外接圆
        (x, y), radius = cv2.minEnclosingCircle(max_contour)
        center = (int(x), int(y))
        radius = int(radius)
        area = np.pi * (radius ** 2)

        if area > area_threshold_xunjiuse:
            # 画最小外接圆和圆心
            cv2.circle(frame_xunji, center, radius, (0, 255, 0), 2)
            cv2.circle(frame_xunji, center, 5, (255, 0, 0), -1)
            cv2.putText(frame_xunji, f"Area: {int(area)}", (center[0] - 50, center[1] - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)

            # 打印圆心坐标
            print(f"Center: {center}, Area: {area}")
            x_pre = decimal_to_hex(int(x))
            y_pre = decimal_to_hex(int(y))
            mid_item1 = 'CE'
            mid_item2 = 'BE'

            if biaoshi == 1:
                send(f"{mid_item1}{x_pre}{mid_item1}{y_pre}")
            elif biaoshi == 2:
                send(f"{mid_item2}{x_pre}{mid_item2}{y_pre}")

        else:
            mid_item2 = 'FE'
            send(f"{mid_item2}")

    # 仅测试需要
    # 显示结果
    cv2.imshow('Frame', frame_xunji)
    # 显示掩码图像
    cv2.imshow('Mask', mask)


def circle_rcg(frame_circle, hsv_circle, area_threshold_circleuse):
    """
    小球识别函数
    :param frame_circle: 传入摄像头获取的一帧图片
    :param hsv_circle: 摄像头获取的图片转化为hsv空间
    :param area_threshold_circleuse: 面积阈值，面积小于阈值的圆不会被识别
    :return: blue_center与red_center为圆心坐标
    """
    # 先腐蚀再膨胀
    kernel = np.ones((5, 5), np.uint8)
    hsv_eroded = cv2.erode(hsv_circle, kernel, iterations=1)
    hsv_dilated = cv2.dilate(hsv_eroded, kernel, iterations=1)

    blue_center = None
    red_center = None

    # 蓝色掩码
    mask_blue = cv2.inRange(hsv_dilated, lower_blue, upper_blue)

    # 霍夫变换检测蓝色圆
    circles_blue = cv2.HoughCircles(mask_blue, cv2.HOUGH_GRADIENT, dp=1, minDist=20,
                                    param1=50, param2=20, minRadius=0, maxRadius=0)
    if circles_blue is not None:
        circles_blue = np.uint16(np.around(circles_blue))
        for i in circles_blue[0, :]:
            center = (i[0], i[1])
            radius = i[2]
            area = np.pi * (radius ** 2)
            if area > area_threshold_circleuse:
                blue_center = center
                # 绘制圆的轮廓和圆心
                cv2.circle(frame_circle, center, radius, (255, 0, 0), 2)
                cv2.circle(frame_circle, center, 5, (255, 0, 0), -1)
                cv2.putText(frame_circle, f"Blue Area: {int(area)}", (center[0] - 50, center[1] - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
                break

    # 红色掩码
    mask_red1 = cv2.inRange(hsv_dilated, lower_red1, upper_red1)
    mask_red2 = cv2.inRange(hsv_dilated, lower_red2, upper_red2)
    mask_red = mask_red1 | mask_red2

    # 霍夫变换检测红色圆, param2越大检测越苛刻
    circles_red = cv2.HoughCircles(mask_red, cv2.HOUGH_GRADIENT, dp=1, minDist=20,
                                   param1=50, param2=20, minRadius=0, maxRadius=0)
    if circles_red is not None:
        circles_red = np.uint16(np.around(circles_red))
        for i in circles_red[0, :]:
            center = (i[0], i[1])
            radius = i[2]
            area = np.pi * (radius ** 2)
            if area > area_threshold_circleuse:
                red_center = center
                # 绘制圆的轮廓和圆心
                cv2.circle(frame_circle, center, radius, (0, 0, 255), 2)
                cv2.circle(frame_circle, center, 5, (0, 0, 255), -1)
                cv2.putText(frame_circle, f"Red Area: {int(area)}", (center[0] - 50, center[1] - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, (255, 255, 255), 2)
                break

    # 显示结果
    cv2.imshow('Frame', frame_circle)
    # 显示掩码图像
    cv2.imshow('Mask_Red', mask_red)
    # 显示掩码图像
    cv2.imshow('Mask_Blue', mask_blue)

    return blue_center, red_center


# 以十六进制的格式发送数据
def send(send_data):
    send_data_hex = bytes.fromhex(send_data)
    if ser.isOpen():
        ser.write(send_data_hex)  # 编码
        print("发送成功", send_data_hex)
    else:
        print("发送失败！")


# 开串口待测试
ser = serial.Serial("/dev/ttyAMA0", 115200)

# 摄像头捕获
cap = cv2.VideoCapture(0)
area_threshold_xunji = 1000  # 可根据实际需求调整
area_threshold_circle = 1000

while True:
    # 读取帧
    ret, frame = cap.read()
    frame = cv2.resize(frame, (320, 240), interpolation=cv2.INTER_AREA)
    if not ret:
        break

    # 高斯滤波
    blurred = cv2.GaussianBlur(frame, (15, 15), 0)

    # 转换为HSV颜色空间
    hsv = cv2.cvtColor(blurred, cv2.COLOR_BGR2HSV)

    # 使用封装的函数进行掩码创建和轮廓检测
    xunji_rcg(frame, hsv, area_threshold_xunji)

    # # 进行小球识别
    # circle_rcg(frame, hsv, area_threshold_circle)

    # 按 'q' 键退出
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# 释放摄像头并关闭窗口
cap.release()
cv2.destroyAllWindows()

# v1.1 将循迹识别封装为函数，更改了红色阈值
# v1.2 增加了小球识别函数
# v1.3 定义了串口发送16进制函数，尝试将引导线中心坐标传输给stm32
# v1.4 修改了循迹函数，使得循迹函数可以同时承担识别引导线和门框的任务，同时使用BE,CE,FE作为分隔符来区分识别到的目标种类
