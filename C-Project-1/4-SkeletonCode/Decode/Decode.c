#include <stdio.h>
#include <string.h>
#include "Include/Decode/decode.h"
#include "Include/colour.h"
#include "Include/types.h"
#include "Include/common.h"

char *ext;

//function for validating the files for decoding
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
	//printf("READ_AND VALIDATE ENTERED\n");
    
	if(argv[2] != NULL)
    	{
		ext = strstr(argv[2],".");
		if(ext != NULL)
		{
			if(strcmp(ext,".bmp") == 0)
			{
				decInfo->stego_image_fname = argv[2];
			}else
			{
				printf("Enter only bmp files\n");
				return d_failure;
			}
       		}else
        	{
            		printf(BRED"Error: Stego image %s format should be .bmp\n", argv[2]);
            		printf(""reset);
            		return d_failure;
        	}
    	}else
    	{
        	Invalid_arguments_for_Decoding();
        	return d_failure; 
    	}

	if(argv[3] != NULL)
	{
		strcpy(decInfo->secret_fname,argv[3]);
		printf("\nop  %s",decInfo->secret_fname);

	}else
	{
		printf(BMAG"INFO : OUTPUT File is not mentioned creating default\n"reset);
		const char *default_filename = "Default";
		strcpy(decInfo->secret_fname, default_filename);    
	       // printf("\t\t\t%s\n\n",decInfo->secret_fname);
	}
    
    return d_success;
}


Status open_decode_source_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname,"r");

    if(decInfo->fptr_stego_image == NULL)
    {
	    printf("Unable to open file.\n");
	    return d_failure;
    }else
    {
        //printf(BMAG"INFO : Source image file opened\n"reset);
        return d_success;
    }
}

Status open_output_file(DecodeInfo *decInfo)
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname,"w");
    if(decInfo->fptr_secret == NULL)
    {
	    printf(BRED"ERROR : Opening o/p File\n"reset); 
	    return e_failure;
    }else
    {
	     return d_success;
    }
   
}

Status do_decoding(DecodeInfo *decInfo)
{
    char magic_string[100];
    char buffer[32];
    
#if 1
    if(open_decode_source_file(decInfo) == d_success)
    {
        printf(BMAG"INFO : Input Source FIle opened \n"reset);

            rewind(decInfo->fptr_stego_image);
            fseek(decInfo->fptr_stego_image,54,SEEK_SET);

#if 1
            if(decode_magic_string_size(decInfo) == d_success)
            {
                printf(BMAG"INFO: Done Decoded magic_string size\n"reset);
#if 1
                if(decode_magic_string(decInfo) == d_success)
                {
                    printf(BMAG"INFO: Done Decoded magic_string \n"reset);
#if 1
                    if(decode_secret_file_extn_size(decInfo)==d_success)
                    {
                        printf(BMAG"INFO: Done Decoded Secret file extension size\n"reset);

#if 1                  
                        if(decode_secret_file_extn(decInfo)== d_success)
                        {
                            printf(BMAG"INFO: Done Decoded Secret file extension\n"reset);
#endif
#if 1

                        if(open_output_file(decInfo)== d_success)
                        {   
                            printf(BMAG"INFO : Output FIle opened \n"reset);
                            
                            if(decode_secret_data_size(decInfo)==d_success)
                            {
                                printf(BMAG"INFO : Secret Data size Decoded \n"reset);
                                
                                if(decode_secret_data(decInfo)==d_success)
                                {
                                    printf(BMAG"INFO: decode secret data Completed\n"reset);
                                }
                                else
                                {
                                    printf("ERROR: Copying secret data failed\n");
                                    return e_failure;
                                }


                            }else
                            {
                                printf("ERROR:While decoding secret data size\n");
                                return e_failure;
                            }

                        }else
                        {
                            printf("ERROR While opening op/file\n");
                            return d_failure;
                        }
#endif
#if 1 

                        }else
                        {
                            printf("ERROR WHILE DECODE SECRET_FILE_EXTENSION\n");
                            return d_failure;
                        }
#endif

                    }else
                    {
                        printf(BRED"ERROR While Decoding Secret file extension size\n"reset); 
                        return d_failure;
                    }
#endif
                }else
                {
                    printf(BRED"ERROR While Decoding the magic string\n"reset);
                    return d_failure;
                }

#endif
            }else
            {
               printf(BRED"ERROR While Decoding the magic string size\n"reset);  
                return d_failure;
            }

#endif


        }else
        {
           printf(BRED"ERROR While Opening SOurce file\n"reset); 
        }
#endif
    
    return d_success;   
   
}

