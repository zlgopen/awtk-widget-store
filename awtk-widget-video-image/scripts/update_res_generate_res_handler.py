#!/usr/bin/python

import os
import platform

# 序列帧的文件名格式
video_image_format="s%02d"

# 视频文件保存的颜色格式，需要对应 LCD 的类型
video_image_bitmap_format="BGR565"

# 序列帧的帧间延迟时间（每一帧的时间间隔，单位为毫秒）
video_image_delay=80

def joinPath(root, subdir):
  return os.path.normpath(os.path.join(root, subdir))

def genVideoImageGen(src_dir):
  data_dir = joinPath(src_dir, 'data')
  if os.path.exists(data_dir):
    image_dir = joinPath(src_dir, 'images/video')
    if os.path.exists(image_dir):
      tool_path = os.path.abspath('bin/diff_image_to_video_gen')
      if platform.system() == "Windows" :
        tool_path += ".exe"
      if os.path.exists(tool_path):
        for video_image_dir in os.listdir(image_dir) :
          os.system(tool_path + " " + os.path.abspath(joinPath(image_dir, video_image_dir)) + " " + video_image_format + " " + joinPath(data_dir, video_image_dir)  + " " + video_image_bitmap_format +" " + str(video_image_delay))
      else :
        print(" not find diff_image_to_video_gen.exe")


def on_generate_res_before(ctx):
    print('======================= '+ ctx['type'] + ' =======================')
    # ctx = {'type': 'ui', 'theme': 'default', 'imagegen_options': 'mono', 'input': './', 'output': './'}
    if ctx['type'] == "data" :
        genVideoImageGen("design/default")


def on_generate_res_after(ctx):
    print('======================= '+ ctx['type'] + ' =======================')
