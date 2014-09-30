/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"

/* 
 * Please fill in the following team struct 
 */
team_t team = {
    "mc467+pac26",          /* Team name */

    "mc467",                /* First member Route-Y ID */
    "Michael C. Clark",     /* First member full name */
    "mc467@email.byu.edu",  /* First member email address */

    "pac26",                /* Second member Route-Y ID */
    "Peter Clevenger",      /* Second member full name (leave blank if none) */
    "pac26@email.byu.edu"   /* Second member email addr (leave blank if none) */
};

/***************
 * ROTATE KERNEL
 ***************/

/******************************************************
 * Your different versions of the rotate kernel go here
 ******************************************************/

/* 
 * naive_rotate - The naive baseline version of rotate 
 */
char naive_rotate_descr[] = "naive_rotate: Naive baseline implementation";
void naive_rotate(int dim, pixel *src, pixel *dst) 
{
    int i, j;

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++)
	    dst[RIDX(dim-1-j, i, dim)] = src[RIDX(i, j, dim)];
}

/* 
 * rotate - Your current working version of rotate
 * IMPORTANT: This is the version you will be graded on
 */
char rotate_descr[] = "rotate: blocking (32), unrolling(32), pointer code";
void rotate(int dim, pixel *src, pixel *dst) {
    int i, j, i5, j5, ii, div = dim >> 5; /* dim/32 */
    pixel *source, *dest, *destore;
    
    for(i = 0; i < div; i++) {
        i5 = i<<5;
        for(j = 0; j < div; j++) { /* which block */
            j5 = j<<5;
            source = src+RIDX(i5, j5, dim);
            dest = destore = dst+RIDX(dim-1-(j5), i5, dim);
            for(ii = 0; ii < 32; ii++,source+=(dim-32),dest = ++destore) {
                /* source = src+RIDX(i5+ii, j5, dim); */
                /* dest = dst+RIDX(dim-1-(j5), i5+ii, dim)+dim; */
                
                *(dest) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                /*
                source+=(dim-32);
                dest = ++destore;
                
                *(dest) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                source+=(dim-32);
                dest = ++destore;
                
                *(dest) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                source+=(dim-32);
                dest = ++destore;
                
                *(dest) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;                
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++; */
            }
        }
    }   
}

char rotate6_descr[] = "rotate: blocking (32), unrolling(32), pointer code";
void rotate6(int dim, pixel *src, pixel *dst) {
    int i, j, i5, j5, ii, div = dim >> 5; /* dim/32 */
    pixel *source, *dest;
    
    for(i = 0; i < div; i++) {
        i5 = i<<5;
        for(j = 0; j < div; j++) { /* which block */
            j5 = j<<5;
            for(ii = 0; ii < 32; ii++) {
                source = src+RIDX(i5+ii, j5, dim);
                dest = dst+RIDX(dim-1-(j5), i5+ii, dim)+dim;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
                *(dest-=dim) = *source++;
            }
        }
    }   
}

char rotate5_descr[] = "rotate: blocking (32), unrolling(8)";
void rotate5(int dim, pixel *src, pixel *dst) {
    int i, j, i5, j5, ii, jj, ridix, rid2, div = dim >> 5; /* dim/32 */
    
    for(i = 0; i < div; i++) {
        i5 = i<<5;
        for(j = 0; j < div; j++) { /* which block */
            j5 = j<<5;
            for(ii = 0; ii < 32; ii++) {
                for(jj = 0; jj < 32; jj+=8) {
                    ridix = RIDX(i5+ii, j5+jj, dim);
                    rid2 = RIDX(dim-1-(j5+jj), i5+ii, dim)+dim;
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                    dst[(rid2-=dim)] = src[ridix++];
                }
            }
        }
    }   
}
 
