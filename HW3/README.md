# HW3 OpenGL初步

### 重要资源
- [教程:https://learnopengl.com/](https://learnopengl.com/)
- [代码仓库:https://github.com/JoeyDeVries/LearnOpenGL.git](https://github.com/JoeyDeVries/LearnOpenGL.git)

现代OpenGL编程的基础知识都可以在教程跟范例代码中找到

### 开发环境要求
-  **Ubuntu 16.04** 
- OpenGL 3.3
- GLFW
- GLAD
### DeadLine: 5月5日 **22** 点

### 安装教程

1. `apt-get install g++ cmake git`
2. `apt-get install libsoil-dev libglm-dev libassimp-dev libglew-dev libglfw3-dev libxinerama-dev libxcursor-dev libxi-dev`
3. `git clone https://github.com/JoeyDeVries/LearnOpenGL.git`
4. `cd LearnOpenGL`
5. `mkdir build`
6. `cd build`
7. `cmake ..`
8. `make -j8`

编译完成后,各章节Demo可执行文件会在build/bin 目录下

### 任务
- 在model loading的Demo中, 示范了如何在OpenGL中导入一个.obj文件
    - 运行范例程序,我们会看到如下画面.同时,我们可以利用鼠标和键盘(WASD),控制镜头的移动
       ![输入图片说明](https://gitee.com/uploads/images/2019/0424/154724_300bb1ec_1194012.png "屏幕截图.png")
- 试编写一个程序,实现模型自身的旋转,平移,缩放
  - 功能需求: 
    - 按下J键, 模型随着时间绕着自身Z轴旋转
    - 按下K键, 模型沿着水平方向往复移动
    - 按下L键, 模型在一定范围内不断放大缩小
-  **注意**
    - **不允许提交所有的范例程序**,请务必新建工程并重新编写CMakeList.txt
    - 确保在你的个人目录下,代码能够独立地编译和执行
    - 确保只保留必要的代码和资源等文件(可以复用范例代码中的文件)
    - 不要上传build目录下的内容
- 报告要求
    - 你是如何利用CMake生成你的可执行程序的
    - .obj格式的文件是怎么保存模型信息的
    - OpenGL与Blender的联系?
    - OpenGL中的坐标系统是怎么样的
    - 你是如何实现功能需求的(简述思路)
    - 你的工程中包含了哪些文件,这些文件的作用是什么

### 提交要求及命名格式
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