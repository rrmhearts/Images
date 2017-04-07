#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

const char binary[16][5] = {"0000\0", "0001\0", "0010\0", "0011\0",
                            "0100\0", "0101\0", "0110\0", "0111\0",
                            "1000\0", "1001\0", "1010\0", "1011\0",
                            "1100\0", "1101\0", "1110\0", "1111\0"
                          };
const int dimx = 24, dimy = 24;

// Out file pbm
FILE *pbm;//, *expand;
FILE * expand_hex;
char line [128];


float scale_g = 1.0; // only in old main
char * inputhex = "bitmap.hex";

char * hex2pbm(char * line)
{

  int i = 0, ref =0;
  for (i=0; i < 6; i++)
  {
    if (line[i] <= '9' && line[i] >= '0')
    {
      ref = line[i] - '0';
      fprintf(pbm, "%s", binary[ref]   );
    } else if (line[i] <= 'F' && line[i] >= 'A')
    {
      ref = line[i] -'A'+10;
      fprintf(pbm, "%s", binary[ref]   );
    }

  }
  fprintf(pbm, "\n");
}

void pbm2hex(char * line)
{
  char substring[5];
  char hex[128];
  //printf("line  %s\n", line);
  int i = 0;

  hex[127] = '\0';
  //printf("len: %u", (unsigned)strlen(line));
  while (line[0] == '1' || line[0] == '0')
  {
    // printf("%c%c%c%c\n", line[0], line[1],line[2],line[3]);
    strncpy(substring, line, 4);
    substring[4] = '\0';
    //printf("sub %s\n", substring);
    //printf("%s\n",substring);

    if (!strcmp(substring, "0000")) {
      hex[i] = '0';
    }  else if (!strcmp(substring, "0001")) {
      hex[i] = '1';
    }  else if (!strcmp(substring, "0010")) {
      hex[i] = '2';
    }  else if (!strcmp(substring, "0011")) {
      hex[i] = '3';
    }  else if (!strcmp(substring, "0100")) {
      hex[i] = '4';
    }  else if (!strcmp(substring, "0101")) {
      hex[i] = '5';
    }  else if (!strcmp(substring, "0110")) {
      hex[i] = '6';
    }  else if (!strcmp(substring, "0111")) {
      hex[i] = '7';
    }  else if (!strcmp(substring, "1000")) {
      hex[i] = '8';
    }  else if (!strcmp(substring, "1001")) {
      hex[i] = '9';
    }  else if (!strcmp(substring, "1010")) {
      hex[i] = 'A';
    }  else if (!strcmp(substring, "1011")) {
      hex[i] = 'B';
    }  else if (!strcmp(substring, "1100")) {
      hex[i] = 'C';
    }  else if (!strcmp(substring, "1101")) {
      hex[i] = 'D';
    }  else if (!strcmp(substring, "1110")) {
      hex[i] = 'E';
    }  else if (!strcmp(substring, "1111")) {
      hex[i] = 'F';
    }
    i++;
    line+=4;
  }
  hex[i] = '\0';
  fprintf(expand_hex, "%s", hex);
  fprintf(expand_hex, "\n");
  //printf("%s,    %d\n", hex, i);
  //return hex;
}

void expandPBM(float scale)
{
  FILE * readf = fopen("output.pbm", "r");
  int len = 26;

  int size = 24 * scale, currentRow = 0;
  bool moredata = true;

  FILE * expand = fopen("expand_out.pbm", "w"); /* b - binary mode */
  fprintf(expand, "P1\n%d %d\n", size, size);

  expand_hex = fopen("expand.hex", "w"); /* b - binary mode */

  int i, j;

  // Throw away first two lines
  fgets(line, len, readf);
  fgets(line, len, readf);

  // Grab first line of data
  moredata = (fgets(line, len, readf) != NULL);
  for (i = 0; i < size && moredata; i++) {
    for (j = 0; j < size && moredata; j++) {
      fprintf(expand, "%c", line[(int)floor(1.0*j/scale)]   );
    }
    fprintf(expand, "\n"   );

    if (currentRow < floor(i/scale)) {
      moredata = (fgets(line, len, readf) != NULL);
      currentRow = (int) floor(i/scale);
    }
  }

  fclose(expand);

  fclose(readf);
}

