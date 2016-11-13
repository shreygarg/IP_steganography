#include <stdio.h>
#include <string.h>  // for strlen
#include <assert.h>
#include "zlib.h"
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

// char b[1000000];
char c[1000000];

vector<int> generatebits(char m)
{
	vector<int> O(8,0);
	int asciival=128+(int)m;
	cout<<asciival<<" ";
	vector<int>bit(8,0);
	for(int i=0;i<8;i++)
	{
		if(asciival!=0)
		{
			bit[i]=asciival%2;
			// cout<<bit[i];
	    	asciival=asciival/2;
	    }
	    else
	    	break;
	}

	for(int j=0,i=7;i>=0;j++,i--)
	{
	    //Assigning the bits one by one.
	    O[j]=bit[i];
	    cout<<O[j];
	}
	return O;
}

char generatechar(vector<int> bit)
{
	int n=1;
	int val=-128;
	for(int i=7;i>=0;i--)
	{
		val+=bit[i]*n;
		n=n*2;
	}
	return (char)(val);
}

// uInt compressString(char a[])
// {
// 	uInt ret;
// 	z_stream defstream;
//     defstream.zalloc = Z_NULL;
//     defstream.zfree = Z_NULL;
//     defstream.opaque = Z_NULL;
//     // setup "a" as the input and "b" as the compressed output
//     defstream.avail_in = (uInt)strlen(a)+1; // size of input, string + terminator
//     defstream.next_in = (Bytef *)a; // input char array
//     defstream.avail_out = (uInt)sizeof(b); // size of output
//     defstream.next_out = (Bytef *)(b); // output char array
//     //cout<<defstream.next_out<<endl;    // the actual compression work.
//     deflateInit(&defstream, Z_BEST_COMPRESSION);
//     deflate(&defstream, Z_FINISH);
//     deflateEnd(&defstream);
//     (ret)=(uInt)((char*)defstream.next_out - (b));
//     return ret;
// }

void decompressString(char b[], uInt ret)
{
	z_stream infstream;
    infstream.zalloc = Z_NULL;
    infstream.zfree = Z_NULL;
    infstream.opaque = Z_NULL;
    // setup "b" as the input and "c" as the compressed output
    infstream.avail_in = (uInt)(ret); // size of input
    infstream.next_in = (Bytef *)b; // input char array
    infstream.avail_out = (uInt)sizeof(c); // size of output
    infstream.next_out = (Bytef *)(c); // output char array
    // the actual DE-compression work.
    inflateInit(&infstream);
    inflate(&infstream, Z_NO_FLUSH);
    inflateEnd(&infstream);
}

int main()
{
    freopen("o1.txt","w",stdout);
	// char a[1000000] = "She was Lo, plain Lo, in the morning, standing four feet ten in one sock. She was Lola in slacks. She was Dolly at school. She was Dolores on the dotted line. But in my arms she was always Lolita."; 
	// strcpy(b,"");
	// strcpy(c,"");
 //    printf("Uncompressed size is: %lu\n", strlen(a));
 //    printf("Uncompressed string is: %s\n", a);


 //    printf("\n----------\n\n");

 //    // STEP 1.
 //    // deflate a into b. (that is, compress a into b)
    
 //    uInt ret=compressString(a);
 //    cout<<"Ret calue is "<<ret<<endl;
 //    // This is one way of getting the size of the output
 //    printf("Compressed size is: %lu\n", strlen(b));
 //    printf("Compressed string is: %s\n", b);
 //    unsigned char x=(b[5]);
    char b[1000000] = "xÚUM\nƒ@…{”w";
    // STEP 2.
    // inflate b into c;
    // zlib struct
    decompressString(b,133);

    printf("Uncompressed size is: %lu\n", strlen(c));
    printf("Uncompressed string is: %s\n", c);
    
    // make sure uncompressed is exactly equal to original.
    // assert(strcmp(a,c)==0);

	return 0;
}