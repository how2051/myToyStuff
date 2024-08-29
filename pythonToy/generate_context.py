# import PyPDF2

# def read_pdf_with_pypdf2(file_path):
#     # 打开 PDF 文件
#     with open(file_path, 'rb') as file:
#         # 创建 PDF 阅读器对象
#         reader = PyPDF2.PdfReader(file)
#         # 初始化一个空字符串来存储所有页面的文本
#         text = ""
#         # 遍历每一页
#         for page_num in range(len(reader.pages)):
#             # 获取页面对象
#             page = reader.pages[page_num]
#             # 提取页面文本
#             text += page.extract_text()
    
#     return text

# # 示例使用
# file_path = 'ESLPod_0069 - Making a Presentation.pdf'
# text = read_pdf_with_pypdf2(file_path)
# print(text)


import os
import re
import fitz  # PyMuPDF

def list_files(directory):
    """列出指定目录下的所有文件"""
    try:
        # 获取目录下的所有文件和目录
        entries = os.listdir(directory)
        
        # 筛选出文件
        files = [entry for entry in entries if os.path.isfile(os.path.join(directory, entry))]
        
        return files
    except Exception as e:
        print(f"发生错误: {e}")
        return []

def extract_info_from_filename(filename):
    """
    从文件名中提取序号和标题。
    文件名格式：ESLPod_XXXX - Title.pdf
    """
    # 使用正则表达式匹配序号和标题部分
    match = re.match(r"ESLPod_(\d+) - (.+)\.pdf", filename)
    if match:
        # 获取匹配的序号和标题
        number = int(match.group(1))  # 转换为整数
        title = match.group(2)        # 提取标题
        return number, title
    else:
        return None, None

def is_end_of_sentence(text):
    """检查文本是否以标点符号结尾，表示句子结束"""
    return text.strip() and text[-1] in ".!?：。！？”’"

def should_ignore_line(line_text):
    """检查是否需要忽略这一行"""
    return line_text.strip().startswith("Audio Index") \
		or line_text.strip().startswith("Slow dialog") \
		or line_text.strip().startswith("Explanation") \
		or line_text.strip().startswith("Fast dialog")

def extract_context(pdf_path):
	doc = fitz.open(pdf_path)

	# 初始化存储结果的字符串
	formatted_text = ""
	current_paragraph = ""  # 存储当前段落内容

	# 遍历每一页
	for page_num in range(doc.page_count):
		page = doc[page_num]
		blocks = page.get_text("dict")["blocks"]  # 获取页面的文本块
		
		for block in blocks:
			if "lines" in block:  # 检查是否包含文本行
				for line in block["lines"]:
					line_text = ""  # 用于拼接同一行的文本
					for span in line["spans"]:
						text = span["text"]
						# 检查字体是否加粗
						if "bold" in span["font"].lower():
							# 用反引号括起来
							line_text += f"`{text}`"
						else:
							line_text += text

					# 忽略以“Audio Index”开头的行
					if should_ignore_line(line_text):
						continue

					# 判断是否为空白行
					if line_text.strip() == "":
						# 如果当前段落不为空，则添加到结果中并换行
						if current_paragraph.strip():
							formatted_text += current_paragraph.strip() + "\n\n"
							current_paragraph = ""
					else:
						# 判断是否是句子末尾
						if is_end_of_sentence(line_text):
							current_paragraph += line_text
							formatted_text += current_paragraph.strip() + "\n"
							current_paragraph = ""
						else:
							# 否则将其拼接到当前段落
							current_paragraph += line_text + " "

	# 最后一个段落加上去
	if current_paragraph.strip():
		formatted_text += current_paragraph.strip() + "\n"

	# 添加分隔符划线
	formatted_text += "\n\n" + "---" + "\n\n"

	return formatted_text

def save_note(file_name, save_path, context):
	# 写入到 Markdown 文件
	markdown_file_path = f"{save_path}\{file_name}.md"
	with open(markdown_file_path, "w", encoding="utf-8") as file:
		file.write(context)




# 指定目录路径
source_pdf_path = "D:\BaiduNetdiskDownload\ESL Podcast Learning Guide"
target_md_path = r"D:\HowPersonal\myObsidian\2_Resource\B_LanguageLearning\01_"

files = list_files(source_pdf_path)
for file in files:
	number, title = extract_info_from_filename(file)
	# print(f"序号: {number}, 标题: {title}")
	markdown_note_title = f"Lesson {number} - {title}"
	print(markdown_note_title)
	context = extract_context(f"{source_pdf_path}\{file}")
	# print(context)
	save_note(markdown_note_title, target_md_path, context)
	# if number is None and title is None:
	# 	print(file)
	print(f"Saving {markdown_note_title}.md in {target_md_path}")



