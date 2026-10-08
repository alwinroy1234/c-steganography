#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>
#include "decode.h"

int main(int argc,char *argv[])
{
    //Check operation type to know whether it is encoding or decoding
    if(check_operation_type(argv)==e_encode)
    {
        EncodeInfo encInfo;
        printf("Selected Encoding\n");
        if(read_and_validate_encode_args(argv,&encInfo)==e_success)
        {
            printf("Read and validate encode arguments is succesful\n");
            if(do_encoding(&encInfo)==e_success)
            {
                printf("Encoding completed\n");
            }
            else
            {
                printf("Failed to encode\n");
            }
        }
        else
        {
            printf("Failed to validate the input arguments\n");
        }

    }
    else if(check_operation_type(argv)==e_decode)
    {
        DecodeInfo decInfo;

        printf("Selected Decoding\n");

        if(read_and_validate_decode_args(argv, &decInfo) == e_success)
        {
            printf("Read and validate decode arguments is successful\n");

            if(do_decoding(&decInfo) == e_success)
            {
                printf("Decoding completed\n");
            }
            else
            {
                printf("Failed to decode\n");
            }
        }
        else
        {
            printf("Failed to validate the input arguments\n");
        }
    }
    else
    {
        printf("Invalid Option\n");
        printf("***********************Usage***********************\n");
        printf("Encoding: ./a.out -e beautiful.bmp secret.txt stego.bmp\n");
        printf("Decoding: ./a.out -d stego.bmp\n");
        printf("***************************************************\n");
    }
    return 0;
}
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}
