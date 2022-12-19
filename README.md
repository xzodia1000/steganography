# steganography
---
## Program design
 
- A steganography tool written in C to encode and decode text in a PPM image file. 
- The text is encoded into the image file by altering the pixels at random and it is decoded by comparing the original and encoded image files.
- A 2D array pointer system is used to store the pixels from the PPM file.

The program follows the following design and structure:

### struct PPM

This is the structure used to store the data of a PPM file. It stores:

1.  The integer part of the *"magic number"* i.e. P3 or P6,
2.  The width and height of the image as integer,
3.  The maximum color value of the colour pixels, and
4.  The RGB values as a dynamic multi-dimensional array using `struct Pixel`.

### PixelStructure

This void method initializes and dynamically allocates memory to store the RGB values of the pixels during runtime based on the width and height of the image read from the PPM file.

### ReadP3

This void method reads the RGB values of the image from a PPM file with P3 as the *"magic number"* and stores it in the multi-dimensional array initialized in PixelStructure method using `fscanf` function.

### ReadP6

This void method reads the RGB values of the image from a PPM file with P6 as the *"magic number"* and stores it in the multi-dimensional array initialized in `PixelStructure` method using `fread` function. Each value is read one by one using a for loop.

### getPPM

This method returns a variable holding all the data after reading the PPM file as a `struct PPM`.

### showPPM

This method prints the `struct PPM` variable in the same format as a PPM file to the stdout.

### encode

This method returns a modified PPM structure after encoding the given text into the red pixel values of the image. If the red pixel value to be replaced by the character is same, it is then replaced with the next non-equal red pixel value using a while loop as given below.

```c
    while (encoded_img->pixel[index]->red == text[i])
    {
        index++;
    }
```

### decode

This method returns the message encoded within a PPM structure by comparing the original and encoded image. The message is returned as a string.

### clear_mem

This void method frees the memory location of `struct PPM` and the multi-dimensional array it holds.

### Error Handling

The program is designed to handle errors in all the main methods.

- **getPPM**

The below code returns `NULL` if the first character of the recieved file is not P which is supposed to indicate a PPM file.

```c
    if (fgetc(f) != 'P')
    {
        return NULL;
    }
```

- **encode**

When the recieved PPM structure is `NULL` or the recieved text is of length 0 or inavild, `NULL` is returned.

```c
    if (!text || !img || strlen(text) == 0)
    {
        return NULL;
    }
```

- **decode**

When the recieved PPM structures is `NULL` or if they dont match in height or width, `NULL` is returned.

```c
    if (!oldimg || !newimg || oldimg->width != newimg->width || oldimg->height != newimg->height)
    {

        return NULL;
    }
```

---

## Choice of data structures and algorithms

### Data Structure

- A dynamic multi-dimensional array of `struct Pixel` is used to store the RGB pixels.
- First pointer array is allocated the size of the entire number of pixels at runtime by calculating the product of width and height read from the file.
- The second pointer array is allocated the size of `struct Pixel` i.e. 3 to hold red, green and blue color values separately.
- One advantage of such a data structure is that the memory is allocated based on the image recieved. Hence no extra memory is used by the program.
- Use of pointers helps to define dynamic arrays which supports the above statement.

### Algorthm

The below algorithm is used to determine the random number generated to encode the message to a PPM file.

```c
    int rand_interval = rand() % (encoded_img->width * encoded_img->height) / strlen(text);
```

```c

    rand_int += rand() % ((encoded_img->width * encoded_img->height) / strlen(text));
```

This ensures that the entire message can be encoded within the bounds of the pixels randomly.

---
