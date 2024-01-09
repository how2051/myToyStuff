from moviepy.editor import VideoFileClip

def remove_audio(input_video, output_video):
    clip = VideoFileClip(input_video)
    clip = clip.set_audio(None)
    clip.write_videofile(output_video, audio_codec='aac')

if __name__ == "__main__":
    input_file = "input.mp4"  # 替换为你的视频文件路径
    output_file = "output.mp4"  # 替换为输出文件路径

    remove_audio(input_file, output_file)
