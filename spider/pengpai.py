
import requests
import json
from bs4 import BeautifulSoup
import os
import datetime


# 25950 # 时事
# 122908 # 国际
# 25951 # 财经
# 119908 # 科技
# 136261 # 暖闻
# 119489 # 智库
# 25952 # 思想


base_url = "https://api.thepaper.cn"
channel_url = "/contentapi/nodeCont/getByChannelId"
right_sidebar_url = "/contentapi/wwwIndex/rightSidebar"
# all_nodes_url = "/contentapi/node/getWwwAllNodes"

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:127.0) Gecko/20100101 Firefox/127.0"
}



def getHotNews():
	response = requests.get(url=base_url+right_sidebar_url, headers=headers)
	response_json = response.json()
	# morningEveningNews = response_json['data']['morningEveningNews']  # 早晚报
	# financialInformationNews = response_json['data']['financialInformationNews']  # 澎湃财讯
	hotNews = response_json['data']['hotNews']  # 澎湃热榜

	for obj in hotNews:
		print(obj['name'])




def getChannelOnePageNews(channel_id, page_index):
	data = {
		"channelId": channel_id,
		"excludeContIds": [],
		"listRecommendIds": None,
		"pageSize": 20,
		"startTime": 1719291533456,
		"pageNum": page_index
	}

	# 创建 Session 对象
	session = requests.Session()

	# 向目标 URL 发送 POST 请求
	response = session.post(url=base_url+channel_url, headers=headers, json=data)
	response_json = response.json()

	for obj in response_json['data']['list']:
		print(obj['name'])


def getChannelMultiPageNews(channel_id, page_number):
    for page_index in range(0, page_number):
    	getChannelOnePageNews(channel_id, page_index)


getChannelMultiPageNews("25950", 3)

# /contentapi/node/getWwwAllNodes
# /contentapi/wwwIndex/recommendNewsRadicalChannelsPage




