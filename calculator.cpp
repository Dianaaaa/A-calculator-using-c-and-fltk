#include "calculator.h"
#include <iostream>
using namespace std;


int main(int argc, char **argv){
	//画函数的窗口和信息窗口需要在main（）里面构造
    help_win = new help_window(270,10,600,300,"Help"); 
    help_win->resizable(help_win);
    plot_win = new plot_window(400,10,580,630,"plot function");
    plot_win->resizable(plot_win);
	//--------------------------------------------
    Cal_window *window = new Cal_window(20, 20, 260, 535, "Calculator");    
    window->color(fl_rgb_color(196, 215, 237));

    window->show(argc,argv);
    return Fl::run();
}