char rotate4_descr[] = "rotate: blocking (32), unrolling(32)";
void rotate4(int dim, pixel *src, pixel *dst) {
    int i, j, i5, j5, ii, ridix, rid2, div = dim >> 5; /* dim/32 */
    
    for(i = 0; i < div; i++) {
        i5 = i<<5;
        for(j = 0; j < div; j++) { /* which block */
            j5 = j<<5;
            for(ii = 0; ii < 32; ii++) {
                ridix = RIDX(i5+ii, j5, dim);
                rid2 = RIDX(dim-1-(j5), i5+ii, dim)+dim;
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
                dst[(rid2-=dim)] = src[ridix++];
            }
        }
    }   
}
 
char rotate3_descr[] = "rotate: blocking (32)";
void rotate3(int dim, pixel *src, pixel *dst) {
    int i, j, i5, j5, ii, jj, div = dim >> 5; /* dim/32 */
    
    for(i = 0; i < div; i++) {
        i5 = i<<5;
        for(j = 0; j < div; j++) { /* which block */
            j5 = j<<5;
            for(ii = 0; ii < 32; ii++) {
                for(jj = 0; jj < 32; jj++) {
                    dst[RIDX(dim-1-(j5+jj), i5+ii, dim)] = src[RIDX(i5+ii, j5+jj, dim)];
                }
            }
        }
    }   
}
 
char rotate1_descr[] = "rotate: unrolling";
void rotate1(int dim, pixel *src, pixel *dst)  {
    int i, j, ridx1, ridx2;

    /*if(src == dst) { printf("Aliasing!\n"); return; }*/
    
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j+=32) {
            ridx1 = RIDX(dim-1-j, i, dim);
            ridx2 = RIDX(i, j, dim);
            
	        dst[ridx1] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++]; 
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++]; 
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++]; 
            
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];
            dst[(ridx1-=dim)] = src[ridx2++];   
        }
}

char rotate2_descr[] = "rotate: paralellism and unrolling";
void rotate2(int dim, pixel *src, pixel *dst)  {
    int i, j, ridx11, ridx12, ridx21, ridx22, dim2 = dim<<1;

    /*if(src == dst) { printf("Aliasing!\n"); return; }*/
    
    for (i = 0; i < dim; i++)
        for (j = 0; j < dim; j+=32) {
            ridx11 = RIDX(dim-1-j, i, dim);
            ridx12 = RIDX(dim-2-j, i, dim);
            ridx21 = RIDX(i, j, dim);
            ridx22 = RIDX(i, j, dim) + 1;
            
	        dst[ridx11] = src[ridx21];
            dst[ridx12] = src[ridx22];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
            dst[(ridx11-=dim2)] = src[ridx21+=2];
            dst[(ridx12-=dim2)] = src[ridx22+=2];
        }
}

/*********************************************************************
 * register_rotate_functions - Register all of your different versions
 *     of the rotate kernel with the driver by calling the
 *     add_rotate_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_rotate_functions() 
{
    add_rotate_function(&naive_rotate, naive_rotate_descr);   
    add_rotate_function(&rotate, rotate_descr);
    /* add_rotate_function(&rotate1, rotate1_descr); */
    /* add_rotate_function(&rotate2, rotate2_descr); */
    /* add_rotate_function(&rotate3, rotate3_descr); */
    /* add_rotate_function(&rotate4, rotate4_descr); */
    /* add_rotate_function(&rotate5, rotate5_descr); */
    /* add_rotate_function(&rotate6, rotate6_descr); */
    /* ... Register additional test functions here */
}


/***************
 * SMOOTH KERNEL
 **************/

/***************************************************************
 * Various typedefs and helper functions for the smooth function
 * You may modify these any way you like.
 **************************************************************/

/* A struct used to compute averaged pixel value */
typedef struct {
    int red;
    int green;
    int blue;
    int num;
} pixel_sum;

/* Compute min and max of two integers, respectively */
static int min(int a, int b) { return (a < b ? a : b); }
static int max(int a, int b) { return (a > b ? a : b); }

/* 
 * initialize_pixel_sum - Initializes all fields of sum to 0 
 */
