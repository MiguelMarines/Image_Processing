// =========================================================================== //
//                                   HISTOGRAM                                 //
// =========================================================================== //
// Author: Miguel Marines                                                      //
// Activity: Histogram                                                         //
// =========================================================================== //


// =========================================================================== //
//                        Prototype Declarations                               //
// =========================================================================== //
void heading (void);      // Displays program heading.
void openfiles (void);    // Opens input and output files in binary.
void userdefined (void);  // Here, the user will implement his routine.
void readhdr (void);      // Read header from input image file.
void addhdr (void);       // Add header to output image file.
void closefiles (void);   // Close all files used in program.


// =========================================================================== //
//                         Include Declarations                                //
// =========================================================================== //
# include <stdio.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>


// =========================================================================== //
//                         Variable Declarations                               //
// =========================================================================== //
int    MRows, NCols;                 // Number of Rows and columns in image.
FILE   *infptr, *outfptr, *outfptrh; // Input and output file pointers.
char   infile[40];                   // Name of input file.
char   outfile[40];                  // Name of output file.


// =========================================================================== //
//                              Main Program                                   //
// =========================================================================== //
int main ()
{
     // Display heading.
     heading ();
     // Open Input and Output Image Files.
     openfiles ();
     // Read header from image file.
     readhdr();
     // Add header to output file.
     addhdr();
     // Execute userdefined procedure.
     userdefined ();
     // Close all image files.
     closefiles ();
     
     printf("\n Bye! Bye!\n");
     system("PAUSE");
     return(1);
} // End main ();


//----------------------------------------------------------------------------//
//                                  Headings                                  //
//----------------------------------------------------------------------------//
void heading ()
{ 
  printf("\n\n\n");
  printf("// ====================================================== //\n");
  printf("//                 Digital Image Processing in C          //\n");
  printf("//                    Author: Miguel Marines              //\n");
  printf("// ====================================================== //\n");
} // End heading ()


//----------------------------------------------------------------------------//
//                                 Open Files                                 //
//----------------------------------------------------------------------------//
void openfiles ()
{
     printf("\n Opening Input and Output image files\n");
     printf(" Enter name of *.pgm INPUT image file (example: lena.pgm) ");
     scanf("%s", &infile);
     
     // Check if input file exists.
     if ((infptr = fopen(infile, "rb")) == NULL)
     {
       printf(" Cannot open input image file: %s\n", infile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
      }
     
     printf(" Enter name of *.pgm OUTPUT image file (example: lenaout.pgm) ");
     scanf("%s", &outfile);
     
     // Check if output file was created succesfully.
     if ((outfptr = fopen(outfile, "wb")) == NULL)
     {
       printf(" Cannot create output image file: %s\n", outfile);
       printf(" Exiting program...\n");
       system("PAUSE");
       exit(1);
      }
       
     // If this point is reached, file are OK.
     printf(" File opened and created OK! \n\n");
     
}  // End openfiles ()


//----------------------------------------------------------------------------//
//                                 Read Header                                //
//----------------------------------------------------------------------------//
void readhdr ()
{
     int   i = 0, MaxRGB;
     char  c, buffer[128];
     
     // Read header of PGM file.
     printf("\n\n File <%s> Header: \n",infile);
     
     do
     {
       c = fgetc(infptr);
       buffer[i] = c;
       i++;
      } while (c != '\n');
     
     // Check if file is P5 (pgm) format.
     if (buffer[1] == '5')
       printf("\n Input file is pgm, OK\n");
     else
     {
       printf("\n Input file is NOT pgm, Exiting program...\n");
       system("PAUSE");
       exit(0);
      }
     
     // File is pgm, read rest of header.
     fscanf(infptr, "%d %d %d", &NCols, &MRows, &MaxRGB);
     c = fgetc(infptr);
     printf("%d ", NCols);
     printf("%d   <---- Width x Height) \n", MRows);
     printf("%d   <---- Max. Grayscale level) \n\n", MaxRGB);
}  // End readhdr ()


//----------------------------------------------------------------------------//
//                                  Add Header                                //
//----------------------------------------------------------------------------//
void addhdr ()
{
     fprintf(outfptr, "P5\n%d %d\n%d\n", NCols, MRows, 255);
} // End addhdr ()


//----------------------------------------------------------------------------//
//                                   Histogram                                //
//----------------------------------------------------------------------------//
void userdefined ()
{
	unsigned char Pixel;
	unsigned int histcount[256];
	char string[10];
	
	// iniciliazar el vector a ceros
	for (int i=0; i<256; i++) {
		histcount[i] = 0; 
	}
	
	// Contar los valores de los pixeles
	// leer el primer pixel
	Pixel = fgetc(infptr);
	do {
		histcount[Pixel]++;
		Pixel = fgetc(infptr);
	} while(!feof(infptr));
	
	// Guardar el histograma en excel
	// convertir los valores enteros a string
	for (int i=0; i<256; i++) {
		itoa(histcount[i],string,10);
		//Guardo en el excel
		fputs(string,outfptrh);
		// incluir delimatores de CSV - Salto de linea
		fputs("\n",outfptrh);
	}
	
}  // End userdefined ()


//----------------------------------------------------------------------------//
//                                 Close Files                                //
//----------------------------------------------------------------------------//
void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // End closefiles ()
