#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Math.h>

int maxnodes;
int **RGraph, **Graph, *visited, **MarkEdge;
int *DegreeV1InV2, *DegreeV1, *Degree, *Solution, **ByPassed;
int position = 0, count = 0, result;
FILE *fptr;


int Acyclic();
int ConnectedComponents();
int BelongToSameTree( int, int, int );
void FindNeighbors();
int CalculateDegree( int );
void Remove( int );
void Return( int );
void ReturnAndPlaceInV2( int );
void ReturnAndPlaceInV1( int );
void ReturnByPassed( int, int, int );
void ByPass( int, int *, int * );
int FeedBack( int, int, int );
void Initialize();
void PrintDegrees();
void PrintGraph();
int FVSMaxDegreeHeuristic();
int MaxDegree();
void InitializeGraph();
int IntToBinary( int *, int, int );

int main(){
   int i, j, k, u, v, a, b, V1_Size, V2_Size, index = 0;
   FILE *ptr;
   int *Binary, *F_Vertex;
   char filename[ 20 ];

   clrscr();
   printf( "Enter the filename that contains the Graph: " );
   scanf( "%s", filename );


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
	if ( ( RGraph = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL )
      printf( "Out of memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 RGraph[ i ] = ( int * ) malloc ( i * sizeof( int ) );
	 if ( RGraph[ i ] == NULL )
	    printf( "Out of memory\n" );
      }
  if ( ( MarkEdge = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL )
      printf( "Out of memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 MarkEdge[ i ] = ( int * ) malloc ( i * sizeof( int ) );
	 if ( MarkEdge[ i ] == NULL )
	    printf( "Out of memory\n" );
      }

	  if ( ( ByPassed = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL )
      printf( "Out of memory\n" );
   else
      for ( i = 1; i < maxnodes; i++ ){
	 ByPassed[ i ] = ( int * ) malloc ( i * sizeof( int ) );
	 if ( ByPassed[ i ] == NULL )
	    printf( "Out of memory\n" );
      }

   visited = ( int * ) malloc ( maxnodes * sizeof ( int ) );
   for ( i = 0; i < maxnodes; i++ )
	   visited[ i ] = 0;

   Solution = ( int * ) malloc ( maxnodes * sizeof ( int ) );
   for ( i = 0; i < maxnodes; i++ )
	   Solution[ i ] = 0;

   for ( i = 0; i < maxnodes; i++ )
	   for ( j = 0; j < i; j++ ){
			Graph[ i ][ j ] = 0;
			RGraph[ i ][ j ] = 0;
			MarkEdge[ i ][ j ] = 0;
			ByPassed[ i ][ j ] = -1;
	   }

	   DegreeV1InV2 = ( int * ) malloc ( maxnodes * sizeof( int ) );
	   for( i = 0; i < maxnodes; i++ )
		   DegreeV1InV2[ i ] = 0;
	   DegreeV1 = ( int * ) malloc ( maxnodes * sizeof( int ) );
	   for( i = 0; i < maxnodes; i++ )
		   DegreeV1[ i ] = 0;

	while ( !feof( ptr ) ){
		fscanf( ptr, "%d%d", &u, &v );
		//printf( "%d %d\n", u, v );
		Graph[ v ][ u ]++;
		RGraph[ v ][ u ]++;
		if ( feof( ptr ) )
			break;
	}

	Degree = ( int * ) malloc ( maxnodes * sizeof ( int ) );
	for ( i = 0; i < maxnodes; i++ )
	   Degree[ i ] = CalculateDegree( i );
	//PrintDegrees();
	//FindNeighbors();
	k = FVSMaxDegreeHeuristic();
	Binary = ( int * ) malloc ( k * sizeof( int ) );
	F_Vertex = ( int * ) malloc( k * sizeof( int ) );

	if( ( fptr = fopen( "OGraph.txt", "w" ) ) == NULL ){
		printf( "Cannot open ouput file\n" );
		return 0;
	}
	fprintf( fptr, "Applying MaxDegreeHeuristic, FVS obtained: " );
	for( i = 0; i < maxnodes; i++ )
		if( Solution[ i ] == 1 ){
			F_Vertex[ index++ ] = i;
			fprintf( fptr, "%d, ", i );
		}
	fprintf( fptr, "\n" );
	//New Testing with iterative compression
	/*printf( "Enter 1 to continue, 0 to exit: " );
	scanf( "%d", &u );
	while( u != 0 ){
	for ( i = 0; i < maxnodes; i++ )
		for ( j = 0; j < i; j++ ){
			Graph[ i ][ j ] = RGraph[ i ][ j ];
			ByPassed[ i ][ j ] = -1;
		}
	for( i = 0; i < maxnodes; i++ ){
		Solution[ i ] = 0;
		Degree[ i ] = 0;
		DegreeV1[ i ] = 0;
		DegreeV1InV2[ i ] = 0;
	}
	printf( "Enter V1 Size: " );
	scanf( "%d", &V1_Size );
	for( i = 0; i < V1_Size; i++ ){
		scanf( "%d", &u );
		DegreeV1[ u ] = 1;
	}
	printf( "Enter V2 Size: " );
	scanf( "%d", &V2_Size );
	for( i = 0; i < V2_Size; i++ ){
		scanf( "%d", &u );
		DegreeV1[ u ] = -1;
	}
	for ( i = 0; i < maxnodes; i++ )
	   Degree[ i ] = CalculateDegree( i );

	FindNeighbors();
	printf( "Enter k value: " );
	scanf( "%d", &k );
	printf( "Do you want to remove any additional vertices: " );
	scanf( "%d", &j );
	for( i = 0; i < j; i++ ){
		printf( "Vertex to be removed: " );
		scanf( "%d", &u );
		Remove( u );
		Solution[ u ] = 1;
	}
	if( FeedBack( V1_Size, V2_Size, k - j ) )
		printf( "Solution exists\n" );
	else printf( "No Solution\n" );

	for( i = 0; i < maxnodes; i++ )
		if( Solution[ i ] == 1 )
			printf( "%d, ", i );
	//PrintDegrees();
	//PrintGraph();
	printf( "Enter 1 to continue, 0 to exit: " );
	scanf( "%d", &u );
	}*/


	//Final Testing

	for( j = 0; j < pow( 2, k ) - 1; j++ ){
		/*if( j <= -1  )
			continue;
		else if( j > 0 )
			break;*/
		InitializeGraph();
		V1_Size = 0;
		V2_Size = 0;
		v = IntToBinary( Binary, j, k );
		fprintf( fptr, "\n%d iteration, F_Vertices: ", j );
		for( i = 0; i < k; i++ )
			fprintf( fptr, "%d ", F_Vertex[ i ] );
		fprintf( fptr, "\n" );
		if( v < k ){
			for( u = k - 1; u >= 0; u-- )
				if( Binary[ u ] == 1 ){
					fprintf( fptr, "Vertex to be removed: %d\n", F_Vertex[ u ] );
					Remove( F_Vertex[ u ] );
					DegreeV1[ F_Vertex[ u ] ] = -2;
				}

			for( i = 0; i < maxnodes; i++ ){
				if( Solution[ i ] != 1 ){
					V1_Size++;
					DegreeV1[ i ] = 1;
				}
				else if( DegreeV1[ i ] == 0 ){
						V2_Size++;
						DegreeV1[ i ] = -1;
						Solution[ i ] = 0;
				}
			}

			for ( i = 0; i < maxnodes; i++ )
				Degree[ i ] = CalculateDegree( i );

			FindNeighbors();



			/*Remove( 1 );
			Solution[ 1 ] = 1;
			Remove( 6 );
			Solution[ 6 ] = 1;
			Remove( 8 );
			Solution[ 8 ] = 1;
			Remove( 10 );
			Solution[ 10 ] = 1;
			Remove( 13 );
			Solution[ 13 ] = 1;
			Remove( 16 );
			Solution[ 16 ] = 1;
			Remove( 21 );
			Solution[ 21 ] = 1;
			Remove( 43 );
			Solution[ 43 ] = 1;
			Remove( 44 );
			Solution[ 44 ] = 1;
			Initialize();
			ConnectedComponents();
			if( result == 1 )
				printf( "Graph is acyclic" );
			else printf( "Graph contains cycles" );*/
			/*//Try Removing 8
			Remove( 8 );
			Solution[ 8 ] = 1;
			//Try Removing 11
			Remove( 11 );
			Solution[ 11 ] = 1;
			//ByPassing 2
			ByPass( 2, &a, &b );
			Solution[ 2 ] = -2;
			//ByPassing 1
			ByPass( 1, &a, &b );
			Solution[ 1 ] = -2;
			//ByPassing 0
			ByPass( 0, &a, &b );
			Solution[ 0 ] = -2;
			//Try Removing 4
			Remove( 4 );
			Solution[ 4 ] = 1;
			//Adding 4 to V2
			ReturnAndPlaceInV2( 4 );
			Solution[ 4 ] = 0;
			DegreeV1[ 4 ] = -1;
			DegreeV1InV2[ 4 ] = 0;
			//Removing 5
			Remove( 5 );
			Solution[ 5 ] = 1;
			//Returning 5
			Return( 5 );
			Solution[ 5 ] = 0;
			//Returning 4 to V1
			ReturnAndPlaceInV1( 4 );
			ReturnByPassed( 0, 4, 3 );
			Solution[ 0 ] = 0;
			ReturnByPassed( 1, 0, 3 );
			Solution[ 1 ] = 0;
			ReturnByPassed( 2, 1, 3 );
			Solution[ 2 ] = 0;
			//Adding 11 to V2
			ReturnAndPlaceInV2( 11 );
			Solution[ 11 ] = 0;
			DegreeV1[ 11 ] = -1;
			DegreeV1InV2[ 11 ] = 0;
			//Removing 10
			Remove( 10 );
			Solution[ 10 ] = 1;
			//ByPassing 2
			ByPass( 2, &a, &b );
			Solution[ 2 ] = -2;
			//ByPassing 1
			ByPass( 1, &a, &b );
			Solution[ 1 ] = -2;
			//ByPassing 0
			ByPass( 0, &a, &b );
			Solution[ 0 ] = -2;
			//Removing 4
			Remove( 4 );
			Solution[ 4 ] = 1;
			//Returning 4
			Return( 4 );
			Solution[ 4 ] = 0;
			ReturnByPassed( 0, 4, 3 );
			Solution[ 0 ] = 0;
			ReturnByPassed( 1, 0, 3 );
			Solution[ 1 ] = 0;
			ReturnByPassed( 2, 1, 3 );
			Solution[ 2 ] = 0;
			//Returning 10
			Return( 10 );
			Solution[ 10 ] = 0;
			//Returning 11 to V1
			ReturnAndPlaceInV1( 11 );
			//Adding 8 to V2
			ReturnAndPlaceInV2( 8 );
			Solution[ 8 ] = 0;
			DegreeV1[ 8 ] = -1;
			DegreeV1InV2[ 8 ] = 0;
			//Try Removing 11
			Remove( 11 );
			Solution[ 11 ] = 1;
			//ByPassing 2
			ByPass( 2, &a, &b );
			Solution[ 2 ] = -2;
			//ByPassing 1
			ByPass( 1, &a, &b );
			Solution[ 1 ] = -2;
			//ByPassing 0
			ByPass( 0, &a, &b );
			Solution[ 0 ] = -2;
			//Try Removing 4
			Remove( 4 );
			Solution[ 4 ] = 1;
			PrintDegrees();*/
			/*//Discard 0
			Remove( 0 );
			Solution[ 0 ] = -1;
			//ByPass 1
			ByPass( 1, &a, &b );
			Solution[ 1 ] = -2;
			//Try Remove 2
			Remove( 2 );
			Solution[ 2 ] = 1;
			//Place 2 in V2
			ReturnAndPlaceInV2( 2 );
			Solution[ 2 ] = 0;
			DegreeV1[ 2 ] = -1;
			DegreeV1InV2[ 2 ] = 0;
			//Remove 9
			Remove( 9 );
			Solution[ 9 ] = 1;
			//Return 9
			Return( 9 );
			Solution[ 9 ] = 0;
			//Place 2 in V1
			ReturnAndPlaceInV1( 2 );
			//Return By Passed 1
			ReturnByPassed( 1, a, b );
			Solution[ 1 ] = 0;
			//Return Discarded 0
			Return( 0 );
			Solution[ 0 ] = 0;
			PrintDegrees();
			Initialize();
			ConnectedComponents();
			if( result == 1 )
				printf( "Graph is acyclic" );
			else printf( "Graph contains cycles" );*/
			/*Remove( 5 );
			Solution[ 5 ] = 1;
			Remove( 8 );
			Solution[ 8 ] = 1;
			Remove( 11 );
			Solution[ 11 ] = 1;
			ByPass( 2, &a, &b );
			Solution[ 2 ] = -2;
			ByPass( 1, &a, &b );
			Solution[ 1 ] = -2;
			ByPass( 0, &a, &b );
			Solution[ 0 ] = -2;
			Remove( 4 );
			Solution[ 4 ] = 1;
			ReturnAndPlaceInV2( 4 );
			Solution[ 4 ] = 0;
			DegreeV1[ 4 ] = -1;
			DegreeV1InV2[ 4 ] = 0;
			ByPass( 7, &a, &b );
			Solution[ 7 ] = -2;
			ByPass( 13, &a, &b );
			Solution[ 13 ] = -2;
			ByPass( 14, &a, &b );
			Solution[ 14 ] = -2;
			ByPass( 15, &a, &b );
			Solution[ 15 ] = -2;
			ByPass( 16, &a, &b );
			Solution[ 16 ] = -2;
			Remove( 20 );
			Solution[ 20 ] = 1;
			ReturnAndPlaceInV2( 20 );
			Solution[ 20 ] = 0;
			DegreeV1[ 20 ] = -1;
			DegreeV1InV2[ 20 ] = 0;
			ByPass( 19, &a, &b );
			Solution[ 19 ] = -2;
			Remove( 31 );
			Solution[ 31 ] = 1;
			ReturnAndPlaceInV2( 31 );
			Solution[ 31 ] = 0;
			DegreeV1[ 31 ] = -1;
			DegreeV1InV2[ 31 ] = 0;
			ByPass( 22, &a, &b );
			Solution[ 22 ] = -2;
			ByPass( 25, &a, &b );
			Solution[ 25 ] = -2;
			Remove( 26 );
			Solution[ 26 ] = 1;
			Return( 26 );
			Solution[ 26 ] = 0;
			printf( "Degree[ 26 ] = %d\n", Degree[ 26 ] );
			ReturnByPassed( 25, a, b );
			Solution[ 25 ] = 0;
			printf( "Degree[ 25 ] = %d\n", Degree[ 25 ] );
			printf( "Degree[ 26 ] = %d\n", Degree[ 26 ] );
			printf( "Degree[ 21 ] = %d\n", Degree[ 21 ] );
			ReturnByPassed( 22, 25, 21 );
			Solution[ 22 ] = 0;
			printf( "Degree[ 22 ] = %d\n", Degree[ 22 ] );
			printf( "Degree[ 25 ] = %d\n", Degree[ 25 ] );
			printf( "Degree[ 21 ] = %d\n", Degree[ 21 ] );
			for( i = 0; i < 31; i++ )
				if( Graph[ 31 ][ i ] >= 1 )
					if( DegreeV1[ i ] == -1 )
						DegreeV1InV2[ 31 ] += Graph[ 31 ][ i ];
			for( i = 31 + 1; i < maxnodes; i++ )
				if( Graph[ i ][ 31 ] >= 1 )
					if( DegreeV1[ i ] == -1 )
						DegreeV1InV2[ 31 ] += Graph[ i ][ 31 ];
			DegreeV1[ 31 ] = Degree[ 31 ] - DegreeV1InV2[ 31 ];
			ReturnByPassed( 19, 31, 6 );
			Solution[ 19 ] = 0;
			PrintDegrees();*/
			/*Initialize();
			ConnectedComponents();
			if( result == 1 )
				printf( "Graph is acyclic" );
			else printf( "Graph contains cycles" );*/

			if( FeedBack( V1_Size, V2_Size, k - v - 1 ) ){
				fprintf( fptr, "Solution exists\n" );
				k = 0;
				j = -1;
				for( i = 0; i < maxnodes; i++ )
					if( Solution[ i ] == 1 || DegreeV1[ i ] == -2){
						fprintf( fptr, "%d, ", i );
						F_Vertex[ k++ ] = i;
					}
			}
			else {
				fprintf( fptr, "No Solution\n" );
				for( i = 0; i < maxnodes; i++ )
					Solution[ i ] = 0;
				for( i = 0; i < k; i++ )
					Solution[ F_Vertex[ i ] ] = 1;
			}
		}
		fprintf( fptr, "\n" );
		//PrintDegrees();
	}



	/*Testing IntToBinary()
	k = 4;
	for( i = 0; i < pow( 2, k ); i++ ){
		printf( "\n%d 1's: ", IntToBinary( Binary, i, k ) );
		for( j = k - 1; j >= 0; j-- )
			printf( "%d", Binary[ j ] );
	}*/
  }
  getch();

   return 0;
}

int Acyclic(){

   int j = 0, Oldposition, Answer;

   if ( count == maxnodes )
      return 1;

   while ( j < maxnodes ){
		  if ( position < j ){
			 if( Graph[ j ][ position ] == 1 && !MarkEdge[ j ][ position ] ){
				 if( !visited[ j ] ){
					//printf( "%d ---> ", j );
					visited[ j ] = 1;
					MarkEdge[ j ][ position ] = 1;
					Oldposition = position;
					position = j;
					count++;
					Answer = Acyclic();
					if( Answer == 1 )
						return 1;
					if( Answer == 2 )
						position = Oldposition;
					else return 0;
				 }
				 else return 0;
			 }
			 else if ( Graph[ j ][ position ] > 1 )
				 return 0;
		  }
		  else if( position > j ){
				if ( Graph[ position ][ j ] == 1 && !MarkEdge[ position ][ j ] ){
					if( !visited[ j ] ){
						//printf( "%d ---> ", j );
						visited[ j ] = 1;
						MarkEdge[ position ][ j ] = 1;
						Oldposition = position;
						position = j;
						count++;
						Answer = Acyclic();
						if( Answer == 1 )
							return 1;
						if( Answer == 2 )
							position = Oldposition;
						else return 0;
					}
					else return 0;
				}
				else if ( Graph[ position ][ j ] > 1 )
					return 0;
			 }
      j++;
   }
   return 2;
}

int ConnectedComponents(){
	int i, components = 0;
	for ( i = 0; i < maxnodes; i++ )
		if ( !visited[ i ] ){
			position = i;
			visited[ position ] = 1;
			count++;
			//printf( "\n%d ---> ", position );
			result = Acyclic();
			components++;
			if( result == 0 )
				break;
		}
	return components; //returns the correct number of connected components only if the graph is acyclic
}

int BelongToSameTree( int a, int b, int V2_Size ){

   int j = 0, Oldposition;

   if ( a == b )
	   return 1;
   if ( count == V2_Size )
      return 0;

   while ( j < maxnodes ){
	   if ( DegreeV1[ j ] == -1 && !visited[ j ] ){
		  if ( position < j ){
			 if( Graph[ j ][ position ] >= 1 ){
				if( j == b )
					return 1;
				//printf( "%d ---> ", j );
				visited[ j ] = 1;
				Oldposition = position;
				position = j;
				count++;
				if( BelongToSameTree( a, b, V2_Size ) )
					return 1;
				position = Oldposition;
			 }
		  }
		  else if( position > j && Graph[ position ][ j ] >= 1 ){
				if( j == b )
					return 1;
			    //printf( "%d ---> ", j );
				visited[ j ] = 1;
				Oldposition = position;
				position = j;
				count++;
				if( BelongToSameTree( a, b, V2_Size ) )
					return 1;
				position = Oldposition;
			 }
	   }
      j++;
   }
   return 0;
}

void FindNeighbors(){
	int i, j;
	for( i = 0; i < maxnodes; i++ )
		if( DegreeV1[ i ] == 1 ){
			for( j = 0; j < maxnodes; j++ )
				if( DegreeV1[ j ] == -1 ){
					if( i > j && Graph[ i ][ j ] >= 1 )
						DegreeV1InV2[ i ] += Graph[ i ][ j ];
					else if( i < j && Graph[ j ][ i ] >= 1 )
						DegreeV1InV2[ i ] += Graph[ j ][ i ];
				}
			DegreeV1[ i ] = Degree[ i ] - DegreeV1InV2[ i ];
		}
}

int CalculateDegree( int i ){
   int j = 0, k = 0;
   for ( j = 0; j < i; j++ )
      if ( Graph[ i ][ j ] >= 1 )
		k += Graph[ i ][ j ];
   while ( j < maxnodes - 1 ){
	   j++;
       if ( Graph[ j ][ i ] >= 1 )
		   k += Graph[ j ][ i ];
   }
   return k;
}

void ByPass( int i, int *x, int *y ){
	int j = 0, a, b, k;
	for ( j = 0; j < maxnodes; j++ ){
		if ( j > i && Graph[ j ][ i ] == 1 ){
			a = j;
			Graph[ j ][ i ] = 0;
			break;
		}
		else if ( j < i && Graph[ i ][ j ] == 1 ){
			a = j;
			Graph[ i ][ j ] = 0;
			break;
		}
	}

	for ( k = j + 1; k < maxnodes; k++ ){
		if ( k > i && Graph[ k ][ i ] == 1 ){
			b = k;
			Graph[ k ][ i ] = 0;
			break;
		}
		else if ( k < i && Graph[ i ][ k ] == 1 ){
			b = k;
			Graph[ i ][ k ] = 0;
			break;
		}
	}

	if ( a > b ){
		Graph[ a ][ b ] += 1;
		if( i > a ){
			ByPassed[ i ][ a ] = b;
			//printf( "ByPassed[ %d ][ %d ] = %d\n", i, a, b );
			if( i > b ){
				ByPassed[ i ][ b ] = a;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", i, b, a );
			}
			else {
				ByPassed[ b ][ i ] = a;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", b, i, a );
			}
		}
		else {
			ByPassed[ a ][ i ] = b;
			//printf( "ByPassed[ %d ][ %d ] = %d\n", a, i, b );
			if( i > b ){
				ByPassed[ i ][ b ] = a;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", i, b, a );
			}
			else {
				ByPassed[ b ][ i ] = a;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", b, i, a );
			}
		}
		//printf( "Graph[ %d ][ %d ] = %d\n", a, b, Graph[ a ][ b ] );
	}
	else{
		if( i > b ){
			ByPassed[ i ][ b ] = a;
			//printf( "ByPassed[ %d ][ %d ] = %d\n", i, b, a );
			if( i > a ){
				ByPassed[ i ][ a ] = b;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", i, a, b );
			}
			else {
				ByPassed[ a ][ i ] = b;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", a, i, b );
			}
		}
		else {
			ByPassed[ b ][ i ] = a;
			//printf( "ByPassed[ %d ][ %d ] = %d\n", b, i, a );
			if( i > a ){
				ByPassed[ i ][ a ] = b;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", i, a, b );
			}
			else {
				ByPassed[ a ][ i ] = b;
				//printf( "ByPassed[ %d ][ %d ] = %d\n", a, i, b );
			}
		}
		Graph[ b ][ a ] += 1;
		//printf( "Graph[ %d ][ %d ] = %d\n", b, a, Graph[ b ][ a ] );
	}

	Degree[ i ] = 0;
	DegreeV1[ i ] = 0;
	DegreeV1InV2[ i ] = 0;

	if( DegreeV1[ a ] >= 0 ){
		*x = a;
		*y = b;
		DegreeV1InV2[ a ]++;
		DegreeV1[ a ]--;
	}
	else {
		*x = b;
		*y = a;
		DegreeV1InV2[ b ]++;
		DegreeV1[ b ]--;
	}
}

void Remove( int i ) {
   int j = 0;
   for ( j = 0; j < i; j++ )
      if( Graph[ i ][ j ] >= 1 ) {
		//printf( "Cutting edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, j, i, j, Graph[ i ][ j ] );
		Degree[ j ] -= Graph[ i ][ j ];
		if( DegreeV1[ j ] > 0 )
			DegreeV1[ j ] -= Graph[ i ][ j ];
		Graph[ i ][ j ] = 0;
      }
   while ( j < maxnodes - 1 ){
		j++;
		if ( Graph[ j ][ i ] >= 1 ) {
			//printf( "Cutting edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", j, i, j, i, Graph[ j ][ i ] );
			Degree[ j ] -= Graph[ j ][ i ];
			if( DegreeV1[ j ] > 0 )
				DegreeV1[ j ] -= Graph[ j ][ i ];
			Graph[ j ][ i ] = 0;
		}
   }
   Degree[ i ] = 0;
   DegreeV1[ i ] = 0;
   DegreeV1InV2[ i ] = 0;
}

void Initialize(){
	int i, j;

	for ( i = 0; i < maxnodes; i++ )
	   visited[ i ] = 0;

	for ( i = 0; i < maxnodes; i++ )
	   for ( j = 0; j < i; j++ )
			MarkEdge[ i ][ j ] = 0;

	count = 0;
}

int FeedBack( int V1_Size, int V2_Size, int k ){
	int i, j, m, h, a, b, Answer;

	Initialize();
	ConnectedComponents();

	if( k < 0 || ( k == 0 && result == 0 ) )
		return 0;
	if( k >= 0 && result == 1 )
		return 1;
	for( i = 0; i < maxnodes; i++ ){
		if( DegreeV1[ i ] >= 0 ){
			if( DegreeV1InV2[ i ] >= 2 ){
				for ( j = 0; j < maxnodes; j++ ){
					if ( DegreeV1[ j ] == -1 && j > i && Graph[ j ][ i ] == 1 ){
						a = j;
						//printf( "a: %d\n", a );
						for ( m = j + 1; m < maxnodes; m++ )
							if ( DegreeV1[ m ] == -1 && m > i && Graph[ m ][ i ] == 1 ){
								b = m;
								//printf( "b: %d\n", b );
								for ( h = 0; h < maxnodes; h++ )
									visited[ h ] = 0;
								position = a;
								visited[ position ] = 1;
								count = 1;
								if( BelongToSameTree( a, b, V2_Size ) ){
									fprintf( fptr, "Removing %d\n", i );
									Remove( i );
									Solution[ i ] = 1;
									//PrintDegrees();
									Answer = FeedBack( V1_Size, V2_Size, k - 1 );
									if( Answer == 0 ){
										fprintf( fptr, "Returning %d\n", i );
										Solution[ i ] = 0;
										Return( i );
										return 0;
									}
									return 1;
								}				
							}
							else if ( DegreeV1[ m ] == -1 && m < i && Graph[ i ][ m ] == 1 ){
								b = m;
								//printf( "b: %d\n", b );
								for ( h = 0; h < maxnodes; h++ )
									visited[ h ] = 0;
								position = a;
								visited[ position ] = 1;
								count = 1;
								if( BelongToSameTree( a, b, V2_Size ) ){
									fprintf( fptr, "Removing %d\n", i );
									Remove( i );
									Solution[ i ] = 1;
									//PrintDegrees();
									Answer = FeedBack( V1_Size, V2_Size, k - 1 );
									if( Answer == 0 ){
										fprintf( fptr, "Returning %d\n", i );
										Solution[ i ] = 0;
										Return( i );
										return 0;
									}
									return 1;
								}

							}
					}
					else if ( DegreeV1[ j ] == -1 && j < i && Graph[ i ][ j ] == 1 ){
							a = j;
							//printf( "a: %d\n", a );
							for ( m = j + 1; m < maxnodes; m++ )
								if ( DegreeV1[ m ] == -1 && m > i && Graph[ m ][ i ] == 1 ){
									b = m;
									//printf( "b: %d\n", b );
									for ( h = 0; h < maxnodes; h++ )
										visited[ h ] = 0;
									position = a;
									visited[ position ] = 1;
									count = 1;
									if( BelongToSameTree( a, b, V2_Size ) ){
										fprintf( fptr, "Removing %d\n", i );
										Remove( i );
										Solution[ i ] = 1;
										//PrintDegrees();
										Answer = FeedBack( V1_Size, V2_Size, k - 1 );
										if( Answer == 0 ){
											fprintf( fptr, "Returning %d\n", i );
											Solution[ i ] = 0;
											Return( i );
											return 0;
										}
										return 1;
									}
								
								}
								else if ( DegreeV1[ m ] == -1 && m < i && Graph[ i ][ m ] == 1 ){
									b = m;
									//printf( "b: %d\n", b );
									for ( h = 0; h < maxnodes; h++ )
										visited[ h ] = 0;
									position = a;
									visited[ position ] = 1;
									count = 1;
									if( BelongToSameTree( a, b, V2_Size ) ){
										fprintf( fptr, "Removing %d\n", i );
										Remove( i );
										Solution[ i ] = 1;
										//PrintDegrees();
										Answer = FeedBack( V1_Size, V2_Size, k - 1 );
										if( Answer == 0 ){
											fprintf( fptr, "Returning %d\n", i );
											Solution[ i ] = 0;
											Return( i );
											return 0;
										}
										return 1;
									}
									
								}
					}
					else if ( DegreeV1[ j ] == -1 && j < i && Graph[ i ][ j ] > 1 ){
						fprintf( fptr, "Removing %d\n", i );
						Remove( i );
						Solution[ i ] = 1;
						//PrintDegrees();
						Answer = FeedBack( V1_Size, V2_Size, k - 1 );
						if( Answer == 0 ){
							fprintf( fptr, "Returning %d\n", i );
							Solution[ i ] = 0;
							Return( i );
							return 0;
						}
						return 1;
					}
					else if ( DegreeV1[ j ] == -1 && j > i && Graph[ j ][ i ] > 1 ){
						fprintf( fptr, "Removing %d\n", i );
						Remove( i );
						Solution[ i ] = 1;
						//PrintDegrees();
						Answer = FeedBack( V1_Size, V2_Size, k - 1 );
						if( Answer == 0 ){
							fprintf( fptr, "Returning %d\n", i );
							Solution[ i ] = 0;
							Return( i );
							return 0;
						}
						return 1;
					}
				}
				fprintf( fptr, "Try Removing %d\n", i );
				Remove( i );
				Solution[ i ] = 1;
				//PrintDegrees();
				Answer = FeedBack( V1_Size, V2_Size, k - 1 );
				if( Answer == 1 )
					return 1;
				else {
					fprintf( fptr, "Adding %d to V2\n", i );
					ReturnAndPlaceInV2( i );
					Solution[ i ] = 0;
					DegreeV1[ i ] = -1;
					DegreeV1InV2[ i ] = 0;
					Answer = FeedBack( V1_Size - 1, V2_Size + 1, k );
					if( Answer == 1 )
						return 1;
					fprintf( fptr, "Returning %d to V1\n", i );
					ReturnAndPlaceInV1( i );
					return 0;
				}
			}
		}
	}
	
	for( i = 0; i < maxnodes; i++ )
		if( DegreeV1[ i ] == 1 ){
			if( Degree[ i ] == 1 ){
				fprintf( fptr, "Discarding %d\n", i );
				Remove( i );
				Solution[ i ] = -1;
				//PrintDegrees();
				Answer = FeedBack( V1_Size, V2_Size, k );
				if( Answer == 1 )
					return 1;
				fprintf( fptr, "Returning %d\n", i );
				Return( i );
				Solution[ i ] = 0;
				return 0;
			}
			if ( Degree[ i ] > 1 ){
				fprintf( fptr, "ByPassing %d\n", i );
				ByPass( i, &a, &b );
				Solution[ i ] = -2;
				//printf( "After ByPassing: a=%d, b=%d\n", a, b );
				//PrintDegrees();
				Answer = FeedBack( V1_Size, V2_Size, k );
				if( Answer == 1 )
					return 1;
				ReturnByPassed( i, a, b );
				//printf( "Must Cut Edge <%d,%d>\n", a, b );
				Solution[ i ] = 0;
				return 0;
			}
		}
	return 0;
}

void PrintDegrees(){
	
	int i, j;
	printf( "Degree: " );
	for( i = 0; i < maxnodes; i++ )
		printf( "%d ", Degree[ i ] );

	printf( "\nDegreeV1: " );

    for( i = 0; i < maxnodes; i++ )
	   printf( "%d ", DegreeV1[ i ] );

	printf( "\nDegreeV1InV2: " );

    for( i = 0; i < maxnodes; i++ )
	   printf( "%d ", DegreeV1InV2[ i ] );

	printf( "\n" );
}

void ReturnAndPlaceInV2( int i ){
	int j = 0, u, v;
	for ( j = 0; j < i; j++ )
      if( RGraph[ i ][ j ] >= 1 ) {
		  if( DegreeV1[ j ] >= 0 ){ 
			  if( Solution[ j ] == 0 ){
				  Graph[ i ][ j ] += RGraph[ i ][ j ];
				  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, j, i, j, Graph[ i ][ j ] );
				  Degree[ j ] += RGraph[ i ][ j ];
				  Degree[ i ] += RGraph[ i ][ j ];
				  DegreeV1InV2[ j ] += RGraph[ i ][ j ];
			  }
			  else if ( Solution[ j ] == -2 ){
					//printf( "%d was ByPassed\n", j );
				  
					if( ByPassed[ i ][ j ] != -1 )
							  v = ByPassed[ i ][ j ];
							 
					if( i > v ){
						Graph[ i ][ v ]++;
						//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, v, i, v, Graph[ i ][ v ] );
						Degree[ i ]++;
						Degree[ v ]++;
					}
					else if( i < v ){
						Graph[ v ][ i ]++;
						//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", v, i, v, i, Graph[ v ][ i ] );
						Degree[ i ]++;
						Degree[ v ]++;
					}
			  }
		  }
		  else if( DegreeV1[ j ] == -1 ){
			  Graph[ i ][ j ] += RGraph[ i ][ j ];
			  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, j, i, j, Graph[ i ][ j ] );
			  Degree[ j ] += RGraph[ i ][ j ];
			  Degree[ i ] += RGraph[ i ][ j ];
		  }
      }
   while ( j < maxnodes - 1 ){
		j++;
		if ( RGraph[ j ][ i ] >= 1 ) {
			if( DegreeV1[ j ] >= 0 ){
				if( Solution[ j ] == 0 ){
					Graph[ j ][ i ] += RGraph[ j ][ i ];
					//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", j, i, j, i, Graph[ j ][ i ] );
					Degree[ j ] += RGraph[ j ][ i ];
					Degree[ i ] += RGraph[ j ][ i ];
					DegreeV1InV2[ j ] += RGraph[ j ][ i ];
				}
				else if ( Solution[ j ] == -2 ){
					//printf( "%d was ByPassed\n", j );
		
					if( ByPassed[ j ][ i ] != -1 )
						v = ByPassed[ j ][ i ];
				
			
				  if( i > v ){
					  Graph[ i ][ v ]++;
					  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, v, i, v, Graph[ i ][ v ] );
					  Degree[ i ]++;
					  Degree[ v ]++;
				  }
				  else if ( i < v ){
						Graph[ v ][ i ]++;
						//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", v, i, v, i, Graph[ v ][ i ] );
						Degree[ i ]++;
						Degree[ v ]++;
				  }
			  }
			}
			else if( DegreeV1[ j ] == -1 ){
				Graph[ j ][ i ] += RGraph[ j ][ i ];
				//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", j, i, j, i, Graph[ j ][ i ] );
				Degree[ j ] += RGraph[ j ][ i ];
				Degree[ i ] += RGraph[ j ][ i ];
			}
		}
   }
}