static void initialize_pixel_sum(pixel_sum *sum) 
{
    sum->red = sum->green = sum->blue = 0;
    sum->num = 0;
    return;
}

/* 
 * accumulate_sum - Accumulates field values of p in corresponding 
 * fields of sum 
 */
static void accumulate_sum(pixel_sum *sum, pixel p) 
{
    sum->red += (int) p.red;
    sum->green += (int) p.green;
    sum->blue += (int) p.blue;
    sum->num++;
    return;
}

/* 
 * assign_sum_to_pixel - Computes averaged pixel value in current_pixel 
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum) 
{
    current_pixel->red = (unsigned short) (sum.red/sum.num);
    current_pixel->green = (unsigned short) (sum.green/sum.num);
    current_pixel->blue = (unsigned short) (sum.blue/sum.num);
    return;
}

/* 
 * avg - Returns averaged pixel value at (i,j) 
 */
static pixel avg(int dim, int i, int j, pixel *src) 
{
    int ii, jj;
    pixel_sum sum;
    pixel current_pixel;

    initialize_pixel_sum(&sum);
    for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) 
	for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) 
	    accumulate_sum(&sum, src[RIDX(ii, jj, dim)]);

    assign_sum_to_pixel(&current_pixel, sum);
    return current_pixel;
}

/******************************************************
 * Your different versions of the smooth kernel go here
 ******************************************************/

/*
 * naive_smooth - The naive baseline version of smooth 
 */
char naive_smooth_descr[] = "naive_smooth: Naive baseline implementation";
void naive_smooth(int dim, pixel *src, pixel *dst) 
{
    int i, j;
    /* pixel temp; */

    for (i = 0; i < dim; i++)
	for (j = 0; j < dim; j++) {
	    dst[RIDX(i, j, dim)] = avg(dim, i, j, src);
        /*
        if(i == 95 && j == 95 && dim == 96) { 
             temp = dst[RIDX(i, j, dim)]; 
             printf("r: %d\tg: %d\tb: %d\tdim: %d\n", temp.red, temp.blue, temp.green, dim);
        } */
    }
}

/*
 * smooth - Your current working version of smooth. 
 * IMPORTANT: This is the version you will be graded on
 */
