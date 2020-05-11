#include "string.h"
#include "iostream"
#include <stdlib.h>
#include "stack"
#include <cstdlib>
#include <cmath>
#include "TXlib.h"

using namespace std;

double calculatePolish(string readString, double graphic);
string convertToPolish(string readString);
void keyboard(int *graphicXZero, int *graphicYZero, int *singlePiece, bool *needHome, bool *needInput, int *onePieceX, int *onePieceY, bool *needHelp);
void calculateGraphicPosition(int *graphicXMax, int *graphicYMax, int *graphicXZero, int *graphicYZero, int windowWidth, int windowHeight, int singlePiece);
void drawCoordGird(int graphicXMax, int graphicYMax, int graphicXZero, int graphicYZero, int singlePiece, int onePieceX, int onePieceY);
void drawGraphic(int graphicXMax, int graphicYMax, int graphicXZero, int graphicYZero, int singlePiece, string formula, int onePieceX, int onePieceY);
void drawHelp(int graphicXMax, int graphicYMax);
int myRandom(int maxNumber);
void drawTextFunc(string formula, int graphicXMax);


int main(){
    srand(time(NULL));
    int windowWidth = GetSystemMetrics(SM_CXSCREEN) - 250;  // Вычисление разрешения экрана по горизонтали
    int windowHeight = GetSystemMetrics(SM_CYSCREEN) - 250; // Вычисление разрешения экрана по вертикали


    txCreateWindow(windowWidth, windowHeight);
    int singlePiece = 100;
    int graphicXMax, graphicYMax, graphicXZero, graphicYZero, lastGraphicXZero, lastGraphicYZero, lastSinglePiece, lastOnePieceY, lastOnePieceX;
    int onePieceX = 2;
    int onePieceY = 2;
    bool needHelp = true;
    string lastPolishString;
    bool needHome, needInput;
    bool lastNeedHelp;
    string readString = "x^2";
    string polishString = convertToPolish(readString);
    char read[200];





    calculateGraphicPosition(&graphicXMax, &graphicYMax, &graphicXZero, &graphicYZero, windowWidth, windowHeight, singlePiece);
    lastGraphicXZero = graphicXZero;
    lastGraphicYZero = graphicYZero;
    lastSinglePiece = singlePiece;
    lastPolishString = polishString;
    lastOnePieceX = onePieceX;
    lastOnePieceY = onePieceY;
    lastNeedHelp = needHelp;

   while(true){
        needHome = false;
        needInput = false;
        keyboard(&graphicXZero, &graphicYZero, &singlePiece, &needHome, &needInput, &onePieceX, &onePieceY, &needHelp);
        if (needHome){
            calculateGraphicPosition(&graphicXMax, &graphicYMax, &graphicXZero, &graphicYZero, windowWidth, windowHeight, singlePiece);
        }
        if (needInput){
            txSleep(1);
            cout << "\r\r\rВведите формулу (Длина входных данных не больше 200 символов): ";
            cin.getline(read, 200);
            readString = read;
            polishString = convertToPolish(readString);

        }
        if (lastGraphicXZero != graphicXZero || lastGraphicYZero != graphicYZero || lastSinglePiece != singlePiece || lastPolishString != polishString || lastOnePieceX != onePieceX || lastOnePieceY != onePieceY || lastNeedHelp != needHelp){
            txSetFillColor(TX_WHITE);
            txClear();
            drawCoordGird(graphicXMax, graphicYMax, graphicXZero, graphicYZero, singlePiece, onePieceX, onePieceY);
            drawGraphic(graphicXMax, graphicYMax, graphicXZero, graphicYZero, singlePiece, polishString, onePieceX, onePieceY);
            if (needHelp){
                drawHelp(graphicXMax, graphicYMax);
            }
            drawTextFunc(readString, graphicXMax);
            txSleep(24);
        }
        else{
            txSleep(2);
        }
        lastGraphicXZero = graphicXZero;
        lastGraphicYZero = graphicYZero;
        lastSinglePiece = singlePiece;
        lastPolishString = polishString;
        lastOnePieceX = onePieceX;
        lastOnePieceY = onePieceY;
        lastNeedHelp = needHelp;
    }
    return 1;
}

void drawHelp(int graphicXMax, int graphicYMax){
    txSetColor(TX_RED, 3);
    txDrawText(30, 30, 300, 500, "Graphic Calculator by Pavel v0.1\n Движение по графику на стрелки\n + и - - изменение масштаба\n End - ввод функции\n A и S - изменение единичного отрезка по X\n Z и X - изменение единичного отрезка по Y\n Home - возврат значений отображения по умолчанию\n F5 - убрать/показать эту подсказку ");

}

