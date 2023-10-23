# -*- coding: utf-8 -*-
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtCore import Qt

import numpy as np
import cv2

def QImage_to_Mat(image):
    image = image.convertToFormat(QImage.Format_RGB888)
    width = image.width()
    height = image.height()
    buffer = image.bits().asstring(width * height * 3)
    cv_image = np.frombuffer(buffer, dtype=np.uint8).reshape((height, width, 3))
    cv_image = cv2.cvtColor(cv_image, cv2.COLOR_RGB2BGR)
    return cv_image
    
def Mat_to_QImage(show):
    show = cv2.cvtColor(show, cv2.COLOR_BGR2RGB)
    showImage = QImage(show.data, show.shape[1],show.shape[0],QImage.Format_RGB888)
    return showImage

def label_show_Mat(matframe, qlabel):
    show_image = Mat_to_QImage(matframe)
    pixmap = QPixmap.fromImage(show_image)
    pixmap = pixmap.scaled(qlabel.size(), Qt.KeepAspectRatio)
    qlabel.setPixmap(pixmap)   

#动态刷新左移图片
def dynamic_frame_append(imageList, num, matframe):
    if len(imageList) < num:
        imageList.append(matframe)
    else:
        imageList[:-1] = imageList[1:]
        imageList[-1] = matframe

#水平图片拼接
def hor_concatenation(imageList, qlabel):
    con_image = cv2.hconcat(imageList)
    label_show_Mat(con_image, qlabel)

def hor_dynamic_show(imageList, num, matframe, qlabel):
     dynamic_frame_append(imageList, num, matframe)
     hor_concatenation(imageList, qlabel)