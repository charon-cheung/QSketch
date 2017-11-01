## <center>QSketch是一个简易的绘图工具。</center>

#### 使用说明


#### To-do list:
1. 自定义箭头图元和渐变的圆形图元
2. 草图模式和普通模式切换时，没有提示保存文件。保存草图文件后再打开，仍然初始化场景，需要对两种模式的文件做区分


#### 问题：
1. 字体进行平移时，上下是相反的，应该与视图对称有关
2. 跨文件复制图元时，所有图元仍然按鼠标位置复制，结果是都聚集到一起
3. 文字进行文件保存后，再打开，位置保存可能不成功
4. 最近打开文件记录和文件修改时间 无法在关闭程序后保存
5. setAngle()的参数为什么是360-angle?
6. 只有自定义的CrossPt类调用setPos得到的坐标是场景坐标，Qt类得到的都是图元坐标
7. QObject::connect: Incompatible sender/receiver arguments QAction::triggered(bool) --> MyView::Paste(QPointF)
9. 原生图元类不设定为ItemIsMovable时，选定后，再右键菜单会取消选择;拖动或用QTransform后，场景坐标实际没有改变。但是自定义类没有这两个问题。旋转图元后，输出坐标值未变
11. 保存函数MyScene::Export代码重复严重，MyView::Copy()直接调用了Export,但MyView::Paste()没有直接调用MyScene::Import，而是修改Import，重复也很严重

