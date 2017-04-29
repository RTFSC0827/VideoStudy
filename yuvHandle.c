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
#define SAVE_RET

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
            k++;
          }

          #ifdef SAVE_RET
          FILE *outPutFp = fopen("Final.yuv", "w+");
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
  //分配一段内存，用于存储裁剪后的Y分量
  unsigned char *tmpY = (unsigned char *)malloc(cutW*cutH);
  //分配一段内存，用于存储裁剪后的UV分量
	unsigned char *tmpUV = (unsigned char *)malloc(cutW*cutH/2);
  switch (yuvType) {
    case NV21OR21:
         for(i=startH; i<cutH+startH; i++) {
           // 逐行拷贝Y分量，共拷贝cutW*cutH
           memcpy(tmpY+j*cutW, srcYuv+startW+i*srcW, cutW);
           j++;
         }
         for(i=startH/2; i<(cutH+startH)/2; i++) {
           //逐行拷贝UV分量，共拷贝cutW*cutH/2
           memcpy(tmpUV+k*cutW, srcYuv+startW+srcW*srcH+i*srcW, cutW);
           k++;
         }
         //将拷贝好的Y，UV分量拷贝到目标内存中
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

int fCutWaterMark(unsigned char *waterMarkSrc, unsigned char *srcYuv, int waterMarkW, int waterMarkH) {
	int i;
	unsigned char tmpData[waterMarkW*waterMarkH*3/2];
	unsigned char tmpWaterMark[waterMarkW*waterMarkH*3/2];
	memcpy(tmpData, srcYuv, waterMarkW*waterMarkH*3/2);
	memcpy(tmpWaterMark, waterMarkSrc, waterMarkW*waterMarkH*3/2);

	for(i=0; i<waterMarkW*waterMarkH*3/2; i++) {
		if(tmpWaterMark[i]!=0x10 && tmpWaterMark[i]!=0x80 && tmpWaterMark[i]!=0xeb) {
			tmpData[i] = tmpWaterMark[i];
			// printf("0x%X\n", tmpData[i]);
		}
	}

	memcpy(waterMarkSrc, tmpData, waterMarkW*waterMarkH*3/2);
	#if 0
	FILE *tarFp = fopen("afterCutWaterMark.yuv", "w+");
	fwrite(waterMarkSrc, 1, waterMarkW*waterMarkH*3/2, tarFp);
	fclose(tarFp);
	#endif
	return 0;
}

//Final Main Function
#if 1
int main(int argc, char *argv[]) {
  FILE *inputFp = fopen(argv[1], "r+");
  FILE *waterMarkFp = fopen(argv[4], "r+");

  int srcW = atoi(argv[2]);
  int srcH = atoi(argv[3]);
  int waterMarkW = atoi(argv[5]);
  int waterMarkH = atoi(argv[6]);

  unsigned char *yuvSrcData = (unsigned char *)malloc(srcW*srcH*3/2);
  unsigned char *waterMarkData = (unsigned char *)malloc(waterMarkW*waterMarkH*3/2);
  unsigned char *afterCutData = (unsigned char *)malloc(waterMarkW*waterMarkH*3/2);

  fread(yuvSrcData, 1, srcW*srcH*3/2, inputFp);
  fread(waterMarkData, 1, waterMarkW*waterMarkH*3/2, waterMarkFp);

  cutYuv(NV21OR21, afterCutData, yuvSrcData, 50, 50, waterMarkW, waterMarkH, srcW, srcH);
  fCutWaterMark( waterMarkData, afterCutData, waterMarkW, waterMarkH);

  #if 1
  FILE *tarFp = fopen("afterCutWaterMarktmp.yuv", "w+");
  fwrite(waterMarkData, 1, waterMarkW*waterMarkH*3/2, tarFp);
  // fwrite(afterCutData, 1, waterMarkW*waterMarkH*3/2, tarFp);
  fclose(tarFp);
  #endif
  yuvAddWaterMark(NV21OR21, 50, 50, waterMarkData, waterMarkW, waterMarkH, yuvSrcData, srcW, srcH);

  return 0;
}
#endif

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
