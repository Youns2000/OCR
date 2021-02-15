#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include </mingw64/include/SDL2/SDL.h>
#include </mingw64/include/SDL2/SDL_image.h>

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}

Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}

Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);
    //printf("%d ", *p);
    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

int max(Uint8 r, Uint8 g, Uint8 b){
	if(r>g && r>b) return r;
    else if(b>g) return b;
   	else return g;
}


/*void print_matrice(int *matrice,size_t width, size_t height){
	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			printf("%d ",*matrice[i][j]);
		}
		printf("\n");
	}
}*/


int main(int argc, char *argv[])
{
	SDL_Surface* image_surface = load_image("/home/ybenr/OCR/ocr/NN/pictures/C.png");

	int width = image_surface->w;
    int height = image_surface->h;

    int matrice[width][height];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            Uint32 pixel = get_pixel(image_surface, x, y);
            Uint8 r, g, b;
            SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
            matrice[x][y] = max(r,g,b);
        }
    }

    for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			printf("%d ",matrice[i][j]);
		}
		printf("\n");
	}
	return 0;
}