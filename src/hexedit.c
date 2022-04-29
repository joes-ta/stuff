#include <stdio.h>
#include <stdbool.h>

struct data {
	unsigned char buffer[ 256 ];
	int offset;
	char filename[81];
	FILE *file;
};

typedef struct data Data;

Data data;

int menu( void );
void display( void );
void file( void );
void next( void );
void previous( void );

int main( void ) {
	int choice = 0;
	bool done = false;
	data.offset = 0;
	data.file = NULL;
	do { 
		choice = menu( );
		switch( choice ) {
			case 'F':
			case 'f':
				file( );
				break;
			case 'N':
			case 'n':
				next( );
				break;
			case 'P':
			case 'p':
				previous( );
				break;
			case 'Q':
			case 'q':
				done = true;
				printf( "bye.\n" );
				break;
			default: 
				printf( "Invalid Choice" );
		}
		if( !done ) display( );
	} while( !done );
	
	return 0;
}

int menu( void ) {
	printf( "\n[F]ile, [N]ext, [P]revious, [Q]uit\n>>>" );
	char choice, junk[80];
//	scanf( "%c%*[^\n]", &choice ); scanf( "%*c" );
//	choice = getchar( ); getchar( );
//	return choice;
	fgets( junk, 80, stdin );
	return junk[0];
// fgets( ) reads newline character
// getchar( ) leaves newline in buffer
}

void display( void ) {
	printf( "\n\n\n          " );
	for( int col = 0; col < 16; ++col ) printf( "%4.2x", col );
	printf( " | " );
	for( int col = 0; col < 16; ++col ) printf( "%x", col );
	printf( "\n          " );
	for( int col = 0; col < 83; ++col ) printf( "%c", '-' );
	printf( "\n" );
	for( int row = 0; row < 16; ++row ) {
		printf( "%08x |", data.offset + row * 16 );
		for( int col = 0; col < 16; ++col ) {
			printf( "%4.2x", data.buffer[ row * 16 + col ] );
		}
		printf( " | " );
		for( int col = 0; col < 16; ++col ) {
			if( data.buffer[ row * 16 + col ] < 32 ) printf( "." );
			else printf( "%c", data.buffer[ row * 16 + col ] );
		}
		printf( "\n" );
	}
	
}

void file( void ) {
	for( int i = 0; i < 256; ++i ) data.buffer[ i ] = 0;
	do {
		printf( "Filename [%s]: ", data.filename );
		scanf( "%80[^ \n]", data.filename );
		scanf( "%*[^\n]" ); scanf( "%*c" );
	
		data.file = fopen( data.filename, "rb" );
		if( data.file == NULL ) {
			printf( "Create %s ? [y/n] : ", data.filename );
			char c;
			scanf( "%c", &c );
			scanf( "%*[^\n]" ); scanf( "%*c" );
			if( c == 'y' || c == 'Y' ) data.file = fopen( data.filename, "w" );
		}
	} while( data.file == NULL );
	
	fread( data.buffer, 1, 256, data.file );
	fclose( data.file );
}

void next( void ) {
	data.file = fopen( data.filename, "rb" );
	data.offset += 0x100;
//	fseek( data.file, data.offset, SEEK_SET ); // doesn't handle multibyte characters
	int c, off = 0;
	for( int i = 0; i < data.offset; ++i ) fgetc( data.file );
	for( int i = 0; i < 256; ++i ) {
		c = fgetc( data.file );
		if( c != EOF ) data.buffer[ off ] = c; else data.buffer[ off ] = 0;
		off++;
	}
//	fread( data.buffer, 1, 256, data.file );
	fclose( data.file );
}

void previous( void ) {
	data.file = fopen( data.filename, "rb" );
	if( data.offset >= 0x100 ) data.offset -= 0x100;
//	fseek( data.file, data.offset, SEEK_SET ); // doesn't handle multibyte characters
	for( int i=0; i<data.offset; ++i ) fgetc( data.file );
	fread( data.buffer, 1, 256, data.file );
	fclose( data.file );
}