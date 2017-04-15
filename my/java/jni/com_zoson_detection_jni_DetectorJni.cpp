#include "com_zoson_detection_jni_DetectorJni.h"
#include <domain/context.hpp>
using namespace zoson;
#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_zoson_detection_jni_DetectorJni_init
  (JNIEnv *, jobject){}


JNIEXPORT void JNICALL Java_com_zoson_detection_jni_DetectorJni_deInit
  (JNIEnv *, jobject){}


JNIEXPORT void JNICALL Java_com_zoson_detection_jni_DetectorJni_run
  (JNIEnv *, jobject){}


JNIEXPORT jstring JNICALL Java_com_zoson_detection_jni_DetectorJni_test
  (JNIEnv * env, jobject)
  {
  	Context * c = new Context();
  	string test_str = c->test() + " hello Detector jni !!";
  	return env->NewStringUTF(test_str.c_str());
  }


#ifdef __cplusplus
}
#endif