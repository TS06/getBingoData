//インクルードファイル指定
#include <opencv2/opencv.hpp>
//静的リンクライブラリの指定
//#include <opencv2/opencv_lib.hpp>
//名前空間の指定
#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<string>
#include<fstream>
#include<iostream>

#define HEAD_CUT 1.7

//#include<opencv2/core/core.hpp>

int toBinary(char *file_name);
void resize(char *img_name,int size);
void cut(char *img_name);
void toStringFile();
void mergeData(char *output_name);
std::string Replace(std::string str,std::string from_str,std::string to_str);

int main(int argc,char *argv[]){
  if(argc!=3){
    printf("Please input start number and end number !\n");
    exit(1);
  }
  int start = atoi(argv[1]);
  int end = atoi(argv[2]);

  printf("%d %d\n",start,end);
  for(int i=start;i<=end;i++){
    char file_name[256];

    sprintf(file_name,"BINGO_%d.jpg",i);

    toBinary(file_name);
    resize("Binary.jpg",6);
    cut("Binary.jpg");
    toStringFile();
    mergeData("merge_data");
  }
}

int toBinary(char *file_name)
{
  IplImage *src_img = 0, *dst_img;
  IplImage *src_img_gray = 0;
  IplImage *tmp_img1, *tmp_img2, *tmp_img3;

  // (1)画像を読み込む
  //if (argc >= 2)
    src_img = cvLoadImage (file_name, CV_LOAD_IMAGE_COLOR);
  if (src_img == 0)
    return -1;

  tmp_img1 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img2 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  tmp_img3 = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  src_img_gray = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_8U, 1);
  cvCvtColor (src_img, src_img_gray, CV_BGR2GRAY);
  dst_img = cvCloneImage (src_img);

  // (2)ガウシアンフィルタで平滑化を行う
  cvSmooth (src_img_gray, src_img_gray, CV_GAUSSIAN, 5);

  // (3)二値化:cvThreshold
  cvThreshold (src_img_gray, tmp_img1, 60, 255, CV_THRESH_BINARY);

  // (4)二値化:cvAdaptiveThreshold
  //cvAdaptiveThreshold (src_img_gray, tmp_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 10);

  // (5)二つの二値化画像の論理積
  //cvAnd (tmp_img1, tmp_img2, tmp_img3);
  //cvCvtColor (tmp_img3, dst_img, CV_GRAY2BGR);

  // (6)元画像と二値画像の論理積
  //cvSmooth (src_img, src_img, CV_GAUSSIAN, 11);
  //cvAnd (dst_img, src_img, dst_img);
  cvSaveImage("Binary.jpg",tmp_img1);  
	  // (7)画像を表示する
  //cvNamedWindow ("Threshold", CV_WINDOW_AUTOSIZE);
  //cvShowImage ("Threshold", tmp_img1);
  //cvNamedWindow ("AdaptiveThreshold", CV_WINDOW_AUTOSIZE);
  //cvShowImage ("AdaptiveThreshold", tmp_img2);
  //cvNamedWindow ("Image", CV_WINDOW_AUTOSIZE);
  //cvShowImage ("Image", dst_img);
  //cvWaitKey (0);

  //cvDestroyWindow ("Threshold");
  //cvDestroyWindow ("AdaptiveThreshold");
  //cvDestroyWindow ("Image");



  //終了処理
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);
  cvReleaseImage (&src_img_gray);
  cvReleaseImage (&tmp_img1);
  cvReleaseImage (&tmp_img2);
  cvReleaseImage (&tmp_img3);

  return 0;
}

//#include<opencv2/core/core.hpp>

//画像サイズの変更
void resize(char *img_name,int size){
  IplImage *src_img,*resize_img;

  src_img = cvLoadImage (img_name, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  if(src_img==0){
    printf("Not found  %s !\n",img_name);
    exit(0);
  }
  
  resize_img =  cvCreateImage (cvSize (src_img->width * size, src_img->height * size), src_img->depth, src_img->nChannels);

  cvResize(src_img,resize_img,CV_INTER_NN);

  cvSaveImage(img_name,resize_img);

}

//画像カット
void cut(char *img_name){
  IplImage* src_img = 0;
  IplImage* cut_img=0;
  int w,h,h_nohead;

  src_img = cvLoadImage(img_name);
  if(!src_img){
    printf("Not reading %s !\n",img_name);
    exit(0);
  }

  w=src_img->width;
  h=src_img->height;
  h_nohead = h - h/8*HEAD_CUT; //頭の部分を抜いた画像の高さ

  char save_name[10];

  //頭の部分だけを抜き出し
  cut_img = cvCloneImage(src_img);
  cvSetImageROI(cut_img,cvRect(cut_img->width/2-cut_img->width/16,0,w/HEAD_CUT,h/20*HEAD_CUT));
  sprintf(save_name,"cut%d.jpg",1);
  cvSaveImage(save_name,cut_img);

  //ビンゴの本体部分の抜き出し
  for(int i=0;i<5;i++){
    cut_img = cvCloneImage(src_img);
    cvSetImageROI(cut_img,cvRect(0,h/8*HEAD_CUT+h_nohead/5*i,w,h_nohead/5));
    sprintf(save_name,"cut%d.jpg",i+2);
    cvSaveImage(save_name,cut_img);
  }

}

//切り取った画像をテキストデータへ
void toStringFile(){
  char command[255];
  for(int i=0;i<7;i++){
    sprintf(command,"tesseract cut%d.jpg data%d -psm 7 tesseract.conf",i,i);
    system(command);
  }
}

//テキストデータを統一
void mergeData(char *output_name){
  std::ifstream ifs;
  std::ofstream ofs(output_name,std::ios::app);
  std::string temp;
  std::string out_str,ID;
  char file_name[128];

  ifs.open("data1.txt");
  if(!ifs){
    printf("File Open Error !\n");
  }
  
  //カードID
  ifs >> out_str;
  ID=out_str;
  out_str+="\t";
  ifs.close();

  //ビンゴのデータ本体
  for(int i=0;i<5;i++){
    sprintf(file_name,"data%d.txt",i+2);
    ifs.open(file_name);

    getline(ifs,temp);
    temp += " ";
    out_str += temp;

    ifs.close();
  }

  std::string str=Replace(out_str," ",",");

  //str+=","+ID;

  //  ofs << out_str<<"\n";
  ofs << str<<"\n";
  ofs.close();
  
}

//文字列置き換え
std::string Replace(std::string str,std::string from_str,std::string to_str){
  std::string::size_type Pos(str.find(from_str));

  while(Pos != std::string::npos){
    str.replace(Pos,from_str.length(),to_str);
    Pos = str.find(from_str, Pos + to_str.length());
  }

  if(str.length()-1==str.find_last_of(to_str)){
    str=str.erase(str.find_last_of(to_str));
  }

  return str;
}
