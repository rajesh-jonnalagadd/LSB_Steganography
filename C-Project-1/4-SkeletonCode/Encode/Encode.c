#include <stdio.h>
#include <string.h>
#include "Include/Encode/encode.h"
#include "types.h"
#include <colour.h>


char magic_string[10];
char *ext;
#define EXTENSION_SIZE 4



/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
   // printf("\nwidth = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
   // printf("\nheight = %u\n", height);

    // Return image capacity
    return ((width * height) *3);
}


/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

Status open_files(EncodeInfo *encInfo)
{

    printf("INFO: Opening required files\n");
   	// Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
   
   	// Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
	printf(BRED"");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
	printf(""reset);
	return e_failure;
    }else
    {
		printf("INFO: Opened  %s\n",encInfo->src_image_fname);
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
   
   	// Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
	printf(BRED"");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
	printf(""reset);
    	return e_failure;
    }else
    {
		printf("INFO: Opened  %s\n",encInfo->secret_fname);
    }

    // Stego Image file
   
   	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    
	// Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
	printf(BRED"");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
	printf(""reset);

    	return e_failure;
    }else
	{
		printf("INFO: Opened  %s\n",encInfo->stego_image_fname);
	}
    
	// No failure return e_success
    return e_success;
}


/* Check for operation to be performed
 * Input: Command line arguments
 * Return: integer constant coresponding to operation
 */

OperationType check_operation_type(char *argv[])
{

	if ((strcmp(argv[1],"-e")) == 0)
	{
	//	printf("\nEncode");
		return e_encode;
	}
	else if ((strcmp(argv[1], "-d")) == 0)
	{
	//	printf("\nDecode");
		return e_decode;
	}
	else
	{
	//	printf("\nUnsupported");
		return e_unsupported;
	}
}



int secret_file_size;	

//Get the size of the secret file
uint get_file_size(FILE *fptr)
{
	rewind(fptr);
	fseek(fptr,0,SEEK_END);
	//collecting the size of the file
	secret_file_size = ftell(fptr);
	rewind(fptr);	
	return secret_file_size;
	
}


/* Read and validate command line argument
 * Input: Command line Arguments wtih File name info
 * Output: File names validated for Encode operation.
 * Return: e_success or e_failure
 */

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{

	//checking the argument is provided or not 
	if(argv[2]!= NULL)
	{
		//Checking the source file name with extension either .bmp or not
		if(strcmp(strstr(argv[2],"."), ".bmp") == 0)
		{
			encInfo->src_image_fname = argv[2];
		}
		else
		{
			printf("\nSource image %s format should be .bmp\n", argv[2]);
			return e_failure;
		}
	}else
	{
		Invalid_no_of_arguments();
		return e_failure;
	}
	
	
	//Checking the secret file name extension either in given format or not 
	if(argv[3] != NULL)
	{
		ext = strstr(argv[3], ".");
        	
		if (ext != NULL )  
       	 	{
			// Copy file extension to extn_secret_file and store the file name and extension size
				strcpy(encInfo->extn_secret_file, ext);
            			encInfo->secret_fname = argv[3];
				encInfo->extn_size = strlen(ext);
		}else
		{
				printf("INFO: Secret File %s Should be With Some .Extension\n",argv[3]);
				return e_failure;
		}
	}
	else
	{
		Invalid_no_of_arguments();
		return e_failure;
	}
	

	if (argv[4] != NULL)
	{
		if (strcmp(strstr(argv[4],"."), ".bmp") == 0)
		{
			encInfo->stego_image_fname = argv[4];
		}
		else
		{
			printf("INFO: Output image %s format should be .bmp\n", argv[2]);
			return e_failure;
		}
	}else
	{
			encInfo->stego_image_fname = "steged_img.bmp";
			printf("INFO:Output File not mentioned. Creating stegged_img.bmp as default\n");
	}
	return e_success;

}

