# IP_steganography
A tool that encodes text in images without distorting the image. Makes use of the zlib library to compress the text as as to increase the encoding capacity of the image.

Please install zlib from https://github.com/madler/zlib or www.zlib.net before using this.

Usage:

Encode takes an image as argument and and the text as input. It compresses the text, encodes in into the image and outputs the encoded image.
To Encode : ./encode image_name < input_text_file 

Decode takes an encoded image as input and outputs te encoded text.
To execute : ./decode image_name
