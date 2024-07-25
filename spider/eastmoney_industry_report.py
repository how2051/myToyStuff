
import requests
import json
from bs4 import BeautifulSoup
import os
import datetime
import re


report_url_prefix = "https://reportapi.eastmoney.com/report/list?"

headers = {
	"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:126.0) Gecko/20100101 Firefox/126.0"
}

industryNameSet = {
'',
'美容护理','医疗服务','玻璃玻纤','通信设备','商业百货','装修建材','汽车整车',
'仪器仪表','电子元件','农牧饲渔','文化传媒','光伏设备','家用轻工','计算机设备',
'软件开发','有色金属','非金属材料','电力行业','生物制品','互联网服务','能源金属',
'纺织服装','环保行业','船舶制造','半导体','钢铁行业','化肥行业','贵金属',
'电网设备','家电行业','通用设备','铁路公路','化学制品','专用设备','水泥建材',
'汽车零部件','石油行业','旅游酒店','航空机场','航天航空','装修装饰','汽车服务',
'化学制药','食品饮料','交运设备','工程机械','专业服务','电池','煤炭行业',
'公用事业','航运港口','教育','房地产服务','风电设备','塑料制品','化纤行业',
'中药','造纸印刷','医疗器械','化学原料','游戏','通信服务','房地产开发','工程建设',
'银行','证券','小金属','橡胶制品','光学光电子','燃气','保险','消费电子',
'电源设备','包装材料','医药商业','酿酒行业','物流行业','多元金融'
}

today = datetime.datetime.today().date()
today = today.strftime("%Y-%m-%d")




def getPdfLink(infoCode):
	report_url = "https://data.eastmoney.com/report/zw_industry.jshtml?infocode=" + infoCode
	response = requests.get(report_url)
	html_content = response.text

	# 使用 BeautifulSoup 解析 HTML
	soup = BeautifulSoup(html_content, 'html.parser')

	# 查找包含 PDF 链接的 a 标签
	pdf_link_tag = soup.find('a', class_='pdf-link')

	# 提取 PDF 链接
	if pdf_link_tag and 'href' in pdf_link_tag.attrs:
		pdf_link = pdf_link_tag['href']
		# print("get PDF link : ", pdf_link)
	else:
		print("can't find PDF link")

	return pdf_link



def downloadPdf(url, filename, is_only_today=False):
    print(url)

    # 定义非法字符的正则表达式模式
    illegal_chars_pattern = r'[<>:"/\\|?*]'

    if re.search(illegal_chars_pattern, filename):
        # 替换非法字符为 '#'
        cleaned_filename = re.sub(illegal_chars_pattern, '#', filename)
        print(f"Original filename: {filename}")
        print(f"Cleaned filename: {cleaned_filename}")
        filename = cleaned_filename

    if(is_only_today):
        folder = today
    else:
        folder = "report"

    if not os.path.exists(folder):
        os.makedirs(folder)

    filepath = os.path.join(folder, filename)

    response = requests.get(url)
    if response.status_code == 200:
        with open(filepath, 'wb') as f:
            f.write(response.content)
        # print("Download finished : ", filename)
    else:
        print("Download failed")



def getReportInfo(pageIndex):
    report_url_postfix = f"industryCode=*&pageSize=50&industry=*&beginTime=*&endTime=*&pageNo={pageIndex}&qType=1"
    url = report_url_prefix + report_url_postfix
    response = requests.get(url=url, headers=headers)
    results = response.json()
    # print(results['data'])
    return results



def fetchReportPdf(report_info, industry='', is_only_today=False):
    for item in report_info['data']:
        if(industry != '' and industry != item['industryName']):
            continue
        if(is_only_today and item['publishDate'].split()[0] != today):
            break
        print(item['industryName'])
        print(f"《{item['title']}》by {item['orgSName']} : {item['researcher']}")
        # print(item['orgName'])
        # print(item['orgSName'])
        # print(item['infoCode'])
        # print(item['publishDate'])
        # print(item['author'])
        # print(item['researcher'])
        # print(item['attachSize'])
        # print(item['count'])
        pdfLink = getPdfLink(item['infoCode'])
        downloadPdf(pdfLink, f"{item['title']}.pdf", is_only_today)
        print('\n')



def fetchOnePageReport(page_index, industry='', is_only_today=False):
    if industry not in industryNameSet:
        return
    report_info = getReportInfo(page_index)
    fetchReportPdf(report_info, industry, is_only_today)



def fetchMultiPageReport(page_number, industry='', is_only_today=False):
    for page_index in range(1, page_number + 1):
        fetchOnePageReport(page_index, industry, is_only_today)



def fetchTodayReport(industry=''):
    fetchMultiPageReport(3, industry, True)



fetchTodayReport()


