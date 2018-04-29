请用在Linux下安装fltk库后用terminator运行：
fltk-config --compile calculator.cpp


/****计算器说明****/
一、支持功能：
1、计算：
  	+
	-
	*
	/
	%
	！
	^(乘方)
	_/(根号)
	ANS

2、函数画图：
    支持：画单个函数（plot）
	  画多个函数（hold on）
	  设置x、y的范围
	  坐标轴自适应x、y的值，以画出舒适的图像
	  使用自然数e，（例：可画y = e^(-x)的图像）
	  除关键字母l、n、e、y，其余小写字母均可以作为函数参数被识别。
	  可使用1.中包含的运算来画函数，不过需要注意定义域（例：将x的范围设置为（0，10）即可画y = _/x的函数图像，同理也可以画y=lnx函数）

3、有help弹窗信息帮助用户更好地使用此计算器。


/****代码说明******/
calculator.h里面是图形化界面的代码
cal_head.h里面是计算逻辑相关的代码
calculator.cpp里面主要是初始化窗口（太懒了全揉作一堆，这可能是我写过的最丑的代码了……）

/****参考资料*****/
《c++程序设计：原理与实践》Bjarne Stroustrup著
Erco's FLTK Cheat Page：http://seriss.com/people/erco/fltk/