import re
import urllib.parse
import os
import argparse

'''
本脚本用于将 Obsidian 中的英文笔记进行预处理，提取被反引号括起来的词汇，写入文末

复制英文笔记的 Obsidian URL，填入 `parse_obsidian_uri` 函数中
其将解析该链接并返回该笔记的实际地址
`extract_and_append_quotes` 函数读入实际地址后会提取相应的关键词
并将关键词逐行写入到 `---` 后
'''



def extract_and_append_quotes(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8') as file:
            content = file.read()

        # 使用正则表达式提取被引号括起来的内容
        quotes = re.findall(r'`([^`]*)`', content)

        # 找到分割线并分割文件内容
        parts = content.split('---')
        if len(parts) != 2:  # 
            raise ValueError("文件中必须有且只有一个分割线（---）")

        before_split = parts[0]
        after_split = parts[1].strip()

        # 生成新的内容
        new_content = before_split + '---\n' + after_split + '\n' + '\n'.join(quotes) + '\n'

        # 写回文件
        with open(file_path, 'w', encoding='utf-8') as file:
            file.write(new_content)

        print(f"处理完成")

    except FileNotFoundError:
        print(f"文件 {file_path} 未找到。")
    except Exception as e:
        print(f"处理文件时发生错误：{e}")



def parse_obsidian_uri(obsidian_uri):
	# 处理空字符情况
	if obsidian_uri == '':
		print('empty input!')
		return None

	# 解析 URI
	parsed_url = urllib.parse.urlparse(obsidian_uri)
	query_params = urllib.parse.parse_qs(parsed_url.query)

	# 获取 vault 和 file 参数
	vault_name = query_params.get('vault', [None])[0]
	file_path = query_params.get('file', [None])[0]

	# 检查参数是否存在
	if not vault_name or not file_path:
		raise ValueError("Invalid Obsidian URI")

	# 解码 file_path 并替换路径分隔符
	decoded_file_path = urllib.parse.unquote(file_path).replace('/', '\\')

	# 构建本地文件路径
	local_base_path = r'D:\HowPersonal'
	local_file_path = os.path.join(local_base_path, vault_name, decoded_file_path) + '.md'

	print(local_file_path)
	return local_file_path


parser = argparse.ArgumentParser()
parser.add_argument('url', type=str)
args = parser.parse_args()
local_file_path = parse_obsidian_uri(args.url)
extract_and_append_quotes(local_file_path)