void setupGraphicFont(int mx_tmp, int my_tmp)
{
  //if (mx_tmp == NULL)    mx_tmp = 1;
  //if (my_tmp == NULL)    my_tmp = 1;

  printf("x, y %d, %d", mx_tmp, mx_tmp);
  // Zebra scaling
  float mx = 3.0/2*mx_tmp;
  float my = 3.0/2*my_tmp;

  // Origin hex file
  FILE * origin_hex;
  origin_hex = fopen(inputhex, "r");
  int len = 8;

  pbm = fopen("output.pbm", "w"); /* b - binary mode */
  fprintf(pbm, "P1\n%d %d\n", dimx, dimy);

  int i = 0, j = 0, ref =0;
  while (( fgets(line, len, origin_hex)) != NULL ) {
    for (i=0; i < 6; i++)
    {
      if (line[i] <= '9' && line[i] >= '0')
      {
        ref = line[i] - '0';
        fprintf(pbm, "%s", binary[ref]   );
      } else if (line[i] <= 'F' && line[i] >= 'A')
      {
        ref = line[i] -'A'+10;
        fprintf(pbm, "%s", binary[ref]   );
      }

    }
    fprintf(pbm, "\n");
  }

  fclose(pbm); fclose(origin_hex);

  // Begin ExpandPBM
  FILE * readf = fopen("output.pbm", "r"); // binary of original data
  int size_x = 24 * mx, size_y = 24 * my, currentRow = 0;
  bool moredata = true;

  i =0, j=0, len = 26;

  FILE * expand = fopen("expand_out.pbm", "w"); // expanded binary
  fprintf(expand, "P1\n%d %d\n", size_x, size_y);

  // Throw away first two lines
  fgets(line, len, readf);
  fgets(line, len, readf);

  // Grab first line of data - For expansion
  moredata = (fgets(line, len, readf) != NULL);
  for (i = 0; i < size_y && moredata; i++) {
    for (j = 0; j < size_x && moredata; j++) {
      fprintf(expand, "%c", line[(int)floor(1.0*j/mx)]   );
    }
    fprintf(expand, "\n"   );

    if (currentRow < floor(i/my)) {
      moredata = (fgets(line, len, readf) != NULL);
      currentRow = (int) floor(i/my);
    }
  } // end expansion for loop
  fclose(expand); fclose(readf); // End ExpandPBM

  expand = fopen("expand_out.pbm", "r"); // Read expanded data to trasit to hex
  expand_hex = fopen("expand.hex", "w"); // hex version of binary expanded data
  len =24*mx+2;
  fgets(line, len, expand);  fgets(line, len, expand); // ignore header lines
  char * pline;
  while (( fgets(line, len, expand)) != NULL ) {
    char substring[5];
    char hex[128];
    i = 0; hex[127] = '\0';
    pline = (char *) line;
    while (pline[0] == '1' || pline[0] == '0')
    {

      // PBM to HEX
      strncpy(substring, pline, 4);
      substring[4] = '\0';

      if (!strcmp(substring, "0000")) {
        hex[i] = '0';
      }  else if (!strcmp(substring, "0001")) {
        hex[i] = '1';
      }  else if (!strcmp(substring, "0010")) {
        hex[i] = '2';
      }  else if (!strcmp(substring, "0011")) {
        hex[i] = '3';
      }  else if (!strcmp(substring, "0100")) {
        hex[i] = '4';
      }  else if (!strcmp(substring, "0101")) {
        hex[i] = '5';
      }  else if (!strcmp(substring, "0110")) {
        hex[i] = '6';
      }  else if (!strcmp(substring, "0111")) {
        hex[i] = '7';
      }  else if (!strcmp(substring, "1000")) {
        hex[i] = '8';
      }  else if (!strcmp(substring, "1001")) {
        hex[i] = '9';
      }  else if (!strcmp(substring, "1010")) {
        hex[i] = 'A';
      }  else if (!strcmp(substring, "1011")) {
        hex[i] = 'B';
      }  else if (!strcmp(substring, "1100")) {
        hex[i] = 'C';
      }  else if (!strcmp(substring, "1101")) {
        hex[i] = 'D';
      }  else if (!strcmp(substring, "1110")) {
        hex[i] = 'E';
      }  else if (!strcmp(substring, "1111")) {
        hex[i] = 'F';
      }
      i++;
      pline+=4;
    }
    hex[i] = '\0';
    fprintf(expand_hex, "%s", hex);
    fprintf(expand_hex, "\n");
  }  fclose(expand_hex);

}

int main(int argc, char* argv[])
{

    float scale_x =2, scale_y =2;
    char * hex = "bitmap.hex";
    if (argc >= 4) {
      char * hex = argv[1];
      scale_x = atoi(argv[2]);
      scale_y = atoi(argv[3]);
    }
  setupGraphicFont(scale_x,scale_y);
}

// int main(int argc, char* argv[])
// {
//
//     float scale =2;
//     char * hex = "bitmap.hex";
//     if (argc >= 3) {
//       char * hex = argv[1];
//       scale = atof(argv[2]);
//     }
//     scale_g = 1.0*scale;
//     // Erase file
//     pbm = fopen("output.pbm", "w");
//     fclose(pbm);
//
//     // expand = fopen("expand_out.pbm", "w");
//     // fclose(expand);
//
//     printf("Running\n");
//
//     // Origin hex file
//     FILE * origin_hex;
//     origin_hex = fopen(hex, "r");
//     //if (fp == NULL)
//     //    return -1; //exit(EXIT_FAILURE);
//     int i, j=0;
//     int len = 8;
//
//
//     pbm = fopen("output.pbm", "a"); /* b - binary mode */
//     fprintf(pbm, "P1\n%d %d\n", dimx, dimy);
//
//     while (( fgets(line, len, origin_hex)) != NULL ) {
//         //printf("%s\n", line);
//         hex2pbm(line);
//     }
//
//
//     fclose(pbm);
//     fclose(origin_hex);
//
//
//     expandPBM(scale);
//
//     FILE * expand = fopen("expand_out.pbm", "r"); /* b - binary mode */
//
//     len =24*scale_g+2;
//     fgets(line, len, expand);
//     fgets(line, len, expand);
//     //printf("size %s, %d", line, sizeof(line));
//     while (( fgets(line, len, expand)) != NULL ) {
//         //printf("%s\n", line);
//         pbm2hex(line);
//     }
//
//
//     fclose(expand_hex);
//     return 0;
// }
