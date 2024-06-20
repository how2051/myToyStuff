
import requests
import json
from bs4 import BeautifulSoup
import os




report_url_prefix = "https://reportapi.eastmoney.com/report/list?"

headers = {
	"User-Agent" : "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:126.0) Gecko/20100101 Firefox/126.0"
}




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




def downloadPdf(url, filename):
    if '/' in filename:
        print(f"skip {filename}")
        return
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




def fetchNPageReport(pageNumber):
    for page in range(1, pageNumber+1):
        report_url_postfix = f"industryCode=*&pageSize=50&industry=*&beginTime=*&endTime=*&pageNo={page}&qType=1"
        url = report_url_prefix + report_url_postfix
        response = requests.get(url=url, headers=headers)
        results = response.json()
        # print(results['data'])
    
        for item in results['data']:
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
            print(pdfLink)
            downloadPdf(pdfLink, f"{item['title']}.pdf")
            print('\n')





fetchNPageReport(3)


