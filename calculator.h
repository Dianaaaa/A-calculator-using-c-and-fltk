#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Text_Display.H>
#include <FL/fl_draw.H>
#include <FL/fl_message.H>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include "cal_head.h"
using namespace std;

/********函数画线类**********/
class plot_func : public Fl_Widget{
public:
    plot_func(int o1, int o2, int w, int h, const char* l=0)
        :Fl_Widget(o1,o2,w,h,l), 
        x1(-10), x2(10), y1(-10), 
        y2(10),function(""),on_flag(0),color(0){}

    

    void set_scale(double n1, double n2, double m1, double m2){
        x1 = n1;
        x2 = n2;
        y1 = m1;
        y2 = m2;   
    }
    void set_function(string a) {function = a + ";";}
    void set_on_flag(bool a) {on_flag = a;}
    void draw();
    void set_color(){color = 0;}
    void change_color(){color++;
                        color %= 7;}
    void get_info(double x1, double x2, double y1, double y2,
        double& oy, double & ox, double& begin_x,double& end_x,
        double& unit_x, double& begin_y, double& end_y,
        double& unit_y,double& interval_x,double& interval_y);
private:
    double x1;//用户所给x和y的范围，初始为（-10，10）。
    double x2;
    double y1;
    double y2;
    string function;
    bool on_flag; //标记，当plot的时候，on_flag为false,将一切清空重新画；当hold on的时候，将在原来的基础上添加一个函数图像。
    int color;    
};

//根据用户所给的范围，得到画函数需要数据（原点（ox、oy）、坐标轴mark的开始大小和结束大小（begin_x,end_x,begin_y,end_y)，
//单位（unit_x,unit_y)、mark的间隔（interval_x,interval_y)
void plot_func::get_info(double x1, double x2, double y1, double y2,
        double& oy, double & ox, double& begin_x,double& end_x,
        double& unit_x, double& begin_y, double& end_y,
        double& unit_y,double& interval_x,double& interval_y){
    if (y1 <= 0 && y2 >= 0) {
        oy = (y2 - 0) / (y2 - y1) * 540 + 20;
        fl_line(20,oy,20+540,oy);
    }
    
    if (x1 <= 0 && x2 >= 0) {
        ox = (0 - x1) / (x2 - x1) * 540 + 20;
        fl_line(ox,20,ox,20+540);
    }
    if (0 < (x2 - x1) && (x2 - x1) <= 2) {
        begin_x = ceil(10 * x1) / 10.0;
        end_x = floor(10 * x2) / 10.0;
        unit_x = 0.1;
        interval_x = 0.1 / (x2-x1) * 540;
    }
    else if (2 < (x2 - x1) && (x2 - x1) <= 20) {
        begin_x = ceil(x1);
        end_x = floor(x2);
        unit_x = 1;
        interval_x = 1 / (x2-x1) * 540;
    }
    else if (20 <(x2 - x1) && (x2 - x1) <= 100) {
        begin_x = ceil(0.2 * x1) / 0.2;
        end_x = floor(0.2 * x2) / 0.2;
        unit_x = 5;
        interval_x = 5 / (x2-x1) * 540;
    }
    else if (100 < (x2 - x1) && (x2 - x1) <= 200) {
        begin_x = ceil(0.1 * x1) / 0.1;
        end_x = floor(0.1 * x2) / 0.1;
        unit_x = 10;
        interval_x = 10 / (x2-x1) * 540;
    }
    if (0 < (y2 - y1) && (y2 - y1) <= 2 ){
        begin_y = ceil(10 * y1) / 10.0;
        end_y = floor(10 * y2) / 10.0;
        unit_y = 0.1;
        interval_y = 0.1 / (y2 -y1) * 540; 
    }
    else if (2 < (y2 - y1) && (y2 - y1) <= 20) {
        begin_y = ceil(y1);
        end_y = floor(y2);
        unit_y = 1;
        interval_y = 1 / (y2-y1) * 540;
    }
    else if (20 < (y2 - y1) && (y2 - y1) <= 100) {
        begin_y = ceil(0.2 * y1) / 0.2;
        end_y = floor(0.2 * y2) / 0.2;
        unit_y = 5;
        interval_y = 5 / (y2-y1) * 540;
    }
    else if (100 < (y2 - y1) && (y2 - y1) <= 200) {
        begin_y = ceil(0.1 * y1) / 0.1;
        end_y = floor(0.1 * y2) / 0.1;
        unit_y = 10;
        interval_y = 10 / (y2-y1) * 540;
    }
    }

