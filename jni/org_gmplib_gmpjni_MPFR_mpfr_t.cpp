#include "org_gmplib_gmpjni_MPFR_mpfr_t.h"
#include "mpfr.h"
#include "malloc.h"

/*
 * Class:     org_gmplib_gmpjni_MPFR_mpfr_t
 * Method:    init
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_gmplib_gmpjni_MPFR_00024mpfr_1t_init
  (JNIEnv *, jobject)
{
    mpfr_t *ptr = 0;
    ptr = (mpfr_t *)malloc(sizeof(mpfr_t));
    if (ptr != NULL) {
	mpfr_init(*ptr);
    }
    return (jlong)ptr;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR_mpfr_t
 * Method:    init2
 * Signature: (I)J
 */
JNIEXPORT jlong JNICALL Java_org_gmplib_gmpjni_MPFR_00024mpfr_1t_init2
  (JNIEnv *, jobject, jint precision)
{
    mpfr_t *ptr = 0;
    ptr = (mpfr_t *)malloc(sizeof(mpfr_t));
    if (ptr != NULL) {
	mpfr_init2(*ptr, static_cast<mpfr_prec_t>(precision));
    }
    return (jlong)ptr;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR_mpfr_t
 * Method:    fini
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_00024mpfr_1t_fini
  (JNIEnv *env, jobject obj)
{
    jclass cls = env->FindClass( "org/gmplib/gmpjni/MPFR$mpfr_t" );
    jfieldID field_id = env->GetFieldID( cls, "handle", "J" );
    jlong handle = env->GetLongField( obj, field_id );
    mpfr_t *ptr = (mpfr_t *)handle;
    if (ptr != NULL) {
	mpfr_clear(*ptr);
	free(ptr);
	env->SetLongField(obj, field_id, (jlong)0);
    }
}
