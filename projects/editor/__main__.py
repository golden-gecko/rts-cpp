import sys

from PySide2.QtWidgets import QApplication

from config import Config
from windows.main import MainWindow


if __name__ == '__main__':
    app = QApplication(sys.argv)

    window = MainWindow(Config.title)
    window.resize(Config.width, Config.height)
    window.show()

    sys.exit(app.exec_())
