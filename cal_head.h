#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

/*--------- 全局变量 ----------*/
const char print = ';';
const char number = '8';
const char variable = 'x';
double ANS = 0;
double var = 0;
double e = 2.7182818;
stringstream ss;
/*----------------------------*/

//-------------------------------------------------------
void error(const string& s)
{
	throw runtime_error(s);
}

string double_to_string(double num) {
    stringstream os;  
    if (os << num){  
        string str;
        getline(os,str);
        return str;
    }
    return "invalid conversion";
}
double string_to_double(string str) {
    stringstream os;  
    if (os << str){  
        double result;
        os >> result;
        return result;
    }
    return 0;
}
//---------------------------------------------------------

/*-------- 类的定义(Token,Token_stream) ---------*/
class Token{
public:
  char kind;
  double value;
  Token(char ch)
    :kind(ch),value(0){}
  Token(char ch, double val)
    :kind(ch),value(val){}
};

class Token_stream{
public:
  Token_stream();
  Token get();
  void putback(Token t);
  void clear_buffer();
  void ignore(char c);

private:
  bool full;
  Token buffer;
};

/*------------Token_stream类中函数的定义-------------------*/
Token_stream::Token_stream()
    :full(false),buffer(0){}

void Token_stream::putback(Token t)
{
    if(full)
        error("putback() into a full butter");
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if(full){
        full = false;
        return buffer;
    }

    char ch;
    ss >> ch;
   
    switch(ch){
        case print:
        case '(':case ')': 
        case '+': case '-': case '*': case '/':case '%':
        case '!': case '_': case '^': case 'l':case 'n':
            return Token(ch);
            case '.':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        {
            ss.putback(ch);
            double val;
            ss >> val;
            return Token(number, val);
        }
        case 'A':
        {
            ss.putback(ch);
            string ans;
            for(int i = 0; i < 3; i++) {
                ss >> ch;
                if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')){
                    ans += ch;
                }
                else{
                    ss.putback(ch);
                    break;
                }
            }
            if (ans == "ANS"){
                return Token(number, ANS);
            }
            else
                error("Bad Token");
        }
        case 'x':case 'a':case 'b':case 'c':case 'd':

        case 'f':case 'g':case 'h':case 'i':case'j':

        case 'k':case 'm':case 'o':case 'p':case 'q':

        case 'r':case 's':case 't':case 'u':case 'v':

        case 'w':case 'z':  {
            return Token(number,var);
        }
		case 'e': return Token(number,e);
        default:
            error("Bad token");
    }
}

void Token_stream::clear_buffer() {
    full = false;
}

void Token_stream::ignore(char c)
{
    if(full && c == buffer.kind){
        full = false;
        return;
    }
    full = false;

    char ch = 0;
    while(ss >> ch)
        if(ch == c)
            return;

}

/*-------- 声明 ---------*/
Token_stream ts;
double term();
double primary();

/*------- 函数定义（expression(),term(),primary()) -------*/

double expression()
{
    double left = term();
    Token t = ts.get();
    while(true){
        switch(t.kind){
            case '+':
                left += term();
                t = ts.get();
                break;
            case '-':
                left -= term();
                t = ts.get();
                break;
            default:
                ts.putback(t);
                return left;
        }
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();
    while(true){
        switch (t.kind){
            case '*':
                left *= primary();
                t = ts.get();
                break;
            case '/':
            {
                double d = primary();
                if (d == 0)
                    error("divide by zero");
                left /= d;
                t = ts.get();
                break;
            }
            case '%':
            {
                double d = primary();
                int i1 = int(left);
                if(i1 != left)
                    error("left-hand operand of % not int");
                int i2 = int(d);
                if(i2 != d)
                    error("right_hand operand of % not int");
                if(i2 == 0)
                    error("%:divide by zero");
                left = i1 % i2;
                t = ts.get();
                break;
            }
            case '!':
            {
                if (left < 0){
                    error("\'!\'error: negative number");
                }
                else if(left != int(left)){
                    error("\'!\'error: not integer");
                }
                else{
                    if(left == 0){
                        left = 1;
                        t = ts.get();
                        break;
                    }
                    else{
                        double result = 1;
                        for (int i = 1; i <= left; ++i){
                            result *= i;
                        }
                        left = result;
                        t = ts.get();
                        break;
                    }
                }
            }

            
            default:
                ts.putback(t);
                return left;
        }
    }
}

double primary()
{
    Token t = ts.get();
    switch (t.kind){
        case '(':
            {
                double d = expression();
                t = ts.get();
                if(t.kind != ')')
                {
                    ts.putback(t);
                    error("')'expected");
                }
                t = ts.get();
                if (t.kind == '^'){
                    double expo = primary();
                    if (d < 0 && expo != int(expo))
                        error("Invalide power.");
                    return pow(d, expo);
                }
                else {
                    ts.putback(t);
                    return d;
                }

            }
        case number:{
           double left = t.value;
           double result = 1;
            t = ts.get();
            if (t.kind == '!'){
                if(left != int(left)){
                    error("\'!\'error: not integer");
                }
                else if(left == 0)
                    return 1;
                else if(left < 0) error("\'!\' error: negative number.");
                for (int i=1 ; i <= left; i++)
                result *= i;
                return result;
            }
            else if (t.kind == '^'){
                double expo = primary();
                return pow(left, expo);
            }
            ts.putback(t);
            return left;
        }
        case '-':
            return -primary();
        case '+':
            return primary();
        case '_': {
            t = ts.get();
            if (t.kind == '/'){
                double right = primary();
                if(right < 0)
                    error("Sqrt error: negative number.");
                else
                    return sqrt(right);
            }
            else
                error("Bad token.");
        }
        case 'l': {
            t = ts.get();
            if (t.kind == 'n'){
                double right = primary();
                if(right < 0)
                    error("log error: negative number.");
                else
                    return log(right);
            }
            else
                error("Bad token.");
        }
        
        default:
            error("primary expected");
    }
}