void Return( int i ) {
   int j = 0, u, v;
   for ( j = 0; j < i; j++ )
      if( RGraph[ i ][ j ] >= 1 ) {
		  if( DegreeV1[ j ] >= 0 ){
			  if( Solution[ j ] == 0 ){
				  Graph[ i ][ j ] += RGraph[ i ][ j ];
				  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, j, i, j, Graph[ i ][ j ] );
				  Degree[ j ] += RGraph[ i ][ j ];
				  Degree[ i ] += RGraph[ i ][ j ];
				  DegreeV1[ j ] += RGraph[ i ][ j ];
				  DegreeV1[ i ] += RGraph[ i ][ j ];
			  }
			  else if ( Solution[ j ] == -2 ){
				  //printf( "%d was ByPassed\n", j );
				  
						  if( ByPassed[ i ][ j ] != -1 )
							  v = ByPassed[ i ][ j ];
					
				  if( i > v ){
					  Graph[ i ][ v ]++;
					  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, v, i, v, Graph[ i ][ v ] );
					  Degree[ i ]++;
					  Degree[ v ]++;
					  DegreeV1InV2[ i ]++;
				  }
				  else if( i < v ){
						Graph[ v ][ i ]++;
						//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", v, i, v, i, Graph[ v ][ i ] );
						Degree[ i ]++;
						Degree[ v ]++;
						DegreeV1InV2[ i ]++;
				  }
			  }
		  }
		  else if ( DegreeV1[ j ] == -1 ){
			  Graph[ i ][ j ] += RGraph[ i ][ j ];
			  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, j, i, j, Graph[ i ][ j ] );
			  Degree[ j ] += RGraph[ i ][ j ];
			  Degree[ i ] += RGraph[ i ][ j ];
			  DegreeV1InV2[ i ] += RGraph[ i ][ j ];
		  }
       }
	while ( j < maxnodes - 1 ){
		j++;
		if ( RGraph[ j ][ i ] >= 1 ) {
			if( DegreeV1[ j ] >= 0 ){
				if( Solution[ j ] == 0 ){
					Graph[ j ][ i ] += RGraph[ j ][ i ];
					//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", j, i, j, i, Graph[ j ][ i ] );
					Degree[ j ] += RGraph[ j ][ i ];
					Degree[ i ] += RGraph[ j ][ i ];
					DegreeV1[ j ] += RGraph[ j ][ i ];
					DegreeV1[ i ] += RGraph[ j ][ i ];
				}
				else if ( Solution[ j ] == -2 ){
					//printf( "%d was ByPassed\n", j );

						  if( ByPassed[ j ][ i ] != -1 )
							  v = ByPassed[ j ][ i ];
					
				  if( i > v ){
					  Graph[ i ][ v ]++;
					  //printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, v, i, v, Graph[ i ][ v ] );
					  Degree[ i ]++;
					  Degree[ v ]++;
					  DegreeV1InV2[ i ]++;
				  }
				  else if( i < v ){
						Graph[ v ][ i ]++;
						//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", v, i, v, i, Graph[ v ][ i ] );
						Degree[ i ]++;
						Degree[ v ]++;
						DegreeV1InV2[ i ]++;
				  }
			  }

			}
			else if( DegreeV1[ j ] == -1 ){
				Graph[ j ][ i ] += RGraph[ j ][ i ];
				//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", j, i, j, i, Graph[ j ][ i ] );
				Degree[ j ] += RGraph[ j ][ i ];
				Degree[ i ] += RGraph[ j ][ i ];
				DegreeV1InV2[ i ] += RGraph[ j ][ i ];
			}
		}
	}
}

void ReturnByPassed( int i, int a, int b ){
	fprintf( fptr, "ReturnByPassed( %d, %d, %d )\n", i, a, b );
	if ( i > a ){
		if( Solution[ a ] == 0 ){
			if( a > b )
				Graph[ a ][ b ]--;
			else Graph[ b ][ a ]--;
			Graph[ i ][ a ]++;
			//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, a, i, a, Graph[ i ][ a ] );
			Degree[ i ]++;
			DegreeV1[ i ]++;
		}
		if( a > b )
			ByPassed[ i ][ a ] = -1;
		else ByPassed[ i ][ b ] = -1;
	}
	else {
		if( Solution[ a ] == 0 ){
		if( a > b )
			Graph[ a ][ b ]--;
		else Graph[ b ][ a ]--;
		Graph[ a ][ i ]++;
		//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", a, i, a, i, Graph[ a ][ i ] );
		Degree[ i ]++;
		DegreeV1[ i ]++;
		}
		if( a > b )
			ByPassed[ a ][ i ] = -1;
		else ByPassed[ b ][ i ] = -1;
	}
	
	if( i > b ){
		Graph[ i ][ b ]++;
		//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", i, b, i, b, Graph[ i ][ b ] );
		Degree[ i ]++;
	}
	else {
		Graph[ b ][ i ]++;
		//printf( "Returning edge <%d,%d>, Graph[ %d ][ %d ] = %d\n", b, i, b, i, Graph[ b ][ i ] );
		Degree[ i ]++;
	}
	DegreeV1InV2[ a ]--;
	DegreeV1[ a ]++;
	DegreeV1InV2[ i ]++;
	//if( a > b )
		//printf( "Graph[ %d ][ %d ] = %d\n", a, b, Graph[ a ][ b ] );
	//else printf( "Graph[ %d ][ %d ] = %d\n", b, a, Graph[ b ][ a ] );
}

