# video_image 控件用法

​	在一般的嵌入式板子上面，解析 JPG 文件或者解析 PNG 文件速度是很慢的，尤其在播放高分辨率序列帧的时候尤为明显，这个时候就可以使用 video_image 控件来实现播放序列帧了。

​	video_image 控件是采用帧间差异的图像算法，把序列帧压缩为一个自定义的视频文件中，这种方法的逻辑是空间换时间的策略，虽然整体视频文件的大小会比 JPG 文件（PNG 文件）要大，但是会比位图文件要小很多的。

### 一，video_image 控件

​	video_image 控件的用法其实很简单，例如：

``` xml
<window anim_hint="htranslate" style:normal:bg_color="#00000000" >
  <video_image x="0" y="0" w="800" h="480" video_name="video_12" />
  <label x="c" y="m" w="200" h="100" visible="false" style:normal:font_size="80" style:normal:text_color="#ff0000" text="the end" />
</window>
```

​	video_image 控件的属性：

| 属性       | 作用                                                 |
| ---------- | ---------------------------------------------------- |
| video_name | 需要播放的视频文件名称（该文件是放在 data 文件夹中） |
| auto_play  | 是否自动播放（默认为自动播放）                       |
| delay_paly | 是否延迟播放（默认不延迟播放）                       |

​	在使用上，video_image 控件只需要配置 video_name 属性就可以播放序列帧了，所以是非常方便的，同时播放完成后，会触发 EVT_ANIM_END 事件，可以通过 widget_on  函数注册对应事件的回调函数。

### 二，序列帧压缩工具（diff_image_to_video_gen）

​	由于 video_image 控件只负责播放显示序列帧而已，真是把序列帧压缩是通过 diff_image_to_video_gen 工具来完成，该工具可以在 window 和 linux 上面运行。

​	diff_image_to_video_gen 工具的用法：diff_image_to_video_gen.exe [空格] image_dir [空格] image_name_format [空格] save_file_path [空格] frame_image_type [空格] delays

| 参数              | 说明                   | 备注                                             |
| ----------------- | ---------------------- | ------------------------------------------------ |
| image_dir         | 序列帧的文件夹路径     |                                                  |
| image_name_format | 序列帧的文件名格式     | 例如：本例子中 s01.jpg 文件，则文件名格式为 s%2d |
| save_file_path    | 视频文件保存路径       |                                                  |
| frame_image_type  | 视频文件保存的颜色格式 | 需要对应 LCD 的类型                              |
| delays            | 序列帧的帧间延迟时间   | 每一帧的时间间隔，单位为毫秒                     |

> 备注：
>
> 本例子的序列帧放在 .\design\default\images\video\video_12 文件夹中，序列帧的文件名格式为 s%2d。

​	而本例子中是统一通过调用 update_res.py 脚本或者 designer 来生成资源的，如下：

```cmd
python scripts/update_res.py all
```

​	同时需要注意的是，虽然是可以通过 update_res.py 脚本或者 designer 来生成资源，但是其生成资源的路径或者序列帧的文件名格式等参数都是被写死了，所以用户如果需要修改的话，请打开 .\scripts\update_res_generate_res_handler.py，如下根据注释来修改：

```python
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
```