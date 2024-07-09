from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import *
from main_ui import Ui_Form
from PyQt5.QtCore import pyqtSignal,Qt,QTimer,QThread, pyqtSignal
import sys
import requests

class TextOutputThread(QThread):
    newText = pyqtSignal(str)
    
    def __init__(self, parent=None):
        super(TextOutputThread, self).__init__(parent)
    


class MainWindow(QWidget,Ui_Form):
    def __init__(self,parent=None):
        super(MainWindow,self).__init__(parent)
        self.setupUi(self)
        self.initUi()
    def initUi(self):
        self.track_state=False
        self.connect_state=False
        self.track_button.clicked.connect(self.track_button_operation)
        self.connect_button.clicked.connect(self.connect_button_operation)
        self.thread = TextOutputThread()
        self.thread.newText.connect(self.updateText)
        self.thread.start()
        # 创建一个定时器，每隔一定时间触发一次timeout信号
        self.timer = QTimer(self)
        self.timer2=QTimer(self)
        self.timer2.timeout.connect(self.refresh)
        self.timer.timeout.connect(self.updateData)
        # 设置定时器触发间隔为1000毫秒（1秒）
        self.timer.start(1000)
    def get_data(self):
        url="http://192.168.4.1:80"
        try:
            response=requests.get(url=url,timeout=0.5)
            if response.status_code==200:
                self.connect_state=True
                if self.track_state:
                    self.thread.newText.emit(response.text)
            else:
                self.connect_state=False
        except requests.Timeout:
                self.connect_state=False
    def refresh(self):
        self.get_data()
    def updateData(self):
        if self.connect_state:
            if not self.timer2.isActive():
                self.timer2.start(200)
            self.lineEdit.setText("已连接二维码扫描器")
        else:
            if self.timer2.isActive():
                self.timer2.stop()
            self.lineEdit.setText("未连接二维码扫描器")
        if self.track_state:
            self.lineEdit_2.setText("正在收集返回数据")
        else:
            self.lineEdit_2.setText("未在收集返回数据")
    def track_button_operation(self):
        if self.track_state:
            self.track_state=False
            self.track_button.setText('开始收集')
        else:
            self.track_state=True
            self.track_button.setText('停止收集')
    def connect_button_operation(self):
        self.get_data()
        if self.connect_state:
            self.thread.newText.emit("已成功连接!")
        else:
            self.thread.newText.emit("连接失败!")
    def updateText(self, newText):
        self.textBrowser.append(newText)
if __name__=="__main__":
    app=QApplication(sys.argv)
    widget=MainWindow()
    widget.show()
    # subscribe=Subscriber()
    
    sys.exit(app.exec_())
    