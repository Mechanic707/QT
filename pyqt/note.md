### 原始界面显示

```python
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

class MyWidget(QWidget, Ui_Form): 
    def __init__(self, parent =None):
        super(MyWidget, self).__init__(parent)
        self.setupUi(self)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWin = MyWidget()
    myWin.show()
    sys.exit(app.exec_())    
```