void plot_func::draw(){
    if (x2 - x1 < 0.2 || x2 - x1 > 200 || y2 - y1 < 0.2 || y2 - y1 > 200) {
        fl_message("Invalid scale!");
        return;
    } 
    double mark_width = 5;
    double oy = -1;
    double ox = -1;
    double begin_x = 0; 
    double end_x;
    double unit_x = 1;
    double begin_y = 0;
    double end_y;
    double unit_y = 1;
    double interval_x;
    double interval_y;
    double x_spot;
    double y_spot;
    if(!on_flag) {
        fl_rectf(15,15,550,550,FL_WHITE); //白色画布
    }
    fl_color(FL_BLACK);
    fl_begin_line();
    get_info(x1, x2, y1, y2,oy, ox, begin_x, end_x,unit_x, begin_y, end_y,
            unit_y, interval_x, interval_y);
    x_spot = (begin_x - x1) / (x2 - x1) * 540 + 20;
    y_spot = 560 - (begin_y - y1) / (y2 - y1) * 540;
    string dot = double_to_string(x1);
	/****画坐标轴，并在坐标轴上标出数据点******/
    if (oy > 0)
        fl_draw(dot.c_str(),dot.length(),20,oy + 15);
    else {
        dot = double_to_string(x1);
        fl_draw(dot.c_str(),dot.length(),20,560 - 5);
    }
        dot = double_to_string(y1);
    if (ox > 0)
        fl_draw(dot.c_str(),dot.length(),ox + 10,560);
    else {
        dot = double_to_string(y1);
        fl_draw(dot.c_str(),dot.length(),560 - 20,560-30);
    }
    for (double i = begin_x; i <= end_x; i+=unit_x) {
        fl_line(x_spot, oy-mark_width, x_spot, oy+mark_width);
        if (i != begin_x && i != end_x && (i == begin_x + unit_x || int(ceil(i /unit_x))%5==0)) {
            dot = double_to_string(ceil(i /unit_x)* unit_x);
            if (dot == "-0") dot = "0";
            if (oy > 0)
                fl_draw(dot.c_str(),dot.length(),x_spot-5,oy+15);
        }
        x_spot += interval_x;
    }
    if (oy > 0) {
        dot = double_to_string(end_x);
        fl_draw(dot.c_str(),dot.length(),x_spot - interval_x-12-dot.length(),oy + 15);
    }
    else {
        dot = double_to_string(x2);
        fl_draw(dot.c_str(),dot.length(),560 - 20,560 - 5);
    }
    for (double i = begin_y; i <= end_y; i+=unit_y) {
        fl_line(ox-mark_width, y_spot, ox+mark_width, y_spot);
        if ((i != begin_y) && ((ceil(i /unit_y)* unit_y) != end_y) && (int(ceil(i /unit_y))!=0)
            && (i == begin_y + unit_y || int(ceil(i /unit_y))%5==0)) {
            dot = double_to_string(ceil(i /unit_y)* unit_y);
            if (ox > 0)
                fl_draw(dot.c_str(),dot.length(),ox + 15,y_spot + 5);
        }
        y_spot -= interval_y;
    }
    if (ox > 0) {
        dot = double_to_string(end_y);
        fl_draw(dot.c_str(),dot.length(),ox + 10,y_spot + interval_y + 10);
    }
    else {
        dot = double_to_string(y2);
        fl_draw(dot.c_str(),dot.length(),560-10,25);
    }
	
	/******画函数**********/
    fl_color(color);
    if (function != "" && function != ";") {
        double gap = (x2 - x1) / 1000;
        double p1;
        double p2;
        double p3;
        double p4;
        if (x1 == 0) {var = x1 + 0.0001;}
        var = x1;
        p1 = (begin_x - x1) / (x2 - x1) * 540 + 20;
        p3 = p1 + (gap / (x2 - x1) * 540);
        try{
			ss << function;            
			Token t = ts.get();
			while(t.kind == print)
				t = ts.get();
			ts.putback(t);
			double py1 = expression();
			double py2;
			for (double i = 0; i < 1000; i++) {
				var += gap;
				ss << function;
            
				Token t = ts.get();
				while(t.kind == print)
					t = ts.get();
				ts.putback(t);

				py2 = expression();
				if (y1 <= py1 && py1 <= y2 && y1 <= py2 && py2 <= y2){
					p2 = (y2 - py1) / (y2 - y1) * 540 + 20;
					p4 = (y2 - py2) / (y2 - y1) * 540 + 20;
					fl_line(p1,p2,p3,p4);   
				}
				if ( i == 750 ) {
                    string f = "y = " + function.substr(0,function.length()-1);
                    if (p2 < 550 && p2 > 30)
                        fl_draw(f.c_str(),f.length(),540 * 0.75,p2);
                    else if(p2 >= 550) fl_draw(f.c_str(),f.length(),540 * 0.75,550);
                    else fl_draw(f.c_str(),f.length(),540 * 0.75,30);
                }
				py1 = py2;
				p1 = p3;
				p3 +=  (gap / (x2 - x1) * 540);
			}
        }
        catch(exception& e) {
            const char* message = e.what();
            fl_message(message);
            return;
        }
    }
/****************/
    fl_end_line();
    redraw();
}

