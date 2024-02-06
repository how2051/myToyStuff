
#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>

// 定义图像大小
const int width = 1920;
const int height = 1080;

// 读取BGGR排列的Bayer图像数据
void readBayerImage(const std::string& filename, std::vector<uint16_t>& bayerData) {
    std::ifstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(bayerData.data()), width * height * sizeof(uint16_t));
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << std::endl;
    }
}

// 将单通道的Bayer图像转换为三通道的RGB图像（每个像素点有 r、g、b 分量，但最多只有一个通道为非零值，颜色的排列同 bayer）
void convert2ThreeChannelBayer(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int targetIndex = y * 1920 + x;
			uint8_t rgbValue = static_cast<uint8_t>(bayerData[targetIndex] >> 8);

			rgbData[targetIndex * 3 + 0] = (x % 2 == 1 && y % 2 == 1) ? rgbValue : 0; // R通道
			rgbData[targetIndex * 3 + 1] = ((x % 2 == 1 && y % 2 == 0) || ((x % 2 == 0 && y % 2 == 1))) ? rgbValue : 0; // G通道
			rgbData[targetIndex * 3 + 2] = (x % 2 == 0 && y % 2 == 0) ? rgbValue : 0; // B通道
		}
	}
}

// 将单通道的Bayer图像转换为三通道的RGB图像（每个像素点有 r、g、b 分量，但 r == g == b, 是灰度图）
void convert2ThreeChannelGray(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
    // 简单示例，仅做颜色平均
    for (int i = 0; i < width * height; ++i) {
        uint16_t pixelValue = bayerData[i];

        // 这里进行简单的颜色平均处理，实际情况可能需要更复杂的插值算法
        uint8_t rgbValue = static_cast<uint8_t>(pixelValue >> 8); // 取高8位作为RGB值

        // 将RGB值复制到每个通道
        rgbData[i * 3 + 0] = rgbValue; // R通道
        rgbData[i * 3 + 1] = rgbValue; // G通道
        rgbData[i * 3 + 2] = rgbValue; // B通道
    }
}

// 将单通道的Bayer图像转换为三通道的RGB图像（正儿八经 demosaicing，最近邻插值算法，每个像素点有 r、g、b 分量，是彩图）
void convert2ThreeChannelRGB_NearestNeighbor(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
	int height = 1080;
	int width  = 1920;
	for (int y = 0; y < height - 1; y += 2) {
		for (int x = 0; x < width - 1; x += 2) {
			// fetch BGGR data
			uint8_t b  = bayerData[y * width + x] >> 8;
			uint8_t g  = ((bayerData[y * width + (x + 1)]
						 + bayerData[(y + 1) * width + x]) / 2) >> 8;
			uint8_t r  = bayerData[(y + 1) * width + (x + 1)] >> 8;

			// fill RGB data
			uint8_t* rgbPtr = &rgbData[(y * width + x) * 3];
			rgbPtr[0] = b;
			rgbPtr[1] = g;
			rgbPtr[2] = r;

			rgbPtr = &rgbData[(y * width + x + 1) * 3];
			rgbPtr[0] = b;
			rgbPtr[1] = g;
			rgbPtr[2] = r;

			rgbPtr = &rgbData[((y + 1) * width + x) * 3];
			rgbPtr[0] = b;
			rgbPtr[1] = g;
			rgbPtr[2] = r;

			rgbPtr = &rgbData[((y + 1) * width + x + 1) * 3];
			rgbPtr[0] = b;
			rgbPtr[1] = g;
			rgbPtr[2] = r;
		}
	}
}

// 将单通道的Bayer图像转换为三通道的RGB图像（正儿八经 demosaicing，双线性插值算法，每个像素点有 r、g、b 分量，是彩图）
void convert2ThreeChannelRGB_Bilinear(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
	int height = 1080;
	int width  = 1920;
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			uint8_t r, g, b;

			//这里直接跳过上下左右四个 corner case，牺牲两行两列像素（会变成灰度图），简化处理逻辑
			if(y == 0 || x == 0 || y == height - 1 || x == width - 1) {
				r = bayerData[y * width + x] >> 8;
				g = bayerData[y * width + x] >> 8;
				b = bayerData[y * width + x] >> 8;
				uint8_t* rgbPtr = &rgbData[(y * width + x) * 3];
				rgbPtr[0] = b;
				rgbPtr[1] = g;
				rgbPtr[2] = r;
				continue;
			}

			if(x % 2 == 1 && y % 2 == 1) {	// R
				r = bayerData[y * width + x] >> 8;
				g = ((bayerData[(y-1) * width + x]
					+ bayerData[(y+1) * width + x]
					+ bayerData[y * width + (x-1)]
					+ bayerData[y * width + (x+1)]) / 4) >> 8;
				b = ((bayerData[(y-1) * width + (x-1)]
					+ bayerData[(y+1) * width + (x+1)]
					+ bayerData[(y+1) * width + (x-1)]
					+ bayerData[(y-1) * width + (x+1)]) / 4) >> 8;
			}

			if(x % 2 == 0 && y % 2 == 0) {	// B
				r = ((bayerData[(y-1) * width + (x-1)]
					+ bayerData[(y+1) * width + (x+1)]
					+ bayerData[(y+1) * width + (x-1)]
					+ bayerData[(y-1) * width + (x+1)]) / 4) >> 8;
				g = ((bayerData[(y-1) * width + x]
					+ bayerData[(y+1) * width + x]
					+ bayerData[y * width + (x-1)]
					+ bayerData[y * width + (x+1)]) / 4) >> 8;
				b = bayerData[y * width + x] >> 8;
			}

			if(x % 2 == 1 && y % 2 == 0) {	// G1
				r = ((bayerData[(y-1) * width + x]
					+ bayerData[(y+1) * width + x]) / 2) >> 8;
				g = bayerData[y * width + x] >> 8;
				b = ((bayerData[y * width + (x-1)]
					+ bayerData[y * width + (x+1)]) / 2) >> 8;
			}

			if(x % 2 == 0 && y % 2 == 1) {	// G2
				r = ((bayerData[y * width + (x-1)]
					+ bayerData[y * width + (x+1)]) / 2) >> 8;
				g = bayerData[y * width + x] >> 8;
				b = ((bayerData[(y-1) * width + x]
					+ bayerData[(y+1) * width + x]) / 2) >> 8;
			}

			uint8_t* rgbPtr = &rgbData[(y * width + x) * 3];
			rgbPtr[0] = b;
			rgbPtr[1] = g;
			rgbPtr[2] = r;
		}
	}
}

