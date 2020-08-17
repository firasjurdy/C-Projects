#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#define SIZE 50

int maxnodes;
int **Graph;
int position;
char *mark;

int *SimulatedAnnealing( double * );
void CopyArray( int *, int * );
void Display( int * );
void FillAtRandom( int * );
int COST( int * );
void RandomSuccessor( int *, int * );

int main(){
   int i, j, result, u, v, w;
   int *path;
   double schedule[ SIZE * 10 * 10 + 1 ];
   FILE *ptr;
   char filename[ 15 ];
   clrscr();
   printf( "Enter the filename that contains the Graph: " );
   scanf( "%s", filename );
   printf( "\nEnter the vertex number from which you would like to start: " );
   scanf( "%d", &position );

   if ( ( ptr = fopen( filename, "r" ) ) == NULL )
      printf( "Invalid File\n" );
   else {
   fscanf( ptr, "%d", &maxnodes );
   if ( ( Graph = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL ){
      printf( "Out of memory\n" );
      return 0;
   }
   for ( i = 1; i < maxnodes; i++ ){
      Graph[ i ] = ( int * ) malloc ( i * sizeof( int ) );
      if ( Graph[ i ] == NULL ){
	 printf( "Out of memory\n" );
	 return 0;
      }
   }

   if ( ( mark = ( char * ) malloc ( maxnodes * sizeof( char ) ) ) == NULL ){
	printf( "Out of memory" );
	return 0;
   }

   for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	 Graph[ i ][ j ] = -1;

   schedule[ 0 ] = SIZE;
   for ( i = 1; i < ( SIZE * 10 * 10 + 1 ); i++ )
	   schedule[ i ] = schedule[ i - 1 ] - 0.01;

   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d%d", &u, &v, &w );
      Graph[ v ][ u ] = w;
      if ( feof( ptr ) )
	 break;
   }

  path = SimulatedAnnealing( schedule );
  printf( "\nCost = %d", COST( path ) );
  printf( "\n\nThe path is: \n\n" );
  Display( path );

  }

  getch();

   return 0;
}

int *SimulatedAnnealing( double *schedule ){
	int *current, *next, i, E;
	float T;
	srand( time( NULL ) );
	current = ( int * ) malloc ( maxnodes * sizeof( int ) );
	next = ( int * ) malloc ( maxnodes * sizeof ( int ) );
	FillAtRandom( current );
	for ( i = 0; i < ( SIZE * 10 * 10 + 1 ); i++ ){
		T = schedule[ i ];
		if ( i == SIZE * 10 * 10 )
			return current;
		RandomSuccessor( current, next );
		E = COST( current ) - COST( next );
		if ( E > 0 )
			CopyArray( current, next );
		else if ( exp( E / T ) >= 0.5 )
			CopyArray( current, next );
	}
}

void CopyArray( int *a, int *b ){
	int i;
	for ( i = 0; i < maxnodes; i++ )
		a[ i ] = b[ i ];
}

void FillAtRandom( int *path ){
	int i, val;

	for ( i = 0; i < maxnodes; i++ )
		mark[ i ] = '0';
	mark[ position ] = '1';
	path[ 0 ] = position;
	for ( i = 1; i < maxnodes; i++ ){
		val = rand() % maxnodes;
		while ( mark[ val ] == '1' )
			val = rand() % maxnodes;
		path[ i ] = val;
		mark[ val ] = '1';
	}

}

void Display ( int *path ){
	int i;
	for ( i = 0; i < maxnodes; i++ ){
		if ( i == maxnodes - 1 )
		   printf( "%d", path[ i ] );
		else
		   printf( "%d ---> ", path[ i ] );
	}
}

int COST ( int *path ){
	int i, TripCost = 0;
	for ( i = 0; i < maxnodes - 1; i++ )
		if ( path[ i ] > path[ i + 1 ] )
			TripCost += Graph[ path[ i ] ][ path[ i + 1 ] ];
		else TripCost += Graph[ path[ i + 1 ] ][ path[ i ] ];
	if ( path[ i ] > position )
		TripCost += Graph[ path[ i ] ][ position ];
	else TripCost += Graph[ position ][ path[ i ] ];
	return TripCost;
}

void RandomSuccessor( int *path, int *newPath ){
	int i, j, hold;
	CopyArray( newPath, path );
	i = 1 + rand() % ( maxnodes - 1 );
	j = 1 + rand() % ( maxnodes - 1 );
	while ( i == j )
		j = 1 + rand() % ( maxnodes - 1 );
	hold = newPath[ j ];
	newPath[ j ] = newPath[ i ];
	newPath[ i ] = hold;
}

