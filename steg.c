#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel
{
    int red;
    int green;
    int blue;
};

/* An image loaded from a PPM file. */
struct PPM
{
    /* Variable to store the "magic number" of the ppm file */
    int format;

    /* Variable to store the width of the image*/
    int width;

    /* Variable to store the height of the image*/
    int height;

    /* Variable to store the maximum colour value of the image*/
    int max;

    /* 2D array to store the rgb pixel value of the image */
    struct Pixel **pixel;
};

/* Method to allocate the memory
 * location to the data structure
 * based on the height and width of
 * the image */
void PixelStructure(struct PPM *data)
{
    /* Allocates the total number of pixels of the image as the size to the array */
    data->pixel = (struct Pixel **)malloc(data->width * data->height * sizeof(struct Pixel *));

    for (int i = 0; i < data->width * data->height; i++)
    {
        /* Each element of the array is allocated the same size as
         * the Pixel structure to store the rgb value */
        data->pixel[i] = (struct Pixel *)malloc(sizeof(struct Pixel));
    }
}

/* Method to read the rgb values of a
 * P3 PPM file */
void ReadP3(FILE *f, struct PPM *data)
{
    for (int i = 0; i < data->width * data->height; i++)
    {
        /* Iterates and assigns each number to
         * red, green and blue pixel respectively */
        fscanf(f, "%d", &data->pixel[i]->red);
        fscanf(f, "%d", &data->pixel[i]->green);
        fscanf(f, "%d", &data->pixel[i]->blue);
    }
}

/* Method to read the rgb values of a
 * P6 PPM file */
void ReadP6(FILE *f, struct PPM *data)
{
    int bytes_size = 1;
    if (data->max > 256)
    {
        /* Changes the byte size to 2 if the maximum colour value is greater than 256 */
        bytes_size = 2;
    }

    /* To read the newline (\n) character after P6*/
    fgetc(f);

    for (int i = 0; i < data->width * data->height; i++)
    {
        /* Iterates and assigns each number to
         * red, green and blue pixel respectively
         * based on the byte size as each byte
         * size holds one colour */
        fread(&data->pixel[i]->red, bytes_size, 1, f);
        fread(&data->pixel[i]->green, bytes_size, 1, f);
        fread(&data->pixel[i]->blue, bytes_size, 1, f);
    }
}

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE *f)
{
    /* Variable of structure PPM to store the PPM file data*/
    struct PPM *data = (struct PPM *)malloc(sizeof(struct PPM));

    if (fgetc(f) != 'P')
    {
        /* Returns null if it is not a valid PPM file*/
        return NULL;
    }

    /* Reads file and stores the "magic number" */
    fscanf(f, "%d", &data->format);

    /* Reads file and stores the height and width of the image */
    fscanf(f, "%d %d", &data->width, &data->height);

    /* Reads file and stores the maximum colour value */
    fscanf(f, "%d", &data->max);

    /* To allocate teh correct memory space for the 2D array to store the pixel values */
    PixelStructure(data);

    if (data->format == 3)
    {
        /* Reads the pixel values if the PPM file is of P3 magic number */
        ReadP3(f, data);
    }
    else
    {
        /* Reads the pixel values if the PPM file is of P6 magic number */
        ReadP6(f, data);
    }

    /* Returns the file specifications as a PPM structure */
    return data;
}