void drawTextFunc(string formula, int graphicXMax){
    string bufferString = formula;
    bufferString = "Текущая функция: y(x)=" + bufferString;
    char buffer[255];
    const char *cstr = bufferString.c_str();
    strcpy(buffer, cstr);
    txSetColor(TX_RED, 3);
    txDrawText(graphicXMax-1000, 10, graphicXMax, 50, buffer);

}
void drawGraphic(int graphicXMax, int graphicYMax, int graphicXZero, int graphicYZero, int singlePiece, string formula, int onePieceX, int onePieceY){
    double y, oldY, x, finish;
    double oldX = 0;
    x = (abs((graphicXMax - graphicXZero))/singlePiece*-3)-5;
    finish = (graphicXMax - graphicXZero)/singlePiece;
    while (x <= finish){
        y = calculatePolish(formula, x)*-1;
        txSetColor(TX_RED, 5);

        if (oldX == 0){
                oldX = (x*singlePiece)+graphicXZero;
                oldY = (y*singlePiece)+graphicYZero;
            } else{
            if(y*oldY > (graphicYZero/2)*-1000){
                txLine(oldX,oldY, (x*onePieceX*singlePiece)+graphicXZero, (y*onePieceY*singlePiece)+graphicYZero);
            }
                oldX = (x*onePieceX*singlePiece)+graphicXZero;
                oldY = (y*onePieceY*singlePiece)+graphicYZero;
            }
        x = x + 0.015;
    }

}

int myRandom(int maxNumber){
    return rand()%maxNumber;
}

void keyboard(int *graphicXZero, int *graphicYZero, int *singlePiece, bool *needHome, bool *needInput, int *onePieceX, int *onePieceY, bool *needHelp){
    if(GetAsyncKeyState(VK_RIGHT)){
        *graphicXZero = *graphicXZero - 10;

    }
    if (GetAsyncKeyState(VK_LEFT)){
        *graphicXZero = *graphicXZero + 10;

    }
    if (GetAsyncKeyState(VK_UP)){
        *graphicYZero = *graphicYZero + 10;

    }
    if (GetAsyncKeyState(VK_DOWN)){
        *graphicYZero = *graphicYZero - 10;

    }
    if (GetAsyncKeyState(VK_HOME)){
        *needHome = true;
        *singlePiece = 100;
        *onePieceX = 1;
        *onePieceY = 1;
    }
    if (GetAsyncKeyState(VK_END)){
        *needInput = true;
    }
    if (GetAsyncKeyState(VK_OEM_PLUS) && *singlePiece < 200){
        *singlePiece = *singlePiece + 2;

    }
    if (GetAsyncKeyState(VK_OEM_MINUS) && *singlePiece > 3){
        *singlePiece = *singlePiece - 2;

    }
    if (GetAsyncKeyState(0x5A) && *onePieceX > 1){
        *onePieceX = *onePieceX - 1;

    }
    if (GetAsyncKeyState(0x58) && *onePieceX < 100){
        *onePieceX = *onePieceX + 1;

    }
    if (GetAsyncKeyState(0x41) && *onePieceX > 1){
        *onePieceY = *onePieceY + 1;


    }
    if (GetAsyncKeyState(0x53) && *onePieceX < 100){
        *onePieceY = *onePieceY - 1;

    }
    if (GetAsyncKeyState(VK_F5)){
        *needHelp = !*needHelp;

    }


}

void drawCoordGird(int graphicXMax, int graphicYMax, int graphicXZero, int graphicYZero, int singlePiece, int onePieceX, int onePieceY){
    // Рисуем границы графика
    txSetColor(TX_BLACK, 1);
    txRectangle(0, 0, graphicXMax, graphicYMax);

    txSetColor(TX_BLACK, 5);
    txLine(graphicXZero, 0, graphicXZero, graphicYMax);



    txSetColor(TX_BLACK, 5);
    txLine(0, graphicYZero, graphicXMax, graphicYZero);

    //Рисуем цифры

    char *str = new char[17];
    int i = 1;
    int out;

    for(int x = graphicXZero+singlePiece; x<graphicXMax; x +=singlePiece){
        out = i*onePieceX;
        itoa(out, str, 10);
        txTextOut(x, graphicYZero+5, str);
        i++;
    }
    i = -1;
    for(int x = graphicXZero-singlePiece; x>0; x -=singlePiece){
        out = i*onePieceX;
        itoa(out, str, 10);
        txTextOut(x, graphicYZero+5, str);
        i--;
    }
    i = 1;
    for(int y = graphicYZero-singlePiece; y>0; y-=singlePiece){
        out = i*onePieceY;
        itoa(out, str, 10);
        txTextOut(graphicXZero+5, y, str);
        i++;
    }
    i = -1;
    for(int y = graphicYZero+singlePiece; y<graphicYMax; y+=singlePiece){
        out = i*onePieceY;
        itoa(out, str, 10);
        txTextOut(graphicXZero+5, y, str);
        i--;
    }


    txSetColor(TX_BLACK, 1);

    for(int x = graphicXZero; x<graphicXMax; x = x+singlePiece){
        txLine(x, 0, x, graphicYMax);
    }
    for(int x = graphicXZero; x>0; x = x-singlePiece){
        txLine(x, 0, x, graphicYMax);
    }
    for(int y = graphicYZero; y<graphicYMax; y = y+singlePiece){
        txLine(0, y, graphicXMax, y);
        }
    for(int y = graphicYZero; y>0; y = y-singlePiece){
        txLine(0, y, graphicXMax, y);
    }
}

