## PAOGD_HW1-弹出的小球-简单建模、插值动画

## 题目介绍

#### 开发环境

Blender2.8 beta

####  场景描述

+ 镜头1：小球在盒子中

![输入图片说明](https://images.gitee.com/uploads/images/2019/0315/144850_f25b314a_1194012.png)

+ 镜头2：盒子底部升起，小球被弹出

![输入图片说明](https://images.gitee.com/uploads/images/2019/0315/145744_e099d572_1194012.png)

+ 镜头3：小球掉落在地

![输入图片说明](https://images.gitee.com/uploads/images/2019/0315/145505_c218e145_1194012.png)

+ 镜头4：小球从地面反弹

![输入图片说明](https://images.gitee.com/uploads/images/2019/0315/145604_f61c5891_1194012.png)

+ 镜头5：小球平稳落下

![输入图片说明](https://images.gitee.com/uploads/images/2019/0315/145640_5cde8cd8_1194012.png)

#### 实现要求

+ 利用关键帧插值的方式完成动画
+ 在动画过程中体现物体的旋转、平移、形变等
+ 利用shape key控制物体形变
+ 利用曲线编辑器graph editor调整物体在动画过程中的属性变化
+ 加分项：尝试给物体添加材质或纹理，使动画更加美观
+ 在满足要求的前提下可适当自由发挥



## 一. 制作一个箱子

+ 新建立方体
+ 调整z轴的高度，压缩成一个平面
+ shift+左键选中一个平面
+ 利用extrude将上移该平面
+ 新增一个平面，后面用来做箱顶弹出的动画

![1552707364555](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1552707364555.png)



+ r 可以进行旋转，或者调整右边面板的旋转角度
+ g键移动调整位置

![1552981119672](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1552981119672.png)



## 二. 新建一个平面

+ shift+a新建 plane

+ 拖动箱子到合适的位置

![1552707596659](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1552707596659.png)

## 三. 制作一个球体

+ shift+a新建 UV shape
+ 并调整至合适的大小
+ 然后将球体放入箱子

![1552707584427](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1552707584427.png)



## 四. 制作动画帧

加入关键帧主要包括这几个画面

+ 球弹出，箱子恢复

+ 球在半空
+ 球落地 变扁
+ 球反弹
+ 球平稳落地



### 添加操作解析：

#### 1.选中物品，这里以小球为例子

在时间轴上选择一个合适的位置插入关键帧，在该帧中调整小球的位置

![1553003520777](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1553003520777.png)



#### 2. 调整小球的位置

这是小球在最高峰时候的位置，添加在时间轴20上

![1553003796120](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1553003796120.png)



#### 3.调试过程

在插入关键帧的时候，我们可以点击时间轴的**开始键**，同时进行调试物品运动的情况。

#### 4.改变小球的运动轨迹

这里用到的是**曲线编辑器**，可以通过拖动来改变小球的运动快慢角度之类

![12](C:\Users\asus\Desktop\12.png)

## 五. 为物品添加一个纹理（加分项）

首先，我们要进入编辑模式，选中所需要加纹理的物品。

然后，在右侧中选择材质框，这里可以调整物品的颜色，透明度等等，这里我简单的改变一下颜色与亮度，更多更难的功能还待继续发掘。

![1553004157561](C:\Users\asus\AppData\Roaming\Typora\typora-user-images\1553004157561.png)



## 六. 实验结果

最后完成的效果如图所示：

![result](C:\Users\asus\Desktop\result.gif)



## 七. 实验感想

​	通过这次实验，我总算掌握了一点Blender的皮毛。Blender的操作十分多，想要一时半会了解完不太现实。这次实验学习到了简单的G键移动，S键缩放，R键旋转，给正方体切掉顶部以及如何切换视角之类的建模操作。除此之外，对于动画帧的设计也有了初步的了解，如何做一个好看的插值动画也需要花上不少的功夫。

​	在做实验期间，能参考的资料也只用一些手册，所以必须静下心来查阅资料才能找到这些按钮以及使用方法，阅读文档的能力也有了不少的进展。之前学习过Unity3D如何做一个插值动画，类比到blender中不算太难，主要是需要熟悉这些操作。而我大概可能更喜欢Unity可以用代码来控制动画的生成吧，但多学点东西总是不坏的。学习Blender之路仍在继续！