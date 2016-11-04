#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <bits/stdc++.h>
#include "zlib.h"

using namespace cv;
using namespace std;
char terminate_ch = '#';
char b[1000000];

int poww(int a,int r)
{
	return (int)pow(a,r);
}
vector<int> generatebits(char c)
{
	int len = 8;
	vector<int> O(len,0);
	int asciival=128+(int)c;
	// cout<<asciival<<" ";
	vector<int>bit(len,0);
	for(int i=0;i<len;i++)
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

	for(int j=0,i=len-1;i>=0;j++,i--)
	{
	    //Assigning the bits one by one.
	    O[j]=bit[i];
	    // cout<<O[j];  
	}
	return O;
}
uInt compressString(char a[])
{
	uInt ret;
	z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)strlen(a)+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)a; // input char array
    defstream.avail_out = (uInt)sizeof(b); // size of output
    defstream.next_out = (Bytef *)(b); // output char array
    //cout<<defstream.next_out<<endl;    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
    (ret)=(uInt)((char*)defstream.next_out - (b));
    return ret;
}
void encode(string s,Mat &img,int &i,int &j,int &k,int avail,bool term)
{
	// cout<<s<<"."<<endl;
	if(term==true && s.length()==0)
		return;
	if(s.length()==0 && term == false)
	{
		string term = "";
		term+= terminate_ch;
		encode(term,img,i,j,k,avail,true);
		encode(term,img,i,j,k,avail,true);
		term = "";
		term+= (char)(avail%100);
		avail/=100;
		term+= (char)(avail%100);
		avail/=100;
		term+= (char)(avail%100);
		encode(term,img,i,j,k,avail,true);
		return;
	}
	vector<int> bits = generatebits(s[0]);
	// cout<<s[0] <<" ";
	// for(int p=0;p<bits.size();p++)
	// 	cout<<bits[p];
	// cout<<"\n";
	for(int p=0;p<bits.size();p++)
	{
		if(k==9)
		{
			if(j==(img.cols-1))
			{
				j=0;
				i++;
				k=0;
			}
			else
			{
				k=0;
				j++;
			}
		}
		if(k<3)
			img.at<Vec3b>(i,j)[0]|=(bits[p]==1?poww(2,k):0);
		else if(k<6)
			img.at<Vec3b>(i,j)[1]|=(bits[p]==1?poww(2,k-3):0);
		else
			img.at<Vec3b>(i,j)[2]|=(bits[p]==1?poww(2,k-6):0);
		k++;
	}
	encode(s.substr(1),img,i,j,k,avail,term);
}
int main( int argc, char** argv )
{
	Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   
    if(! image.data )  
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    int bits= 8;
    int totbits = image.rows*image.cols*9;
    int noofchars = totbits/bits;
    string input;
    int mask = 248;
    for(int i=0;i<image.rows;i++)
    	for(int j=0;j<image.cols;j++)
    	{
    		image.at<Vec3b>(i,j)[0]&= mask;
    		image.at<Vec3b>(i,j)[1]&= mask;
    		image.at<Vec3b>(i,j)[2]&= mask;
    	}
    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );
   	char a[1000000];
   	scanf("%[^\n]s",a);// = "Hello jwd wed wed we dw ed we D XEX DXW wedwed wed wedwdDX a";   
    int aa=0,bb=0,cc=0;
    strcpy(b,"");
	printf("Uncompressed size is: %lu\n", strlen(a));
    // printf("Uncompressed string is: %s\n", a);
	uInt ret= compressString(a);
	cout<< "the ret value is "<<ret<<endl;
	uInt compressedLength = strlen(b);
	printf("Compressed size is: %lu\n", strlen(b));
    printf("Compressed string is: %s\n", b);
    if((compressedLength+1+3)> noofchars)
    {
    	printf("Given image is too small to encode the given amount of data\n");
    	return 0;
    }
    // double perc = strlen(a)-str(len
    // printf("")
    encode(string(b),image,aa,bb,cc,ret,false);
    namedWindow( "Encoded Image", WINDOW_AUTOSIZE );
    imshow( "Encoded Image", image );
    imwrite("encoded.png",image);
}