Status do_encoding(EncodeInfo *encInfo)
{
	
	if (open_files(encInfo) == e_success)
	{

    		printf(BMAG"INFO: Done\n"reset);
    		printf("INFO: ## Encoding Procedure Started ##\n");
    		printf("INFO: Checking %s size\n",encInfo->secret_fname);
    			
    			
		encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
		
		if(encInfo->size_secret_file == '\0')
		{
			printf(BRED"ERROR: Secret_File is Empty\n"reset);
			return e_failure;
		}else
		{
			printf(BMAG"INFO: Done. Not Empty\n"reset);
		}
		
		printf(BYEL"REQ:Enter the magic string:\n"reset);
		scanf("%s",magic_string);

    		printf("INFO: Checking for %s size capacity to handle %s\n",encInfo->src_image_fname,encInfo->secret_fname);

		int Source_Image_capacity =0;
		Source_Image_capacity =	get_image_size_for_bmp(encInfo->fptr_src_image);
#if 1
		if(check_capacity(encInfo,Source_Image_capacity) == e_success)
		{
		
	    		printf(BMAG"INFO: Done Found ok\n"reset);	
			printf("INFO: Copying Image Header\n");
#if 1			
			//Copying Image Header
			if (copy_bmp_header(encInfo) == e_success)
			{
				printf(BMAG"INFO: Done \n"reset);	
				printf("INFO: Encoding Magic String Size\n");
#if 1
				int magic_string_size = strlen(magic_string);
				if(encode_magic_string_size(magic_string_size,encInfo) == e_success)
				{
					printf(BMAG"INFO: Done \n"reset);	
					printf("INFO: Encoding Magic String\n");
#if 1
					if(encode_magic_string(magic_string,encInfo) == e_success)
					{
						printf(BMAG"INFO: Done \n"reset);	
						printf("INFO: Encoding %s File Extension \n",encInfo->secret_fname);

						int extn_size = strlen(encInfo->extn_secret_file);
						if(encode_secret_file_extn_size(extn_size,encInfo) == e_success)
						{

							printf(BMAG"INFO: Done \n"reset);	
							printf("INFO: Encoding %s File Extension Size \n",encInfo->secret_fname);
#if 1
						if(encode_secret_file_extn(encInfo) == e_success)
						{
							
							printf(BMAG"INFO: Done \n"reset);	
							printf("INFO: Encoding %s File Size \n",encInfo->secret_fname);
#if 1
							//printf("rrr = %d",secret_file_size);
							if(encode_secret_file_size(secret_file_size,encInfo) == e_success)
							{
								printf(BMAG"INFO: Done \n"reset);	
								printf("INFO: Encoding %s File Data \n",encInfo->secret_fname);
#if 1
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf(BMAG"INFO: Done \n"reset);	
									printf("INFO: Copying remaining data \n");

#if 1		
											if(copy_remaining_img_data(encInfo)==e_success)
											{
												printf(BMAG"INFO: Done\n"reset);

											}else
											{
												printf(BRED"ERROR: While Copying Remaining Data\n");
											}
#endif
								}else
								{
									printf(BRED"ERROR:while Encoding secret File Data"reset);
									return e_failure;
								}
#endif

							}else
							{
								printf("ERROR:Encoding %s File Size\n",encInfo->secret_fname);
							}
						}else
						{
							printf("ERROR While Encoding the SEcret file extension size\n");
						}
#endif 
						}else
						{
							printf(BRED"ERROR: While Encoding the Secret file extension\n"reset);
						}
#endif
					}else
					{
						printf(BRED"ERROR:while Encoding Magic String\n");
						return e_failure;
					}

#endif				
				}else
				{
					printf("ERROR: While Encoding Magic String Size\n");
					return e_failure;
				}
			
#endif
			}else
			{
				printf(BRED"Error: %s function failed\n","copy_bmp_header()"reset);
				return e_failure;
			}
#endif
		}else
		{
			printf(BRED"Error while checking capicity\n"reset);
			return e_failure;
		}
#endif
		return e_success;
	}
}


