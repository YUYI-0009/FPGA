import binascii
import serial.tools.list_ports
import time
from colorama import init, Fore, Style
import tkinter as tk
from tkinter import ttk, scrolledtext, messagebox
import threading

# 初始化颜色输出
init(autoreset=True)

class DeviceScoringApp:
    def __init__(self, root):
        self.root = root
        self.root.title("设备评分系统")
        self.root.geometry("900x700")
        
        # 先创建界面组件
        self.create_widgets()
        
        # 然后读取配置
        self.config = self.read_config('config.txt')
        self.TIMEOUT = self.config[0]      # 超时时间
        self.DEVICE_UPPER = self.config[1] # 设备编号上限
        self.POLLING_NUM = self.config[2]  # 轮询次数
        self.DEBUG = self.config[3]        # 调试模式
        
        # 串口对象
        self.ser = None
        
        # 状态变量
        self.online_devices = []
        self.total_score = 0
        self.valid_devices = 0
        self.device_logs = {}  # 存储每个设备的日志
        
    def create_widgets(self):
        """创建所有界面组件"""
        # 配置信息显示框
        self.config_frame = ttk.LabelFrame(self.root, text="系统配置", padding=10)
        self.config_frame.pack(fill=tk.X, padx=10, pady=5)
        
        # 临时显示配置文本
        self.config_label = ttk.Label(self.config_frame, text="正在加载配置...")
        self.config_label.pack(anchor=tk.W)
        
        # 串口选择区域
        self.port_frame = ttk.LabelFrame(self.root, text="串口设置", padding=10)
        self.port_frame.pack(fill=tk.X, padx=10, pady=5)
        
        self.port_combobox = ttk.Combobox(self.port_frame)
        self.port_combobox.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 10))
        
        self.refresh_ports()
        
        ttk.Button(self.port_frame, text="刷新串口", command=self.refresh_ports).pack(side=tk.LEFT)
        
        # 设备日志区域 - 使用Notebook实现分页
        self.log_notebook = ttk.Notebook(self.root)
        self.log_notebook.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # 主日志标签页
        self.main_log_frame = ttk.Frame(self.log_notebook)
        self.log_notebook.add(self.main_log_frame, text="主日志")
        
        self.main_log_text = scrolledtext.ScrolledText(self.main_log_frame, wrap=tk.WORD, height=10)
        self.main_log_text.pack(fill=tk.BOTH, expand=True)
        
        # 为每个设备创建日志标签页
        self.device_log_frames = {}
        self.device_log_texts = {}
        
        # 功能按钮区域
        self.btn_frame = ttk.Frame(self.root)
        self.btn_frame.pack(fill=tk.X, padx=10, pady=5)
        
        ttk.Button(self.btn_frame, text="检测设备", command=self.start_check_devices_thread).pack(side=tk.LEFT, padx=5)
        ttk.Button(self.btn_frame, text="读取分数", command=self.start_read_scores_thread, state=tk.DISABLED).pack(side=tk.LEFT, padx=5)
        self.read_btn = self.btn_frame.winfo_children()[1]  # 保存引用以便修改状态
        
        ttk.Button(self.btn_frame, text="复位设备", command=self.reset_devices).pack(side=tk.LEFT, padx=5)
        ttk.Button(self.btn_frame, text="清除日志", command=self.clear_logs).pack(side=tk.RIGHT, padx=5)
        
        # 结果显示区域
        self.result_frame = ttk.LabelFrame(self.root, text="评分结果", padding=10)
        self.result_frame.pack(fill=tk.BOTH, padx=10, pady=5)
        
        self.result_text = tk.Text(self.result_frame, height=5, wrap=tk.WORD)
        self.result_text.pack(fill=tk.BOTH, expand=True)
        
        # 在线设备显示区域
        self.device_frame = ttk.LabelFrame(self.root, text="在线设备", padding=10)
        self.device_frame.pack(fill=tk.BOTH, padx=10, pady=5)
        
        self.device_listbox = tk.Listbox(self.device_frame)
        self.device_listbox.pack(fill=tk.BOTH, expand=True)
        
    def refresh_ports(self):
        """刷新可用串口列表"""
        ports = serial.tools.list_ports.comports()
        self.port_combobox['values'] = [f"{p.device} - {p.description}" for p in ports]
        if ports:
            self.port_combobox.current(0)
            
    def log_message(self, message, color="black", device_id=None):
        """在日志区域添加消息"""
        if device_id is not None:
            # 设备特定日志
            if device_id not in self.device_log_frames:
                self.add_device_log_tab(device_id)
                
            text_widget = self.device_log_texts[device_id]
            text_widget.config(state=tk.NORMAL)
            text_widget.insert(tk.END, message + "\n", color)
            text_widget.tag_config(color, foreground=color)
            text_widget.config(state=tk.DISABLED)
            text_widget.see(tk.END)
        else:
            # 主日志
            self.main_log_text.config(state=tk.NORMAL)
            self.main_log_text.insert(tk.END, message + "\n", color)
            self.main_log_text.tag_config(color, foreground=color)
            self.main_log_text.config(state=tk.DISABLED)
            self.main_log_text.see(tk.END)
            
    def add_device_log_tab(self, device_id):
        """为设备添加新的日志标签页"""
        frame = ttk.Frame(self.log_notebook)
        text = scrolledtext.ScrolledText(frame, wrap=tk.WORD, height=10)
        text.pack(fill=tk.BOTH, expand=True)
        
        self.device_log_frames[device_id] = frame
        self.device_log_texts[device_id] = text
        
        # 添加到Notebook
        self.log_notebook.add(frame, text=f"设备 {device_id} 日志")
        
    def clear_logs(self):
        """清除所有日志"""
        # 清除主日志
        self.main_log_text.config(state=tk.NORMAL)
        self.main_log_text.delete(1.0, tk.END)
        self.main_log_text.config(state=tk.DISABLED)
        
        # 清除所有设备日志
        for device_id, text_widget in self.device_log_texts.items():
            text_widget.config(state=tk.NORMAL)
            text_widget.delete(1.0, tk.END)
            text_widget.config(state=tk.DISABLED)
            
    def read_config(self, file_name):
        """读取配置文件并返回解析后的数据"""
        try:
            with open(file_name, 'r', encoding='utf-8') as file:
                lines = [line.strip() for line in file if line.strip()]
                config = []
                for line in lines:
                    parts = line.split()
                    if not parts:
                        continue
                    # 第一行特殊处理（timeout值）
                    value = float(parts[1]) if len(config) == 0 else int(float(parts[1]))
                    config.append(value)
            
            # 更新配置显示
            config_text = (f"超时时间: {config[0]}s | "
                         f"最大从机数: {config[1]} | "
                         f"轮询次数: {config[2]} | "
                         f"调试模式: {'开启' if config[3] else '关闭'}")
            self.config_label.config(text=config_text)
            
            self.log_message("配置文件加载成功", "green")
            return config
        except Exception as e:
            # 使用默认配置
            default_config = [0.04, 15, 15, 1]
            config_text = (f"超时时间: {default_config[0]}s | "
                         f"最大从机数: {default_config[1]} | "
                         f"轮询次数: {default_config[2]} | "
                         f"调试模式: {'开启' if default_config[3] else '关闭'}")
            self.config_label.config(text=config_text)
            
            self.log_message(f"配置文件读取错误: {e}，使用默认配置", "red")
            return default_config
            
    def open_serial(self):
        """打开串口连接"""
        port_str = self.port_combobox.get()
        if not port_str:
            messagebox.showerror("错误", "请选择串口")
            return False
            
        port = port_str.split(' - ')[0]
        try:
            self.ser = serial.Serial(port, 9600, timeout=self.TIMEOUT)
            self.log_message(f"已连接串口: {port}", "green")
            return True
        except Exception as e:
            self.log_message(f"串口连接失败: {e}", "red")
            return False
            
    def close_serial(self):
        """关闭串口连接"""
        if self.ser and self.ser.is_open:
            self.ser.close()
            self.log_message("串口已关闭", "blue")
            
    def send_command(self, data, debug=False):
        """发送命令到串口并返回响应"""
        if not self.ser or not self.ser.is_open:
            self.log_message("串口未连接", "red")
            return None
            
        self.ser.write(data)
        response = self.ser.read(5)
        
        if response:
            hex_str = binascii.hexlify(response).decode()
            return [hex_str[i:i+2] for i in range(0, len(hex_str), 2)]
        return None
        
    def start_check_devices_thread(self):
        """启动检测设备的线程"""
        thread = threading.Thread(target=self.check_devices)
        thread.daemon = True
        thread.start()
        
    def check_devices(self):
        """检测在线设备"""
        self.close_serial()

        if not self.open_serial():
            return
            
        self.online_devices = []
        self.device_listbox.delete(0, tk.END)
        self.read_btn.config(state=tk.DISABLED)
        
        self.log_message("\n开始检测在线设备...", "blue")
        
        for device in range(self.DEVICE_UPPER):
            data = [0x5A, device, 0x08, 0x13]
            data.append(sum(data))
            found = False
            
            if self.DEBUG:
                self.log_message(f"检测设备 {device}: 发送数据 {[hex(x) for x in data]}", "purple", device)
            
            for _ in range(self.POLLING_NUM):
                response = self.send_command(data, self.DEBUG)
                if response:
                    int_response = [int(x, 16) for x in response]
                    if self.DEBUG:
                        self.log_message(f"收到响应: {response} -> 整型: {int_response}", "black", device)
                    
                    if int_response == data:
                        self.online_devices.append(device)
                        self.device_listbox.insert(tk.END, f"设备 {device}")
                        self.log_message(f"设备 {device} 在线", "green", device)
                        found = True
                        break
            
            if not found and self.DEBUG:
                self.log_message(f"设备 {device} 无响应", "red", device)
        
        self.log_message(f"检测完成! 共发现 {len(self.online_devices)} 个在线设备", "blue")
        
        if self.online_devices:
            self.read_btn.config(state=tk.NORMAL)
            
    def start_read_scores_thread(self):
        """启动读取分数的线程"""
        thread = threading.Thread(target=self.read_scores)
        thread.daemon = True
        thread.start()
            
    def read_scores(self):
        """从在线设备读取分数"""
        if not self.online_devices:
            self.log_message("没有在线设备", "red")
            return
            
        self.total_score = 0
        self.valid_devices = 0
        self.result_text.config(state=tk.NORMAL)
        self.result_text.delete(1.0, tk.END)
        
        self.log_message("\n开始读取设备分数...", "blue")
        
        for device in self.online_devices:
            data = [0x5A, 0x00, 0x03, device]
            data.append(sum(data))
            response = None
            
            if self.DEBUG:
                self.log_message(f"读取设备 {device} 分数: 发送数据 {[hex(x) for x in data]}", "purple", device)
            
            for _ in range(3):  # 最多尝试3次
                response = self.send_command(data, self.DEBUG)
                if response:
                    break
                time.sleep(0.1)
            
            if response:
                int_response = [int(x, 16) for x in response]
                if self.DEBUG:
                    self.log_message(f"收到响应: {response} -> 整型: {int_response}", "black", device)
                
                if len(int_response) >= 5 and int_response[4] == sum(int_response[:4]):
                    if int_response[3] == 0x6F:
                        self.log_message(f"设备 {device}: 分数未确认", "red", device)
                        self.result_text.insert(tk.END, f"设备 {device}: 分数未确认\n", "red")
                    else:
                        score = int_response[3]
                        if score > 100:
                            self.log_message(f"设备 {device}: 无效分数({score})，记为0", "red", device)
                            self.result_text.insert(tk.END, f"设备 {device}: 无效分数({score})，记为0\n", "red")
                        else:
                            self.log_message(f"设备 {device}: 分数 = {score}", "green", device)
                            self.result_text.insert(tk.END, f"设备 {device}: 分数 = {score}\n", "green")
                            self.total_score += score
                            self.valid_devices += 1
                else:
                    self.log_message(f"设备 {device}: 响应校验失败", "red", device)
                    self.result_text.insert(tk.END, f"设备 {device}: 响应校验失败\n", "red")
            else:
                self.log_message(f"设备 {device}: 无响应", "red", device)
                self.result_text.insert(tk.END, f"设备 {device}: 无响应\n", "red")
        
        # 计算平均分
        self.result_text.insert(tk.END, "\n", "black")
        if self.valid_devices > 0:
            average = self.total_score / self.valid_devices
            self.log_message(f"总分: {self.total_score}, 有效设备数: {self.valid_devices}", "blue")
            self.result_text.insert(tk.END, f"总分: {self.total_score}, 有效设备数: {self.valid_devices}\n", "blue")
            self.log_message(f"平均分: {average:.2f}", "green")
            self.result_text.insert(tk.END, f"平均分: {average:.2f}\n", "green")
        else:
            self.log_message("无有效分数数据", "red")
            self.result_text.insert(tk.END, "无有效分数数据\n", "red")
            
        self.result_text.config(state=tk.DISABLED)
        
    def reset_devices(self):
        """复位所有设备"""
        if not self.ser or not self.ser.is_open:
            self.log_message("串口未连接", "red")
            return
            
        self.log_message("\n正在复位设备...", "blue")
        reset_cmd = [0x5A, 0x00, 0x01, 0x00, 0x5B]
        for _ in range(10):
            self.ser.write(reset_cmd)
            time.sleep(0.05)
        self.log_message("设备已复位，可以开始下一轮评分", "green")
        
    def on_closing(self):
        """窗口关闭事件处理"""
        self.close_serial()
        self.root.destroy()

def main():
    root = tk.Tk()
    app = DeviceScoringApp(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":
    main()
