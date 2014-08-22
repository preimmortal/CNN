#include "mnist_parse.h"

typedef struct{
	unsigned int magic_num;
	unsigned int num_items;
	unsigned int num_rows;
	unsigned int num_cols;
}mnist_header;

int parse_mnist(void){
	printf("Parsing MNIST Dataset\n");

	parse_mnist_images();
	
	return 0;
}

//Endian Reverse Function
unsigned int reverse_endian(unsigned int value){
	return ((value>>24)&0x000000ff)
		|((value<<24)&0xff000000)
		|((value>>8)&0x0000ff00)
		|((value<<8)&0x00ff0000);

}

int parse_mnist_images(void){
	FILE *train_images_fp;
	unsigned int magic_num, num_items, num_rows, num_cols;
	train_images_fp = fopen("../datasets/train-images.idx3-ubyte", "r");

	//Read Header
	fread(&magic_num, 1, 4, train_images_fp);
	fread(&num_items, 1, 4, train_images_fp);
	fread(&num_rows, 1, 4, train_images_fp);
	fread(&num_cols, 1, 4, train_images_fp);

	//Reverse Endianess if Little Endian
	#ifdef MNIST_LITTLE_ENDIAN
	magic_num = reverse_endian(magic_num);
	num_items = reverse_endian(num_items);
	num_rows = reverse_endian(num_rows);
	num_cols = reverse_endian(num_cols);
	#endif

	printf("Magic_Number: %d\nNum_Items: %d\nNum_Rows: %d\nNum_Cols: %d\n", magic_num, num_items, num_rows, num_cols); 
	return 0;
}




