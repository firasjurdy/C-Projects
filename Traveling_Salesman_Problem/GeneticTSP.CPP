#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>
int maxnodes, position;
int **Graph, *visited;
char *mark;

int * GeneticAlgorithm ( int ** );
void Display( int * );
void FillAtRandom ( int * );
int FitnessFunction( int * );
int RandomSelection ( int **, int );
int * Reproduce ( int **, int **, int, int, int );
int CheckMax( int **, int );
void Copy( int *, int * );

int main(){
   int i, j, result, u, v, w;
   int *path;
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
   if ( ( Graph = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL )
      printf( "Out of memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 Graph[ i ] = ( int * ) malloc ( i * sizeof( int ) );
	 if ( Graph[ i ] == NULL )
	    printf( "Out of memory\n" );
      }

   path = ( int * ) malloc ( maxnodes * sizeof ( int ) );
   visited = ( int * ) malloc ( maxnodes * sizeof ( int ) );
   mark = ( char * ) malloc ( maxnodes * sizeof ( char ) );
   for ( i = 0; i < maxnodes; i++ )
	   visited[ i ] = 0;

   for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	 Graph[ i ][ j ] = -1;

   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d%d", &u, &v, &w );
      Graph[ v ][ u ] = w;
      if ( feof( ptr ) )
	 break;
   }

   path = GeneticAlgorithm( Graph );
   printf( "\n\nthe way is: \n\n" );
   for ( i = 0; i < maxnodes; i++ )
	if ( i == maxnodes - 1 )
	    printf( "%d", path[ i ] );
	else
	    printf( "%d ---> ", path[ i ] );
   printf( "\n\n" );

  }
  getch();

   return 0;
}

int * GeneticAlgorithm ( int ** Graph ){
	int **population, **NewPopulation;
	int *child, value, *Fittest;
	int i, j, x, y, m, hold = 2000, Max = 0;
	srand( time( NULL ) );
	child = ( int * ) malloc ( maxnodes * sizeof( int ) );
	Fittest = ( int * ) malloc ( maxnodes * sizeof ( int ) );
	population = ( int ** ) malloc ( 10 * sizeof ( int * ) );
	for ( i = 0; i < 10; i++ )
		population[ i ] = ( int * ) malloc ( maxnodes * sizeof( int ) );
	NewPopulation = ( int ** ) malloc ( 10 * sizeof ( int * ) );
	for ( i = 0; i < 10; i++ )
		NewPopulation[ i ] = ( int * ) malloc ( maxnodes * sizeof( int ) );
	for ( i = 0; i < 10; i++ )
		FillAtRandom( population[ i ] );
	for ( i = 0; i < 10; i++ ){
		//Display( population[ i ] );
		//printf( " = %d\n\n", FitnessFunction( population[ i ] ) );
		value = FitnessFunction( population[ i ] );
		if ( value > Max )
			Max = value;
	}
	for ( m = 0; m < 30; m++ ){
		for ( i = 0; i < 10; i++ ){
			for ( j = 0; j < maxnodes; j++ )
				visited[ j ] = 0;
			if ( !CheckMax( population, Max ) ){
				printf( "\n\nLocal minimum is %d", hold );
				return Fittest;
			}
			x = RandomSelection ( population, Max );
			y = RandomSelection ( population, Max );
			//printf( "{ %d, %d } ---> ", x, y );
			child = Reproduce( population, NewPopulation, x, y, i );
			//Display( child );
			//printf( " = %d\n", FitnessFunction( child ) );
			value = FitnessFunction( child );
			if ( value < hold ) {
				hold = value;
				Copy( Fittest, child );
			}
		}
		for ( i = 0; i < 10; i++ )
			for ( j = 0; j < maxnodes; j++ )
				population[ i ][ j ] = NewPopulation[ i ][ j ];
		Max = 0;
		for ( i = 0; i < 10; i++ ){
		//Display( population[ i ] );
		//printf( " = %d\n\n", FitnessFunction( population[ i ] ) );
		value = FitnessFunction( population[ i ] );
		if ( value > Max )
			Max = value;
		}
	}
	printf( "\n\nLocal minimum is %d", hold );

	return Fittest;
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
	for ( i = 0; i < maxnodes; i++ )
		printf( "%d ", path[ i ] );
}

int FitnessFunction ( int *path ){
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

int RandomSelection( int ** population, int Max ){
	int i;
	i = rand() % 10;

	while ( FitnessFunction( population[ i ] ) == Max )
		i = rand() % 10;
	return i;
}

int * Reproduce( int **population, int **NewPopulation, int x, int y, int j ){
	int c, i, h;
	c = 1 + rand() % ( maxnodes - 1 );
	for ( i = 0; i < c; i++ ){
		NewPopulation[ j ][ i ] = population[ x ][ i ];
		visited[ population[ x ][ i ] ] = 1;
	}
	h = i;
	for ( i = c; i < maxnodes; i++ )
		if ( !visited[ population[ y ][ i ] ] ){
			NewPopulation[ j ][ h++ ] = population[ y ][ i ];
			visited[ population[ y ][ i ] ] = 1;
		}
	for ( i = 0; i < maxnodes; i++ )
		if ( !visited[ i ] )
			NewPopulation[ j ][ h++ ] = i;

	return NewPopulation[ j ];
}

int CheckMax( int ** population, int Max ){
	int i;
	for ( i = 0; i < 10; i++ )
		if ( FitnessFunction( population[ i ] ) != Max )
			return 1;
	return 0;
}

void Copy( int *a, int *b ){
	int i;
	for ( i = 0; i < maxnodes; i++ )
		a[ i ] = b[ i ];
}
