/************************************************************
FileName: yuvHandle.c
Author: GofranChang
Version : 1.0.0
Date: 2017.4.28
History:
Gofran 17/04/28 1.0 build this moudle
***********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <malloc.h>

#define NV21OR21 1
#define YUV420SP 2

/**************************************************************************
 *Function    : yuvAddWaterMark
 *Description : Add a watermark in the specified place
 *Input       : yuvType                 YUV Data Type
 *              startX,startY           Watermark starting position
 *              waterMarkData           Watermark yuv data
 *              waterMarkW,waterMarkH   Watermark size
 *              yuvData                 Yuv sourceData(Storage data after add watermark)
 *              yuvW,yuvH               Yuv source size
 *Return      : NULL
 *Others      ：NULL
 **************************************************************************/
void yuvAddWaterMark(int yuvType, int startX, int startY, unsigned char *waterMarkData,
                int waterMarkW, int waterMarkH, unsigned char *yuvData, int yuvW, int yuvH) {
  int i=0;
  int j=0;
  int k=0;
  switch(yuvType) {
    case NV21OR21:
         for(i=startY; i<waterMarkH+startY; i++) {
           memcpy(yuvData+startX+i*yuvW, waterMarkData+j*waterMarkW, waterMarkW);
           j++;
         }
         for(i=startY/2; i<(waterMarkH+startY)/2; i++) {
           memcpy(yuvData+startX+yuvW*yuvH+i*yuvW, waterMarkData+waterMarkW*waterMarkH+k*waterMarkW, waterMarkW);
         }

         #ifdef SAVE_RET
         FILE *outPutFp = fopen(outPutFile, "w+");
         fwrite(yuvData, 1, yuvW*yuvH*3/2, outPutFp);
         fclose(outPutFp);
         #endif

         break;
    case YUV420SP:
     //Not FInished
            break;
    default:
    //Not FInished
            break;
  }
}

/**************************************************************************
 *Function    : cutYuv
 *Description : Crop YUV image for a specific size
 *Input       : yuvType           YUV Data Type
 *              tarYuv            Target memory(Storage data after cut)
 *              srcYuv            YUV source data
 *              startW,startH     Starting cutting position
 *              cutW,cutH         Cut size
 *              srcW,srcH         Source data size
 *Return      : NULL
 *Others      ：NULL
 **************************************************************************/
void cutYuv(int yuvType, unsigned char *tarYuv, unsigned char *srcYuv, int startW,
            int startH, int cutW, int cutH, int srcW, int srcH) {
  int i;
  int j = 0;
  int k = 0;
  unsigned char *tmpY = (unsigned char *)malloc(cutW*cutH);
	unsigned char *tmpUV = (unsigned char *)malloc(cutW*cutH/2);
  switch (yuvType) {
    case NV21OR21:
         for(i=startH; i<cutH+startH; i++) {
           memcpy(tmpY+j*cutW, srcYuv+startW+i*srcW, cutW);
           j++;
         }
         for(i=startH/2; i<(cutH+startH)/2; i++) {
           memcpy(tmpUV+k*cutW, srcYuv+startW+srcW*srcH+i*srcW, cutW);
           k++;
         }
         memcpy(tarYuv, tmpY, cutW*cutH);
         memcpy(tarYuv+cutW*cutH, tmpUV, cutW*cutH/2);
         free(tmpY);
         free(tmpUV);
         break;
    case YUV420SP:
    //Not FInished
         break;
   }
}

#if 0
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

  yuvAddWaterMark(NV21OR21, startX, startY, waterMarkData, waterMarkW, waterMarkH, yuvData, yuvDataW, yuvDataH, argv[9]);

  free(waterMarkData);
  free(yuvData);
  fclose(waterMarkFp);
  fclose(inPutFp);
  return 0;
}
#endif

int main(int argc, char *argv[]) {
  FILE *inPutFp = fopen(argv[1], "r+");
  if(inPutFp==NULL) {
    printf("Open Input File Error!\n");
    return -1;
  }

  int yuvDataW = atoi(argv[2]);
  int yuvDataH = atoi(argv[3]);
  int startX = atoi(argv[4]);
  int startY = atoi(argv[5]);
  int cutX = atoi(argv[6]);
  int cutY = atoi(argv[7]);

  unsigned char *yuvData = (unsigned char *)malloc(yuvDataW*yuvDataH*3/2);
  if( yuvData==NULL) {
    printf("Malloc memory Failed!\n");
    return -1;
  }

  unsigned char *tarYuv = (unsigned char *)malloc(cutX*cutY*3/2);
  fread(yuvData, 1, yuvDataW*yuvDataH*3/2, inPutFp);
  cutYuv( NV21OR21, tarYuv, yuvData, startX, startY, cutX, cutY, yuvDataW, yuvDataH);

  FILE *outPutFp = fopen("afterCut.yuv", "w+");
  if(!outPutFp) {
    printf("Open Out Put File Error!\n");
    return -1;
  }
  fwrite(tarYuv, 1, cutX*cutY*3/2, outPutFp);
  fclose(inPutFp);
  fclose(outPutFp);
  free(yuvData);
  free(tarYuv);
  return 0;
}

/*********************************************************************
 *                             _ooOoo_
 *                            o8888888o
 *                            88" . "88
 *                            (| -_- |)
 *                            O\  =  /O
 *                         ____/`---'\____
 *                       .'  \\|     |//  `.
 *                      /  \\|||  :  |||//  \
 *                     /  _||||| -:- |||||-  \
 *                     |   | \\\  -  /// |   |
 *                     | \_|  ''\---/''  |   |
 *                     \  .-\__  `-`  ___/-. /
 *                   ___`. .'  /--.--\  `. . __
 *                ."" '<  `.___\_<|>_/___.'  >'"".
 *               | | :  `- \`.;`\ _ /`;.`/ - ` : | |
 *               \  \ `-.   \_ __\ /__ _/   .-` /  /
 *          ======`-.____`-.___\_____/___.-`____.-'======
 *                             `=---='
 *          ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 *                     佛祖保佑        永无BUG
 *            佛曰:
 *                   写字楼里写字间，写字间里程序员；
 *                   程序人员写程序，又拿程序换酒钱。
 *                   酒醒只在网上坐，酒醉还来网下眠；
 *                   酒醉酒醒日复日，网上网下年复年。
 *                   但愿老死电脑间，不愿鞠躬老板前；
 *                   奔驰宝马贵者趣，公交自行程序员。
 *                   别人笑我忒疯癫，我笑自己命太贱；
 *                   不见满街漂亮妹，哪个归得程序员？
 **********************************************************************/