char smooth_descr[] = "smooth: Current working version";
void smooth(int dim, pixel *src, pixel *dst) 
{
  int i, j, r,g,b, index;
  pixel *pp;
  /* int c[3][3]; */
  /* case 1 top left corner */
  /*printf("top left before: r %d g %d b %d\n",dst[0].red,dst[0].green,dst[0].blue);*/
  r = src[0].red + src[1].red;
  g = src[0].green + src[1].green;
  b = src[0].blue + src[1].blue;
  r += src[dim].red +src[dim+1].red;
  g += src[dim].green +src[dim+1].green;
  b += src[dim].blue +src[dim+1].blue;
  dst[0].red = (unsigned short) (r>>2);
  dst[0].green = (unsigned short) (g>>2);
  dst[0].blue = (unsigned short) (b>>2);
  /*printf("top left after: r %d g %d b %d\n",dst[0].red,dst[0].green,dst[0].blue);*/


  /* case 2 top row without corners */
  /* printf("top middle before: r %d g %d b %d\n",dst[27].red,dst[27].green,dst[27].blue);*/
  for(j=1;j<dim-1;j++){
      r = src[j-1].red + src[j].red + src[j+1].red;
      g = src[j-1].green + src[j].green + src[j+1].green;
      b = src[j-1].blue + src[j].blue + src[j+1].blue;
      r += src[dim+j-1].red + src[dim+j].red + src[dim+j+1].red;
      g += src[dim+j-1].green + src[dim+j].green + src[dim+j+1].green;
      b += src[dim+j-1].blue + src[dim+j].blue + src[dim+j+1].blue;
      dst[j].red = (unsigned short) (r/6);
      dst[j].green = (unsigned short) (g/6);
      dst[j].blue = (unsigned short) (b/6);
  }
  /*printf("top left after: r %d g %d b %d\n",dst[27].red,dst[27].green,dst[27].blue);*/

  /* case 3 top right corner */
  r = src[dim-2].red + src[dim-1].red;
  g = src[dim-2].green + src[dim-1].green;
  b = src[dim-2].blue + src[dim-1].blue;
  r += src[2*dim-2].red +src[2*dim-1].red;
  g += src[2*dim-2].green +src[2*dim-1].green;
  b += src[2*dim-2].blue +src[2*dim-1].blue;
  dst[dim-1].red = (unsigned short) (r>>2);
  dst[dim-1].green = (unsigned short) (g>>2);
  dst[dim-1].blue = (unsigned short) (b>>2);

  /* case 4 right column without corners */
  for(i=2,index=2*dim-1;i<dim;i++,index+=dim){
    r = src[(index-dim)-1].red + src[index-dim].red;
    g = src[(index-dim)-1].green+ src[index-dim].green;
    b = src[(index-dim)-1].blue+ src[index-dim].blue;
    r += src[index-1].red + src[index].red;
    g += src[index-1].green + src[index].green;
    b += src[index-1].blue + src[index].blue;
    r += src[(index+dim)-1].red + src[index+dim].red;
    g += src[(index+dim)-1].green + src[index+dim].green;
    b += src[(index+dim)-1].blue + src[index+dim].blue;


    dst[index].red = (unsigned short) (r/6);
    dst[index].green = (unsigned short) (g/6);
    dst[index].blue = (unsigned short) (b/6);
  }


  /* case 5 bottom right corner */
  r = src[(dim)*(dim-1)-1].red + src[(dim)*(dim-1)-2].red;
  g = src[(dim)*(dim-1)-1].green + src[(dim)*(dim-1)-2].green;
  b = src[(dim)*(dim-1)-1].blue + src[(dim)*(dim-1)-2].blue;
  r += src[dim*dim-1].red +src[dim*dim-2].red;
  g += src[dim*dim-1].green +src[dim*dim-2].green;
  b += src[dim*dim-1].blue +src[dim*dim-2].blue;
  dst[dim*dim-1].red = (unsigned short) (r>>2);
  dst[dim*dim-1].green = (unsigned short) (g>>2);
  dst[dim*dim-1].blue = (unsigned short) (b>>2);


  /* case 6 bottom row without corners */
  for(j=1,index=dim*(dim-1)+1;j<dim-1;j++,index++){
    r = src[(index-dim)-1].red + src[index-dim].red + src[(index-dim)+1].red;
    g = src[(index-dim)-1].green + src[index-dim].green + src[(index-dim)+1].green;
    b = src[(index-dim)-1].blue + src[index-dim].blue + src[(index-dim)+1].blue;
    r += src[index-1].red + src[index].red + src[index+1].red;
    g += src[index-1].green + src[index].green + src[index+1].green;
    b += src[index-1].blue + src[index].blue + src[index+1].blue;
    dst[index].red = (unsigned short) (r/6);
    dst[index].green = (unsigned short) (g/6);
    dst[index].blue = (unsigned short) (b/6);
  }
  
  /* case 7 bottom left corner */
  r = src[(dim)*(dim-1)].red + src[(dim)*(dim-1)+1].red;
  g = src[(dim)*(dim-1)].green + src[(dim)*(dim-1)+1].green;
  b = src[(dim)*(dim-1)].blue + src[(dim)*(dim-1)+1].blue;
  r += src[dim*(dim-2)].red +src[dim*(dim-2)+1].red;
  g += src[dim*(dim-2)].green +src[dim*(dim-2)+1].green;
  b += src[dim*(dim-2)].blue +src[dim*(dim-2)+1].blue;
  dst[dim*(dim-1)].red = (unsigned short) (r>>2);
  dst[dim*(dim-1)].green = (unsigned short) (g>>2);
  dst[dim*(dim-1)].blue = (unsigned short) (b>>2);
  
  /* case 8 left column without corners */
  for(i=1,index=dim;i<dim-1;i++,index+=dim){
    r = src[index-dim].red + src[(index-dim)+1].red;
    g = src[index-dim].green+ src[(index-dim)+1].green;
    b = src[index-dim].blue+ src[(index-dim)+1].blue;
    r += src[index].red + src[index+1].red;
    g += src[index].green + src[index+1].green;
    b += src[index].blue + src[index+1].blue;
    r += src[index+dim].red + src[(index+dim)+1].red;
    g += src[index+dim].green + src[(index+dim)+1].green;
    b += src[index+dim].blue + src[(index+dim)+1].blue;


    dst[index].red = (unsigned short) (r/6);
    dst[index].green = (unsigned short) (g/6);
    dst[index].blue = (unsigned short) (b/6);
  }
  
  /* case 9 all the middle with no border sections */
  for(i=1,index=dim,pp=(src+dim);i<dim-1;i++,index++,pp++){
    /*
    c[0][0] = src[index-dim].red +src[index].red + src[index+dim].red;
    c[0][1] = src[index-dim].green +src[index].green + src[index+dim].green;
    c[0][2] = src[index-dim].blue +src[index].blue + src[index+dim].blue;
    c[1][0] = src[index-dim+1].red +src[index+1].red + src[index+dim+1].red;
    c[1][1] = src[index-dim+1].green +src[index+1].green + src[index+dim+1].green;
    c[1][2] = src[index-dim+1].blue +src[index+1].blue + src[index+dim+1].blue;

    for(j=1,index++;j<dim-1;j++,index++){
      c[(j+1)%3][0] = src[index-dim+1].red +src[index+1].red + src[index+dim+1].red;
      c[(j+1)%3][1] = src[index-dim+1].green +src[index+1].green + src[index+dim+1].green;
      c[(j+1)%3][2] = src[index-dim+1].blue +src[index+1].blue + src[index+dim+1].blue;

      dst[index].red = (unsigned short) ((c[0][0]+c[1][0]+c[2][0])/9);
      dst[index].green = (unsigned short) ((c[0][1]+c[1][1]+c[2][1])/9);
      dst[index].blue = (unsigned short) ((c[0][2]+c[1][2]+c[2][2])/9);
    } */
    /*
    for(j=1,index++;j<dim-1;j++,index++){
      r = src[(index-dim)-1].red + src[(index-dim)].red + src[(index-dim)+1].red;
      g = src[(index-dim)-1].green + src[(index-dim)].green + src[(index-dim)+1].green;
      b = src[(index-dim)-1].blue + src[(index-dim)].blue + src[(index-dim)+1].blue;
      r += src[index-1].red + src[index].red + src[index+1].red;
      g += src[index-1].green + src[index].green + src[index+1].green;
      b += src[index-1].blue + src[index].blue + src[index+1].blue;
      r += src[(index+dim)-1].red + src[(index+dim)].red + src[(index+dim)+1].red;
      g += src[(index+dim)-1].green + src[(index+dim)].green + src[(index+dim)+1].green;
      b += src[(index+dim)-1].blue + src[(index+dim)].blue + src[(index+dim)+1].blue;

      dst[index].red = (unsigned short) (r/9);
      dst[index].green = (unsigned short) (g/9);
      dst[index].blue = (unsigned short) (b/9);
    }
    */
    for(j=1,index++,pp++;j<dim-1;j++,index++,pp++){
      r = ((pp-dim)-1)->red + (pp-dim)->red + ((pp-dim)+1)->red;
      g = ((pp-dim)-1)->green + (pp-dim)->green + ((pp-dim)+1)->green;
      b = ((pp-dim)-1)->blue + (pp-dim)->blue + ((pp-dim)+1)->blue;
      r += (pp-1)->red + pp->red + (pp+1)->red;
      g += (pp-1)->green + pp->green + (pp+1)->green;
      b += (pp-1)->blue + pp->blue + (pp+1)->blue;
      r += ((pp+dim)-1)->red + (pp+dim)->red + ((pp+dim)+1)->red;
      g += ((pp+dim)-1)->green + (pp+dim)->green + ((pp+dim)+1)->green;
      b += ((pp+dim)-1)->blue + (pp+dim)->blue + ((pp+dim)+1)->blue;

      dst[index].red = (unsigned short) (r/9);
      dst[index].green = (unsigned short) (g/9);
      dst[index].blue = (unsigned short) (b/9);
    }
  }
} 
 
