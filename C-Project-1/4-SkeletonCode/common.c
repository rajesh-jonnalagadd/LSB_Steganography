/*************************************************************************************************************************************
** Name                 : Rajesh jonnalgadda.
** Date                 : //2024.
** 
** @File	        : common.c
** Description 		: This file consist of main program for choose which operation we want.
** 
**
***********************************************************************************************************************************/

#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"
#include "common.h"
#include "colour.h"

int main(int argc, char **argv)
{

	//Struct Declaration	
	EncodeInfo encInfo;
	DecodeInfo decInfo;
	//Checking operation type Encoding or Decoding 
	if(argc >1 && argc<=6)
	{
		// Checking the operation type
		if ((check_operation_type(argv)) == e_encode)
		{	
			if((argc ==2) || (argc ==3))
			{
				Invalid_arguments_for_Encoding();
				return e_failure;
			}

			//Read and validate th no of arguments 
			if(read_and_validate_encode_args(argv, &encInfo) == e_success)
			{
				//Checking the encoding process success or not 
				if(do_encoding(&encInfo) == e_success)
				{
					printf("\nEncoding Done successfully\n");
				}else
				{
					printf("\nError while doing Encoding \n");
					return e_failure;
				}
			}else
			{
				printf("Read and validated failure fro Encoding\n");
				return e_failure;
			}
			
		}else if((check_operation_type(argv)) == e_decode)  //Decode operation
		{
			if((argc == 2))
			{
				Invalid_arguments_for_Decoding();
				return e_failure;
			}

			//printf("\nDecoding procedure starts\n");
			
			if(read_and_validate_decode_args(argv,&decInfo) == d_success)
			{
				if(do_decoding(&decInfo)==d_success)
				{
					printf(BMAG"Decoding completed\n"reset);
				}else
				{
					printf("Error during the decoding\n");
					return d_failure;
				}
			}
		
		}else
		{
			printf(BYEL"\nUnsupported format \n\n"reset);
			Invalid_no_of_arguments();
		}
	}else
	{
		Invalid_no_of_arguments();
	}
				
	return 0;
}

void Invalid_no_of_arguments()
{
	printf("######################################################################################\n");
	printf(BBLU"");
	printf("\nInvalid Arguments count\n");
	printf("\n./lsb_steg: Encoding: ./lsb_steg  -e   < In_image.bmp file >   < Input secret file > [ Op_Image.bmp ]\n");
	printf("\n./lsb_steg: Encoding: ./lsb_steg  -e   < In_image.bmp file >   < INput secret file > \n");
	
	printf("\n./lsb_steg: Decoding: ./lsb_steg  -d   < op_image.bmp file >   [ Output_file ]\n");
	printf("\n./lsb_steg: Decoding: ./lsb_steg  -d   < op_image.bmp file >   \n");
	printf(""reset);
	printf("\n######################################################################################\n");
}
void Invalid_arguments_for_Encoding()
{
	printf("\n###############################################################################\n\n");
	printf(BBLU"");
	printf("\nInvalid arguments for Encoding\n");
	printf("\n./lsbsteg: Encoding: ./lsbsteg -e <.bmp file> <.txt file > [output file] \n");
	printf("\n./lsbsteg: Encoding: ./lsbsteg -e <.bmp file> <.txt file >  \n");
	printf(""reset);
	printf("\n###############################################################################\n");
}
void Invalid_arguments_for_Decoding()
{
	
	printf("\n###############################################################################\n");
	printf(BBLU"");
	printf("Invalid arguments for Decoding\n");
	printf("./lsbsteg: Decoding: ./lsbsteg -d < output .bmp>  [ output file ]\n");
	printf("./lsbsteg: Decoding: ./lsbsteg -d < output .bmp> \n");
	printf(""reset);
	printf("\n###############################################################################\n");
}
