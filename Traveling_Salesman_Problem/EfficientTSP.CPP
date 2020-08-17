#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef struct Record{
	int data;
	struct Record *next;
	struct Record *previous;
}QueueAdt;

typedef struct record{
	QueueAdt *head;
	QueueAdt *tail;
}*Queue;

int maxnodes;
int *Degree;
int **Graph;
int **MST;
int e;
char *visited;
int *index;


void Enqueue( Queue, int );
void printQueue( Queue );
int IsEmpty( Queue );
void DequeueFromRight( Queue );
int TSBranch( int, Queue, int, int, int );
void InsertInHeap( int *, int, int );
int PickFromHeap( int * );
void MinSpanTree( int *, int * );
int Father( int, int * );
int FindMin( int );
int Indexing( int *, int *, int );

int main (){
	int i, s = 0, j, *Heap, u, v, w, k, result, position, count = 1, cost = 0;
	int *a, v1, v2;
	FILE *ptr;
	Queue queue;
	queue -> head = queue -> tail = NULL;
	char filename[ 30 ];
	clrscr();
	printf( "Enter the filename that contains the Graph: " );
    scanf( "%s", filename );
    printf( "\nEnter the vertex number from which you would like to start: " );
    scanf( "%d", &position );
    printf( "\nEnter a cost that you suppose is the minimum cost: " );
    scanf( "%d", &k );

	if ( ( ptr = fopen( filename, "r" ) ) == NULL ){
		printf( "file couldnot be opened" );
		return 0;
	}
	else{

	fscanf( ptr, "%d", &maxnodes );
	e = ( maxnodes * ( maxnodes - 1 ) ) / 2;

	if ( ( Graph = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL ){
      printf( "Out of memory\n" );
	  return 0;
	}
   else
      for ( i = 1; i < maxnodes; i++ ){
	 Graph[ i ] = ( int * ) malloc ( i * sizeof( int ) );
	 if ( Graph[ i ] == NULL ){
	    printf( "Out of memory\n" );
		return 0;
	 }
      }

	if ( ( Degree = ( int * ) malloc ( maxnodes * sizeof( int ) ) ) == NULL ){
		printf( "No Memory\n" );
	    return 0;
	}

	if ( ( index = ( int * ) malloc ( maxnodes * sizeof( int ) ) ) == NULL ){
		printf( "No Memory\n" );
	    return 0;
	}

    if ( ( visited = ( char * ) malloc ( maxnodes * sizeof( char ) ) ) == NULL ){
		printf( "No Memory\n" );
	    return 0;
	}

	if ( ( a = ( int * ) malloc ( maxnodes * sizeof( int ) ) ) == NULL ){
		printf( "No Memory\n" );
	    return 0;
	}

	if ( ( Heap = ( int * ) malloc ( e * sizeof( int ) ) ) == NULL ){
		printf( "No Memory\n" );
	    return 0;
	}

	for ( i = 0; i < maxnodes; i++ ){
		Degree[ i ] = 0;
		index[ i ] = 0;
		visited[ i ] = '0';
		a[ i ] = -1;
	}

	for ( i = 0; i < e; i++ )
		Heap[ i ] = -1;

	for ( i = 0; i < maxnodes; i++ )
     for ( j = 0; j < i; j++ )
	 Graph[ i ][ j ] = -1;

   while ( !feof( ptr ) ){
      fscanf( ptr, "%d%d%d", &u, &v, &w );
      Graph[ v ][ u ] = w;
      if ( feof( ptr ) )
	 break;
   }

   for ( i = 1; i <= e; i++ )
	   InsertInHeap( Heap, s++, i );

	MinSpanTree( Heap, a );

	if ( ( MST = ( int ** ) malloc ( maxnodes * sizeof( int * ) ) ) == NULL ){
		printf( "no memory\n" );
		return 0;
	}

	for ( i = 0; i < maxnodes; i++ ){
		MST[ i ] = ( int * ) malloc ( Degree[ i ] * sizeof( int ) );
		if ( MST[ i ] == NULL ){
			printf( "no memory\n" );
			return 0;
		}
	}


	for ( i = 0; i < maxnodes; i++ )
		if ( a[ i ] != -1 ){
		MST[ i ][ index[ i ] ] = a[ i ];
		MST[ a[ i ] ][ index[ a[ i ] ] ] = i;
		index[ i ]++;
		index[ a[ i ] ]++;
		}

		  printf( "\n\n" );

   Enqueue( queue, position );
   visited[ position ] = '1';

   result = TSBranch( k, queue, position, count, cost );

   if ( result == 1 ){
      printf( "\n\nThe way is:\n\n" );
      printQueue( queue );
   }
   else printf ( "\nThe Travel is impossible due to the entered cost\n" );

   }
   getch();
	return 0;
}

void InsertInHeap( int *Heap, int i, int number ){
	int v1, v2, value;

	if ( i == 0 ){
		Heap[ i ] = number;
		return;
	}

	Heap[ i ] = number;
	value = Indexing( &v1, &v2, number );

	while ( value < Indexing( &v1, &v2, Heap[ ( i - 1 )/2 ] ) ){
		Heap[ i ] = Heap[ ( i - 1 )/2 ];
		Heap[ ( i - 1 )/2 ] = number;
		i = ( i - 1 )/2;
		if ( i == 0 )
			return;
	}
}

int PickFromHeap( int *Heap ){
	int temp, hold, i = 0, v1, v2;
	static int size = e - 1;
	temp = Heap[ 0 ];
	if ( size < 0 ){
		printf( "No more Elements in the Heap\n" );
		return -1;
	}
	if ( size == 0 ){
		size--;
		return temp;
	}

	hold = Heap[ size ];

	Heap[ size ] = Heap[ 0 ];

	if ( size == 1 ){
		Heap[ i ] = hold;
		size--;
		return temp;
	}
	if ( size == 2 ){

		if ( Indexing( &v1, &v2, hold ) >= Indexing( &v1, &v2, Heap[ 1 ] ) ){

			Heap[ i ] = Heap[ 1 ];

			Heap[ 1 ] = hold;

			size--;
			return temp;
		}
		else {
			Heap[ i ] = hold;
			size--;
			return temp;
		}
	}
	while( 1 ){

		if ( Indexing( &v1, &v2, Heap[ 2 * i + 1 ] ) <= Indexing( &v1, &v2, Heap[ 2 * i + 2 ] ) ){
			if ( Indexing( &v1, &v2, hold ) < Indexing( &v1, &v2, Heap[ 2 * i + 1 ] ) )
				break;
			Heap[ i ] = Heap[ 2 * i + 1 ];
			i = 2 * i + 1;
			if ( 2 * i + 1 >= size )
				break;
			if ( 2 * i + 2 == size ){
				if ( Indexing( &v1, &v2, hold ) < Indexing( &v1, &v2, Heap[ 2 * i + 1 ] ) )
					break;
				Heap[ i ] = Heap[ 2 * i + 1 ];
				i = 2 * i + 1;
				break;
			}
		}
		else {
			if ( Indexing( &v1, &v2, hold ) < Indexing( &v1, &v2, Heap[ 2 * i + 2 ] ) )
				break;
			Heap[ i ] = Heap[ 2 * i + 2 ];
			i = 2 * i + 2;
			if ( 2 * i + 1 >= size )
				break;
			if ( 2 * i + 2 == size ){
				if ( Indexing( &v1, &v2, hold ) < Indexing( &v1, &v2, Heap[ 2 * i + 1 ] ) )
					break;
				Heap[ i ] = Heap[ 2 * i + 1 ];
				i = 2 * i + 1;
				break;
			}
		}
	}
	Heap[ i ] = hold;
	size--;
	return temp;
}

void MinSpanTree( int *Heap, int *a ){
	int i, n = 0, k = 0, hold, Ohold, temp, u, v, v1, v2;

	while ( k < e ){
		Indexing( &v1, &v2, Heap[ 0 ] );
		u = Father( v1, a );
		v = Father( v2, a );
		if ( a[ v2 ] == -1 && u != v ){
			a[ v2 ] = v1;
			Degree[ v2 ]++;
			Degree[ v1 ]++;
			n++;
		}
		else
			if ( a[ v2 ] != -1 && u != v ){
				if ( a[ v1 ]!= -1 ){

					hold = v1;
					Ohold = hold;
					hold = a[ hold ];
					a[ Ohold ] = v2;

					while ( a[ hold ] != -1 ){
						temp = a[ hold ];
						a[ hold ] = Ohold;
						Ohold = hold;
						hold = temp;
					}

					    a[ hold ] = Ohold;

					}

				else
					a[ v1 ] = v2;

				Degree[ v2 ]++;
			    Degree[ v1 ]++;
				n++;
			}
		PickFromHeap( Heap );
		if ( n == maxnodes - 1 )
			return;
		k++;
	}
}

int Father( int u, int *a ){
	int i;
	i = a[ u ];
	if ( i == -1 )
		return u;
	return Father( i, a );
}

int TSBranch( int k, Queue queue, int position, int count, int cost ){

   int j = 0, Oldposition, flag = 1, d;

   if ( cost <= k && count == maxnodes )
      return 1;
   if ( cost >= k )
      return 0;

   if ( Degree[ position ] == 0 ){
	   d = FindMin( position );
       while ( j < maxnodes ) {
		   if ( visited[ j ] == '0' ){
			   if ( position < j ){
				 if ( Graph[ j ][ position ] == d ){
					cost += Graph[ j ][ position ];
					Enqueue( queue, j );
			    	visited[ j ] = '1';
					Oldposition = position;
					position = j;
					count++;
					flag = 1;
					if( TSBranch( k, queue, position, count, cost ) )
						return 1;
					if ( !IsEmpty( queue ) )
						DequeueFromRight( queue );
					count--;
					visited[ j ] = '0';
					if ( position < Oldposition )
						cost -= Graph[ Oldposition ][ position ];
					else cost -= Graph[ position ][ Oldposition ];
					position = Oldposition;
					flag = 0;
                 }
			   }
			   else if ( Graph[ position ][ j ] == d ){
					cost += Graph[ position ][ j ];
					Enqueue( queue, j );
			    	visited[ j ] = '1';
					Oldposition = position;
					position = j;
					count++;
					flag = 1;
					if( TSBranch( k, queue, position, count, cost ) )
						return 1;
					if ( !IsEmpty( queue ) )
						DequeueFromRight( queue );
					count--;
					visited[ j ] = '0';
					if ( position < Oldposition )
						cost -= Graph[ Oldposition ][ position ];
					else cost -= Graph[ position ][ Oldposition ];
					position = Oldposition;
					flag = 0;
                 }
		   }
		 j++;
	   }
   }
   else if ( Degree[ position ] > 1 )
	   while ( j < index[ position ] ){
		   if ( visited[ MST[ position ][ j ] ] == '0' ){
			   if ( MST[ position ][ j ] > position )
				   cost += Graph[ MST[ position ][ j ] ][ position ];
			   else cost += Graph[ position ][ MST[ position ][ j ] ];
			   Enqueue( queue, MST[ position ][ j ] );
			   visited[ MST[ position ][ j ] ] = '1';
			   Degree[ position ]--;
			   Degree[ MST[ position ][ j ] ]--;
			   Oldposition = position;
			   position = MST[ position ][ j ];
			   count++;
			   flag = 1;
			   if( TSBranch( k, queue, position, count, cost ) )
						return 1;
				if ( !IsEmpty( queue ) )
					DequeueFromRight( queue );
				count--;
				visited[ MST[ Oldposition ][ j ] ] = '0';
				if ( position < Oldposition )
					cost -= Graph[ Oldposition ][ position ];
				else cost -= Graph[ position ][ Oldposition ];
				Degree[ position ]++;
				Degree[ Oldposition ]++;
				position = Oldposition;
				flag = 0;
		   }
		   j++;
	   }
   else if ( Degree[ position ] == 1 )
	   while ( j < index[ position ] ){
		   if ( visited[ MST[ position ][ j ] ] == '0' ){
			   if ( MST[ position ][ j ] > position )
				   cost += Graph[ MST[ position ][ j ] ][ position ];
			   else cost += Graph[ position ][ MST[ position ][ j ] ];
			   Enqueue( queue, MST[ position ][ j ] );
			   visited[ MST[ position ][ j ] ] = '1';
			   Degree[ position ]--;
			   Degree[ MST[ position ][ j ] ]--;
			   Oldposition = position;
			   position = MST[ position ][ j ];
			   count++;
			   flag = 1;
			   if( TSBranch( k, queue, position, count, cost ) )
						return 1;
			    if ( !IsEmpty( queue ) )
					DequeueFromRight( queue );
				count--;
				visited[ MST[ Oldposition ][ j ] ] = '0';
				if ( position < Oldposition )
					cost -= Graph[ Oldposition ][ position ];
				else cost -= Graph[ position ][ Oldposition ];
				Degree[ position ]++;
				Degree[ Oldposition ]++;
				position = Oldposition;
				flag = 0;
				return 0;
		   }
		   j++;
	   }
   if ( flag == 1 ){
		Degree[ position ] = 0;
		if( TSBranch( k, queue, position, count, cost ) )
						return 1;
   }
   return 0;
}

void Enqueue( Queue queue, int value ){
	QueueAdt *newptr, *temp;
	if ( IsEmpty( queue ) ){
		if ( ( newptr = ( struct Record * ) malloc ( sizeof ( struct Record ) ) ) == NULL ){
			printf( "no memory\n" );
			return;
		}
		newptr -> data = value;
		newptr -> next = newptr -> previous = NULL;
		queue -> head = queue -> tail = newptr;
		return;
	}
	if ( ( newptr = ( struct Record * ) malloc ( sizeof ( struct Record ) ) ) == NULL ){
		printf( "no memory\n" );
		return;
	}
	newptr -> data = value;
	newptr -> next = newptr -> previous = NULL;
	temp = queue -> tail;
	( queue -> tail ) -> next = newptr;
	queue -> tail = newptr;
    ( queue -> tail ) -> previous = temp;
}

int IsEmpty( Queue queue ){
	return queue -> head == NULL;
}

void printQueue( Queue queue ){
	QueueAdt *current;
	current = queue -> head;
	while ( current != NULL ){
		if ( current -> next == NULL )
		   printf( "%d\n", current -> data );
		else
		   printf( "%d ----> ", current -> data );
		current = current -> next;
	}
}

void DequeueFromRight( Queue queue ){
	QueueAdt *temp;
	temp = queue -> tail;

	if ( ( queue -> tail ) -> previous == NULL )
		queue -> head = NULL;

	queue -> tail = ( queue -> tail ) -> previous;

	if ( queue -> tail != NULL ){
	   ( queue -> tail )-> next = NULL;
	   if ( ( queue -> tail ) -> previous == NULL )
	      ( queue -> head ) -> next = NULL;
	}
	temp -> previous = NULL;
	free( temp );
}

int FindMin( int v ){
	int i, flag = 0, min;
	for ( i = 0; i < maxnodes; i++ ){
		if ( flag == 0 ){
			if ( visited[ i ] == '0' ){
				if ( v > i )
				   min = Graph[ v ][ i ];
				else min = Graph[ i ][ v ];
				flag = 1;
			}
		}
		else if ( visited[ i ] == '0' ){
				if ( v > i ){
					if ( Graph[ v ][ i ] < min )
						min = Graph[ v ][ i ];
				}
				else if ( Graph[ i ][ v ] < min )
						min = Graph[ i ][ v ];
			}
		}
	return min;
}

int Indexing( int *v1, int *v2, int number ){
	int i = 1, hold, h = 0;
	hold = maxnodes - 1;
	while ( maxnodes - h - 1 != 0 ){
		if ( number <= hold ){
			*v1 = h;
			break;
		}
		h++;
		hold = hold + maxnodes - h - 1;
	}
	if ( *v1 == 0 )
		*v2 = number;
	else {
		
	hold = maxnodes - 2;
	while ( i < *v1 ){
		i++;
		hold = hold + maxnodes - i - 1;
	}
	*v2 = number - hold;
	}
	if ( *v1 > *v2 )
		return Graph[ *v1 ][ *v2 ];
	return Graph[ *v2 ][ *v1 ];
}


