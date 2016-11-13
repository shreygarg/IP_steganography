#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <bits/stdc++.h>
#include "zlib.h"

using namespace cv;
using namespace std;
char terminate_ch = '#';
char b[1000000];
char c[1000000];
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

int poww(int a,int r)
{
	return (int)pow(a,r);
}
void encode(string s,Mat &img,int &i,int &j,int &k,int avail,bool term)
{
	// cout<<s<<"."<<endl;
	if(term==true  && s.length()==0)
		return;
	if(s.length()==0 && term == false)
	{
		string term = "";
		term+= terminate_ch;
		encode(term,img,i,j,k,avail,true);
		term = "";
		term = 

		term+= (char)(avail%100);
		avail/=100;
		term+= (char)(avail%100);
		avail/=100;
		term+= (char)(avail%100);
		encode(term,img,i,j,k,avail,true);
		return;
	}
	vector<int> bits = generatebits(s[i]);
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
char getch(Mat img,int &i,int &j,int &k)
{
	vector<int> bits;
	for(int p=0;p<8;p++)
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
			bits.push_back(img.at<Vec3b>(i,j)[0]&poww(2,k)&7);
		else if(k<6)
			bits.push_back(img.at<Vec3b>(i,j)[1]&poww(2,k-3)&7);
		else
			bits.push_back(img.at<Vec3b>(i,j)[2]&poww(2,k-6)&7);
		k++;
		if(bits[p]>0)
			bits[p]=1;
	}
	// for(int p=0;p<bits.size();p++)
	// 	cout<<bits[p];
	return generatechar(bits);
}
string decode(Mat img,int &i,int &j,int &k,uInt &avail)
{
	char ch = 'a';
	string hiddenstr = "";
	while(ch!=terminate_ch)
	{
		fflush(stdout);
		ch = getch(img,i,j,k);
		if(ch!= terminate_ch)
			hiddenstr += ch;
	}
	char ch1 = getch(img,i,j,k);
	char ch2 = getch(img,i,j,k);
	char ch3 = getch(img,i,j,k);
	// cout<< ch<<endl;
	avail = (uInt)ch1 + 100*(uInt)ch2 + 10000*(uInt)ch3;
	return hiddenstr;
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
	strcpy(c,"");
	printf("Uncompressed size is: %lu\n", strlen(a));
    printf("Uncompressed string is: %s\n", a);
	uInt ret= compressString(a);
	uInt compressedLength = ret;//strlen(b);
	printf("Compressed size is: %lu\n", strlen(b));
    printf("Compressed string is: %s\n", b);
    if((compressedLength+1+3)> noofchars)
    {
    	printf("Given image is too small to encode the given amount of data\n");
    	return 0;
    }
    string tobeencoded = "";
    for(int i=0;i<ret;i++)
    {
    	tobeencoded+= b[i];
    }
    encode(tobeencoded,image,aa,bb,cc,ret,false);
    namedWindow( "Encoded Image", WINDOW_AUTOSIZE );
    imshow( "Encoded Image", image );
    aa=0,bb=0,cc=0;
    uInt extracted=5;
    imwrite("encoded.png",image);
    image = imread("encoded.png", CV_LOAD_IMAGE_COLOR);
    string tmp = decode(image,aa,bb,cc,extracted);
    char tmpc[1000000];
    for(int i=0;i<extracted;i++)
    	tmpc[i]=tmp[i];
    // strcpy(tmpc,tmp.c_str());
    decompressString(tmpc,extracted);
    printf("Uncompressed size is: %lu\n", strlen(c));
    printf("Uncompressed string is: %s\n", c);
    waitKey(0);
    return 0;
}