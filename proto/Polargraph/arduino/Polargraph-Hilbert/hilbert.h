#ifndef HILBERT
#define HILBERT

//rotate/flip a quadrant appropriately
void rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
 
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}

//convert (x,y) to d
int xy2d (int n, int x, int y) {
    int rx, ry, s, d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, &x, &y, rx, ry);
    }
    return d;
}
 
//convert d to (x,y)
void d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}

void hilbert(int cell_size=10) {
  
  int n = 0;
  int s = 4096;
  int ox = 0;
  int oy = 0;
  int x = 0;
  int y = 0;
  
  while(1) {
   
    d2xy(s, n, &x, &y);
    
    int dx = x - ox;
    int dy = y - oy; 
   
    move({dx*cell_size,0});
    move({0,dy*cell_size});
    
    ox = x;
    oy = y;
    
    n++;
  }
}

#endif
