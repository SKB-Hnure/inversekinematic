#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "three_angles.h"
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
#define Pi 3.1415
#define d3 20    //смещение оси стопы относительно основной
using namespace std;
struct Imp           //крайние положения серводвигателя
{
    int min;
    int max;
    };
int n[2][6]={{1,2,3,4,5,6},      //номера пинов для правой ноги
             {7,8,9,10,11,12}};    //для левой ноги
/*
     n[i][0] - поворот влево-вправо всей ноги
     n[i][1] - бедро вперед-назад
     n[i][2] - колено вперед-назад
     n[i][3] - стопа вперед-назад
     n[i][4] - бедро влево-вправо
     n[i][5] - стопа влево-вправо
*/
int P[2][6]={{1304,1543,550,2093, 1435, 1109},             //начальные положения
           {1500,1478,2500,1435,1283,1565}};
Imp I[2][6]={{         //крайние положения для правой ноги
 1104, 1504,       //поворот влево-вправо всей ноги
 1543, 2500,       //бедро вперед-назад
 550, 2240,        //колено вперед-назад
 700, 2250,        //стопа вперед-назад
 1226, 2326,       //бедро влево-вправо
 500, 1826         //стопа влево-вправо
},
{               //крайние положения для правой ноги
1300,1700,        //поворот влево-вправо всей ноги
 500,1800,        //бедро вперед-назад
 500, 2500,       //колено вперед-назад
 1217, 2500,      //стопа вперед-назад
 630, 2000,       //бедро влево-вправо
 950, 2400        //стопа влево-вправо
 }};
int dir[2][6]={      //направления от начального положения
{1,1,1,-1,1,1},
{1,-1,-1,1,-1,1}
};
string to_string(int val)
{
 char buff[32];
 sprintf(buff,"%d",val);
 return string(buff);
}
int sends(char st[], int length)                                         //ОТКРЫТИЕ COM-ПОРТА
{
  int fd; /* Файловый дескриптор для порта */
  fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
       perror("open_port: Unable to open /dev/ttyACM0 - ");
  }
  else
        fcntl(fd, F_SETFL, 0);   // Установить значение флагов описателя, заданное в arg
        tcflush(fd,TCIFLUSH);
int n = write(fd, st, length);
if (n < 0)
  fputs("write() of 4 bytes failed!\n", stderr);
  close(fd);
  return (fd);
}
int allow_val(int q, Imp a)        //находится ли угол q в диапазоне a
{
if ((q>=a.min)&&(q<=a.max))
     return q;
 else
     {
     if (q<a.min) return a.min;
     if (q>a.max) return a.max;
     }
}
int grad_to_microsec(int angle)
{
return 1856*angle/180;
}
double rad_to_grad(double angle)
{
int P=3.1415;
return 180*angle/P;
}
void leg_move(double x, double y, double dZ, double dY, double L[3], int P[6], Imp I[6], int n[6], int dir[6])
{
/*
x- координата х (вперед-назад)
y- координата y (влево-вправо)
dZ- высота ( для нулевого положения равна L[1]+L[2])
L[3] - длины ребер
P[6] - начальные положения
I[6] - крайние положения
n[6] - номера пинов
dir[6] - направления от начальных положений
*/
double q[6];  //углы в градусах
int Q[6];     //углы в импульсах
for (int i=0;i<3;i++)
	q[i]=angles(L[0],L[1],L[2],x,y,dZ)[i];
for (int i=0;i<3;i++)
	cout<<"q["<<i+1<<"]="<<q[i]<<" grad\n";
Q[0]=P[0]+dir[0]*grad_to_microsec(90-q[0]);         //поворот влево-вправо всей ноги
Q[1]=P[1]+dir[1]*grad_to_microsec(90+q[1]);         //бедро вперед-назад
Q[2]=P[2]+dir[2]*grad_to_microsec(q[2]);            //колено вперед-назад
bool b=true;
for (int i=1;i<3;i++)
     if (Q[i]!=allow_val(Q[i],I[i]))
      b=false;
 for (int i=0;i<3;i++)
     Q[i]=allow_val(Q[i],I[i]);
double g=rad_to_grad(atan((double) d3/(double) L[1]));
 if (b==true)
 {
 q[3]=q[2]-(90+q[1])+g;                             //угол стопы в градусах
 cout<<"q[4]="<<q[3]<<" grad\n";
 Q[3]=P[3]+dir[3]*grad_to_microsec(q[3]);           //стопа вперед-назад (из градусов)
 }
 else
 {g=grad_to_microsec(g);
 Q[3]=P[3]+dir[3]*(((Q[2]-P[2])*dir[2])-((Q[1]-P[1]))*dir[2]+g);}    //стопа вперед-назад (из микросекунд); здесь может быть ошибка!
Q[3]=allow_val(Q[3],I[3]);
q[4]=rad_to_grad(atan((double)dY/(double)dZ));
Q[4]=P[4]+dir[4]*grad_to_microsec(q[4]);                             //бедро влево-вправо
Q[4]=allow_val(Q[4],I[4]);
q[5]=q[4];
Q[5]=P[5]+dir[5]*grad_to_microsec(q[5]);                             //стопа влево-вправо
Q[5]=allow_val(Q[5],I[5]);
for (int i=4;i<6;i++)
	cout<<"q["<<i+1<<"]="<<q[i]<<" grad\n";
string str, str1;
for (int i=0;i<6;i++)
{
str.append("#");
str.append(to_string(n[i]));
str.append("P");
str.append(to_string(Q[i]));
}
const int d=50;
str.append("T");
str.append(to_string(d));
str.append("\n\r");
cout<<str;
char st[50];
int length=str.length();
for (int i=0;i<length;i++)
{
    st[i]=str[i];
}
sends(st,length);

}
int main ()
{
double x,y,dZ,dY;
double L[3];   //расстояния между серводвигателями
L[0]=0;
for (int i=1;i<3;i++)
   {
   cout<<"Длина L["<<i<<"]";       //длины колена и голени
   cin>>L[i];
   }
    while (x==x)
{
    cout<<"X="; cin>>x;
	cout<<"Y="; cin>>y;
	cout<<"Z="; cin>>dZ;
	cout<<"dY=";cin>>dY;
    leg_move(x,y,dZ,dY,L,P[1],I[1],n[1],dir[1]);
    cout<<endl;
   }
  return 0;
}
