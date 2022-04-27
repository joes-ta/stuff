#include <stdio.h>

struct point {
	int x;
	int y;
	int ray[4];
};

int main( void ) {
	struct point there = { 23, 17, 1, 2, 3, 4 };
	struct point *that = &there;
	
	printf( "Before serialization:\n" );
	printf( "%d, %d, %d, %d\n", there.x, there.y, there.ray[0], there.ray[1] );
	printf( "%d, %d, %d, %d\n", that->x, that->y, that->ray[0], that->ray[1] );

	FILE *ser = fopen( "/com/joesta/GitHub/C/src/ser.dat", "w" );
	fwrite( that, 1, sizeof (struct point), ser );
	fclose( ser );
	
	struct point here;
	struct point *this = &here;
	
	FILE *deser = fopen( "/com/joesta/GitHub/C/src/ser.dat", "r" );
	fread( this, 1, sizeof (struct point), deser );
	fclose( deser );

	printf( "After de-serialization:\n" );
	printf( "%d, %d, %d, %d\n", here.x, here.y, here.ray[0], here.ray[1] );
	printf( "%d, %d, %d, %d\n", this->x, this->y, this->ray[0], this->ray[1] );
	
	return 0;
}