Status decode_magic_string_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int size = 0;
    fread(buffer,32,1,decInfo->fptr_stego_image);
    decInfo->magic_string_size = decode_size_from_lsb(size,buffer);
    
   // printf("Magic string size is %d\n",decInfo->magic_string_size);
    
    if(decInfo->magic_string_size < 0)
    {
    	printf("Magic string size is INVALID\nFAILURE\n");
        return d_failure;
    }
    return d_success;
}



Status decode_size_from_lsb(int size,char *buffer)
{
    size = 0;
    for (int i = 0; i < 32; i++)
    {
        size = (size << 1) | (buffer[i] & 1);
    }
    return size;
}



char decode_byte_from_lsb(char *buffer)
{
    char ch=0;
    for(int i=7; i>=0; i--)
    {
	ch = (((buffer[7-i] & 1) << i) | ch);
	//printf("\t %c",ch);
    }
    return ch;

}



char decode_magic_string(DecodeInfo *decInfo)
{
    unsigned char character;
    char magic_string[decInfo->magic_string_size];
    for (int i = 0; i < decInfo->magic_string_size; i++)
    {
        magic_string[i] = 0;
        for (int j = 7; j >= 0; j--)
        {
            fread(&character, sizeof(character), 1, decInfo->fptr_stego_image);
            magic_string[i] |= (character & 0x01) << j;
        }
    }
    magic_string[decInfo->magic_string_size] = '\0';

    //printf("\nmagic_string = %s\n",magic_string);
 

    char entered_magic_string[20];
    printf(BMAG"Enter the magic string: ");
    scanf(" %[^\n]",entered_magic_string);
    printf(""reset);
    if(!strcmp(magic_string,entered_magic_string))
    {
	printf(BMAG"The magic string is matched with encoded magic string\n"reset);
    }
    else
    {
	printf("The magic string is not matched with encoded magic string\n");
	return e_failure;
    }
    return e_success;
    
}


Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char buffer[32];
    int res = 0;
    fread(buffer,32,1,decInfo->fptr_stego_image);
    decInfo->extn_size = decode_size_from_lsb(res,buffer);

    if(decInfo->extn_size > 0)
    {
     //   printf("The Extn size is %d\n",decInfo->extn_size);
        return d_success;
    }else
    {
        return d_failure;
    }
    
    
}

char decode_secret_file_extn(DecodeInfo *decInfo)
{
    char buffer[8];
    char extn[decInfo->extn_size];
    for(int i=0; i<decInfo->extn_size; i++)
    {
        fread(buffer,8,1,decInfo->fptr_stego_image);
	    extn[i] = decode_byte_from_lsb(buffer);
    }
    extn[decInfo->extn_size] = '\0';

    //printf("rajesh decode %s\n",extn);    // check once here!!!!!
    strcat(decInfo->secret_fname,extn);
    return d_success;

}

Status decode_secret_data_size(DecodeInfo *decInfo)
{
    //printf("Decoding secret data size\n");
    char buffer[32];
    int res = 0;

   // fseek(decInfo->fptr_stego_image,32,SEEK_CUR);

    fread(buffer,32,1,decInfo->fptr_stego_image);
    decInfo->secret_data_size = decode_size_from_lsb(res,buffer);
    //printf("secret data size is %d\n",decInfo->secret_data_size);
    return d_success;
}


Status decode_secret_data(DecodeInfo *decInfo)
{
    char buffer[8];
    char secret[decInfo->secret_data_size];
    for (int i = 0; i < decInfo->secret_data_size; i++)
    {
    
            fread(buffer, 1, 8 ,decInfo->fptr_stego_image);
            secret[i] = decode_byte_from_lsb(buffer);
            fwrite(&secret[i],1,1,decInfo->fptr_secret);
        
    }
    secret[decInfo->magic_string_size] = '\0';
    return d_success;
}

