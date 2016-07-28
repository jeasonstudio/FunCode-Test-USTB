# FunCode-Test-USTB

[![Join the chat at https://gitter.im/jeasonstudio/FunCode-Test-USTB](https://badges.gitter.im/jeasonstudio/FunCode-Test-USTB.svg)](https://gitter.im/jeasonstudio/FunCode-Test-USTB?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
Coding test with FunCode Games in USTB 2016.7

**声明：所有试验内容都基于CodeBlocks，用VC++6.0的童鞋需要把源码和地图模板拿出来重新合成一下**


## **实验一:海底世界**
（可能有问题，Pull Request的时候电脑死机了，估计实验一会打不开？？Who Care？？！！）

[https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Seagame](https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Seagame)

>海底有五条鱼，其中四条来回随机游动，由玩家用键盘W A S D控制另外一条游动。
要求如下：
* 游戏初始界面如下图。
* 来回游动的四条鱼，从屏幕左边游进来，均已一个随机速度水平游动。从屏幕右侧游出去，很快又从屏幕右侧游出来，就这样来回游动。
* 玩家用WASD键控制一条鱼向上、左、下、右方向游动。松开按键时，玩家的鱼就停止游动。
* 玩家的鱼游到屏幕边界时，不能继续往前游。
 

## **实验二: 黄金矿工**

[https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Goldman](https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Goldman)

>玩家通过键盘的按键控制矿工抓取金块，将钩子碰触到的金块抓取过来。
要求如下：
* 游戏的初始界面如下图（一），单机键盘上的空格键进入游戏，进入后界面如图（二），金块的总数是20，大小位置是随机的。
* 在没有抓取状态下，钩子左右摆动，此时矿工的是静止的。当钩子摆动到一定角度，玩家可以单击键盘上的上下左右键中的下方向键控制矿工伸出长钩，抓取金子，此时矿工是向下摇动转轴。获取到金子往回拉后，矿工是不断转动转轴，知道金子拉动到钩子初始处，矿工恢复静止，钩子继续左右摇摆，直到玩家再次单击向下方向键。
* 伸出的钩子如果碰触到金子，则钩子和金子一起往回拉，回收的速度根据抓取到的金子的大小变化而变化，金子越大，回拉的速度越慢，反之亦然。如果钩子没有碰触到金子，而是碰触到左右和下的边界，则钩子保持原来的速度往回收。
* 抓取到的金子拉回到转轴处消失，此时金子数目减少一个。如果玩家将所有金子抓取完，游戏返回到如图（一）的初始界面。
* **我增加的功能: 实现记录分数,黄金的大小不同,分数不同**

## **实验三: 美女拼图游戏**

[https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Puzzle](https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Puzzle)

>玩家通过鼠标单击相邻位有空位的方块来移动方块，从而最终将一副散乱的图片拼成完整的图片。要求如下：
* 游戏的初始界面如图一，单击空格键进入游戏，进入游戏之后系统将完成的图片分成大小相同的15分并随机摆放成如图二。
* 启动游戏，单击空格键进入游戏。通过鼠标单击周围有空格的方块来移动方块，直到全图拼接成图二中右下角的图案样式，游戏结束，重新回到图一界面。
* 游戏的原理是定义一个4 * 4的方块矩阵(二维数组)，前15个的值按顺序从1-15依次递增，第16个留空为0。按照这个顺序排列的矩阵值代表游戏胜利。初始化的时候，将该16个矩阵值随机排布即得到本局关卡。为0的空位代表附近上下左右的4个方块可以移动过来
* 每次单击鼠标左键，方块移动一格。鼠标必须移动到方块范围内单击才能有效移动方块。

## **实验十：坦克大战**

[https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/Waroftank](https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/WarofTank)

>玩家坦克与敌方坦克在街道中进行巷战，玩家坦克被击中、玩家指挥部被击中或游戏时间到，一局游戏结束。
* 按下空格键后游戏开始，“空格开始”消失，载入地图，并把玩家坦克设置在指挥部左侧。
* 游戏时间到，比如30秒，玩家坦克被敌方坦克摧毁，或者玩家指挥部被摧毁，一局游戏结束，游戏回到初始界面，并显示上一局的分数。
* 游戏区域为下图中最内部的黑色区域，左上角坐标[-26, -22]，右下角坐标为[26, 22]。墙为正方形，边长为4，坦克也是正方形，比墙略小一点。
* 玩家用WASD键控制坦克上、下、左、右运行，按J键开炮。玩家坦克碰到墙就停下来，需要调转方向才能继续前进。玩家坦克开炮，一炮就能摧毁一块墙，或者一辆敌方坦克。玩家没摧毁一辆敌方坦克，加1分。
* 玩家指挥部被坦克或者炮弹（不管玩家还是敌方）碰上，都会被摧毁。
* 每隔几秒钟，比如3秒，就会产生一辆敌方坦克。敌方坦克每隔一段时间，比如1秒，就自动开炮。敌方坦克遇到墙就会停下来。停下来的坦克，前方的墙如果被摧毁了，又能继续前进。每隔几秒钟，比如2秒，敌方坦克就会顺时针变换一个方向前进。


## **实验附加：Flappy Frog**

[https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/FlappyFrog](https://github.com/jeasonstudio/FunCode-Test-USTB/tree/master/FlappyFrog)

>这是一款来源于经典的游戏————Flappy Bird，借鉴了Flappy Frog。涉及到敏感的政治问题，避免被和谐，暂不解释寓意。
* 呦呦。
* 盛唐死来意。
* Excited！
* 年轻人，不要总想搞个大新闻。

