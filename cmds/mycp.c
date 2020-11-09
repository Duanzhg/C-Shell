#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define COPYMODE 0644
#define BUF 4096



int main(int argc, char *argv[])
{

    int in_fd = -1, out_fd = -1;
    char n_chars[BUF];
    int len = 0;
    
    if( ( in_fd = open( argv[1], O_RDONLY ) ) == -1 ) {
        perror( "file open" );
        exit( -1 );
    }

    if ( ( out_fd = creat( argv[2], COPYMODE ) ) == -1 ) {
        perror( "file copy" );
        exit( -1 );
    }
    
    
    while( ( len = read( in_fd, n_chars, sizeof( n_chars ) ) ) > 0 ) {
        if ( write( out_fd, n_chars, len ) != len ) {
            printf( "file:%s copy failed\n", argv[2] );
            exit( -1 );
        }
    }
    if( len == -1 ) {
        printf( "read %s failed\n", argv[1] );
        exit( -1 );
    }
    
    if( close( in_fd ) == -1 ) {
        printf( "%s close failed \n", argv[1] );
        exit( -1 );
    }
    if( close( out_fd ) == -1 ) {
        printf( "%s close failed \n", argv[2] );
        exit( -1 );
    }

    return 0;
}


