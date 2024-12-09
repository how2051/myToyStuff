
import requests
from bs4 import BeautifulSoup
import chardet

def parsePeoplePaper(news_url):
	response = requests.get(news_url)
	html_content = response.text

	soup = BeautifulSoup(html_content, 'html.parser')

	contents = soup.find_all('div', class_='content', attrs={'data-v-e6acc8cc': True})
	for content in contents:
		page_ban = content.find('div', class_='page_ban')
		if page_ban:
			span = page_ban.find('span')
			if span:
				print(span.get_text(strip=True) + "\n")

		titles = content.find_all('div', class_='title')
		introduces = content.find_all('div', class_='introduce')
		for title, intro in zip(titles, introduces):
			# if(intro.get_text(strip=True) == ""): continue
			print(title.get_text(strip=True))
			print("-" * len(title.get_text(strip=True)) * 2)
			print(intro.get_text(strip=True))
			print("\n\n")



def parsePeopleOnline(news_url):
	response = requests.get(news_url)

	# # 使用 chardet 自动检测编码
	# detected_encoding = chardet.detect(response.content)['encoding']
	# print(f"Detected encoding: {detected_encoding}")

	# 根据自动检测的编码来解码内容
	html_content = response.content.decode('GB2312')
	# html_content = response.text

	soup = BeautifulSoup(html_content, 'html.parser')
	with open("test_ppl_html.txt", "a", encoding="utf-8") as file:
		file.write(soup.prettify())
	print(soup.prettify())


ppl_paper_url = "https://www.peopleapp.com/newspaper"
# parsePeoplePaper(ppl_paper_url)

ppl_online_url = "http://www.people.com.cn/"
# parsePeopleOnline(ppl_online_url)

