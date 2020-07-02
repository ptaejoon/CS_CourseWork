#include <jni.h>
#include "android/log.h"
#include <stdio.h>
#include<unistd.h>
#include<asm/ioctl.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<stdlib.h>
#define DEVICE_MAJOR_NUM 242
#define IOCTL_GAME_FINISH _IO(DEVICE_MAJOR_NUM,0)
#define IOCTL_START_GAME _IO(DEVICE_MAJOR_NUM,1)
#define IOCTL_GET_SCORE _IO(DEVICE_MAJOR_NUM,2)
//IOCTL Seperator
#define LOG_TAG "MyTag"
#define LOGV(...)   __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

 jint JNICALL Java_org_example_ndk_NDKExam_driveOpen(JNIEnv *env, jobject obj)
 {
	 int dev = open("/dev/timer",O_RDWR);
	 return (jint) dev;
 }
//driveOpen function opens device driver for program.
jint JNICALL Java_org_example_ndk_NDKExam_getScore
  (JNIEnv *env, jobject obj,jint dev)
{
	int cdev = (int)dev;
	ioctl(cdev, IOCTL_GET_SCORE);
}
//if a pair is same, score on FND should be increased. IOCTL_GET_SCORE does it

/*
 * Class:     org_example_ndk_NDKExam
 * Method:    startGame
 * Signature: ()V
 */
jint JNICALL Java_org_example_ndk_NDKExam_startGame
  (JNIEnv *env, jobject obj,jint dev){
	int cdev = (int)dev;
	ioctl(cdev,IOCTL_START_GAME);
}
//startGame calls IOCTL_START_GAME that initialize variables in driver for game.
/*
 * Class:     org_example_ndk_NDKExam
 * Method:    finishGame
 * Signature: ()V
 */
jint JNICALL Java_org_example_ndk_NDKExam_finishGame
  (JNIEnv *env, jobject obj,jint dev){
	int cdev = (int)dev;
	if(ioctl(cdev,IOCTL_GAME_FINISH) == 1)
	{
		return (jint)1;
	}else return (jint)0;

}
//When the game is finished, check whether it is best score and delete timer.
/*
 * Class:     org_example_ndk_NDKExam
 * Method:    checkSame
 * Signature: (II)I
 */