void calculateGraphicPosition(int *graphicXMax, int *graphicYMax, int *graphicXZero, int *graphicYZero, int windowWidth, int windowHeight, int singlePiece){
    *graphicXMax = windowWidth;
    *graphicYMax = windowHeight;
    *graphicXZero = *graphicXMax/2;
    *graphicYZero = *graphicYMax/2;
}

double calculatePolish(string readString, double graphic)
{
	char ab[200];
	const char *cstr = readString.c_str();
	strcpy(ab, cstr);

	stack <double> out;
	double buffer;
	string bufferString;

	char *ex = strtok(ab, " "); // Разделение на лексемы (по пробелу)


	while (ex != NULL)
	{
		buffer = 0;
		bufferString = ex;
        if (bufferString == "x"){
            out.push(graphic);
            ex = strtok(NULL, " ");
            continue;
        }
        if(bufferString == "-x"){
            out.push(graphic*-1);
            ex = strtok(NULL, " ");
            continue;
        }

		if (isdigit(ex[0]) || isdigit(ex[1]))
		{
			out.push(atof(ex));
		}
		else
		{

            if (bufferString == "sin"){
                buffer = sin(out.top() / (M_PI/180));
                out.push(buffer);
            }
            if (bufferString == "cos"){
                buffer = cos(out.top() / (M_PI/180));
                out.push(buffer);
            }
            if (bufferString == "tg"){
                buffer = tan(out.top());
                out.push(buffer);
            }
            if (bufferString == "ctg"){
                buffer = cos((out.top()) / sin(out.top()));
                out.push(buffer);
            }
            if (bufferString == "log"){
                buffer = log10(abs(out.top()));
                out.push(buffer);
            }
            if (bufferString == "v"){
                if (out.top() < 0){
                    out.push(0);
                }
                buffer = sqrt(out.top());
                out.push(buffer);
            }
            if (bufferString == "abs"){
                buffer = abs(out.top());
                out.push(buffer);
            }
			switch (ex[0])
			{
			case '+':
				for (int i = 0; i < 2; i++)
				{
					if (buffer == 0)
					{
						buffer = out.top();
					}
					else
					{
						buffer = buffer + out.top();
					}
					out.pop();
				}
				out.push(buffer);
				break;
			case '-':
				for (int i = 0; i < 2; i++)
				{
					if (buffer == 0)
					{
						buffer = out.top() * -1;
					}
					else
					{
						buffer = buffer + out.top();
					}
					out.pop();
				}
				out.push(buffer);
				break;
			case '/':
				for (int i = 0; i < 2; i++)
				{
					if (buffer == 0)
					{
                        if (i == 1 && buffer == 0){
                            cout << "Деление на 0! Выражение получится не правильное!!" << endl;
                            break;
                        }
						buffer = out.top();
					}
					else
					{
						buffer = out.top() / buffer;
					}
					out.pop();
				}
				out.push(buffer);
				break;
			case '*':
				for (int i = 0; i < 2; i++)
				{
					if (buffer == 0)
					{
						buffer = out.top();
					}
					else
					{
						buffer = buffer * out.top();
					}
					out.pop();
				}
				out.push(buffer);
				break;

			case '^':
				for (int i = 0; i < 2; i++)
				{
					if (buffer == 0)
					{
						buffer = out.top();
					}
					else
					{
						buffer = pow(out.top(), buffer);
					}
					out.pop();
				}
				out.push(buffer);
				break;
			}
		}

		ex = strtok(NULL, " ");
	}
	return out.top();
}

