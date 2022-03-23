#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#include <stdio.h>

#include "color_struct.h"
#include "key.h"

#define NO_ERROR 0
#define TRUE 1
#define BUFLEN 16
#define NBITEM 256
#define ZeroMemory(a,b) memset(a,0,b)



int main(int argc, char const *argv[])
{
	int iResult = 0, iCounter = 0, iFileDescriptor = 0 ;
	char szBuffer[BUFLEN] = {0} , szFilename[] = "items.txt" ;
	char* cCurrentChar = szBuffer ;

	struct __icon__ icItem[NBITEM] ;
	struct __icon__* icCurrentIcon = icItem ;
	ZeroMemory( icItem , ICONSIZE*NBITEM ) ;


	iFileDescriptor = open( szFilename , O_RDONLY ) ;
	if( iFileDescriptor < NO_ERROR )
		return errno ;


	do
	{
		iResult = read( iFileDescriptor , cCurrentChar , TRUE ) ;

		if( *(cCurrentChar++) < KEY_SPACE )
		{
			cCurrentChar-- ;
			
			while( --cCurrentChar >= szBuffer )
			{
				if( *cCurrentChar > KEY_SPACE )
					if( *(cCurrentChar+1) > KEY_SPACE )
						iCounter = *cCurrentChar - KEY_0 ;
					else
						iCounter += (*cCurrentChar - KEY_0)*10 ;

				else
				{
					if( icCurrentIcon+1 != icItem+NBITEM )
						(*(icCurrentIcon++)).color |= 1 << iCounter ;
					else
						(*icCurrentIcon).color |= 1 << iCounter ;
				}
					
			}

			cCurrentChar++ ;

			ZeroMemory( szBuffer , BUFLEN ) ;
		} 

	} while( iResult > NO_ERROR ) ;

	if( iResult < NO_ERROR )
		return errno ;

	iResult = 24 ;

	while( iResult-- )
		printf("%d  :  %d\n", iResult , icItem[0].color>>iResult & 1 ) ;

	iResult = close( iFileDescriptor ) ;
	if( iResult != NO_ERROR )
		return errno ; 


	/*int iTest = 0 ;
	iTest |= 1 << 1 ;
	iTest |= 1 << 0 ;
	iTest |= 1 << 0 ;
	iTest |= 1 << 17 ;*/

	

	return 0 ;
}