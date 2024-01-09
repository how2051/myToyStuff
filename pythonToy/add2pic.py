from PIL import Image

def addtwoimage(image1_path, image2_path, output_path):
    # 打开两个图片
    image1 = Image.open(image1_path)
    image2 = Image.open(image2_path)

    # 获取图片1和图片2的大小
    width1, height1 = image1.size
    width2, height2 = image2.size

    # 计算拼接后图片的大小
    new_width = max(width1, width2)
    new_height = height1 + height2

    # 创建一个新的图片，大小为拼接后图片的较大宽度和高度
    new_image = Image.new("RGB", (new_width, new_height))

    # 将两张图片粘贴到新图片中
    new_image.paste(image1, (0, 0))
    new_image.paste(image2, (0, height1))  # 将第二张图片的顶部与第一张图片的底部对齐

    # 保存拼接后的图片
    new_image.save(output_path)

# 调用函数上下堆叠拼接图片
addtwoimage("pic1.png", "pic2.png", "output.png")
