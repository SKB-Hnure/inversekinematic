#ifndef THREE_ANGLES_H_INCLUDED
#define THREE_ANGLES_H_INCLUDED


double* angles(double L1, double L2, double L3, double x, double y, double dZ)
/*
   L1, L2, L3 - длины звеньев, где L1 поворачивается в горизонтальной плоскости, L2, L3 - в вертикальной
   x,y - координаты захвата манипулятора
   dZ - высота
   возвращает массив из трех углов в градусах
   [0] - угол между осью Х и L1, c y>0
   [1] - угол, ограниченный снизу прямой, параллельной плоскости XY, а сверху звеном L2
   [2] - угол, ограниченный сверху прямой, продолжающей звено L2, в снизу звеном L3
   */
{ double *Q = new double[3];
Q[0]=atan(y/x);
  y2=sqrt(pow(x,2)+pow(y,2))-L1;
 B=sqrt(dZ*dZ+pow(y2,2));
 if (y2>0)
 Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-atan(dZ/y2);
 else  {
     if (y2<0)  Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-(Pi-abs(atan(dZ/y2)));
     if (y2==0) Q[1]=Pi/2-asin((L2*L2-L3*L3+B*B)/(2*B*L2))-Pi/2;
 }
 Q[2]=Pi/2+asin((L2*L2+L3*L3-B*B)/(2*L2*L3));
 for (int i=0;i<3;i++)
 {
     Q[i]=180*Q[i]/Pi;
 }
    return Q;
}


#endif // THREE_ANGLES_H_INCLUDED
