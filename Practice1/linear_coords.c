#include <stdio.h>
#include <stdlib.h>

/* Linear Equation:	y = mx + b
	where:	m = slope
			b = displacement
*/

int main(int argc, char const *argv[]){
	int x1,x2,y1,y2,i;
	float slope, displace, y;

	x1 = atoi(argv[1]);
	y1 = atoi(argv[2]);

	x2 = atoi(argv[3]);
	y2 = atoi(argv[4]);

	printf("Coords: (%d,%d) - (%d,%d)\n\n",x1,y1,x2,y2);

	slope =(float) (y2 - y1)/(x2 - x1);
	displace =(float)y1 - slope*x1;

	printf(" Slope: %.2f\n Displacement: %.2f\n", slope,displace);

	printf("\n x\t\ty = mx + b\ty\n");

	for(i=x1;i<=x2;i++){

		y = i*slope + displace;
		printf(" %d\t%.2f*%d + %.2f = %.2f\t%d\n",i,slope,i,displace,y,(int)y);
	}

	return 0;
}