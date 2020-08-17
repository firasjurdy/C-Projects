#include <stdio.h>
#include <stdlib.h>

int *visited;
int maxnodes;
int *degree;
int *prevented;
char **Graph;
char ***Mask;
int set = 0;
int position = 0;
int level = 0;

int Degree( int );
int MinDegree();
void PutInSet( int );
void RemoveFromSet( int );
int MISBranch( int );

int main(){
   int i, j, result, k, u, v, h;
   FILE *ptr;
   char filename[ 15 ];

   printf( "Enter the filename: " );
   scanf( "%s", filename );
   printf( "\nEnter a number you assume that it is the maximum Independent Set: " );
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

   if ( ( Mask = ( char *** ) malloc ( maxnodes * sizeof( char ** ) ) ) == NULL ){
      printf( "Out of memory\n" );
	  return 0;
   }
      for ( i = 0; i < maxnodes; i++ ){
	     Mask[ i ] = ( char ** ) malloc ( maxnodes * sizeof( char * ) );
	     if ( Mask[ i ] == NULL ){
		printf( "Out of memory\n" );
		    return 0;
	     }
      }

	  Mask[ 0 ][ 0 ] = ( char * ) malloc ( maxnodes * sizeof ( char ) );

	  for ( i = 1; i < maxnodes; i++ )
		  for ( j = 0; j < maxnodes; j++ ){
			Mask[ i ][ j ] = ( char * ) malloc ( maxnodes * sizeof( char ) );
			if ( Mask[ i ][ j ] == NULL ){
				printf( "Out of memory" );
				return 0;
			}
   }

   if ( ( visited = ( int * ) malloc ( maxnodes * sizeof ( int ) ) ) == NULL )
      printf( "No memory\n" );

   if ( ( degree = ( int * ) malloc ( maxnodes * sizeof ( int ) ) )  == NULL )
     printf( "No memory\n" );

   if ( ( prevented = ( int * ) malloc ( maxnodes * sizeof ( int ) ) )  == NULL )
     printf( "No memory\n" );

   for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	    Graph[ i ][ j ] = '0';

   for( i = 1; i < maxnodes; i++ )
	   for ( j = 0; j < maxnodes; j++ )
		   for ( h = 0; h < maxnodes; h++ )
		      Mask[ i ][ j ][ h ] = '0';

   for ( i = 0; i < maxnodes; i++ )
	   Mask[ 0 ][ 0 ][ i ] = '0';

   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d", &u, &v );
      Graph[ v ][ u ] = '1';
      if ( feof( ptr ) )
	     break;
   }

   for ( i = 0; i < maxnodes; i++ )
      visited[ i ] = prevented[ i ] = 0;

   for ( i = 0; i < maxnodes; i++ )
      degree[ i ] = Degree( i );

   result = MISBranch( k );

   if ( result == 1 ){
   for ( i = 0; i < maxnodes; i++ )
      if ( visited[ i ] == 1 )
	     printf( "%d ", i );
   }
   else
      printf( "No solution" );

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

int MinDegree(){
   int i = 0, j = 0, low;

   while ( j < maxnodes ){
	   if ( !visited[ j ] && !prevented[ j ] )
		   if ( Mask[ level ][ position ][ j ] == '0' ){
		      low = degree[ j ];
		      break;
		   }
       j++;
   }
   while ( i < maxnodes ){
      if ( !visited[ i ] && !prevented[ i ] )
		  if ( Mask[ level ][ position ][ i ] == '0' )
	     if ( degree[ i ] < low )
		    low = degree[ i ];
	  i++;
   }
   return low;
}

void PutInSet( int v ){
	int i;
	visited[ v ] = 1;
	for ( i = 0; i < maxnodes; i++ ){
		if ( v > i )
		   if ( Graph[ v ][ i ] == '1' )
			   prevented[ i ] = 1;
	    if ( v < i )
	   if ( Graph[ i ][ v ] == '1' )
			   prevented[ i ] = 1;
    }
	set++;
}

void RemoveFromSet( int v ){
	int i, flag = 0, j;
	visited[ v ] = 0;
	for ( i = 0; i < maxnodes; i++ ){
		if ( v > i )
			if ( Graph[ v ][ i ] == '1' ){
				for( j = 0; j < maxnodes; j++ ){
					if ( i > j )
		       if ( Graph[ i ][ j ] == '1' && j != v && visited[ j ] ){
					      flag = 1;
					      break;
				       }
				    if ( i < j )
					   if ( Graph[ j ][ i ] == '1' && j != v && visited[ j ] ){
						   flag = 1;
						   break;
					   }
				}
			   if ( flag == 0 )
				   prevented[ i ] = 0;
			   flag = 0;
			}
	    if ( v < i )
	   if ( Graph[ i ][ v ] == '1' ){
				for( j = 0; j < maxnodes; j++ ){
					if ( i > j )
		       if ( Graph[ i ][ j ] == '1' && j != v && visited[ j ] ){
					      flag = 1;
					      break;
				       }
				    if ( i < j )
					   if ( Graph[ j ][ i ] == '1' && j != v && visited[ j ] ){
						   flag = 1;
						   break;
					   }
				}
			   if ( flag == 0 )
				   prevented[ i ] = 0;
			   flag = 0;
			}
		   flag = 0;
    }
	set--;
}

int MISBranch( int k ){
   int j = 0, d, oldposition;

   if ( set >= k )
      return 1;

   d = MinDegree();

   while ( j < maxnodes ) {
      if ( degree[ j ] == d && !visited[ j ] && !prevented[ j ] ){
	     PutInSet( j );
		 Mask[ level ][ position ][ j ] = '1';
		 oldposition = position;
		 position = j;
		 level++;
	     if( MISBranch( k ) )
		return 1;
	     RemoveFromSet( j );
		 level--;
		 position = oldposition;
      }
      j++;
   }
   return 0;
}