/* Write img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    /* Prints P3 as the "magic number" */
    fprintf(stdout, "P3\n");

    /* Prints the height and width of the image */
    fprintf(stdout, "%d %d\n", img->width, img->height);

    /* Prints the maximum colour of the image */
    fprintf(stdout, "%d\n", img->max);

    for (int i = 0; i < img->width * img->height; i++)
    {
        /* Prints all the pixel values as r g b */
        fprintf(stdout, "%d %d %d\n", img->pixel[i]->red, img->pixel[i]->green, img->pixel[i]->blue);
    }
}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL)
    {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Load the image using getPPM */
    struct PPM *img = getPPM(f);

    /* Close the file */
    fclose(f);

    if (img == NULL)
    {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text, const struct PPM *img)
{
    if (!text || !img || strlen(text) == 0)
    {
        /* Returns null if the PPM image file or the message to be encoded is invalid */
        return NULL;
    }

    /*Variable of structure PPM to store the new PPM with the encoded text in the red pixels */
    struct PPM *encoded_img = (struct PPM *)malloc(sizeof(struct PPM));

    /* Cloning the PPM specifications seaprately to avoid the "const" warning */
    encoded_img->format = img->format;
    encoded_img->width = img->width;
    encoded_img->height = img->height;
    encoded_img->max = img->max;
    encoded_img->pixel = img->pixel;

    /* Generates random number in a way such that the message can be encoded within the image bounds */
    int rand_int = rand() % ((encoded_img->width * encoded_img->height) / strlen(text));

    for (int i = 0; i < strlen(text); i++)
    {
        /* Iterates throught the text to be encoded */

        while (encoded_img->pixel[rand_int]->red == text[i])
        {
            /* If the character to be encoded and the pixel value is the same, the index is incremented by 1 */
            rand_int++;
        }

        /* A random red pixel value is changed to the character of the text to be encoded */
        encoded_img->pixel[rand_int]->red = text[i];

        /* Next random index is generated to occupy the rest of the txt within the image bounds */
        rand_int += rand() % ((encoded_img->width * encoded_img->height) / strlen(text));
    }

    /* Returns the encoded file specifications as a PPM structure */
    return encoded_img;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    if (!oldimg || !newimg || oldimg->width != newimg->width || oldimg->height != newimg->height)
    {
        /* Returns null if either of the PPM files is invalid or don't match with each other */
        return NULL;
    }

    /* Variable to store the message to be decoded */
    char *message = malloc(1000);

    /* Variable to index through the message variable */
    int k = 0;

    for (int i = 0; i < newimg->width * newimg->height; i++)
    {
        /* Iterates through the entire pixel values */

        if (newimg->pixel[i]->red != oldimg->pixel[i]->red)
        {
            /* If the red pixel value does not match, it is appended to the message variable */
            message[k] = newimg->pixel[i]->red;
            k++;
        }
    }

    /* Declares it as a string by adding '\0' at the end of the message */
    message[k] = '\0';

    /* Return the decoded text as a string */
    return message;
}

/* This method clears the memory locations of a struct PPM */
void clear_mem(struct PPM *data)
{
    for (int i = 0; i < data->width * data->height; i++)
    {
        free(data->pixel[i]);
    }

    free(data->pixel);
    free(data);
}

int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0)
    {
        /* Mode "t" - test PPM reading and writing */

        struct PPM *img = readPPM(argv[2]);
        showPPM(img);

        /* Clears the memory locations */
        clear_mem(img);
    }
    else if (argc == 3 && strcmp(argv[1], "e") == 0)
    {
        /* Mode "e" - encode PPM */

        /* Reads the PPM file and stores it to PPM structure */
        struct PPM *oldimg = readPPM(argv[2]);

        /* Variable to store the message to be encoded */
        char *message = malloc(1000);

        /* Prompts the user to enter a message to be encoded */

        /* sterr is used so that the text is not redirected to the file */
        fprintf(stderr, "Enter message to be encoded (less than 1000 characters): ");

        /* fgets is used to store the space as characters*/
        fgets(message, 1000, stdin);

        /* The last character, which is '\n' is replaced by '\0' to declare the message as a string */
        message[strlen(message) - 1] = '\0';

        /* Variable to store the encoded PPM specifications */
        struct PPM *newimg = encode(message, oldimg);

        if (!newimg)
        {
            /* When the encoded PPM structure is null, an error message is printed */
            fprintf(stderr, "File or message cannot be recognized.\n");
        }
        else
        {
            /* The encoded PPM structure is printed if it is not null */
            showPPM(newimg);

            /* Clears the memory locations */
            free(message);
            clear_mem(newimg);
        }
    }
    else if (argc == 4 && strcmp(argv[1], "d") == 0)
    {
        /* Mode "d" - decode PPM */

        /* Variable to store the original PPM image as a PPM structure */
        struct PPM *oldimg = readPPM(argv[2]);

        /* Variable to store the PPM image with the encoded message as a PPM structure */
        struct PPM *newimg = readPPM(argv[3]);

        /* Variable to store the decoded message */
        char *message = decode(oldimg, newimg);

        if (!message)
        {
            /* When the decoded message is null, an error message is printed */
            fprintf(stderr, "Files don't match.\n");
        }
        else
        {
            /* The decoded message is printed if it is not null */
            fprintf(stdout, "The decoded message is: %s\n", message);

            /* Clears the memory locations */
            free(message);
            clear_mem(oldimg);
            clear_mem(newimg);
        }
    }
    else
    {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}