class plot_window : public Fl_Window{
public:
    plot_window(int x, int y, int w, int h, const char* title);
private:

    Fl_Input* func;
    Fl_Input* set_x1;
    Fl_Input* set_x2;
    Fl_Input* set_y1;
    Fl_Input* set_y2;
    Fl_Button* plot_but;
    Fl_Button* set_scale;
    Fl_Button* hold_on;
    Fl_Button* quit;
    plot_func* plot;
    Fl_Text_Buffer *buff;
    Fl_Text_Display *disp;
 
    void set(){	//修改x、y范围
        double x1 = string_to_double(set_x1->value());
        double x2 = string_to_double(set_x2->value());
        double y1 = string_to_double(set_y1->value());
        double y2 = string_to_double(set_y2->value());
        plot->set_scale(x1,x2,y1,y2);
        plot->set_on_flag(false);
        plot->draw();
    }
    static void set_cb(Fl_Widget *v, void* data){
        plot_window* win = (plot_window*)data;
        win->set();
    }

    void plot_cb2() {	//画单独一个函数
        plot->set_on_flag(false);
        plot->set_function(func->value());
        plot->set_color();
        plot->draw();
    }

    static void plot_cb(Fl_Widget *v, void* data) {
        plot_window* win = (plot_window*)data;
        win->plot_cb2();
    }

    void quit_cb2() {hide();}

    static void quit_cb(Fl_Widget *v, void* data) {
        plot_window* win = (plot_window*)data;
        win->quit_cb2();
    }

    void hold_cb2() {	//连着画多个函数
        plot->set_on_flag(true);
        plot->set_function(func->value());
        plot->change_color();
        plot->draw();
    }

    static void hold_cb(Fl_Widget *v, void* data) {
        plot_window* win = (plot_window*)data;
        win->hold_cb2();
    }
    

};

plot_window::plot_window(int x, int y, int w, int h, const char* title)
    :Fl_Window(x,y,w,h,title) {
        plot = new plot_func(15,15,550,550);
        func = new Fl_Input(40,620-50, 150,20,"y = ");
        plot_but = new Fl_Button(200,620-50,40,20,"draw");
        plot_but->callback(plot_cb,this);
        hold_on = new Fl_Button(250,620-50,60,20,"hold on");
        hold_on->callback(hold_cb,this);
        set_x1 = new Fl_Input(40,620-20,30,20,"x: ");
        set_x1->value("-10");
        set_x2 = new Fl_Input(90,620-20,30,20, " ");
        set_x2->value("10");
        set_y1 = new Fl_Input(150,620-20,30,20,"y: ");
        set_y1->value("-10");
        set_y2 = new Fl_Input(200,620-20,30,20, " ");
        set_y2->value("10");
        set_scale = new Fl_Button(250,620-20,70,20,"set scale"); 
        set_scale->callback(set_cb,this);
        quit = new Fl_Button(580-60,630-40,40,30,"quit");
        quit->callback(quit_cb,this);
        end();
        
    }

/********帮助信息的窗口类*******/
class help_window : public Fl_Window{
public:
    help_window(int x, int y, int w, int h, const char* title);
     void set_text(string& a) {
        buff->text(a.c_str());
    }
private:    
    Fl_Text_Buffer *buff;
    Fl_Text_Display *disp;
    Fl_Button *but;

