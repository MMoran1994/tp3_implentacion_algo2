#ifndef __LTexture_H__
#define __LTexture_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>

// Texture wrapper class
class LTexture {
public:
  // Initializes variables
  LTexture(SDL_Renderer *gRenderer);

  // Deallocates memory
  ~LTexture();

  // Loads image at specified path
  bool loadFromFile(std::string path);

  // Deallocates texture
  void free();

  // Renders texture at given point
  void render(int x, int y, SDL_Rect *clip = NULL, int scale = 1) const;

  // Gets image dimensions
  int getWidth();
  int getHeight();

private:
  // The actual hardware texture
  SDL_Texture *mTexture;
  SDL_Renderer *gRenderer;

  // Image dimensions
  int mWidth;
  int mHeight;
};

#endif
