//�C���N���[�h�t�@�C���w��
#include <opencv2/opencv.hpp>
//�ÓI�����N���C�u�����̎w��
//#include <opencv2/opencv_lib.hpp>
//���O��Ԃ̎w��
#include<stdio.h>
#include<cv.h>
#include<highgui.h>
#include<string>
#include<fstream>
#include<iostream>

//#include<opencv2/core/core.hpp>

IplImage *toBinary(char *file_name);
void cut(IplImage *src_img);
void toStringFile();
void mergeData(char *output_name);
std::string Replace(std::string str,std::string from_str,std::string to_str);

int main(){
  IplImage *tmp;

  tmp = toBinary("t02200269_0800097710223657231.jpg");
  cut(tmp);
  toStringFile();
  mergeData("merge_data");
}

int toBinary(char *file_name)
{
  IplImage *src_img = 0, *dst_img;
  IplImage *src_img_gray = 0;
  IplImage *tmp_img1, *tmp_img2, *tmp_img3;

  // (1)�摜��ǂݍ���
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

  // (2)�K�E�V�A���t�B���^�ŕ��������s��
  cvSmooth (src_img_gray, src_img_gray, CV_GAUSSIAN, 5);

  // (3)��l��:cvThreshold
  cvThreshold (src_img_gray, tmp_img1, 50, 255, CV_THRESH_BINARY);

  // (4)��l��:cvAdaptiveThreshold
  //cvAdaptiveThreshold (src_img_gray, tmp_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 11, 10);

  // (5)��̓�l���摜�̘_����
  //cvAnd (tmp_img1, tmp_img2, tmp_img3);
  //cvCvtColor (tmp_img3, dst_img, CV_GRAY2BGR);

  // (6)���摜�Ɠ�l�摜�̘_����
  //cvSmooth (src_img, src_img, CV_GAUSSIAN, 11);
  //cvAnd (dst_img, src_img, dst_img);
  //cvSaveImage("Binary.jpg",tmp_img1);  
	  // (7)�摜��\������
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



  //�I������
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);
  cvReleaseImage (&src_img_gray);
  cvReleaseImage (&tmp_img2);
  cvReleaseImage (&tmp_img3);

  return tmp_img1;
}

//#include<opencv2/core/core.hpp>

//�摜�J�b�g
void cut(IplImage *src_img){
  IplImage* cut_img=0;
  int w,h,h_nohead;

  w=src_img->width;
  h=src_img->height;
  h_nohead = h - h*1.5; //���̕����𔲂����摜�̍���

  char save_name[10];

  //���̕��������𔲂��o��
  cut_img = cvCloneImage(src_img);
  cvSetImageROI(cut_img,cvRect(0,0,w,h/8*1.5));
  sprintf(save_name,"cut%d.jpg",1);
  cvSaveImage(save_name,cut_img);

  //�r���S�̖{�̕����̔����o��
  for(int i=0;i<6;i++){
    cut_img = cvCloneImage(src_img);
    cvSetImageROI(cut_img,cvRect(0,h/6*i,w,h/6));
    sprintf(save_name,"cut%d.jpg",i+1);
    cvSaveImage(save_name,cut_img);
  }

  cvReleaseImage(&src_img);
  cvReleaseImage(&cut_img);
}

//�؂������摜���e�L�X�g�f�[�^��
void toStringFile(){
  char command[255];
  for(int i=0;i<7;i++){
    sprintf(command,"tesseract cut%d.jpg data%d -psm 7 tesseract.conf",i,i);
    system(command);
  }
}

//�e�L�X�g�f�[�^�𓝈�
void mergeData(char *output_name){
  std::ifstream ifs;
  std::ofstream ofs(output_name,std::ios::app);
  std::string temp;
  std::string out_str;
  char file_name[128];

  ifs.open("data1.txt");
  if(!ifs){
    printf("File Open Error !\n");
  }
  
  //�J�[�hID
  ifs >> out_str;
  out_str+="\t";
  ifs.close();

  //�r���S�̃f�[�^�{��
  for(int i=0;i<5;i++){
    sprintf(file_name,"data%d.txt",i+2);
    ifs.open(file_name);

    getline(ifs,temp);
    temp += " ";
    out_str += temp;

    ifs.close();
  }

  ofs << out_str<<"\n";
  ofs.close();
  
}

//������u�������i�܂������j
std::string Replace(std::string str,std::string from_str,std::string to_str){
  std::string::size_type Pos(str.find(from_str));

  while(Pos != std::string::npos){
    str.replace(Pos,from_str.length(),to_str);
    Pos = str.find(from_str, Pos + to_str.length());
  }

  return str;
}
