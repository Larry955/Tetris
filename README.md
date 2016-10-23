-----update on Oct 6,2016

--------------------------------------------------------
 Preface·前言
--------------------------------------------------------
- Before introducing the project(based on Win32),a small mistake I have to say is that I used the wrong word "**Teris**" in my whole project,while the right word is "**Tetris**" ,I didn't notice the mistake until I used baidu search enginee to  looked for the English word of "俄罗斯方块".So hope you don't  get any uncomfortable feelings while viewing  code in  **Teris.cpp**  


--------------------------------------------------------
A Detailed Introduction  of Teris.cpp·Teris.cpp的详细介绍
-------------------------------------------------------- 
- Tetris is one of the most famous game in the world,I used to rottened in this game when I was a child.For me it's an exciting time when I implement it(though still have some little bugs that I can't fix ).Now, I'd like to share you the logical implement of this game.



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



···



- Second,**draw Tetris into game area**.**VOID DrawTeris(HDC hdc,int nStartX,int nStartY,BOOL bTeris[4][4])** function implement it.When we need to call it,the statement shall like this:

```
DrawTeris(hdc, TerisX, TerisY, g_CurrTeris);
```
TerisX and TerisY is position of the Tetris when it is initialied(Most of the initialization is implemented in the **VOID InitGame()**).

g_CurrTeris[4][4] is a BOOL array,it stores the current Tetris.	




- Third,**rotate current Tetris**.**VOID RotateTeris(BOOL bTeris[4][4])** function  implement it.This function may be a little difficult to understand: Parameter **bTeris[4][4])** is the Tetris we want to rotate.But before this operation,we need an new array to store the changed array(**bNewTeris[4][4]**),then we need a **flag** named **bFlag** to judge that if the left or the top has no element,the state of Tetris (such as)below is considered to be invalid:

```
0 0 0 1         0 0 0 0 
0 0 0 1   or    0 0 0 0 
0 0 0 1         0 0 0 0
0 0 0 1         1 1 1 1
(left has 0 only,so the right does)
```
only situation like this is right:

```
1 1 1 1       1 0 0 0 
0 0 0 0   or  1 0 0 0 
0 0 0 0       1 0 0 0 
0 0 0 0       1 0 0 0 
```
bFlag equals to TRUE means valid,otherwise means invalid.



-------------update on Oct,12,2016--------------------

if invalid,what we need to do is to update the position of the Tetris to make it valid,**++posX/++posY** does the work.

- Forth,make Tetris falls.In this case,we need to check if the Tetris touch the bound area(left,right and buttom) and handle these situations,function **BOOL CheckTeris(int nStartX, int nStartY, BOOL bTeris[4][4],BOOL bGame[GAME_X][GAME_Y])** does the work. 


- Fifth,Update the Tetris,before this step,we need to generate next Tetris,function **INT	GetRandNum(int iMin, int iMax)** generates a random number between 0 and TERIS_TYPE for us so that we can generate a random Tetris with random shape,then we call **VOID RefreshTeris(int nStartX, int nStartY, BOOL bTeris[4][4], BOOL bGame[GAME_X][GAME_Y])** to generate next Tetris,we also update score in this function.