char smooth1_descr[] = "smooth: Slow, attemps never worked.";
void smooth1(int dim, pixel *src, pixel *dst) 
{
    int i, j, num, red, green, blue;
    pixel *dest, *source;
    /* unsigned short *value; */
    
    /* int ii, jj; */
    /* pixel_sum sum; */
    pixel current_pixel;
    
    /* unsigned short c[3][3]; */
    
    dest = dst;
    
    /* Corner 1 */ /*
    red = src[0][0].red + src[1][0].red;
    c[0][0] = red/2;
    red = src[0][1].red + src[1][1].red;
    c[1][0] = red/2;
    
    green = src[0][0].green + src[1][0].green;
    c[0][1] = green/2;
    green = src[0][1].green + src[1][1].green;
    c[1][1] = green/2;
    
    blue = src[0][0].blue + src[1][0].blue;
    c[0][2] = blue/2;
    blue = src[0][1].blue + src[1][1].blue;
    c[1][2] = blue/2;
    
    current_pixel.red = (c[0][0] + c[1][0])/2;
    current_pixel.green = (c[0][1] + c[1][1])/2;
    current_pixel.blue = (c[0][2] + c[1][2])/2;
    *dest = current_pixel;
    dest++;
    */
    /* Top Row */ /*
    for(j=1; j < dim-1; j++,dest++) {
        red = src[0][j+1].red + src[1][j+1].red;
        c[(j+1)%3][0] = red/2;
        green = src[0][j+1].green + src[1][j+1].green;
        c[(j+1)%3][1] = green/2;
        blue = src[0][j+1].blue + src[1][j+1].blue;
        c[(j+1)%3][2] = blue/2;
        
        current_pixel.red = (c[0][0] + c[1][0] + c[2][0])/3;
        current_pixel.green = (c[0][1] + c[1][1] + c[2][1])/3;
        current_pixel.blue = (c[0][2] + c[1][2] + c[2][2])/3;
        *dest = current_pixel;
    }
    */
    /* Corner 2 */ /*
    current_pixel.red = (c[(dim-2)%3][0] + c[(dim-1)%3][0])/2;
    current_pixel.green = (c[(dim-2)%3][1] + c[(dim-1)%3][1])/2;
    current_pixel.blue = (c[(dim-2)%3][2] + c[(dim-1)%3][2])/2;
    *dest = current_pixel;
    dest++;
    */
    for (i = 0; i < dim; i++/*,dest = dst+i*dim*/) {
        /*
        source = src+RIDX(i, 0, dim);
        
        red = (source-dim)->red + source->red + (source+dim)->red;
        green = (source-dim)->green + source->green + (source+dim)->green;
        blue = (source-dim)->blue + source->blue + (source+dim)->blue;
        c[0][0] = (unsigned short) red/3;
        c[0][1] = (unsigned short) green/3;
        c[0][2] = (unsigned short) blue/3;
        source++;
        red = (source-dim)->red + source->red + (source+dim)->red;
        green = (source-dim)->green + source->green + (source+dim)->green;
        blue = (source-dim)->blue + source->blue + (source+dim)->blue;
        c[1][0] = (unsigned short) red/3;
        c[1][1] = (unsigned short) green/3;
        c[1][2] = (unsigned short) blue/3;
        
        current_pixel.red = (unsigned short) ((c[0][0]+c[1][0])/2);
        current_pixel.green = (unsigned short) ((c[0][1]+c[1][1])/2);
        current_pixel.blue = (unsigned short) ((c[0][2]+c[1][2])/2);
        *dest = current_pixel;
        dest++;
        */
	    for (j = 0; j < dim; j++,dest++) {
            /* dest = dst+RIDX(i, j, dim); */
            source = src+RIDX(i, j, dim);
            
            /*source = src;*/
            
            /* initialize_pixel_sum(&sum); */
            red = green = blue = num = 0;

            if(i-1 >= 0) {
                source-=dim;
                if(j-1 >= 0) {
                    /* accumulate_sum(&sum, *(source-1)); */
                    red += (int) (source-1)->red;
                    green += (int) (source-1)->green;
                    blue += (int) (source-1)->blue;
                    num++;
                }
                /* accumulate_sum(&sum, *(source)); */
                red += (int) source->red;
                green += (int) source->green;
                blue += (int) source->blue;
                num++;
                if(j+1 <= dim-1) {
                    /* accumulate_sum(&sum, *(source+1)); */
                    red += (int) (source+1)->red;
                    green += (int) (source+1)->green;
                    blue += (int) (source+1)->blue;
                    num++;
                }
                source+=dim;
            }
                if(j-1 >= 0) {
                    /* accumulate_sum(&sum, *(source-1)); */
                    red += (int) (source-1)->red;
                    green += (int) (source-1)->green;
                    blue += (int) (source-1)->blue;
                    num++;
                }
                /* accumulate_sum(&sum, *(source)); */
                red += (int) source->red;
                green += (int) source->green;
                blue += (int) source->blue;
                num++;
                if(j+1 <= dim-1) {
                    /* accumulate_sum(&sum, *(source+1)); */
                    red += (int) (source+1)->red;
                    green += (int) (source+1)->green;
                    blue += (int) (source+1)->blue;
                    num++;
                }
            if(i+1 <= dim-1) {
                source+=dim;
                if(j-1 >= 0) {
                    /* accumulate_sum(&sum, *(source-1)); */
                    red += (int) (source-1)->red;
                    green += (int) (source-1)->green;
                    blue += (int) (source-1)->blue;
                    num++;
                }
                /* accumulate_sum(&sum, *(source)); */
                red += (int) source->red;
                green += (int) source->green;
                blue += (int) source->blue;
                num++;
                if(j+1 <= dim-1) {
                    /* accumulate_sum(&sum, *(source+1)); */
                    red += (int) (source+1)->red;
                    green += (int) (source+1)->green;
                    blue += (int) (source+1)->blue;
                    num++;
                }
                source-=dim;
            }
            
            /* assign_sum_to_pixel(&current_pixel, sum); */
            current_pixel.red = (unsigned short) (red/num);
            current_pixel.green = (unsigned short) (green/num);
            current_pixel.blue = (unsigned short) (blue/num);
            
            /* dst[RIDX(i, j, dim)] = current_pixel; */
            *dest = current_pixel;
        }
        /*
        current_pixel.red = (unsigned short) ((c[(dim-2)%3][0]+c[(dim-1)%3][0])/2);
        current_pixel.green = (unsigned short) ((c[(dim-2)%3][1]+c[(dim-1)%3][1])/2);
        current_pixel.blue = (unsigned short) ((c[(dim-2)%3][2]+c[(dim-1)%3][2])/2);
        *dest = current_pixel;
        dest++;
        */
    }
}


/********************************************************************* 
 * register_smooth_functions - Register all of your different versions
 *     of the smooth kernel with the driver by calling the
 *     add_smooth_function() for each test function.  When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_smooth_functions() {
    add_smooth_function(&smooth, smooth_descr);
    add_smooth_function(&naive_smooth, naive_smooth_descr);
    /* add_smooth_function(&smooth1, smooth1_descr); */
    /* ... Register additional test functions here */
}