void PrintGraph(){
	int i, j;
	for( i = 0; i < maxnodes; i++ ){
		for( j = 0; j < i; j++ )
			printf( "%d ", Graph[ i ][ j ] );
		printf( "\n" );
	}
}

int MaxDegree(){
   int i, high = Degree[ 0 ], index = 0;
   for ( i = 1; i < maxnodes; i++ )
	   if ( Degree[ i ] > high ){
			high = Degree[ i ];
			index = i;
	   }
   return index;
}

int FVSMaxDegreeHeuristic(){
	int vertex, k = 0;
	vertex = MaxDegree();
	Remove( vertex );
	Solution[ vertex ] = 1;
	k++;
	ConnectedComponents();
	while( result == 0 ){
		vertex = MaxDegree();
		Remove( vertex );
		Solution[ vertex ] = 1;
		k++;
		Initialize();
		ConnectedComponents();
	}
	return k;
}

void InitializeGraph(){
	int i, j;
	for ( i = 0; i < maxnodes; i++ )
		for ( j = 0; j < i; j++ ){
			Graph[ i ][ j ] = RGraph[ i ][ j ];
			ByPassed[ i ][ j ] = -1;
		}
	for( i = 0; i < maxnodes; i++ ){
		Degree[ i ] = 0;
		DegreeV1[ i ] = 0;
		DegreeV1InV2[ i ] = 0;
	}
}

