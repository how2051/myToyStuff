from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
    QPushButton, QTableWidget, QTableWidgetItem, QMessageBox,
    QDialog, QLabel, QLineEdit, QDateEdit, QDoubleSpinBox, QFormLayout, QSpinBox
)
from PyQt5.QtCore import Qt, QDate
import sys
from decimal import Decimal, getcontext

# 提高小数计算精度
getcontext().prec = 12


# 交易日期、基金名称、基金代码、交易金额、单位净值、交易份额、累计份额、累计投入、当前净值、当前市值、盈亏金额、收益率。
# 表格列定义（顺序固定）
COLUMNS = [
    "id",
    "交易日期", "基金名称", "基金代码",
    "交易金额", "单位净值", "交易份额",
    "累计份额", "累计投入",
    "当前净值", "当前市值", "盈亏金额", "收益率",
    "备注"
]

class AddRecordDialog(QDialog):
    """
    新增记录弹窗
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setWindowTitle("新增定投记录")
        self.setModal(True)
        self.resize(360, 240)

        form = QFormLayout()

        self.date_edit = QDateEdit(QDate.currentDate())
        self.date_edit.setCalendarPopup(True)
        form.addRow("交易日期:", self.date_edit)

        self.fund_name_edit = QLineEdit()
        form.addRow("基金名称:", self.fund_name_edit)

        self.fund_code_edit = QLineEdit()
        form.addRow("基金代码:", self.fund_code_edit)

        self.amount_spin = QDoubleSpinBox()
        self.amount_spin.setRange(0.01, 1e9)
        self.amount_spin.setDecimals(2)
        self.amount_spin.setValue(1000.00)
        form.addRow("交易金额 (元):", self.amount_spin)

        self.nav_spin = QDoubleSpinBox()
        self.nav_spin.setRange(0.000001, 1e6)
        self.nav_spin.setDecimals(6)
        self.nav_spin.setValue(1.000000)
        form.addRow("单位净值 (NAV):", self.nav_spin)

        # 确认/取消按钮
        btn_layout = QHBoxLayout()
        self.ok_btn = QPushButton("确定")
        self.cancel_btn = QPushButton("取消")
        btn_layout.addWidget(self.ok_btn)
        btn_layout.addWidget(self.cancel_btn)

        layout = QVBoxLayout()
        layout.addLayout(form)
        layout.addLayout(btn_layout)
        self.setLayout(layout)

        self.ok_btn.clicked.connect(self.accept)
        self.cancel_btn.clicked.connect(self.reject)

    def get_data(self):
        """
        返回字典形式的输入数据（基本字段）
        """
        return {
            "date": self.date_edit.date().toString("yyyy-MM-dd"),
            "fund_name": self.fund_name_edit.text().strip(),
            "fund_code": self.fund_code_edit.text().strip(),
            "trade_amount": float(self.amount_spin.value()),
            "nav": float(self.nav_spin.value())
        }

class FundTrackerMain(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("基金定投跟踪")
        self.resize(1100, 600)

        # 数据存储（内存）: list of records (dict)
        # id 自增
        self._next_id = 1
        self.records = []

        # 主界面布局
        central = QWidget()
        main_layout = QVBoxLayout()
        btn_layout = QHBoxLayout()

        self.add_btn = QPushButton("新增记录")
        self.delete_btn = QPushButton("删除选中")
        self.refresh_btn = QPushButton("刷新表格")

        btn_layout.addWidget(self.add_btn)
        btn_layout.addWidget(self.delete_btn)
        btn_layout.addWidget(self.refresh_btn)
        btn_layout.addStretch()

        # 表格
        self.table = QTableWidget(0, len(COLUMNS))
        self.table.setHorizontalHeaderLabels(COLUMNS)
        self.table.verticalHeader().setVisible(False)  # 隐藏行号
        self.table.setColumnHidden(0, True)  # 隐藏第一列，即不显示 id

        self.table.setEditTriggers(QTableWidget.NoEditTriggers)
        self.table.setSelectionBehavior(QTableWidget.SelectRows)
        self.table.setSelectionMode(QTableWidget.SingleSelection)

        main_layout.addLayout(btn_layout)
        main_layout.addWidget(self.table)
        central.setLayout(main_layout)
        self.setCentralWidget(central)

        # 事件连接
        self.add_btn.clicked.connect(self.on_add)
        self.delete_btn.clicked.connect(self.on_delete)
        self.refresh_btn.clicked.connect(self.load_table)

        # 加入一些示例数据（可删除）
        self._load_sample_data()
        self.load_table()

    def _load_sample_data(self):
        # 添加两条示例记录（不同基金）
        sample = [
            {"date": "2025-06-09", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 5000.0, "nav": 1.5425},
            {"date": "2025-06-09", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": 500.0, "nav": 1.1172},
            {"date": "2025-06-09", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 500.0, "nav": 0.7417},

            {"date": "2025-07-09", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 5000.0, "nav": 1.5940},
            {"date": "2025-07-09", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": 500.0, "nav": 1.1325},
            {"date": "2025-07-09", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 500.0, "nav": 0.7320},

            {"date": "2025-08-11", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 20000.0, "nav": 1.6541},
            {"date": "2025-08-11", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": 2000.0, "nav": 1.2694},
            {"date": "2025-08-11", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 2000.0, "nav": 0.7710},

            {"date": "2025-09-09", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 10000.0, "nav": 1.7758},
            {"date": "2025-09-09", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": 1000.0, "nav": 1.5522},
            {"date": "2025-09-09", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 1000.0, "nav": 0.8334},
            {"date": "2025-09-09", "fund_name": "易方达纳什达克100ETF联接C", "fund_code": "012870",
             "trade_amount": 5000.0, "nav": 3.7557},

            {"date": "2025-09-30", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": 0.0, "nav": 1.7767},
            {"date": "2025-09-30", "fund_name": "易方达人工智能ETF联接A", "fund_code": "012733",
             "trade_amount": -4725.49, "nav": 1.7767},

            {"date": "2025-10-09", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 10000.0, "nav": 1.8812},
            {"date": "2025-10-09", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 1000.0, "nav": 0.7759},
            {"date": "2025-10-09", "fund_name": "南方纳斯达克100指数发起（QDII）A", "fund_code": "016452",
             "trade_amount": 10000.0, "nav": 2.0632},
            {"date": "2025-10-09", "fund_name": "华宝纳斯达克精选股票发起式（QDII）A", "fund_code": "017436",
             "trade_amount": 1000.0, "nav": 2.3426},

            {"date": "2025-10-14", "fund_name": "中欧丰利债券A", "fund_code": "014000",
             "trade_amount": 20000.0, "nav": 1.1410},

            {"date": "2025-11-10", "fund_name": "易方达沪深300ETF联接A", "fund_code": "110020",
             "trade_amount": 10000.0, "nav": 1.8798},
            {"date": "2025-11-10", "fund_name": "招商中证白酒指数（LOF）A", "fund_code": "161725",
             "trade_amount": 1000.0, "nav": 0.8051},
            {"date": "2025-11-10", "fund_name": "中欧丰利债券A", "fund_code": "014000",
             "trade_amount": 20000.0, "nav": 1.1479},
            {"date": "2025-11-10", "fund_name": "长盛全债指数增强A", "fund_code": "510080",
             "trade_amount": 30000.0, "nav": 1.7227},
            {"date": "2025-11-10", "fund_name": "富国中国中小盘香港上市QDII", "fund_code": "100061",
             "trade_amount": 10000.0, "nav": 3.4380},

        ]
        for item in sample:
            self._insert_record_internal(item)

    def _insert_record_internal(self, base):
        """
        将一条基本输入（包含 date,fund_name,fund_code,trade_amount,nav）转成完整记录并插入内存列表
        会计算：trade_share, total_share, total_invest
        current_nav/current_value/profit/return_rate 初始留空或按当前 nav 计算占位
        """
        # 计算本次交易份额 = 金额 / 单位净值
        amount = Decimal(str(base["trade_amount"]))
        nav = Decimal(str(base["nav"]))
        if nav == 0:
            trade_share = Decimal("0")
        else:
            trade_share = (amount / nav).quantize(Decimal("0.0000001"))

        # 找到该基金之前的累计份额与投入
        prev_share = Decimal("0")
        prev_invest = Decimal("0")
        for r in self.records:
            if r["fund_code"] == base["fund_code"]:
                prev_share += Decimal(str(r["trade_share"]))
                prev_invest += Decimal(str(r["trade_amount"]))

        total_share = (prev_share + trade_share).quantize(Decimal("0.0000001"))
        total_invest = (prev_invest + amount).quantize(Decimal("0.01"))

        current_nav = nav
        current_value = (total_share * current_nav).quantize(Decimal("0.000001"))
        profit = (current_value - total_invest).quantize(Decimal("0.01"))
        return_rate = (profit / total_invest)

        record = {
            "id": self._next_id,
            "date": base["date"],
            "fund_name": base["fund_name"],
            "fund_code": base["fund_code"],
            "trade_amount": float(amount),
            "nav": float(nav),
            "trade_share": float(trade_share),
            "total_share": float(total_share),
            "total_invest": float(total_invest),
            "current_nav": float(current_nav),
            "current_value": float(current_value),
            "profit": float(profit),
            "return_rate": float(return_rate)
        }
        self.records.append(record)
        self._next_id += 1

    def on_add(self):
        dlg = AddRecordDialog(self)
        if dlg.exec() == QDialog.Accepted:
            data = dlg.get_data()
            # 简单校验
            if not data["fund_name"] or not data["fund_code"]:
                QMessageBox.warning(self, "输入错误", "基金名称与基金代码不能为空。")
                return
            if data["trade_amount"] <= 0 or data["nav"] <= 0:
                QMessageBox.warning(self, "输入错误", "交易金额与单位净值必须大于 0。")
                return

            # 插入内存并刷新表格
            self._insert_record_internal(data)
            self.load_table()

    def on_delete(self):
        sel = self.table.currentRow()
        if sel < 0:
            QMessageBox.information(self, "提示", "请先选中一行要删除的记录。")
            return
        rid_item = self.table.item(sel, 0)
        if not rid_item:
            return
        rid = int(rid_item.text())
        # 删除记录
        self.records = [r for r in self.records if r["id"] != rid]
        # 为了简化，这里不重编号 id，直接刷新表格
        self.load_table()

    def load_table(self):
        """
        将内存中的 records 渲染到表格。
        对于当前净值为空的记录，当前市值/盈亏/收益率显示为空。
        """
        self.table.setRowCount(0)
        # 为了保证按时间或 id 排序，可以按 id 排序（这里按 id 升序）
        for r in sorted(self.records, key=lambda x: x["id"]):
            row = self.table.rowCount()
            self.table.insertRow(row)
            # 填充每一列
            for col, key in enumerate(COLUMNS):
                if key == "id":
                    v = str(r["id"])
                else:
                    # 将中文字段名映射回字典 key
                    mapping = {
                        "交易日期": "date",
                        "基金名称": "fund_name",
                        "基金代码": "fund_code",
                        "交易金额": "trade_amount",
                        "单位净值": "nav",
                        "交易份额": "trade_share",
                        "累计份额": "total_share",
                        "累计投入": "total_invest",
                        "当前净值": "current_nav",
                        "当前市值": "current_value",
                        "盈亏金额": "profit",
                        "收益率": "return_rate"
                    }
                    if key in mapping:
                        k = mapping[key]
                        v = r.get(k, "")
                        if v is None:
                            v = ""
                        else:
                            # 数值格式化
                            if isinstance(v, float):
                                if key in ("收益率",):
                                    v = f"{v:.4%}"
                                elif key in ("交易份额", "累计份额"):
                                    v = f"{v:.6f}"
                                elif key in ("单位净值",):
                                    v = f"{v:.6f}"
                                elif key in ("当前净值",):
                                    v = f"{v:.6f}"
                                else:
                                    v = f"¥{v:.2f}"
                    else:
                        v = ""
                item = QTableWidgetItem(str(v))
                item.setTextAlignment(Qt.AlignCenter)
                self.table.setItem(row, col, item)

def main():
    app = QApplication(sys.argv)
    win = FundTrackerMain()
    win.show()
    sys.exit(app.exec_())

if __name__ == "__main__":
    main()