    static void quit_cb(Fl_Widget *o, void *v) {
        help_window* help = (help_window*)v;
        help->quit();
    }

    void quit() {hide();}

   

    
};

help_window::help_window(int x, int y,int w, int h, const char* title)
    :Fl_Window(x,y,w,h,title) {
        buff = new Fl_Text_Buffer();
        but = new Fl_Button(600-50,300-30,30,20,"Quit");
        disp = new Fl_Text_Display(20, 20, 600-40, 300-60, "About");

        but->callback(quit_cb,this);
        disp->buffer(buff);
        end();
    }

help_window* help_win;
plot_window* plot_win;

class Cal_window : public Fl_Window{
public:
    Cal_window(int x, int y,int w, int h, const char* title);
private:
    Fl_Menu_Bar *menu;
    Fl_Input* in; 
    Fl_Output* out;
                 
    bool clear_flag;

    static void button_cb(Fl_Widget *w, void *data);
    void button_input(Fl_Widget *w);

    void clear();

    void quit() {hide();}
    static void quit_cb(Fl_Widget *w, void *data);

    void help();
    static void help_cb(Fl_Widget* w, void* data);
    
    void calculate();
    string get_result();
    static void calculate_cb(Fl_Widget* o,void *v);

    void about();
    static void about_cb(Fl_Widget* w, void* data);

    void plot_cb2();
    static void plot_cb(Fl_Widget* w, void* data);
    
};

Cal_window::Cal_window(int x, int y,int w, int h, const char* title)
    :Fl_Window(x,y,w,h,title) {
        clear_flag = false;

        menu = new Fl_Menu_Bar(0,0,w,25);
        menu->add("File/Quit", FL_CTRL+'q', quit_cb);
        menu->add("About/Help", FL_CTRL+'h', help_cb);
        menu->add("About/Author",0,about_cb);
        menu->color(fl_rgb_color(225,230,250));

        const int bsize = 50;

        in = new Fl_Input(x+10,y+35,w-60,30);
        out = new Fl_Output(x+50, y+70, w-100, 30);

        Fl_Button* b;
        int colstart = x + 10,
            col = colstart,
            row = out->y()+out->h()+30;
        b = new Fl_Button(col,row,bsize,bsize,  "CE");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(55, 93, 129));
        b = new Fl_Button(col,row,bsize,bsize,  "C");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(55, 93, 129));
        b = new Fl_Button(col,row,bsize,bsize,  "ln");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "plot"); b->callback(plot_cb, (void*)this); b->labelsize(10);b->color(fl_rgb_color(55, 93, 129));
                                                                                             col=colstart; row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  "^");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "_/");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "!");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "\%"); b->callback(button_cb, (void*)this); b->labelsize(10);b->color(fl_rgb_color(171,200,226));
                                                                                             col=colstart; row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  "ANS");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "(");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  ")");   b->callback(button_cb, (void*)this); col+=b->w();b->color(fl_rgb_color(171,200,226));
        b = new Fl_Button(col,row,bsize,bsize,  "/"); b->callback(button_cb, (void*)this); b->labelsize(10);b->color(fl_rgb_color(171,200,226));
                                                                                             col=colstart; row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  "7");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "8");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "9");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "x"); b->callback(button_cb, (void*)this); b->labelsize(10);b->color(fl_rgb_color(171,200,226));
                                                                                             col=colstart; row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  "4");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "5");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "6");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "-");   b->callback(button_cb, (void*)this); col=colstart; b->color(fl_rgb_color(171,200,226));
                                                                                                        row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  "1");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "2");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "3");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "+");   b->callback(button_cb, (void*)this); col=colstart; b->color(fl_rgb_color(171,200,226));
                                                                                                        row+=b->h();
        b = new Fl_Button(col,row,bsize,bsize,  ".");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize,bsize,  "0");   b->callback(button_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(225,230,250));
        b = new Fl_Button(col,row,bsize*2,bsize,"=");   b->callback(calculate_cb, (void*)this); col+=b->w(); b->color(fl_rgb_color(55, 93, 129));
        end();
}

void Cal_window::quit_cb(Fl_Widget *w, void *data) {
    Cal_window* cal = (Cal_window*)data;
    cal->quit();
}
/***********弹出帮助信息窗口**********/
void Cal_window::help_cb(Fl_Widget *w, void *data) {
    Cal_window* cal = (Cal_window*)data;
    cal->help();
}

