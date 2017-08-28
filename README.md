## QSketch是一个简易的绘图工具。
#### 待实现：
1. item各种对齐
2. 分解，合并
3. 倒角
4. 旋转,镜像,撤销,放缩

#### 问题：
1. 增加草图模式
2. 无法完美实现“画图”的效果，第1,2次绘图有问题
3. 最近打开文件记录和文件修改时间 无法在关闭程序后保存
5. setAngle()的参数为什么是360-angle?
6. 只有自定义的CrossPt类调用setPos得到的坐标是场景坐标，Qt类得到的都是图元坐标
7. QObject::connect: Incompatible sender/receiver arguments QAction::triggered(bool) --> MyView::Paste(QPointF)
8. 放大视图如何跟随鼠标
9. 原生图元类不设定为ItemIsMovable时，选定后，再右键菜单会取消选择;拖动或用QTransform后，场景坐标实际没有改变。但是自定义类没有这两个问题。
10. 拖动时，如果出现橡胶手，可能造成拖动的位置不准