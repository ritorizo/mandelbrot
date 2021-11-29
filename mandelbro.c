#include <stdio.h>
#include <complex.h>
#include <string.h>
#include <math.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define JULIA

void writeImage(int,int,char*,char*,int);

int main(int argc) {
    int sizex = 10000;
    int sizey = 10000;
    double a = 4;
    double b = 4;
    double xmin = -2.0;
    double ymax = 2.0;
    int depth = 100;

#ifdef JULIA
    double complex c = 0.092 + -0.5*I;
#endif

    double pasx = a/sizex;
    double pasy = b/sizey;
    
    int ch=3;
    char* buffer = malloc(sizex*sizey*ch);

    printf("palette\n");
    char palette[3*depth];
    for (int i = 0; i < depth; i++) {
        double x = (double)i/depth;
        palette[i*3+0] = (char)(128.0*(0.5+0.5*(1+sin(x*10+3))));
        palette[i*3+1] = (char)(128.0*(0.5+0.5*(1-sin(x*10))));
        palette[i*3+2] = (char)(128.0*(0.5+0.5*(1-sin(x*20+1))));
    }
    // for (int i = 0; i < 3*depth; i+=3) {
    //     printf("%d %d %d\n", palette[i+0],palette[i+1],palette[i+2]);
    // }

    printf("mandelbro\n");
    double y = ymax;
    for (int i = 0; i < sizey; i++) {
        double x = xmin;
        for(int j = 0; j < sizex; j++){
#ifdef JULIA
            double complex z = x+y*I;
#else
            double complex c = x+y*I;
            double complex z = 0;
#endif
            double module = 0;
            int n = 0;
            while ((module < 2.0) && (n < depth)) {
                z = z*z + c;
                module = cabs(z);
                n++;
            }
            if (n != depth) {
                int index = (i*sizex+j)*3;
                buffer[index++] = palette[n*3+0];
                buffer[index++] = palette[n*3+1];
                buffer[index++] = palette[n*3+2];
            }
            x += pasx;
        }
        y -= pasy;
    }


    printf("write\n");
    writeImage(sizex,sizey,"darone.png",buffer, ch);
    free(buffer);
    return 0;
}

void writeImage(int width, int height, char* filepath, char* buffer, int nrChannels) {
    int stride = nrChannels * width;
    stride += (stride % nrChannels ? nrChannels - (stride % nrChannels) : 0);
    stbi_write_png(filepath, width, height, nrChannels, buffer, stride);
}
