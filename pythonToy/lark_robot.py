import requests
import json
from datetime import datetime

url = "https://open.feishu.cn/open-apis/bot/v2/hook/b9e440d8-24d0-444d-abf3-e0f070963b73"
headers = {
    "Content-Type": "application/json"
}

today = datetime.now()
lastday = datetime(2025, 4, 17)
days_of_leaving = (today - lastday).days

plaintext = "别卷啦！"

data_dict = {
	"plaintext" : {
		"msg_type": "text",
		"content": {
			"text": f"{plaintext}"
		}
	},
	"leetcode" : {
		"msg_type": "post",
		"content": {
			"post": {
				"zh_cn": {
					"title": "每日刷题提醒",
					"content": [
						[{
							"tag": "text",
							"text": "你今天刷题了吗？ "
						}, {
							"tag": "a",
							"text": "点击开始刷题",
							"href": "https://leetcode.cn/problemset/"
						}]
					]
				}
			}
		}
	},
	"count" : {
		"msg_type": "post",
		"content": {
			"post": {
				"zh_cn": {
					"title": "正计时提醒",
					"content": [
						[{
							"tag": "text",
							"text": f"距离离开 PCAI 已经 {days_of_leaving} 天啦"
						}]
					]
				}
			}
		}
	},
}

# # for test
# for item in data_dict:
# 	print(item, data_dict[item])

data = data_dict["count"]  # plaintext/leetcode/count
response = requests.post(url, headers=headers, data=json.dumps(data))

print(response.status_code)
print(response.text)
