#ifndef DECODE_H
#define DECODE_H

#include "types.h"

/*Structure foe Decoded Information*/

typedef struct _DecodeInfo
{

    char *stego_image_fname;
    FILE *fptr_stego_image;

    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[5];

    int magic_string_size;
    int extn_size;
    char extn_output_file[5];
    int secret_data_size;
    
} DecodeInfo;


/*Function Declarations*/

/* Read and validate the decode arguments */
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo);

/* For Decoding the Encoded file*/
Status do_decoding(DecodeInfo *decInfo);

/* Decode the magic_String_size*/
Status decode_magic_string_size(DecodeInfo *decInfo);

/*DEcode size from lsb*/
Status decode_size_from_lsb(int size,char *buffer);

/*DEcode the magic string*/
char decode_magic_string(DecodeInfo *decInfo);

/*Decode byte from lsb*/
char decode_byte_from_lsb(char *buffer);

/*Deode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/*Decode secret file extension*/
char decode_secret_file_extn(DecodeInfo *decInfo);

/*Decode secret date size*/
Status decode_secret_data_size(DecodeInfo *decInfo);

/*Decode secret data*/
Status decode_secret_data(DecodeInfo *decInfo);

#endif