void Cal_window::help(){
    string a = "\tThis calculator provide the basic calculations we use nowadays.\n\
                Please be careful about the rules when you use this calculator:\n\
                1. + - : puted between two expression, or before one expression as a sign.\n\
                2. x / : used as we usually do.\n\
                3. \% : be careful to put it between two integers.\n\
                4. ! : put it behind a positive expression. It is not available to negative numbers.\n\
                5. _/ : this operator is sqrt(). Please be alerted that the expression sqrted should be no less than 0.\n\
                6. ^ : the power operator. Be aware of that the left number should be positive when the right number is a float.\n\
                7. 'plot' : click this button, a widget for ploting function will pop up.\n\
                \t\n\
				8. ln : be careful that function ln() is defined for positive numbers.\n\
                As for the plot window, please make sure that the scale is not less than 0.2, \n\
                or larger than 200. And it should be coordinate with the definition domain of the\n\
                function.\n\
                For example: if you want to draw y=_/x, you need to set the scale of x to be larger than 0,\n\
                because the _/x is only defined for positive numbers.\n\
				Please don't use the key letters (l, n, e, y) as variables.\n\
                \t\n\
				Just explore it and have fun!";
    help_win->set_text(a);
    help_win->show();
}

/************弹出作者信息窗口*************/
void Cal_window::about_cb(Fl_Widget *w, void *data) {
    Cal_window* cal = (Cal_window*)data;
    cal->about();
}

void Cal_window::about(){
    string a = "\tAuthor: Xingyue Qian\n\
                Date: 2018.04\n\
                Contact me: qianxingyueyx6@sjtu.edu.cn";
    help_win->set_text(a);
    help_win->show();
}
/***********弹出画函数信息窗口***********/
void Cal_window::plot_cb(Fl_Widget *w, void *data) {
    Cal_window* cal = (Cal_window*)data;
    cal->plot_cb2();
}

void Cal_window::plot_cb2() {
    plot_win->show();
}
/***********通过按键输入算式和清除操作***************/
void Cal_window::button_cb(Fl_Widget* w, void* data) {
    Cal_window* cal = (Cal_window*)data;
    cal->button_input(w);
}

void Cal_window::button_input(Fl_Widget* w) {
    Fl_Button* b = (Fl_Button*)w;
    if(clear_flag) {
        in->value("");
        clear_flag = false;
    }
    if ( strcmp(b->label(),"C") == 0 ) {                  
        if (in->mark() != in->position()) in->cut();
        else in->cut(-1);
    }
    else if ( strcmp(b->label(),"CE") == 0 ) {                  
        clear();
    }
    else if ( strcmp(b->label(),"plot") == 0 ) {                  
        
    }
    else if ( strcmp(b->label(),"ANS") == 0 ) {                  
        in->replace(in->position(), in->mark(), b->label(), 3);
    }
    else if ( strcmp(b->label(),"x") == 0 ) {    
        string ch = "*";              
        in->replace(in->position(), in->mark(), ch.c_str(), 1);
    }
    else if ( strcmp(b->label(),"ln") == 0 ) {                 
        in->replace(in->position(), in->mark(), b->label(), 2);
    }
    else if ( strcmp(b->label(),"_/") == 0 ) {                  
        in->replace(in->position(), in->mark(), b->label(), 2);
    }
    else {                                                 
        in->replace(in->position(), in->mark(), b->label(), 1);
    }
}

void Cal_window::clear() {
    in->value("");
    out->value("");
}

void Cal_window::calculate_cb(Fl_Widget* o,void *v){
    Cal_window* cal = (Cal_window*)v;
    cal->calculate();
}

/*********得到并输出计算结果，包括错误信息**********/
string Cal_window::get_result() {
    try{
        ts.clear_buffer();
        ss.str("");
        const char* conch = in->value();
        string result;
        string str(conch);
        str += ';';
        ss << str;

        Token t = ts.get();
        while(t.kind == print)
            t = ts.get();
        ts.putback(t);

        ANS = expression();
        result = double_to_string(ANS);
        return result;
    }
    catch(exception& e){
        string message = e.what();
        return message;
    }
}

void Cal_window::calculate() {
        string output = get_result();
        out->value(output.c_str());
        clear_flag = true;
    
}

