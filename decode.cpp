#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <bits/stdc++.h>
#include "zlib.h"

using namespace cv;
using namespace std;
char terminate_ch = '#';
char c[1000000];
int poww(int a,int r)
{
	return (int)pow(a,r);
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
	char prev=ch;
	while(ch!=terminate_ch || prev!=terminate_ch)
	{
		prev=ch;
		ch = getch(img,i,j,k);
		if(ch!= terminate_ch || prev!=terminate_ch)
			hiddenstr += ch;
	}
	char ch1 = getch(img,i,j,k);
	char ch2 = getch(img,i,j,k);
	char ch3 = getch(img,i,j,k);
	// cout<< ch<<endl;
	avail = (uInt)ch1 + 100*(uInt)ch2 + 10000*(uInt)ch3;
	return hiddenstr.substr(0,hiddenstr.length()-1);
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
    strcpy(c,"");
	int aa=0,bb=0,cc=0;
    uInt extracted=5;
    string tmp = decode(image,aa,bb,cc,extracted);
    cout<<tmp.length();
    char tmpc[1000000];
    strcpy(tmpc,tmp.c_str());
    decompressString(tmpc,extracted);
    cout<< "the ret value is "<<extracted<<endl;
    cout<<tmp<<endl;
    printf("Uncompressed size is: %lu\n", strlen(c));
    printf("Uncompressed string is: %s\n", c);
    waitKey(0);
	return 0;
}