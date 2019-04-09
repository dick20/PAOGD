## PAOGD_HW2

#### 介绍
PAOGD个人作业2-角色动画基础

#### 开发环境
Blender2.8 beta
#### DeadLine: 4月12日 **22** 点
#### 场景描述

实现一个简单的行走循环动画

![HW2.gif](./HW2.gif "HW2.gif")


#### Tips
- 导入模型,得到角色网格（根目录下的stickman.obj或其他模型，可自己构造，在report中说明）
- 在角色网格的基础上创建基础骨架(Armature)

![输入图片说明](https://gitee.com/uploads/images/2019/0404/155350_38fd066f_1194012.png "屏幕截图.png")
- 为骨架添加约束器,利用反向运动学(Inverse Kinematics)实现腿部装配

![输入图片说明](https://gitee.com/uploads/images/2019/0404/155700_50b377db_1194012.png "屏幕截图.png")
- 进入权重绘制(Weight Paint)模式修改顶点权重

![输入图片说明](https://gitee.com/uploads/images/2019/0404/160319_c684d7e8_1194012.png "屏幕截图.png")
- 在动作编辑器(Action Editor)模式创建行走动作

![输入图片说明](https://gitee.com/uploads/images/2019/0404/160744_7db6398f_1194012.png "屏幕截图.png")
- 利用姿态镜像使创建的动作可循环
- 在NonLinear Animation模式下重复Walk动作

![输入图片说明](https://gitee.com/uploads/images/2019/0404/161454_76e6c449_1194012.png "屏幕截图.png")
![输入图片说明](https://gitee.com/uploads/images/2019/0404/161518_95378fbd_1194012.png "屏幕截图.png")
- 添加曲线,并让小人沿着曲线行走
- 合理地摆放、控制镜头

#### 注意：
- 渲染后的视频较大，不要求提交，若想提交建议转成gif格式再提交
- 提交前请自己审核提交内容，是否按照目录结构要求提交，是否存在冲突等；
- 请不要更改主目录下的内容，不要提交无关的文件（.keep文件、mac系统下的目录文件等);
- 提交Pull Request前请先自行合并commit记录

#### 提交要求及命名格式
- /src 存放项目文件
- /report 存放项目报告

### 个人项目提交方式
1. 布置的个人项目先fork到个人仓库下；
2. clone自己仓库的个人项目到本地目录；
3. 在个人项目中，在src、report目录下，新建个人目录，目录名为“学号+姓名”，例如“**12345678WangXiaoMing**”；
4. 在“src\12345678WangXiaoMing”目录下，保存项目，按要求完成作业
5. 实验报告以md的格式，写在“report\12345678WangXiaoMing”目录下；
6. 完成任务需求后，Pull Request回主项目的master分支，PR标题为“学号+姓名”， 如“**12345678王小明**”；
7. 一定要在deadline前PR。因为批改后，PR将合并到主项目，所有同学都能看到合并的结果，所以此时是不允许再PR提交作业的。