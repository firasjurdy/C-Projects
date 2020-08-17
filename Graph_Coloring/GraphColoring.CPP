#include <stdio.h>
#include <stdlib.h>

char **Mask;
int *Color;
int maxnodes;
char **Graph;
int pos = 0;

int AllColored();
int Available( int, int );
void InitializeMask( int, int );
int GraphColoring( int );

int main(){
   int i, j, result, k, u, v;
   FILE *ptr;
   char filename[ 15 ];
  
   printf( "Enter the filename: " );
   scanf( "%s", filename );
   printf( "\nEnter the number of colors to color the Graph: " );
   scanf( "%d", &k );
   if ( ( ptr = fopen( filename, "r" ) ) == NULL )
      printf( "Invalid File\n" );
   else {
   printf( "\n\n\n" );
   fscanf( ptr, "%d", &maxnodes );
   if ( ( Graph = ( char ** ) malloc ( maxnodes * sizeof( char * ) ) ) == NULL )
      printf( "Out of memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 Graph[ i ] = ( char * ) malloc ( i * sizeof( char ) );
	 if ( Graph[ i ] == NULL )
	    printf( "Out of memory\n" );
      }
   if ( ( Mask = ( char ** ) malloc ( k * sizeof ( char * ) ) ) == NULL )
      printf( "No memory\n" );
   else
      for ( i = 0; i < maxnodes; i++ ){
	 Mask[ i ] = ( char * ) malloc ( k * sizeof ( char ) );
	 if ( Mask[ i ] == NULL )
	    printf( "No memory\n" );
      }
   if ( ( Color = ( int * ) malloc ( maxnodes * sizeof ( int ) ) ) == NULL )
      printf( "No memory\n" );
   
   for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	 Graph[ i ][ j ] = '0';
   
   for ( i = 0; i < maxnodes; i++ )
	   for ( j = 0; j < k; j++ )
		   Mask[ i ][ j ] = '0';

   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d", &u, &v );
      Graph[ v ][ u ] = '1';
      if ( feof( ptr ) )
	 break;
   }

   for ( i = 0; i < maxnodes; i++ )
      Color[ i ] = -1;
   
   result = GraphColoring( k );
   if ( result == 1 )
   for ( i = 0; i < maxnodes; i++ )
	 printf( "Color[ %d ] is %d\n", i, Color[ i ] );
   
   else
      printf( "\nThe Graph cannot be colored" );
   
   }
   return 0;
}

int AllColored( void ){
	int i, j;
	for ( i = 0; i < maxnodes; i++ )
		if ( Color[ i ] == -1 )
			return 0;
	return 1;
}

int Available( int i, int j ){
	int n;
	if ( Mask[ i ][ j ] == '0' ){
		for ( n = 0; n < maxnodes; n++ ){
			if ( i > n )
			   if ( Graph[ i ][ n ] == '1' )
				   if ( Color[ n ] == j )
					   return 0;
			if ( i < n )
				if ( Graph[ n ][ i ] == '1' )
					if ( Color[ n ] == j )
						return 0;
		}
		return 1;
	}
	return 0;
}

void InitializeMask( int v, int k ){
	int i;
	for ( i = 0; i < k; i++ )
		Mask[ v ][ i ] = '0';
}

int GraphColoring( int k ){
   int j = 0, v = pos++;
   
   if ( AllColored() )
      return 1;
   
   while ( j < k ) {
      if ( Color[ v ] != j && Available( v, j ) ){
		  Color[ v ] = j;
		  Mask[ v ][ j ] = '1';
		  if( GraphColoring( k ) )
	         return 1;
	      j = -1;
      }
      j++;
   }
   Color[ v ] = -1;
   InitializeMask( v, k );
   pos--;
   return 0;
}



