# dirty_rect_view 控件

​	由于三缓冲的状态下，是无法启用 awtk 的默认脏矩形机制，但是脏矩形机制可以大幅度提高绘图效率，所以就写了一个自定义分段式脏矩形控件来满需这个需求，同时该控件只适用于顺序切换缓冲区的三缓冲机制。

​	自定义分段式脏矩形控件分为 dirty_rect_view 控件和 dirty_rect_view_item 控件，而  dirty_rect_view 控件是作为父集，dirty_rect_view_item 控件为子集。

​	自定义分段式脏矩形控件，会根据 dirty_rect_view_item 控件为默认的绘制区域（可能的脏矩形区域）， dirty_rect_view 控件遍历 dirty_rect_view_item 控件下面的所以子控件，然后判断该区域是否需要重绘，如果需要就重绘，不需要就不重绘。

> 备注：具体的用法请看 docs 文件中的文档。

## 准备

1. 获取 awtk 并编译

```
git clone https://github.com/zlgopen/awtk.git
cd awtk; scons; cd -
```

## 运行

1. 生成示例代码的资源

```
python scripts/update_res.py all
```
> 也可以使用 Designer 打开项目，之后点击 “打包” 按钮进行生成
> 如果资源发生修改，则需要重新生成资源。


2. 编译

```
scons
```
> 注意：
> 编译前先确认 SConstruct 文件中的 awtk_root 是否为 awtk 所在目录，不是则修改。
> 默认使用动态库的形式，如果需要使用静态库，修改 SConstruct 文件中的 BUILD_SHARED = 'false' 即可。

3. 运行
```
./bin/demo
```

## 文档

[AWTK 自定义控件规范](https://github.com/zlgopen/awtk/blob/master/docs/custom_widget_rules.md)
