
import requests
import json
from bs4 import BeautifulSoup
import os
import time
import datetime

# pip install requests beautifulsoup4 lxml

# 25950 # 时事
# 122908 # 国际
# 25951 # 财经
# 119908 # 科技
# 136261 # 暖闻
# 119489 # 智库
# 25952 # 思想

channel = [
	"25950", "122908", "25951",	"119908",
	# "136261", "119489",	"25952"
]


today = datetime.datetime.today().date()

base_url = "https://api.thepaper.cn"
channel_url = "/contentapi/nodeCont/getByChannelId"
right_sidebar_url = "/contentapi/wwwIndex/rightSidebar"
# all_nodes_url = "/contentapi/node/getWwwAllNodes"
# recommend_url = "/contentapi/wwwIndex/recommendNewsRadicalChannelsPage"

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:127.0) Gecko/20100101 Firefox/127.0"
}



def getNDaysBeforeTimestamp(days=1):
	pre_day = today - datetime.timedelta(days=days)
	pre_day_with_time = datetime.datetime.combine(pre_day, datetime.time(0, 0, 0))
	timestamp = int(time.mktime(pre_day_with_time.timetuple()) * 1000)
	return timestamp



def parseTimestamp(timestamp):
	timestamp_in_seconds = timestamp / 1000
	dt_object = datetime.datetime.fromtimestamp(timestamp_in_seconds)
	return dt_object



def getHotNews():
	response = requests.get(url=base_url+right_sidebar_url, headers=headers)
	response_json = response.json()
	# morningEveningNews = response_json['data']['morningEveningNews']  # 早晚报
	# financialInformationNews = response_json['data']['financialInformationNews']  # 澎湃财讯
	hotNews = response_json['data']['hotNews']  # 澎湃热榜

	for obj in hotNews:
		print(obj['name'])



def showNewsContent(news_url):
	response = requests.get(news_url)
	html_content = response.text

	soup = BeautifulSoup(html_content, 'html.parser')

	find_tag = soup.find('div', class_='index_cententWrap__Jv8jK')
	# print(find_tag)

	if(find_tag):
		paragraphs = find_tag.find_all('p')
		for p in paragraphs:
			print(p.get_text() + '\n')



def showTags(obj):
    tag_list = obj.get('tagList', [])
    tags = [tag_obj['tag'] for tag_obj in tag_list]
    tag_string = ", ".join(tags)
    if(tag_string):
    	print("关键词：" + tag_string)



def showPubTime(obj):
	pub_time = parseTimestamp(obj['pubTimeLong'])
	print("发布时间：" + pub_time.strftime("%Y-%m-%d %H:%M:%S"))



def getChannelOnePageNews(channel_id, page_index):
	data = {
		"channelId": channel_id,
		"excludeContIds": [],
		"listRecommendIds": None,
		"pageSize": 20,
		"startTime": getNDaysBeforeTimestamp(page_index+1),
		"pageNum": page_index
	}

	# 创建 Session 对象
	session = requests.Session()

	# 向目标 URL 发送 POST 请求
	response = session.post(url=base_url+channel_url, headers=headers, json=data)
	response_json = response.json()

	for obj in response_json['data']['list']:
		# print(obj)
		news_url = "https://www.thepaper.cn/newsDetail_forward_" + obj['contId']
		print(obj['name'] + " by @" + obj['nodeInfo']['name'])  # news title
		print(news_url)  # news url
		showPubTime(obj)
		showTags(obj)
		showNewsContent(news_url)  # news content
		print("======================================================")
		print('\n')



def getChannelMultiPageNews(channel_id, page_number):
	for page_index in range(0, page_number):
		getChannelOnePageNews(channel_id, page_index)



def getMultiChannelNews():
    for channel_id in channel:
        getChannelOnePageNews(channel_id, 0)



# getHotNews()
getChannelMultiPageNews("25950", 3)
# getMultiChannelNews()






