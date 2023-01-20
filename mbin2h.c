/*
 * mbin2h, My Binary to header converter
 * Original author unknown,
 * comments and format by Scirese.
 *
 * As original license is unknown, I could only let this program to be
 * distributed freely under MIT License without authorization XD.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *infoText = "Created with mbin2h";

/*
 * By default we both use ASCII chars and plain HEX in the output.
 */
char outputInChar = 1;

static void help(int status) {

    printf("mbin2h Options \n\n"
           "-c Not put any ASCII characters in the output\n"
           "-h Show this help\n\n"
    );
    exit(status);

}

int getSize(FILE *f) {
    int size,last;

    last = ftell(f) ;  fseek(f,0,SEEK_END) ;
    size = ftell(f) ;  fseek(f,last,SEEK_SET) ;

    return size;
}

void bin2h(const char *file)
{
    FILE *f = fopen(file, "rb");
    int size = getSize(f);
    unsigned char * flength = (unsigned char * ) malloc(size);
    char path[256];

    fread(flength, size, 1, f);
    fclose(f);

    sprintf(path, "%s.h", file);
    f = fopen(path, "wb");

    /*
     * Headings
     */
    fprintf(f, "#define binSize %i\n\n"
               "static const unsigned char bin[binSize] = {\n"
               , size);

    for (int c = 0; c < size - 1; c++) {

      /*
       * If -C supplyed, then don't use ASCII char mapping. Output in plain HEX directly.
       * fprintf() prints the whole file into a array.
       */
        if (outputInChar && ((flength[c] >= 'a' && flength[c] <= 'z') || (flength[c] >= 'A' && flength[c] <= 'Z'))) {
            fprintf(f, "\t\'%c\',", flength[c]);
        } else {
            fprintf(f, "\t0x%X,", flength[c]);
        }

        if (c > 1){
            if (!((c + 1) % 8)) {
                fprintf(f, "\n");
            }
        }

    };

    /*
     * This would write actual content.
     */
    fprintf(f, "\t0x%X \n};\n", flength[size - 1]);
    fclose(f);

    printf("File - %s - %d Bytes\n", file, size);;
}

int main(int argc, char * argv[]) {
    int *bin;
    int fileNb = 1;
    int opt;

    while ((opt = getopt(argc, argv, "ch")) != -1) {
        switch (opt) {
        case 'c':
            if (argc < 3) {
                fprintf(stderr, "ERROR: Please supply a file. \n\n");
                help(EXIT_FAILURE);
            }
            outputInChar = 0 ;
            break;
        case 'h':
            help(EXIT_SUCCESS);
        default:
            break;
        }
    }

    if (!argv[fileNb]) {
        fprintf(stderr, "ERROR: Please supply a file. \n\n");

        if (fileNb == 1) printf("Type \"mbin2h -h\" for help\n");

        printf("%s\n\n", infoText);
        return 0;
    }

    while (argv[fileNb]) {
        bin2h(argv[fileNb++]);
    };
    return 0;
}
