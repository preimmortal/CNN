#include "mnist_parse.h"

//Endian Reverse Function
unsigned int reverse_endian(unsigned int value){
	return ((value>>24)&0x000000ff)
		|((value<<24)&0xff000000)
		|((value>>8)&0x0000ff00)
		|((value<<8)&0x00ff0000);
}

mnist_data *parse_image_file(FILE *image_fp){
	mnist_data *mh = (mnist_data *)malloc(sizeof(mnist_data));

	//Read Header
	fread(&mh->magic_num, 1, 4, image_fp);
	fread(&mh->num_items, 1, 4, image_fp);
	fread(&mh->num_rows, 1, 4, image_fp);
	fread(&mh->num_cols, 1, 4, image_fp);

	//Reverse Endianess if Little Endian
	#ifdef MNIST_LITTLE_ENDIAN
	mh->magic_num = reverse_endian(mh->magic_num);
	mh->num_items = reverse_endian(mh->num_items);
	mh->num_rows = reverse_endian(mh->num_rows);
	mh->num_cols = reverse_endian(mh->num_cols);
	#endif

	printf("------------------------\n");
	printf("Magic_Number: %d\nNum_Items: %d\nNum_Rows: %d\nNum_Cols: %d\n", mh->magic_num, mh->num_items, mh->num_rows, mh->num_cols); 
	
	if(mh->magic_num != 0x0803){
		error("MNUM Incorrect - Check Endian\n");	
		return NULL;
	}

	//Declare Data
	char *data = (char *)malloc(sizeof(char)*mh->num_items*mh->num_rows*mh->num_cols);

	printf("Reading Data..\n");
	int i, j, k;
	for(i=0; i<mh->num_items; i++){
		for(j=0; j<mh->num_rows; j++){
			for(k=0; k<mh->num_cols; k++){
				fread(&data[i*mh->num_rows*mh->num_cols + j*mh->num_cols + k], 1, 1, image_fp);
			}
		}
	}
	printf("Done Reading Data\n");
	mh->data = data;
	printf("------------------------\n");
	return mh;
}

mnist_data *parse_label_file(FILE *label_fp){
	mnist_data *mh = (mnist_data *)malloc(sizeof(mnist_data));
	mh->num_rows = 0;
	mh->num_cols = 0;

	//Read Header
	fread(&mh->magic_num, 1, 4, label_fp);
	fread(&mh->num_items, 1, 4, label_fp);

	//Reverse Endianess if Little Endian
	#ifdef MNIST_LITTLE_ENDIAN
	mh->magic_num = reverse_endian(mh->magic_num);
	mh->num_items = reverse_endian(mh->num_items);
	#endif

	printf("------------------------\n");
	printf("Magic_Number: %d\nNum_Items: %d\n", mh->magic_num, mh->num_items); 
	
	if(mh->magic_num != 0x0801){
		error("MNUM Incorrect - Check Endian\n");	
		return NULL;
	}

	printf("Reading Data..\n");
	char *data = malloc(sizeof(char)*mh->num_items);
	int i;
	for(i=0; i<mh->num_items; i++){
		fread(&data[i], 1, 1, label_fp);
		//printf("data[%d]: %d\n", i, data[i]); 
	}
	mh->data = data;
	printf("Done Reading Data\n");
	printf("------------------------\n");
	return mh;
}