Status check_capacity(EncodeInfo *encInfo,int Source_Image_capacity)
{
    encInfo->size_secret_file = secret_file_size;	  

    int size_of_magic_string = strlen(magic_string);
    int length_of_extension = encInfo->extn_size;
    int length_of_secret_message = secret_file_size;

   
   // printf("\nlength of secret_message %d\n", length_of_secret_message);


    uint bytes_for_magic_string_size = 4; // 4 bytes to encode the length of the magic string  
    uint bytes_for_secret_file_size = 4; // 4 bytes to encode file size
    uint bytes_for_file_extension_size = 4; // Assuming 4 bytes for extension size   

    // Calculate required capacity
    int required_capacity = 0;
 

    required_capacity = 54 + ( bytes_for_magic_string_size * 8)    + ( size_of_magic_string * 8 )
			   + ( bytes_for_file_extension_size * 8 ) + ( length_of_extension * 8 ) 
			   + ( bytes_for_secret_file_size * 8)     + ( length_of_secret_message * 8);
   

    //printf("Capacity required:    %d\n", required_capacity);
     //printf("Source_Image_Capacity %d\n",Source_Image_capacity);
    
    if (Source_Image_capacity >= required_capacity)
    {
        printf("Capacity check successful.\n");
        return e_success;
    }
    else
    {
        printf("Error: Insufficient capacity in the source image.\n");
        return e_failure;
    }
}


Status copy_bmp_header(EncodeInfo *encInfo)
{
	char buffer[54];
    // Check if the file pointers are valid
    if (encInfo->fptr_src_image == NULL || encInfo->fptr_stego_image== NULL)
    {
        return e_failure;  // Return failure if any file pointer is NULL
    }
    // Rewind the source image file to the beginning
	rewind(encInfo->fptr_src_image);
	fread(buffer,54,1,encInfo->fptr_src_image);
	fwrite(buffer,54,1,encInfo->fptr_stego_image);
	return e_success;
}


// Function definition for encode secret file size
Status encode_magic_string_size(int magic_string_size, EncodeInfo *encInfo)
{

	char buffer[32];
    fread(buffer,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(magic_string_size,buffer);
    fwrite(buffer,32,1,encInfo->fptr_stego_image);
    return e_success;

}


Status encode_size_to_lsb(long size, char *image_buffer)
{
    unsigned int mask = 1 << 31, i;
    for (i = 0; i < 32; i++)
    {
        image_buffer[i] = ((size & mask) >> (31 - i) | (image_buffer[i] & 0xFE) );
        mask = mask >> 1;
    }
}






Status encode_secret_file_extn(EncodeInfo *encInfo)
{
	int size=(strlen(encInfo->extn_secret_file));
	char buffer[8];

	for(int i=0;i<size;i++)
	{
		fread(buffer,8,1,encInfo->fptr_src_image);
		string_bitwise(buffer,encInfo->extn_secret_file[i]);
		fwrite(buffer,8,1,encInfo->fptr_stego_image);
	}
	//printf("sssss = %s\n",encInfo->extn_secret_file);
	return e_success;
}





Status encode_byte_to_lsb(char data, char *image_buffer)
{
	for (int i = 0; i < 8; i++)
    {
        // Write each bit of the byte to the LSB of the image buffer
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> (7 - i)) & 0x01);
    }
    return e_success;
}


