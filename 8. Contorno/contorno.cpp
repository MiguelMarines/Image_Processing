// =========================================================================== //
//                                    CONTORNO                                 //
// =========================================================================== //
// Author: Miguel Marines                                                      //
// Activity: Contorno                                                          //
// =========================================================================== //


// =========================================================================== //
//                        Prototype Declarations                               //
// =========================================================================== //
void heading (void);      // Displays program heading.
void openfiles (void);    // Opens input and output files in binary.
void negative(void);      // Convert original image to negative.
void readhdr (void);      // Read header from input image file.
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
int      MRows, NCols;              // Number of Rows and columns in image.
FILE     *infptr, *outfptr;         // Input and output file pointers.
char     infile[40];                // Name of input file.
char     static  outfile[40];       // Name of output file.


// =========================================================================== //
//                              Main Program                                   //
// =========================================================================== //
int main()
{
    //--------------(Display Heading with Information)--------------------------
    heading();
    //---------------(Open Input & Output Image Files)--------------------------
    openfiles();
    //---------------------(Obtain Negative of Image)---------------------------
    negative();
    //------------------(Close any Open Image Files)----------------------------
    closefiles();
    //--------------------------------------------------------------------------
    printf("\n Bye! Bye!\n");
    system("PAUSE");
    return (1);
} // End main()


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


//------------------------------------------------------------------------------
//--------------------------Obtain convolved image------------------------------
//---------------by taking the one's complement of every pixel------------------
//------------------------------------------------------------------------------
void negative()
{
  unsigned char Gray, GrayH, GrayV;
  unsigned char Mag;
  unsigned char mat [512][512];  // imagen original
  unsigned char convH[512][512];
  unsigned char convV[512][512];
  // Blur mask
  //         char mask[  3][  3] ={{ 1, 1, 1},
  //                               { 1, 1, 1},
  //								 { 1, 1, 1}};

  // Sobel horizontal mask (edge detection)
           int maskH[  3][  3] = {{ 1, 2, 1},
                                 { 0, 0, 0},
  								 {-1,-2,-1}};
								 
  // Sobel vertical mask (edge detection)
           int maskV[  3][  3] = {{-1, 0, 1},
                                 {-2, 0, 2},
  								 {-1, 0, 1}};
								 
  // Sharpening mask (second derivative)
  //          int mask[  3][  3] = {{ 0, 1, 0},
  //                                 { 1,-4, 1},
  //								 { 0, 1, 0}};
								 
  // Sharpening mask (second derivative)
  //         int mask[  3][  3] = {{ 1, 1, 1},
  //                               { 1,-8, 1},
  //		     					 { 1, 1, 1}};
								 
  int col, row, value;
  
  int dividefactor = 0;
  // Calcula dividefactor
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskH[col][row];
      }
      
  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despues) = %d \n",dividefactor);
    
  //Add *.pgm Header to output file
  fprintf(outfptr,"P5\n512 512\n255\n");
  
  //Read input image and store en matrix
  for (col = 0; col < MRows; col++)
    for (row = 0; row < NCols; row++) {
        Gray = fgetc(infptr);
        mat[col][row] = Gray;
    }
  
  // Clear convolution matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	convH[col][row] = 0;

  //Convolve image with mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskH[0][0] * mat[col-1][row-1]  +
      	                   maskH[0][1] * mat[col  ][row-1]  +
      	                   maskH[0][2] * mat[col+1][row-1]  +
      	                   maskH[1][0] * mat[col-1][row  ]  +
      	                   maskH[1][1] * mat[col  ][row  ]  +
      	                   maskH[1][2] * mat[col+1][row  ]  +
      	                   maskH[2][0] * mat[col-1][row+1]  +
      	                   maskH[2][1] * mat[col  ][row+1]  +
      	                   maskH[2][2] * mat[col+1][row+1]) / dividefactor);
    	convH[col][row] = value; /// gradiente x
    	if      (value <   0) convH[col][row] = 0;
    	else if (value > 255) convH[col][row] = 255;
    	else                  convH[col][row] = value;
      }
  
  // printf("Fisrt convolved pixel = %d \n",conv[1][1]);






  // Calcula dividefactor
  for (col = 0; col < 3; col++)
	for (row = 0; row < 3; row++) {
      	dividefactor += maskV[col][row];
      }
      
  printf ("dividefactor (antes) = %d \n",dividefactor);
      
  if (dividefactor == 0)
		dividefactor = 1;
		
  printf ("dividefactor (despues) = %d \n",dividefactor);
    
  
  // Clear convolution matrix
  for (col = 0; col < MRows; col++)
      for (row = 0; row < NCols; row++)
      	convV[col][row] = 0;

  //Convolve image with mask
    for (col = 1; col < MRows-1; col++)
      for (row = 1; row < NCols-1; row++) {
      	value          = ((maskV[0][0] * mat[col-1][row-1]  +
      	                   maskV[0][1] * mat[col  ][row-1]  +
      	                   maskV[0][2] * mat[col+1][row-1]  +
      	                   maskV[1][0] * mat[col-1][row  ]  +
      	                   maskV[1][1] * mat[col  ][row  ]  +
      	                   maskV[1][2] * mat[col+1][row  ]  +
      	                   maskV[2][0] * mat[col-1][row+1]  +
      	                   maskV[2][1] * mat[col  ][row+1]  +
      	                   maskV[2][2] * mat[col+1][row+1]) / dividefactor);
    	convV[col][row] = value;  // gradiente y
    	if      (value <   0) convV[col][row] = 0;
    	else if (value > 255) convV[col][row] = 255;
    	else                  convV[col][row] = value;
      }






  
  //Save countour image 
  for (col = 0; col < NCols; col++)
    for (row = 0; row < MRows; row++) {
    	GrayH = convH[col][row];
        GrayV = convV[col][row];
        Mag = sqrt(GrayH*GrayH + GrayV*GrayV);
        fputc(Mag,outfptr);
    }   
}


//----------------------------------------------------------------------------//
//                                 Close Files                                //
//----------------------------------------------------------------------------//
void closefiles ()
{
     fclose(infptr);
     fclose(outfptr);
}  // End closefiles ()
