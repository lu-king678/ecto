# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file '/ssd/wg/ecto/python/ecto/ui/ecto.ui'
#
# Created: Fri Mar 18 16:08:37 2011
#      by: pyside-uic 0.2.7 running on PySide 1.0.0
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(670, 440)
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.gridLayout = QtGui.QGridLayout(self.centralwidget)
        self.gridLayout.setMargin(0)
        self.gridLayout.setSpacing(0)
        self.gridLayout.setObjectName("gridLayout")
        self.plasmView = PlasmWidget(self.centralwidget)
        self.plasmView.setObjectName("plasmView")
        self.gridLayout.addWidget(self.plasmView, 0, 0, 1, 1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.dockWidget = QtGui.QDockWidget(MainWindow)
        self.dockWidget.setObjectName("dockWidget")
        self.dockWidgetContents = QtGui.QWidget()
        self.dockWidgetContents.setObjectName("dockWidgetContents")
        self.verticalLayout = QtGui.QVBoxLayout(self.dockWidgetContents)
        self.verticalLayout.setSpacing(0)
        self.verticalLayout.setMargin(0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.modulesView = PlasmWidget(self.dockWidgetContents)
        self.modulesView.setObjectName("modulesView")
        self.verticalLayout.addWidget(self.modulesView)
        self.dockWidget.setWidget(self.dockWidgetContents)
        MainWindow.addDockWidget(QtCore.Qt.DockWidgetArea(1), self.dockWidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "Ecto Plasm Editor", None, QtGui.QApplication.UnicodeUTF8))
        self.dockWidget.setWindowTitle(QtGui.QApplication.translate("MainWindow", "Modules", None, QtGui.QApplication.UnicodeUTF8))

from plasmwidget import PlasmWidget
