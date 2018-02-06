
/* Coded By: Ryan Boudreaux */


#include <stdio.h>
#include <stdlib.h>



/* Throw the instructions */
void instruct_error() {
    system("clear");    /* Clear the display screen */
    printf("Operating Systems Homework Code Assignment 1\n\nUsage: ./HW1 [-s | -f] [input-file] [output-file(if using -f option)]\n\n");
}


/* If the arguments are not valid, throw the instructions and exit */

void validateArgs(int n, char *c) {
    if (n < 3 || (strcmp(c, "-s") != 0) && (strcmp(c, "-f") != 0)) {
        instruct_error();
        exit(0);
    }
}


/* Cry 'Havoc!', and let slip the dogs of war */

int main(int argc, char *argv[]) {
    FILE *input, *output;
    char output_txt[] = "The number of non-whitespace characters: %d\n"; /* Set the output text for both screen and file */
    int c; /* Allocate the fgetc() result */
    unsigned int chars = 0; /* Allocate and set the character counter to 0, unsigned because we are not dealing with negatives */
    validateArgs(argc, argv[1]); /* arg validation */
    
    /* Open the file as read only. If it does not exist, throw the instruct_error and return 0 */
    input = fopen(argv[2], "r");
    if (!input) {
        instruct_error();
        return 0;
    }
    
    /* Using a while loop to count. When EOF is reached, c will return -1 */
    while ((c = fgetc(input)) != -1) {
        if (!isspace(c)) {      /* isspace() function to find the  */ 
            chars++;            /* whitespace in the file. If c is */
        }                       /* a NOT whitespace, the counter   */
    }                           /* will be incrimented by 1 for    */
    fclose(input);              /* each pass in the loop           */


    /* If the -s flag was used, display the output text on the console. If -f flag was used, print that the output was written to the output file and write the output text to the file. */
    
    if (strcmp(argv[1], "-s") == 0) {
        printf(output_txt, chars);
    }
    if (strcmp(argv[1], "-f") == 0) {
        printf("Output written to output.txt\n");
        output = fopen("output.txt", "w");
        fprintf(output, output_txt, chars);
        fclose(output);
    }
  return 0;
}
