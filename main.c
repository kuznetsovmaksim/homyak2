#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <strings.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
//uiijijijijijik
///////////////////////
void dfs(int i,int j,int iw,int ih,unsigned char* FinIm, int* DfsIm,int col_num)
{
    DfsIm[iw*i+j]=col_num;
    if(i>0 && i<ih && j>2 && j<iw)
    {
        if((abs(FinIm[iw*i+j]-FinIm[iw*i+j-2])<=63)&&(DfsIm[iw*i+j-2]==0))
            dfs(i,j-2,iw,ih,FinIm,DfsIm,col_num);
    }
    if(i>2 && i<ih+2 && j>-1 && j<iw-1)
    {
        if((abs(FinIm[iw*i+j]-FinIm[iw*(i-2)+j+1])<=63)&&(DfsIm[iw*(i-2)+j+1]==0))
            dfs(i-2,j+1,iw,ih,FinIm,DfsIm,col_num);
    }
    if(i>-2 && i<ih-2 && j>-1 && j<iw-1)// i+2 j+1
    {
        if((abs(FinIm[iw*i+j]-FinIm[iw*(i+2)+j+1])<=63)&&(DfsIm[iw*(i+2)+j+1]==0))
            dfs(i+2,j+1,iw,ih,FinIm,DfsIm,col_num);
    }
}
int main()
{
    int iw,ih,n,col_num=0,check=0,c;
    unsigned char *idata = stbi_load("hampster.png", &iw, &ih, &n, 0);
    if (idata == NULL)
	{
        printf("ERROR: can't read file\n");
        return -1;
    }
    unsigned char* FinIm = (unsigned char*)malloc(ih*iw*sizeof(unsigned char));
    int i,j,k=0;
    for (i=0; i<ih*iw*n; i=i+n)//одноканальный массив
	{
        FinIm[k]=0.34375*idata[i]+0.5*idata[i+1]+0.15625*idata[i+2];
        k++;
    }
    for (i=2;i<=ih-2;i++)//контрастность
	{
        for (j=2;j<=iw-2;j++)
		{
            if(FinIm[iw*i+j]>155)FinIm[iw*i+j]=255;
            if(FinIm[iw*i+j]<92)FinIm[iw*i+j]=0;
        }
    }
    int* DfsIm=(int*)malloc((iw*ih)*sizeof(int));//массив после dfs
    for(i=0;i<iw*ih;i++)
		DfsIm[i]=0;
    for (i=1;i<ih;i++)
    {
        for (j=1;j<iw;j++)
        {
            if(DfsIm[iw*i+j]==0)
            {
                col_num++;
                dfs(i,j,iw,ih,FinIm,DfsIm,col_num);
            }
        }
    }
    /*for(i=1;i<ih-1;i++)
    {
        for(j=1;j<iw-1;j++)
        {
            DfsIm[i*iw+j]+=(int)DfsIm[iw*(i-1)+j-1]*(0.0924)+(int)DfsIm[iw*(i-1)+j]*(0.1192)+(int)DfsIm[iw*(i-1)+j+1]*(0.0924)+(int)DfsIm[iw*i+j-1]*(0.1192)\
            +(int)DfsIm[iw*i+j]*(0.1538)+(int)DfsIm[iw*i+j+1]*(0.1192)+(int)DfsIm[iw*(i+1)+j-1]*(0.0924)+(int)DfsIm[iw*(i+1)+j]*(0.1192)+(int)DfsIm[iw*(i+1)+j+1]*(0.0924);
        }
    }*/
    unsigned char* Findata=(unsigned char*)malloc(iw*ih*n*sizeof(unsigned char));//финальный 4-канальный
    k=0;
    for (i=0; i<ih*iw*n; i=i+n)
    {
        c=DfsIm[k]%20+DfsIm[k]%35;
        Findata[i]=4*c-27;
        Findata[i+1]=3*c+11;
        Findata[i+2]=3*c+11;
        Findata[i+3]=255;
        k++;
    }

    // записываем картинку
    stbi_write_png("DFSHampster.png", iw, ih, n, Findata, 0);
    stbi_image_free(idata);
    stbi_image_free(FinIm);
    stbi_image_free(Findata);
    return 0;
}
