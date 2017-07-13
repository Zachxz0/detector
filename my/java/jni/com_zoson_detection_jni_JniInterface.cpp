#include "com_zoson_detection_jni_JniInterface.h"
#include <app/dcontext.hpp>
#include <detector/detector.hpp>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
using namespace std;
using namespace zoson;
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    newContext
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_zoson_detection_jni_JniInterface_newContext
  (JNIEnv *env, jclass, jstring spath)
  {
    cout<<"init context"<<endl;
    const char* path = env->GetStringUTFChars(spath,0);
    string str_path = path;
    cout<<str_path<<endl;
    cout<<"before init context"<<endl;
    DContext *context = new DContext(str_path);
    //context->getDetector(0)->initForTest();
    cout<<"after init context"<<endl;
    env->ReleaseStringUTFChars(spath,path);
    return (long)context;
  }

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    newDetector
 * Signature: (JLjava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_zoson_detection_jni_JniInterface_newDetector
  (JNIEnv *, jclass, jlong ptr, jstring path)
  {
//todo
    return -1;
  }

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    getDetector
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_com_zoson_detection_jni_JniInterface_getDetector
  (JNIEnv *, jclass, jlong ptr, jint index)
  {
    DContext* context = (DContext*)ptr;
    if(context==NULL)return -1;
    Detector *detector = context->getDetector(index).get();
    cout<<"get detector"<<endl;
    return (long)detector;
  }

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    initJni
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_zoson_detection_jni_JniInterface_initJni
  (JNIEnv *, jclass, jlong ptr)
  {
    Detector *detector = (Detector*)ptr;
    if(detector==NULL)return ;
    detector->initForTest();
  }

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    deInitJni
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_zoson_detection_jni_JniInterface_deInitJni
  (JNIEnv *, jclass, jlong ptr)
  {
    Detector *detector = (Detector*)ptr;
    if(detector==NULL)return ;
    detector->deInit();
  }

/*
 * Class:     com_zoson_detection_jni_JniInterface
 * Method:    doDetectJni
 * Signature: (J[B)[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_zoson_detection_jni_JniInterface_doDetectJni
  (JNIEnv *env, jclass, jlong ptr, jbyteArray bytes,jint w,jint h,jint c)
  {
    Detector *detector = (Detector*)ptr;
    if(detector==NULL)return NULL;
    jbyte* bBuffer = env->GetByteArrayElements(bytes,0);
    unsigned char* buf=(unsigned char*)bBuffer;
    cv::Mat input(h,w,CV_8UC3);
    memcpy(input.data,buf,w*h*c);
    shared_ptr<DetectOutput> out = detector->doDetect(input);
    string output;
    out->SerializeToString(&output);
    int size = output.size();
    //Result res = out.results();
    //cout<<size<<endl;
    int num_det = out->results_size();
    for(int i=0;i<num_det;++i){
      //if(result[i*7+2]<0.5f)continue;
      Result res = out->results(i); 
    }
    jbyteArray jarray = env->NewByteArray(size);
    env->SetByteArrayRegion(jarray, 0, size, (jbyte*)const_cast<char*>(output.c_str()));  
    env->ReleaseByteArrayElements(bytes,bBuffer,0);
    return jarray;
  }


// JNIEXPORT jstring JNICALL Java_com_zoson_detection_jni_DetectorJni_test
//   (JNIEnv * env, jobject)
//   {
//   	Context * c = new Context();
//   	string test_str = c->test() + " hello Detector jni !!";
//   	return env->NewStringUTF(test_str.c_str());
//   }


#ifdef __cplusplus
}
#endif