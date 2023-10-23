# -*- coding: utf-8 -*-

import sys
from PyQt5.QtWidgets import QApplication,QWidget,QFileDialog
from PyQt5.QtCore import QThread, pyqtSignal, QTimer,QDateTime
from PyQt5.QtGui import QImage, QPixmap
from Ui_untitled import Ui_Form  
from PyQt5.QtMultimedia import QCamera, QCameraInfo, QCameraImageCapture
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QImage, QPixmap
from PyQt5.QtGui import QDesktopServices
from PyQt5.QtCore import QUrl, QStandardPaths
from PyQt5 import QtGui

import numpy as np
import cv2
import time
import cvlib
import qtlib

class MyWidget(QWidget, Ui_Form): 
    def __init__(self, parent =None):
        super(MyWidget, self).__init__(parent)
        self.setupUi(self)

        self.camera = cv2.VideoCapture(0)

        self.timer = QTimer(self)
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(50)


        self.imageList = []

    def update_frame(self):
        ret, frame = self.camera.read()
        if ret:
            cvlib.hor_dynamic_show(self.imageList, 7, frame, self.frame)
 
    
    

if __name__ == "__main__":
    app = QApplication(sys.argv)
    import pyrealsense2 as rs
    myWin = MyWidget()
    myWin.show()
    sys.exit(app.exec_())    