Status encode_secret_file_size(long size_of_secret_file, EncodeInfo *encInfo)
{
    char str[32];
    fread(str, 32, 1, encInfo->fptr_src_image);
    encode_size_to_lsb(size_of_secret_file, str);
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
		unsigned char character;            //Read and store each byte into a character
		
		//Outer iteration till the size of given string
		for (uint i = 0; i < strlen(magic_string); i++)
		{
				for (int j = 7; j >= 0; j--)   //8 times inner iteration
				{
						fread(&character, sizeof(character), 1, encInfo->fptr_src_image);

						if (ferror(encInfo->fptr_src_image))//Error handling
						{
							    printf("ERROR: Error while reading from source image file.\n\n");
								return e_failure;
						}

						character &= 0xFE;		//Clear the least significant bit of fetched character

						if (magic_string[i] & (01 << j))	//Check every bit of magic string
						{
								character |= 01;//Set the least significant bit of obtained character
						}
						else
						{
								character |= 00; //Clear the least significant bit of obtained character
						}


						//Write the obtained byte onto output file
						fwrite(&character, sizeof(character), 1, encInfo->fptr_stego_image);
				        
						if (ferror(encInfo->fptr_stego_image))//Error handling
				           {
				              printf("ERROR: Error while writing onto output image file.\n\n");
					          return e_failure;
				           }
				}
		}
		return e_success;
}

Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char data[(int) encInfo->size_secret_file];


	fread(data, sizeof(char), (uint) encInfo->size_secret_file, encInfo->fptr_secret);

	if (ferror(encInfo->fptr_secret))
	{
		printf("Error: While reading the data to secret file\n");
		return e_failure;
	}

	// Encoding the text in secret file to dest image
	if (encode_string_to_image(data, encInfo->size_secret_file, encInfo) == e_failure)
	{
		fprintf(stderr, "Error: %s function failed\n", "encode_string_to_image()");
		return e_failure;
	}
	return e_success;
}

Status encode_string_to_image(char *data, int size, EncodeInfo *encInfo)
{
	char image_buffer[MAX_IMAGE_BUF_SIZE];
	for(uint i = 0; i < size; i++)
	{
		fread(image_buffer, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_src_image);
		if (encode_byte_to_lsb(data[i], image_buffer)  == e_success)
		{

			fwrite(image_buffer, sizeof(char), MAX_IMAGE_BUF_SIZE, encInfo->fptr_stego_image);
		}
		else
		{
			fprintf(stderr,"Error: %s function failed\n", "encode_byte_to_lsb()");
			return e_failure;
		}
	}
	return e_success;
}

Status copy_remaining_img_data(EncodeInfo *encInfo)
{
	int ch;
	while ((ch = getc(encInfo->fptr_src_image)) != EOF)  // EOF is macro with value -1
    {
		if (ferror(encInfo->fptr_src_image))
		{
			printf("Error: unable to read from %s", "Source image file");
			return e_failure;
		}

      	putc(ch, encInfo->fptr_stego_image);

		if (ferror(encInfo->fptr_stego_image))
		{
			fprintf(stderr, "Error: unable to write to  %s", "Destination image file");
			return e_failure;
		}
    }
	return e_success;


}

Status encode_secret_file_extn_size(int extn_size,EncodeInfo *encInfo)
{
    char buffer[32];  // Buffer to hold 32 bits (4 bytes) of image data
    // Read 32 bytes of image data (since long is typically 4 bytes and each byte is 8 bits)
    fread(buffer, 32, 1, encInfo->fptr_src_image);
    // Encode the extension size into the LSB of each byte in the image buffer
    if (encode_size_to_lsb(extn_size, buffer) == e_failure)
    {
        printf("Error: Failed to encode extension size into the image.\n");
        return e_failure;
    }
    // Write the modified image data back to the stego image file
    fwrite(buffer, 32, 1, encInfo->fptr_stego_image);

    return e_success;
}

Status string_bitwise(char *buffer,const char string)
{
	
	long onebit;
	
	char x=string;
	for(int j=7;j>=0;j--)
	{
	
		onebit=(x>>j)&1;
		buffer[7-j]=(buffer[7-j]&~1)|onebit;
		
	}
	
	return e_success;
}
