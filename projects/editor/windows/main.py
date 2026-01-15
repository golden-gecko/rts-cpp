import os

from PyQt6.QtGui import QAction, QFileSystemModel
from PyQt6.QtWidgets import QApplication, QHBoxLayout, QVBoxLayout, QMainWindow, QTreeView, QWidget


class Widget(QWidget):
    def __init__(self):
        QWidget.__init__(self)

        model = QFileSystemModel()
        model.setRootPath(os.path.join('..', '..', 'bin'))

        tree = QTreeView()
        tree.setModel(model)
        tree.setRootIndex(model.index(os.path.join('..', '..', 'bin')))
        tree.hideColumn(1)
        tree.hideColumn(2)
        tree.hideColumn(3)

        left = QVBoxLayout()
        # left.setMargin(10)
        left.addWidget(tree)

        right = QVBoxLayout()
        # right.setMargin(10)
        # right.addWidget(add)

        layout = QHBoxLayout()
        layout.addLayout(left, 1)
        layout.addLayout(right, 2)

        self.setLayout(layout)


class MainWindow(QMainWindow):
    def __init__(self, title):
        QMainWindow.__init__(self)

        self.setWindowTitle(title)
        self.create_menu()
        self.move(0, 0)
        self.setCentralWidget(Widget())

    def create_menu(self):
        menu = self.menuBar()

        file_menu = menu.addMenu('File')
    
        open_action = QAction('Open', self)
        open_action.setShortcut('Ctrl+O')

        exit_action = QAction('Exit', self)
        exit_action.setShortcut('Alt+F4')
        exit_action.triggered.connect(self.menu_exit)

        file_menu.addAction(open_action)
        file_menu.addSeparator()
        file_menu.addAction(exit_action)

    def menu_exit(self, checked):
        QApplication.quit()