string convertToPolish(string readString)
{
    char currentChar;
    string outStack = "";
    stack <string> Stack;
	for(int i = 0; i<readString.length(); i++){
        // Этап 1 - Чтение токена
        currentChar = readString[i];
        if (currentChar == 'x'){
            outStack = outStack + currentChar + ' ';
        }

        // Этап 2 - Токен является числом?
        if (isdigit(currentChar)){
            outStack += currentChar;
            continue;
        } else {
            if (currentChar == '.'){
                outStack = outStack + currentChar;
                continue;
			}
            if (isdigit(outStack.back()) && (currentChar != '-')){
                outStack = outStack + ' ';
            }
			if (currentChar == ' '){
				continue;
			}

			if (!(Stack.empty()) && (Stack.top() == "sin") && ((currentChar == 'i') || (currentChar == 'n'))){
                continue;
			}
            if (!(Stack.empty()) && (Stack.top() == "cos") && ((currentChar == 'o') || (currentChar == 's'))){
                continue;
			}
            if (!(Stack.empty()) && (Stack.top() == "tg") && (currentChar == 'g')){
                continue;
			}
            if (!(Stack.empty()) && (Stack.top() == "ctg") && ((currentChar == 't') || (currentChar == 'g'))){
                continue;
			}
            if (!(Stack.empty()) && (Stack.top() == "log") && ((currentChar == 'o') || (currentChar == 'g'))){
                continue;
			}
            if (!(Stack.empty()) && (Stack.top() == "abs") && ((currentChar == 'b') || (currentChar == 's'))){
                continue;
			}
        }

        // Этап 3 - Является ли токен оператором op1?
        switch(currentChar){
            // Пока на вершине стека оператор op2 - перекладываем эллементы стека в выходную строку
            case '+':
                while (!(Stack.empty()) && ((Stack.top() == "*") || (Stack.top() == "/") || (Stack.top() == "^") || (Stack.top() == "+") || (Stack.top() == "-"))){
                    outStack = outStack + Stack.top() + ' ';
                    Stack.pop();
                }
                Stack.push("+");
                break;
            case '-':

                // Проверка на унарный минус
                if (i == 0){
                        outStack = outStack + '-';
                        break;

                } else{
                    if(!(isdigit(readString[i-1]))){
                        outStack = outStack + '-';
                        break;
                    }
                }

                // ЕСЛИ БИНАРНЫЙ минус
                while (!(Stack.empty()) && ((Stack.top() == "*") || (Stack.top() == "/") || (Stack.top() == "^") || (Stack.top() == "+") || (Stack.top() == "-"))){
                    outStack = outStack + Stack.top() + ' ';
                    Stack.pop();
                }
                Stack.push("-");
                break;
            case '*':
                while (!(Stack.empty()) && ((Stack.top() == "*") || (Stack.top() == "/") || (Stack.top() == "^"))){
                    outStack = outStack + Stack.top() + ' ';
                    Stack.pop();
                }
                Stack.push("*");
                break;
            case '/':
                while (!(Stack.empty()) && ((Stack.top() == "*") || (Stack.top() == "/") || (Stack.top() == "^"))){
                    outStack = outStack + Stack.top() + ' ';
                    Stack.pop();
                }
                Stack.push("/");
                break;
            case '^':
                while (!(Stack.empty()) && (Stack.top() == "^")){
                    outStack = outStack + Stack.top() + ' ';
                    Stack.pop();
                }
                Stack.push("^");
                break;
            case '(':
                Stack.push("(");
                break;
            case ')':
				while (!(Stack.empty()) && (Stack.top() != "(") || ((Stack.top() == "sin") || (Stack.top() == "cos") || (Stack.top() == "tg") || (Stack.top() == "ctg") || (Stack.top() == "log"))){
					outStack = outStack + Stack.top() + ' ';
					Stack.pop();
				}
                Stack.pop();
                break;
            case 'v':
                Stack.push("v");
            case 's':
                if (readString[i+1] == 'i' && readString[i+2] == 'n'){
                    Stack.push("sin");
                }
                break;
            case 'c':
                if (readString[i+1] == 'o' && readString[i+2] == 's'){
                    Stack.push("cos");
                } else if (readString[i+1] == 't' && readString[i+2] == 'g'){
                    Stack.push("ctg");
                }
                break;
            case 't':
                if (readString[i+1] == 'g'){
                    Stack.push("tg");
                }
                break;
            case 'l':
                if (readString[i+1] == 'o' && readString[i+2] == 'g'){
                    Stack.push("log");
                }
                break;
            case 'a':
                if (readString[i+1] == 'b' && readString[i+2] == 's'){
                    Stack.push("abs");
                }
                break;

        }
	}

    outStack = outStack + ' ';
    while(!Stack.empty()){
        if (Stack.top() == "(" || Stack.top() == ")"){
            cout << "В выражении присутствует не закрытая скобка! Не надо так!";
            break;
        }
        outStack = outStack + Stack.top() + ' ';
        Stack.pop();
    }
    return outStack;
}