// pixelize rgb image
void pixelizeRgbImage(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
	// 既然都要 pixelize 了，所以就用最近邻算法就够用了
	convert2ThreeChannelRGB_NearestNeighbor(bayerData, rgbData);

	// 将图像划分为若干个 10x10 的小块，求小块的平均颜色，并将其赋值给小块内所有像素点
	for(int y = 0; y < height; y += 10) {
		for(int x = 0; x < width; x += 10) {
			int sumR = 0;
			int sumG = 0;
			int sumB = 0;
			for(int i = 0; i < 100; i++) {
				int targetIndex = (y + i / 10) * 1920 + (x + i % 10);
				sumR += rgbData[targetIndex*3 + 0];
				sumG += rgbData[targetIndex*3 + 1];
				sumB += rgbData[targetIndex*3 + 2];
			}

			sumR /= 100;
			sumG /= 100;
			sumB /= 100;
			//以上部分用于求像素块的颜色平均值

			for(int i = 0; i < 10; i++) {
				for(int j = 0; j < 10; j++) {
					int targetIndex = (y+i) * 1920 + (x+j);
					if(i == 0 || j == 0 || i == 9 || j == 9) { // 将像素块边缘描黑
						//注释掉以下内容的话则边缘是底图的原图，即用最近邻算法得到的彩图
						rgbData[targetIndex * 3 + 0] = 0;
						rgbData[targetIndex * 3 + 1] = 0;
						rgbData[targetIndex * 3 + 2] = 0;
					} else {
						rgbData[targetIndex * 3 + 0] = sumR;
						rgbData[targetIndex * 3 + 1] = sumG;
						rgbData[targetIndex * 3 + 2] = sumB;
					}
				}
			}
		}
	}
}

// slice rgb image
void sliceRgbImage(const std::vector<uint16_t>& bayerData, std::vector<uint8_t>& rgbData) {
	convert2ThreeChannelRGB_NearestNeighbor(bayerData, rgbData);

	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			int targetIndex = y * 1920 + x;
			if(x % 10 < 2) {
				rgbData[targetIndex * 3 + 0] = 0;
				rgbData[targetIndex * 3 + 1] = 0;
				rgbData[targetIndex * 3 + 2] = 0;
			} else {
				//
			}
		}
	}
}



// 保存RGB图像数据到文件
void saveThreeChannelRGBRawData(const std::string& filename, const std::vector<uint8_t>& rgbData) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(rgbData.data()), width * height * 3); // 3通道（RGB）每个像素用一个字节表示
        file.close();
        std::cout << "RGB image saved to file: " << filename << std::endl;
    } else {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
    }
}

// 保存RGB图像数据到PNG文件（使用OpenCV库）
void saveRGBRawDataToPNG(const std::string& filename, const std::vector<uint8_t>& rgbData, int width, int height) {
    cv::Mat rgbMat(height, width, CV_8UC3, const_cast<uint8_t*>(rgbData.data()));
    cv::imwrite(filename, rgbMat);
    std::cout << "PNG image saved to file: " << filename << std::endl;
}



//g++ -o main bayerConvert.cpp -lpthread $(pkg-config --cflags --libs opencv4)
int main() {
    std::vector<uint16_t> bayerData(width * height);
    std::vector<uint8_t> rgbData(width * height * 3); // 3通道（RGB）

    readBayerImage("output.raw", bayerData);

    convert2ThreeChannelBayer(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputBayer.rgb", rgbData);
	saveRGBRawDataToPNG("outputBayer.png", rgbData, width, height);

    convert2ThreeChannelGray(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputGray.rgb", rgbData);
	saveRGBRawDataToPNG("outputGray.png", rgbData, width, height);

    convert2ThreeChannelRGB_NearestNeighbor(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputRGB_NearestNeighbor.rgb", rgbData);
	saveRGBRawDataToPNG("outputRGB_NearestNeighbor.png", rgbData, width, height);

    convert2ThreeChannelRGB_Bilinear(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputRGB_Bilinear.rgb", rgbData);
	saveRGBRawDataToPNG("outputRGB_Bilinear.png", rgbData, width, height);



	// -------------------------------------------------------------------- //

    pixelizeRgbImage(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputRGB_pixelize.rgb", rgbData);
	saveRGBRawDataToPNG("outputRGB_pixelize.png", rgbData, width, height);

    sliceRgbImage(bayerData, rgbData);
    // saveThreeChannelRGBRawData("outputRGB_slice.rgb", rgbData);
	saveRGBRawDataToPNG("outputRGB_slice.png", rgbData, width, height);

    return 0;
}
