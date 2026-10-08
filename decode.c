#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "common.h"
#include "types.h"


// Open stego image and output file 
Status open_decode_files(DecodeInfo *decInfo)
{
    // Open stego image 
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");

    if(decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                decInfo->stego_image_fname);

        return e_failure;
    }

    // Open output file 
    decInfo->fptr_output = fopen(decInfo->output_fname, "w");

    if(decInfo->fptr_output == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",
                decInfo->output_fname);

        return e_failure;
    }

    return e_success;
}


/* Read and validate decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //validating the .bmp file
    if(argv[2] != NULL && strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        return e_failure;
    }

    if(argv[3] != NULL)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = "decoded.txt";
    }

    return e_success;
}


/* Decode one character from 8 image bytes */
char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;

    for(int i = 0; i < 8; i++)
    {
        data = data << 1;
        data = data | (image_buffer[i] & 1);
    }

    return data;
}


/* Decoding 4 byte integer from 32 image bytes */
int decode_size_from_lsb(char *image_buffer)
{
    int data = 0;

    for(int i = 0; i < 32; i++)
    {
        data = data << 1;
        data = data | (image_buffer[i] & 1);
    }

    return data;
}


// Decoding the  magic string 
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    int size = strlen(MAGIC_STRING);

    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    for(int i = 0; i < size; i++)
    {
        fread(decInfo->image_data, 8, sizeof(char),
              decInfo->fptr_stego_image);

        magic_string[i] = decode_byte_from_lsb(decInfo->image_data);
    }

    magic_string[size] = '\0';

    return e_success;
}


/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    fread(decInfo->image_data, 32, sizeof(char),
          decInfo->fptr_stego_image);

    decInfo->extn_size = decode_size_from_lsb(decInfo->image_data);

    return e_success;
}


/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for(int i = 0; i < decInfo->extn_size; i++)
    {
        fread(decInfo->image_data, 8, sizeof(char),decInfo->fptr_stego_image);

        decInfo->extn_secret_file[i] = decode_byte_from_lsb(decInfo->image_data);
    }

    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    return e_success;
}


// Decoding secret file size
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    fread(decInfo->image_data, 32, sizeof(char),decInfo->fptr_stego_image);

    decInfo->size_secret_file = decode_size_from_lsb(decInfo->image_data);

    return e_success;
}


// Decode secret file data 
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(int i = 0; i < decInfo->size_secret_file; i++)
    {
        fread(decInfo->image_data, 8, sizeof(char), decInfo->fptr_stego_image);

        ch = decode_byte_from_lsb(decInfo->image_data);

        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }

    return e_success;
}


/* Main decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
    char decode_magic[20];

    /* Open files */
    if(open_decode_files(decInfo) == e_success)
    {
        printf("Opened all the files successfully\n");
        printf("Started Decoding\n");

        /* Decode magic string */
        if(decode_magic_string(decode_magic, decInfo) == e_success)
        {
            printf("Decoded Magic String: %s\n", decode_magic);

            /* Validate magic string */
            if(strcmp(decode_magic, MAGIC_STRING) == 0)
            {
                printf("Magic String matched successfully\n");

                /* Decode extension size */
                if(decode_secret_file_extn_size(decInfo) == e_success)
                {
                    printf("Decoded secret file extension size: %d\n", decInfo->extn_size);

                    /* Decode extension */
                    if(decode_secret_file_extn(decInfo) == e_success)
                    {
                        printf("Decoded secret file extension: %s\n", decInfo->extn_secret_file);

                        /* Decode secret file size */
                        if(decode_secret_file_size(decInfo) == e_success)
                        {
                            printf("Decoded secret file size: %d\n",decInfo->size_secret_file);

                            /* Decode secret data */
                            if(decode_secret_file_data(decInfo) == e_success)
                            {
                                printf("Secret file data decoded successfully\n");
                            }
                            else
                            {
                                printf("Failed to decode secret file data\n");
                                return e_failure;
                            }
                        }
                        else
                        {
                            printf("Failed to decode secret file size\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("Failed to decode secret file extension\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Failed to decode secret file extension size\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Magic String does not match\n");
                printf("No secret data found\n");
                return e_failure;
            }
        }
        else
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
    }
    else
    {
        printf("Failed to open files\n");
        return e_failure;
    }

    return e_success;
}