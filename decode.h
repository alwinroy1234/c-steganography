#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    // Stego Image info 
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Output file info */
    char *output_fname;
    FILE *fptr_output;

    /* Secret file info */
    char extn_secret_file[MAX_FILE_SUFFIX + 1];
    int extn_size;
    int size_secret_file;

    char image_data[MAX_IMAGE_BUF_SIZE];

} DecodeInfo;


/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open files */
Status open_decode_files(DecodeInfo *decInfo);

/* Decode a byte from LSB */
char decode_byte_from_lsb(char *image_buffer);

/* Decode an integer from LSB */
int decode_size_from_lsb(char *image_buffer);

/* Decode magic string */
Status decode_magic_string(char *magic_string, DecodeInfo *decInfo);

/* Decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif