/*************************************************************************************************************************************
** Name                 : Rajesh jonnalgadda.
** Date                 : //2024.
** 
** @File	            : encode.h
** Description 			: This file consist of functions declarations for each and every operations used for Encoding  process.
** 
**
***********************************************************************************************************************************/

#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
- * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)  //using the above macro MAX_SECRET_BUF_SIZE
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;  // to store the name of the sorce file 
    FILE *fptr_src_image; //store address of the source file 

#if 0   
    uint image_capacity;  // 
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];  //using the macro MAX_IMAGE_BUF_SIZE and the size is 8 
#endif

    /* Secret File Info */
    char *secret_fname; 			 // store address of the secrete file 
    FILE *fptr_secret;  			//


    char extn_secret_file[MAX_FILE_SUFFIX]; 	// store the secrete file extension
    char secret_data[MAX_SECRET_BUF_SIZE];     //  store the secrete data  
    long size_secret_file; 					  //   store teh size of the secrete file 
    long extn_size;   			   // length of the extension
    
    /* Stego Image Info */
    char *stego_image_fname;   // o/p file name
    FILE *fptr_stego_image;   //  o/p file pointer address

} EncodeInfo;


/* Encoding function prototype */


/*check proper arguments given or not*/
void Invalid_no_of_arguments();

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo,int Source_Image_capacity);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(EncodeInfo *encInfo);

/* Store Magic String */
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo);

/* Encode magic string size */
Status encode_magic_string_size(int magic_String_size,EncodeInfo *encInfo);

/* Encode secret file extenstion */
Status encode_secret_file_extn(EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode the secret file extension size */
Status encode_secret_file_extn_size(int size,EncodeInfo *encInfo);


/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);


/* Encode size to lsb */
Status encode_size_to_lsb(long data,char *image_buffer);


/* Encode function, which does the real encoding */
Status encode_string_to_image(char *data, int size, EncodeInfo *encInfo);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(EncodeInfo *encInfo);


Status string_bitwise(char *buffer,const char string);

#endif
