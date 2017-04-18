#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NV12ToNV21(const char *srcFile, const char *tarFile, int yuvW, int yuvH) {
  int i,j;
  int iRet;
  // int uSt

  FILE *srcFp = fopen(srcFile, "r+");
  FILE *tarFp = fopen(tarFile, "w+");
  if(srcFp==NULL || tarFp==NULL) {
    printf("Open File Error!\n");
    return -1;
  }

  unsigned char *tmpSrc = (unsigned char *)malloc(yuvW*yuvH*3/2);
  unsigned char *tmpTar = (unsigned char *)malloc(yuvW*yuvH*3/2);
  if(tmpSrc==NULL || tmpSrc==NULL) {
    printf("Malloc Error!\n");
    return -1;
  }

  iRet = fread(tmpSrc, 1, yuvW*yuvH*3/2, srcFp);
  if(iRet < yuvW*yuvH*3/2) {
    printf("Read YUV 420P File Error");
    return -1;
  }

  /*Write Y*/
  memcpy(tmpTar, tmpSrc, yuvW*yuvH);
  /*Write U & V*/
  for(i=0; i<yuvW*yuvH/2; i++) {
    if(i&1) {
      tmpTar[yuvW*yuvH+i] = tmpSrc[yuvW*yuvH+i-1];
    } else {
      tmpTar[yuvW*yuvH+i] = tmpSrc[yuvW*yuvH+i+1];
    }
  }

  fwrite(tmpTar, 1, yuvW*yuvH*3/2, tarFp);
  fclose(srcFp);
  fclose(tarFp);
  free(tmpSrc);
  free(tmpTar);
  return 0;
}

int main(int argc, char *argv[]) {
  int yuvW = atoi(argv[3]);
  int yuvH = atoi(argv[4]);
  NV12ToNV21(argv[1], argv[2], yuvW, yuvH);
  return 0;
}
