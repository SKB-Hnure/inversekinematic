#include <iostream>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "three_angles.h"
#include <unistd.h>  /* Объявления стандартных функций UNIX */
#include <fcntl.h>   /* Объявления управления файлами */
#include <errno.h>   /* Объявления кодов ошибок */
#include <termios.h> /* Объявления управления POSIX-терминалом */
using namespace std;
#define Pi 3.1415
double Qa, Qb, Qc, B, x, y, dZ, x02,y02,y2, L1,L2,L3;

double abs(double a)
{
    if (a>=0) return a;
    else return (-a);
}
string to_string(int val) {
 char buff[32];
 sprintf(buff,"%d",val);
 return string(buff);
}

int sends(char st[], int length)                                         //ОТКРЫТИЕ COM-ПОРТА
{
  int fd; /* Файловый дескриптор для порта */


  fd = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
       perror("open_port: Unable to open /dev/ttyUSB1 - ");
  }
  else
    fcntl(fd, F_SETFL, 0);   // Установить значение флагов описателя, заданное в arg
int n = write(fd, st, length);
if (n < 0)
  fputs("write() of 4 bytes failed!\n", stderr);
  close(fd);
  return (fd);
}
int main ()
{
    int n[3];
    for(int i=0;i<3;i++)
 {
     cout<<"L"<<i+1<<" Servo №: ";
     cin>>n[i];
 }
   cout<<"L1="; cin>>L1;
   cout<<"L2="; cin>>L2;
   cout<<"L3="; cin>>L3;
    	while (x!=-1)
{
	cout<<"X="; cin>>x;
	cout<<"Y="; cin>>y;
	cout<<"Z="; cin>>dZ;
	for (int i=0;i<3;i++)
	cout<<"Q["<<i+1<<"]="<<angles(L1,L2,L3,x,y,dZ)[i]<<" grad\n";
int Q[3];
Q[0]=(1856*angles(L1,L2,L3,x,y,dZ)[0])/180+1500;
Q[1]=(1856*(90-angles(L1,L2,L3,x,y,dZ)[1]))/180+544;
Q[2]=(1856*(180-angles(L1,L2,L3,x,y,dZ)[2]))/180+544;
struct Imp{
    int min;
    int max;};
Imp I[3];
 I[0].min=1300;
 I[0].max=1700;
 I[1].min=1180;
 I[1].max=2340;
 I[2].min=1650;
 I[2].max=2330;
 for (int i=0;i<3;i++)
 {
     if (Q[i]<I[i].min) Q[i]=I[i].min;
     if (Q[i]>I[i].max) Q[i]=I[i].max;
 }
string str, str1;
for (int i=0;i<3;i++)
{ str.append("#");
str.append(to_string(n[i]));
str.append("P");
str.append(to_string(Q[i]));
}
const int d=700;
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
char getch;
cin>>getch;
}
  return 0;
}
