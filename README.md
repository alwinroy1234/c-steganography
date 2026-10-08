# C Steganography

A C-based image steganography project that hides and extracts secret text data within BMP images using Least Significant Bit (LSB) manipulation.

## Features

- Encode secret text data into a BMP image
- Decode hidden text data from a stego image
- Store and retrieve the secret file extension
- Store and retrieve the secret file size
- Validate input arguments
- Verify encoded data using a magic string
- Preserve the remaining image data after encoding

## Technologies & Concepts

- C Programming
- File Handling
- Pointers
- Structures
- Bitwise Operations
- LSB Manipulation
- BMP Image Processing
- Modular Programming

## Project Structure

| File | Description |
|---|---|
| `test_encode.c` | Main program and command-line argument handling |
| `encode.c` | Encoding implementation |
| `encode.h` | Encoding structures and function declarations |
| `decode.c` | Decoding implementation |
| `decode.h` | Decoding structures and function declarations |
| `common.h` | Common definitions including the magic string |
| `types.h` | User-defined types and status definitions |

## Compilation

```bash
gcc test_encode.c encode.c decode.c -o steganography
```
## Encoding
```bash
./steganography -e beautiful.bmp secret.txt stego.bmp
```
## Decoding
```bash
./steganography -d stego.bmp decoded.txt
```

