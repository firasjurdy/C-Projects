#include <stdio.h>
#include <stdlib.h>
#include <time.h>
char **Realgraph;
int *mask;
int maxnodes;
int *degree;
char **Graph;
int m = 0;
int Degree( int );
int MaxDegree();
void CutEdges( int );
void ReturnEdges( int );
int Edgeless( );
void CutNeighbors( int );
void ReturnNeighbors( int );
int VCBranch( int );
int main(){
   int i, j, cover, k, u, v;
   FILE *ptr;
   char filename[ 15 ];
   float t0, t1;
   printf( "Enter the filename: " );
   scanf( "%s", filename );
   printf( "\nEnter the cover size: " );
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
   if ( ( Realgraph = ( char ** ) malloc ( maxnodes * sizeof ( char * ) ) ) == NULL )
      printf( "No memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 Realgraph[ i ] = ( char * ) malloc ( i * sizeof ( char ) );
	 if ( Realgraph[ i ] == NULL )
	    printf( "No memory\n" );
      }
   if ( ( mask = ( int * ) malloc ( maxnodes * sizeof ( int ) ) ) == NULL )
      printf( "No memory\n" );
   if ( ( degree = ( int * ) malloc ( maxnodes * sizeof ( int ) ) )  == NULL )
     printf( "No memory\n" );
   for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	 Graph[ i ][ j ] = Realgraph[ i ][ j ] = '0';
   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d", &u, &v );
      Graph[ v ][ u ] = Realgraph[ v ][ u ] = '1';
      if ( feof( ptr ) )
	 break;
   }

   for ( i = 0; i < maxnodes; i++ )
      mask[ i ] = -1;
   for ( i = 0; i < maxnodes; i++ )
      degree[ i ] = Degree( i );
   t0 = ( float ) clock() / CLOCKS_PER_SEC;
   cover = VCBranch( k );
   t1 = ( float ) clock() / CLOCKS_PER_SEC;
   if ( cover == 1 ){
   for ( i = 0; i < maxnodes; i++ )
      if ( mask[ i ] != -1 )
	 printf( "%d ", i );
   }
   else
      printf( "no cover" );
   printf( "\ntime = %f", ( float ) ( t1 - t0 ) );
   }
   return 0;
}
int Degree( int i ){
   int j, k = 0;
   for ( j = 0; j < i; j++ )
      if ( Graph[ i ][ j ] == '1' )
	 k++;
   while ( i < maxnodes - 1 )
      if ( Graph[ ++i ][ j ] == '1' )
	 k++;
   return k;
}
int MaxDegree(){
   int i, high = degree[ 0 ];
   for ( i = 1; i < maxnodes; i++ )
      if ( degree[ i ] > high )
	 high = degree[ i ];
   return high;
}
void CutEdges( int i ) {
   int j;
   for ( j = 0; j < i; j++ )
      if ( Graph[ i ][ j ] == '1' ) {
	 Graph[ i ][ j ] = '0';
	 --degree[ j ];
      }
   while ( i < maxnodes - 1 )
      if ( Graph[ ++i ][ j ] == '1' ) {
	 Graph[ i ][ j ] = '0';
	 --degree[ i ];
      }
   degree[ j ] = 0;
}
void ReturnEdges( int i ) {
   int j;
   for ( j = 0; j < i; j++ )
      if ( Realgraph[ i ][ j ] == '1' && mask[ j ] == -1 ){
	 Graph[ i ][ j ] = '1';
	 ++degree[ j ];
	 ++degree[ i ];
      }
   while ( i < maxnodes - 1 )
      if ( Realgraph[ ++i ][ j ] == '1' && mask[ i ] == -1 ){
	 Graph[ i ][ j ] = '1';
	 ++degree[ i ];
	 ++degree[ j ];
      }
}
int Edgeless( ) {
   int i, j;
   for ( i = 0; i < maxnodes; i++ )
      for ( j = 0; j < i; j++ )
	 if ( Graph[ i ][ j ] == '1' )
	    return 0;
   return 1;
}
void CutNeighbors( int i ){
   int j;
   for ( j = 0; j < i; j++ )
      if ( Realgraph[ i ][ j ] == '1' && mask[ j ] == -1 ){
	 CutEdges( j );
	 mask[ j ] = i;
	 m++;
      }
   while ( i < maxnodes - 1 )
      if ( Realgraph[ ++i ][ j ] == '1' && mask[ i ] == -1 ){
	 CutEdges( i );
	 mask[ i ] = j;
	 m++;
      }
}
void ReturnNeighbors( int i ){
   int j;
   for ( j = 0; j < i; j++ )
      if ( Realgraph[ i ][ j ] == '1' && mask[ j ] == i ){
	 ReturnEdges( j );
	 mask[ j ] = -1;
	 m--;
      }
   while ( i < maxnodes - 1 )
      if ( Realgraph[ ++i ][ j ] == '1' && mask[ i ] == j ){
	 ReturnEdges( i );
	 mask[ i ] = -1;
	 m--;
      }
}
int VCBranch( int k ){
   int j = 0, d;
   if ( Edgeless( ) && m <= k )
      return 1;
   if ( m >= k )
      return 0;
   d = MaxDegree();
   while ( j < maxnodes ) {
      if ( degree[ j ] == d ){
	 CutEdges( j );
	 mask[ j ] = -2;
	 m++;
	 if( VCBranch( k ) )
	    return 1;
	 m--;
	 CutNeighbors( j );
	 mask[ j ] = -1;
	 if ( VCBranch( k ) )
	    return 1;
	 ReturnNeighbors( j );
	 return 0;
      }
      j++;
   }
   return 0;
}



