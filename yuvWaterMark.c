#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <malloc.h>

#define NV21 1
#define NV12 2
#define YUV420SP 3

int yuvAddWaterMark(int yuvType, int startX, int startY, unsigned char *waterMarkData,
                int waterMarkW, int waterMarkH, unsigned char *yuvData, int yuvW, int yuvH, const char *outPutFile) {
  int i=0;
  int j=0;
  int k=0;
  switch(yuvType) {
    case NV21:
            for(i=startY; i<waterMarkH+startY; i++) {
              memcpy(yuvData+startX+i*yuvW, waterMarkData+j*waterMarkW, waterMarkW);
              j++;
            }
            for(i=startY/2; i<(waterMarkH+startY)/2; i++) {
              memcpy(yuvData+startX+yuvW*yuvH+i*yuvW, waterMarkData+waterMarkW*waterMarkH+k*waterMarkW, waterMarkW);
            }
            FILE *outPutFp = fopen(outPutFile, "w+");
            fwrite(yuvData, 1, yuvW*yuvH*3/2, outPutFp);
            fclose(outPutFp);
            break;
    case NV12:
      //Not FInished
            break;
    case YUV420SP:
     //Not FInished
            break;
    default:
    //Not FInished
            break;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  FILE *inPutFp = fopen(argv[4], "r+");
  FILE *waterMarkFp = fopen(argv[1], "r+");
  if(inPutFp==NULL || waterMarkFp==NULL) {
    printf("Open Input File Error!\n");
    return -1;
  }

  int waterMarkW = atoi(argv[2]);
  int waterMarkH = atoi(argv[3]);
  int yuvDataW = atoi(argv[5]);
  int yuvDataH = atoi(argv[6]);
  int startX = atoi(argv[7]);
  int startY = atoi(argv[8]);

  unsigned char *waterMarkData = (unsigned char *)malloc(waterMarkW*waterMarkH*3/2);
  unsigned char *yuvData = (unsigned char *)malloc(yuvDataW*yuvDataH*3/2);
  if(waterMarkData==NULL || yuvData==NULL) {
    printf("Malloc memory Failed!\n");
    return -1;
  }

  fread(waterMarkData, 1, waterMarkW*waterMarkH*3/2, waterMarkFp);
  fread(yuvData, 1, yuvDataW*yuvDataH*3/2, inPutFp);

  yuvAddWaterMark(NV21, startX, startY, waterMarkData, waterMarkW, waterMarkH, yuvData, yuvDataW, yuvDataH, argv[9]);

  free(waterMarkData);
  free(yuvData);
  fclose(waterMarkFp);
  fclose(inPutFp);
  return 0;
}