int IntToBinary( int *a, int number, int k ){
    int i = 0, q = 1, r, index = 0, num;

	index = 0;
	q = 1;
	num = number;
	for ( r = 0; r < k; r++ )
		a[ r ] = 0;
	while( q != 0 ){
		q = num / 2;
		r = num % 2;
		a[ index++ ] = r;
		num = q;
		if( r == 1 )
			i++;
	}
	return i;
}

void ReturnAndPlaceInV1( int i ){
	int h;
	for( h = 0; h < i; h++ )
		if( Graph[ i ][ h ] >= 1 )
			if( DegreeV1[ h ] == -1 )
				DegreeV1InV2[ i ] += Graph[ i ][ h ];
			else if( DegreeV1[ h ] >= 0 ){
				DegreeV1[ h ] += Graph[ i ][ h ];
				DegreeV1InV2[ h ] -= Graph[ i ][ h ];
			}
	for( h = i + 1; h < maxnodes; h++ )
		if( Graph[ h ][ i ] >= 1 )
			if( DegreeV1[ h ] == -1 )
				DegreeV1InV2[ i ] += Graph[ h ][ i ];
			else if( DegreeV1[ h ] >= 0 ){
				DegreeV1[ h ] += Graph[ h ][ i ];
				DegreeV1InV2[ h ] -= Graph[ h ][ i ];
			}
	DegreeV1[ i ] = Degree[ i ] - DegreeV1InV2[ i ];
}