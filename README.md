-----update on Oct 6,2016

--------------------------------------------------------
 Preface·前言
--------------------------------------------------------
- Before introducing the project(based on Win32),a small mistake I have to say is that I used the wrong word "**Teris**" in my whole project,while the right word is "**Tetris**" ,I didn't notice the mistake until I used baidu search enginee to  looked for the English word of "俄罗斯方块".So hope you don't  get any uncomfortable feelings while viewing  code in  **Teris.cpp**  
在介绍这个（基于Win32的）项目前，我首先要说的是我在这个项目里犯的一个小错误：我用的（表示俄罗斯方块）的单词是Teris而不是Tetris。我直到用百度搜索了俄罗斯方块的英文单词才意识到这个错误，希望您在阅读Teris.cpp的代码时不要因为我用了“Teris”而感到有任何不适。


--------------------------------------------------------
A Detailed Introduction  of Teris.cpp·Teris.cpp的详细介绍
-------------------------------------------------------- 
- Tetris is one of the most famous game in the world,I used to rottened in this game when I was a child.For me it's an exciting time when I implement it(though still have some little bugs that I can't fix ).Now, I'd like to share you the logical implement of this game.

俄罗斯方块是世界上最有名的游戏之一，我小时候经常沉迷于此游戏无法自拔，对我来说实现这个游戏的时候实在激动不已（尽管还有一些我无法修改的小bug）。接下来我将向您展示这个游戏的逻辑实现。

- First,we need a BOOL array to store the shape of the Tetris,"1" means the current block has element, "0" means nothing there. Like this: 

```
{ 1, 1, 1, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }
```
make a little change and it could be more sensible:

```
1 1 1 0
0 0 1 0
0 0 0 0
0 0 0 0
```

All the kinds of Tetris will be stored in **g_shape_Teris[][4][4]** array.

首先，我们需要用一个布尔数组来保存俄罗斯方块的形状，1表示当前的方块有数据，0表示没有。

···

所有形状的俄罗斯方块都被存放在 **g_shape_Teris[][4][4]** 数组中。

- Second,**draw Tetris into game area**.**VOID DrawTeris(HDC hdc,int nStartX,int nStartY,BOOL bTeris[4][4])** function implement it.When we need to call it,the statement shall like this:

```
DrawTeris(hdc, TerisX, TerisY, g_CurrTeris);
```
TerisX and TerisY is position of the Tetris when it is initialied(Most of the initialization is implemented in the **VOID InitGame()**).

g_CurrTeris[4][4] is a BOOL array,it stores the current Tetris.	

二，将俄罗斯方块绘至游戏区。DrawTeris函数实现此项功能。调用的语句类似于：
```
DrawTeris(hdc, TerisX, TerisY, g_CurrTeris);
```
TerisX, TerisY是俄罗斯方块的初始位置（大部分的初始化工作都在InitGame函数中进行）。
g_CurrTeris[4][4]是一个保存当前方块的布尔数组。

- Third,**rotate current Tetris**.**VOID RotateTeris(BOOL bTeris[4][4])** function  implement it.This function may be a little difficult to understand: Parameter **bTeris[4][4])** is the Tetris we want to rotate.But before this operation,we need an new array to store the changed array(**bNewTeris[4][4]**),then we need a **flag** named **bFlag** to judge that if the left or the top has no element,the state of Tetris (such as)below is considered to be invalid:

```
0 0 0 1         0 0 0 0 
0 0 0 1   or    0 0 0 0 
0 0 0 1         0 0 0 0
0 0 0 1         1 1 1 1
(left has nothing)  (top has nothing)
```
only situation like this is right:

```
1 1 1 1       1 0 0 0 
0 0 0 0   or  1 0 0 0 
0 0 0 0       1 0 0 0 
0 0 0 0       1 0 0 0 
```
bFlag equals to TRUE means right,otherwise means invalid.

三，旋转当前俄罗斯方块。RotateTeris函数实现了此项功能。这个函数稍微有些难理解，首先，参数bTeris[4][4]表示我们想要旋转的方块，在做旋转操作操作之前我们需要一个bNewTeris来保存旋转后的数组，然后我们需要一个呗命名为bFlag的标志来判断数组的第一行或第一列是否有数据，以下情况被认为无效：

...

旋转后像这样的才是对的：
 
 ...


bFlag等于TRUE时，说明这种旋转是正确的，否则就是无效的。


First,I used **GetWindowRect(hWnd, &rect);** function to get the size of the Window I need,it's a system function,


**The document below is created by VS2013 while creating the project**

**以下部分系创建项目时VS2013自动生成**

应用程序向导已为您创建了此 Teris 应用程序。

本文件概要介绍组成 Teris 应用程序的每个文件的内容。


Teris.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

Teris.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。它包含有关项目文件与筛选器之间的关联信息。在 IDE 中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。例如，“.cpp”文件与“源文件”筛选器关联。

Teris.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
应用程序向导创建了下列资源：

Teris.rc
    这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。

Resource.h
    这是标准头文件，可用于定义新的资源 ID。Microsoft Visual C++ 将读取并更新此文件。

Teris.ico
    这是用作应用程序图标 (32x32) 的图标文件。此图标包括在主资源文件 Teris.rc 中。

small.ico
    这是一个图标文件，其中包含应用程序的图标的较小版本 (16x16)。此图标包括在主资源文件 Teris.rc 中。

/////////////////////////////////////////////////////////////////////////////
其他标准文件:

StdAfx.h, StdAfx.cpp
    这些文件用于生成名为 Teris.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释:

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
