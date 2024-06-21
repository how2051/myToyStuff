
import requests
import json
from bs4 import BeautifulSoup
import os
import datetime


base_url = "http://www.njlyw.cn"
news_url = "/websitenew/web/list?m=86"

headers = {
	"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:126.0) Gecko/20100101 Firefox/126.0"
}

today = datetime.datetime.today().date()
today = today.strftime("%Y-%m-%d")






def getOnePageNewsInfo(page_index=1):
    page_postfix = f"&p={page_index}"
    response = requests.get(url=base_url+news_url+page_postfix, headers=headers)
    html_content = response.text

    # 使用 BeautifulSoup 解析 HTML
    soup = BeautifulSoup(html_content, 'html.parser')

    # 找到包含新闻列表的 <ul> 标签
    news_list = soup.find('ul', class_='list_news_sec')

    # 提取所有 <li> 标签中的内容
    news_items = news_list.find_all('li')

    # 打印每个新闻的标题和日期
    for item in news_items:
        title = item.find('a').text
        date = item.find('span', class_='span_R').text.strip('[]')
        relative_url = item.find('a')['href']
        full_url = base_url + relative_url
        print(f"{date},《{title}》")
        print(f"        {full_url}\n")


getOnePageNewsInfo()
