//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#include "libpixyusb2.h"

Pixy2        pixy;

int writePPM(uint16_t width, uint16_t height, uint32_t *image, const char *filename, char *fileno)
{
  int i, j;
  char fn[32];

  sprintf(fn, "/tmp/imgs/%s_%s.ppm", filename, fileno);
  printf("%s,%s \n",fn,fileno);
  FILE *fp = fopen(fn, "wb");
  if (fp==NULL)
    return -1;
  fprintf(fp, "P6\n%d %d\n255\n", width, height);
  for (j=0; j<height; j++)
  {
    for (i=0; i<width; i++)
      fwrite((char *)(image + j*width + i), 1, 3, fp);
  }
  fclose(fp);
  return 0;
}


int writergb(uint16_t width, uint16_t height, uint32_t *image, const char *filename, char *fileno)
{
  int i, j;
  char fn[32];

  sprintf(fn, "/tmp/imgs/%s_%s.rgb", filename, fileno);
  printf("%s,%s \n",fn,fileno);
  FILE *fp = fopen(fn, "wb");
  if (fp==NULL)
    return -1;
  for (j=0; j<height; j++)
  {
    for (i=0; i<width; i++)
      fwrite((char *)(image + j*width + i), 1, 3, fp);
  }
  fclose(fp);
  return 0;
}

int writebayer(uint16_t width, uint16_t height, uint8_t *image, const char *filename, char *fileno)
{
  char fn[32];

  sprintf(fn, "/tmp/imgs/%s_%s.bayer", filename, fileno);
  printf("%s,%s \n",fn,fileno);
  FILE *fp = fopen(fn, "wb");
  if (fp==NULL)
    return -1;
  fwrite(image, 1, width*height, fp);
  fclose(fp);
  return 0;
}

void demosaic(uint16_t width, uint16_t height, const uint8_t *bayerImage, uint32_t *image)
{
  uint32_t x, y, xx, yy, r, g, b;
  uint8_t *pixel0, *pixel;

  for (y=0; y<height; y++)
  {
    yy = y;
    if (yy==0)
      yy++;
    else if (yy==height-1)
      yy--;
    pixel0 = (uint8_t *)bayerImage + yy*width;
    for (x=0; x<width; x++, image++)
    {
      xx = x;
      if (xx==0)
        xx++;
      else if (xx==width-1)
        xx--;
      pixel = pixel0 + xx;
      if (yy&1)
      {
        if (xx&1)
        {
          r = *pixel;
          g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
          b = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
        }
        else
        {
          r = (*(pixel-1)+*(pixel+1))>>1;
          g = *pixel;
          b = (*(pixel-width)+*(pixel+width))>>1;
        }
      }
      else
      {
        if (xx&1)
        {
          r = (*(pixel-width)+*(pixel+width))>>1;
          g = *pixel;
          b = (*(pixel-1)+*(pixel+1))>>1;
        }
        else
        {
          r = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
          g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
          b = *pixel;
        }
      }
      *image = (b<<16) | (g<<8) | r; 
    }
  }
}


int main(int argc, char *argv[])
{
  int  Result;
  int i,j,k = 0;
  char format;
  char *no_of_frames_c = NULL;
  int no_of_frames = 0;
  int no_of_chars = 0;
  uint8_t *bayerFrame;
  uint32_t rgbFrame[PIXY2_RAW_FRAME_WIDTH*PIXY2_RAW_FRAME_HEIGHT];
  char fileno[8] = {0,};

  if(argc > 1){
    no_of_frames_c = argv[1];
    format = *argv[2];
    no_of_frames = atoi(no_of_frames_c);
    printf("no of frames: %d\nforamt: %c\nno of frames: %s\n", no_of_frames, format, no_of_frames_c);
  }else{
    printf("Requires two arguments\n\t1 no of frames\n\t2 format\n\t\t-r for rgb\n\t\t-p for ppm\n\t\t-b for bayer BGGR 8bit\n");
    return 0;
  }

  printf ("=============================================================\n");
  printf ("= PIXY2 Starting dump                                       =\n");
  printf ("=============================================================\n");

  // Fine no of chars required for file name
  {
    while(*no_of_frames_c != '\0'){
      no_of_frames_c++;
      no_of_chars++;
    }
  }

  printf("Connecting to Pixy2...");

  // Initialize Pixy2 Connection //
  {
    Result = pixy.init();

    if (Result < 0)
    {
      printf ("Error\n");
      printf ("pixy.init() returned %d\n", Result);
      return Result;
    }

    printf ("Success\n");
  }

  // Get Pixy2 Version information //
  {
    Result = pixy.getVersion();

    if (Result < 0)
    {
      printf ("pixy.getVersion() returned %d\n", Result);
      return Result;
    }

    pixy.version->print();
  }

  // need to call stop() befroe calling getRawFrame().
  // Note, you can call getRawFrame multiple times after calling stop().
  // That is, you don't need to call stop() each time.
  pixy.m_link.stop();

  for(i = 0; i < no_of_frames; i++){
    k = i;
    //grab raw frame, BGGR Bayer format, 1 byte per pixel
    pixy.m_link.getRawFrame(&bayerFrame);

    for (j = 0; j < no_of_chars; j++){
      fileno[no_of_chars - 1 - j] = (k % 10) + 48;
      k /= 10;
    }

    switch(format){
      case 'b':
        // write frame to PPM file for verification
        writebayer(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, bayerFrame, "out", fileno);
        break;
      case 'r':
        // convert Bayer frame to RGB frame
        demosaic(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, bayerFrame, rgbFrame);
        writergb(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, rgbFrame, "out", fileno);
        break;
      case 'p':
        // convert Bayer frame to RGB frame
        demosaic(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, bayerFrame, rgbFrame);
        writePPM(PIXY2_RAW_FRAME_WIDTH, PIXY2_RAW_FRAME_HEIGHT, rgbFrame, "out", fileno);
        break;

    }
  }
  if (Result==0)
    printf("Write frame to out.ppm\n");

  // Call resume() to resume the current program, otherwise Pixy will be left
  // in "paused" state.  
  pixy.m_link.resume();
}
