# 本脚本在 linux 上运行，需要先安装以下依赖：
# pip install pdf2image Pillow
# sudo apt-get install poppler-utils

import os
from pdf2image import convert_from_path

# 定义 PDF 文件路径
pdf_path = '排序算法.pdf'

# 定义输出图片的文件夹
output_folder = 'Pic/'

# 检查文件夹是否存在，不存在则创建
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# 转换 PDF 每一页为图片
images = convert_from_path(pdf_path, dpi=600)  # 设置 dpi 为 600 以获得高清图片（至少为 300）

# 保存每一页为 PNG 格式
for i, image in enumerate(images):
    image.save(f'{output_folder}/page_{i + 1}.png', 'PNG')

print("转换完成！")

