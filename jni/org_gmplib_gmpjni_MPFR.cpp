#include <stdio.h>
#include <errno.h>
#include "org_gmplib_gmpjni_MPFR.h"
#include "org_gmplib_gmpjni_MPFR_MPFRException.h"
#include "mpfr.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

void throwMPFRException
  ( JNIEnv * env, jint p0, jstring p1)
{
    jclass exception_class = env->FindClass("org/gmplib/gmpjni/MPFR$MPFRException");
    if (0 != exception_class) {
	jmethodID ctor_id = env->GetMethodID(exception_class, "<init>", "(ILjava/lang/String;)V");
	if (0 != ctor_id) {
	    jobject exception_object = env->NewObject(exception_class, ctor_id, p0, p1);
	    if (0 != exception_object) {
		if (0 != env->Throw( (jthrowable)exception_object)) {
		    env->DeleteLocalRef(exception_object);
		}
	    }
	}
	env->DeleteLocalRef(exception_class);
    }
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_version
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1version
  (JNIEnv *env, jclass cl)
{
    jstring ret = env->NewStringUTF(mpfr_get_version());
    return ret;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_set_str
 * Signature: (JLjava/lang/String;II)V
 */
JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1str
  (JNIEnv *env, jclass cl, jlong x, jstring str, jint base, jint rnd)
{
    jboolean is_copy;
    char const *str_chars = NULL;
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    str_chars = env->GetStringUTFChars(str, &is_copy);
    if (mpfr_set_str(*xptr, str_chars, base, static_cast<mpfr_rnd_t>(rnd)) != 0) {
	throwMPFRException(env, org_gmplib_gmpjni_MPFR_MPFRException_BAD_FORMAT,
				env->NewStringUTF("bad format"));
    }
    env->ReleaseStringUTFChars( str, str_chars );
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_get_str
 * Signature: (Lorg/gmplib/gmpjni/GMP/MutableInteger;JIJI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1str
  (JNIEnv *env, jclass cl, jobject exp, jlong op, jint base, jlong n_digits, jint rnd)
{
    jstring ret = 0;
    char *buffer;
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);
    mpfr_exp_t e;
    jclass mic = env->FindClass("org/gmplib/gmpjni/GMP$MutableInteger");
    jfieldID fid = env->GetFieldID(mic, "value", "I");

    buffer = (char *)malloc( MAX((n_digits + 2), 7)*sizeof(char) );
    if (buffer != NULL) {
        mpfr_get_str(buffer, &e, base, n_digits, *opptr, static_cast<mpfr_rnd_t>(rnd));
        ret = env->NewStringUTF(buffer);
        env->SetIntField(exp, fid, e);
        free(buffer);
    } else {
	throwMPFRException(env, org_gmplib_gmpjni_MPFR_MPFRException_OUT_OF_MEMORY,
				env->NewStringUTF("out of memory"));
    }
    return ret;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_get_d_2exp
 * Signature: (Lorg/gmplib/gmpjni/GMP/MutableInteger;JI)D
 */
JNIEXPORT jdouble JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1d_12exp
  (JNIEnv *env, jclass cl, jobject exp, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);
    signed long int e;
    jdouble r;
    jclass mic = env->FindClass("org/gmplib/gmpjni/GMP$MutableInteger");
    jfieldID fid = env->GetFieldID(mic, "value", "I");

    r = mpfr_get_d_2exp(&e, *opptr, static_cast<mpfr_rnd_t>(rnd));
    env->SetIntField(exp, fid, e);
    return r;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_frexp
 * Signature: (Lorg/gmplib/gmpjni/GMP/MutableInteger;JJI)I
 */
JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1frexp
  (JNIEnv *env, jclass cl, jobject exp, jlong y, jlong x, jint rnd)
{
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_exp_t e;
    jint r;
    jclass mic = env->FindClass("org/gmplib/gmpjni/GMP$MutableInteger");
    jfieldID fid = env->GetFieldID(mic, "value", "I");

    r = mpfr_frexp(&e, *yptr, *xptr, static_cast<mpfr_rnd_t>(rnd));
    env->SetIntField(exp, fid, e);
    return r;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_lgamma
 * Signature: (JLorg/gmplib/gmpjni/GMP/MutableInteger;JI)I
 */
JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1lgamma
  (JNIEnv *env, jclass cl, jlong rop, jobject signp, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);
    int sp;
    jclass mic = env->FindClass("org/gmplib/gmpjni/GMP$MutableInteger");
    jfieldID fid = env->GetFieldID(mic, "value", "I");
    int r;

    r = mpfr_lgamma(*ropptr, &sp, *opptr, static_cast<mpfr_rnd_t>(rnd));
    env->SetIntField(signp, fid, sp);
    return r;
}

/*
 * Class:     org_gmplib_gmpjni_MPFR
 * Method:    native_mpfr_remquo
 * Signature: (JLorg/gmplib/gmpjni/GMP/MutableInteger;JJI)I
 */
JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1remquo
  (JNIEnv *env, jclass cl, jlong r, jobject q, jlong x, jlong y, jint rnd)
{
    mpfr_t *rptr = reinterpret_cast<mpfr_t *>(r);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);
    long quotient;
    jclass mic = env->FindClass("org/gmplib/gmpjni/GMP$MutableInteger");
    jfieldID fid = env->GetFieldID(mic, "value", "I");
    int ret;

    ret = mpfr_remquo(*rptr, &quotient, *xptr, *yptr, static_cast<mpfr_rnd_t>(rnd));
    env->SetIntField(q, fid, quotient);
    return ret;
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emin
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emin();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1emin
  (JNIEnv *env, jclass cl, jint exp)
{

    return mpfr_set_emin(exp);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emin_1min
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emin_min();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emin_1max
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emin_max();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emax
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emax();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1emax
  (JNIEnv *env, jclass cl, jint exp)
{

    return mpfr_set_emax(exp);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emax_1min
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emax_min();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1emax_1max
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_emax_max();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1default_1rounding_1mode
  (JNIEnv *env, jclass cl, jint rnd)
{

    mpfr_set_default_rounding_mode(static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1default_1rounding_1mode
  (JNIEnv *env, jclass cl)
{

    return static_cast<int>(mpfr_get_default_rounding_mode());
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1flags
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_flags();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1underflow
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_underflow();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1overflow
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_overflow();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1divby0
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_divby0();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1nanflag
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_nanflag();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1inexflag
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_inexflag();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1clear_1erangeflag
  (JNIEnv *env, jclass cl)
{

    mpfr_clear_erangeflag();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1underflow
  (JNIEnv *env, jclass cl)
{

    mpfr_set_underflow();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1overflow
  (JNIEnv *env, jclass cl)
{

    mpfr_set_overflow();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1divby0
  (JNIEnv *env, jclass cl)
{

    mpfr_set_divby0();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1nanflag
  (JNIEnv *env, jclass cl)
{

    mpfr_set_nanflag();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1inexflag
  (JNIEnv *env, jclass cl)
{

    mpfr_set_inexflag();
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1erangeflag
  (JNIEnv *env, jclass cl)
{

    mpfr_set_erangeflag();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1underflow_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_underflow_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1overflow_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_overflow_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1divby0_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_divby0_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1nanflag_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_nanflag_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1inexflag_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_inexflag_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1erangeflag_1p
  (JNIEnv *env, jclass cl)
{

    return mpfr_erangeflag_p();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1check_1range
  (JNIEnv *env, jclass cl, jlong x, jint t, jint rnd)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_check_range(*xptr, t, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1prec_1round
  (JNIEnv *env, jclass cl, jlong x, jint prec, jint rnd)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_prec_round(*xptr, prec, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1can_1round
  (JNIEnv *env, jclass cl, jlong b, jint err, jint rnd1, jint rnd2, jint prec)
{
    mpfr_t *bptr = reinterpret_cast<mpfr_t *>(b);

    return mpfr_can_round(*bptr, err, static_cast<mpfr_rnd_t>(rnd1), static_cast<mpfr_rnd_t>(rnd2), prec);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1min_1prec
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_min_prec(*xptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1exp
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_get_exp(*xptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1exp
  (JNIEnv *env, jclass cl, jlong x, jint e)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_set_exp(*xptr, e);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1prec
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_get_prec(*xptr);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1prec
  (JNIEnv *env, jclass cl, jlong x, jint prec)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_set_prec(*xptr, prec);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1prec_1raw
  (JNIEnv *env, jclass cl, jlong x, jint prec)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_set_prec_raw(*xptr, prec);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1default_1prec
  (JNIEnv *env, jclass cl, jint prec)
{

    mpfr_set_default_prec(prec);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1default_1prec
  (JNIEnv *env, jclass cl)
{

    return mpfr_get_default_prec();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1d
  (JNIEnv *env, jclass cl, jlong rop, jdouble op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_d(*ropptr, op, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1flt
  (JNIEnv *env, jclass cl, jlong rop, jfloat op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_flt(*ropptr, op, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpz_t *opptr = reinterpret_cast<mpz_t *>(op);

    return mpfr_set_z(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1z_12exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint e, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpz_t *opptr = reinterpret_cast<mpz_t *>(op);

    return mpfr_set_z_2exp(*ropptr, *opptr, e, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1nan
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_set_nan(*xptr);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1inf
  (JNIEnv *env, jclass cl, jlong x, jint sign)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_set_inf(*xptr, sign);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1zero
  (JNIEnv *env, jclass cl, jlong x, jint sign)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_set_zero(*xptr, sign);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1f
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpf_t *opptr = reinterpret_cast<mpf_t *>(op);

    return mpfr_set_f(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1f
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpf_t *ropptr = reinterpret_cast<mpf_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_f(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1si
  (JNIEnv *env, jclass cl, jlong rop, jint op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_si(*ropptr, op, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_ui(*ropptr, static_cast<unsigned long int>(op), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1si_12exp
  (JNIEnv *env, jclass cl, jlong rop, jint op, jint e, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_si_2exp(*ropptr, op, e, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1ui_12exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint e, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_set_ui_2exp(*ropptr, static_cast<unsigned long int>(op), e, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set_1q
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpq_t *opptr = reinterpret_cast<mpq_t *>(op);

    return mpfr_set_q(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1abs
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_abs(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1set
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_set(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1neg
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_neg(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1signbit
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_signbit(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1setsign
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint s, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_setsign(*ropptr, *opptr, s, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1copysign
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_copysign(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1z_12exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op)
{
    mpz_t *ropptr = reinterpret_cast<mpz_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_z_2exp(*ropptr, *opptr);
}

JNIEXPORT jfloat JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1flt
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_flt(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jdouble JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1d
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_d(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jdouble JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1d1
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_d1(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1si
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_si(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jlong JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1ui
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_ui(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1get_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpz_t *ropptr = reinterpret_cast<mpz_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_get_z(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1urandom
  (JNIEnv *env, jclass cl, jlong rop, jlong state, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    gmp_randstate_t *stateptr = reinterpret_cast<gmp_randstate_t *>(state);

    return mpfr_urandom(*ropptr, *stateptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1grandom
  (JNIEnv *env, jclass cl, jlong rop1, jlong rop2, jlong state, jint rnd)
{
    mpfr_t *rop1ptr = reinterpret_cast<mpfr_t *>(rop1);
    mpfr_t *rop2ptr = reinterpret_cast<mpfr_t *>(rop2);
    gmp_randstate_t *stateptr = reinterpret_cast<gmp_randstate_t *>(state);

    return mpfr_grandom(*rop1ptr, *rop2ptr, *stateptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1urandomb
  (JNIEnv *env, jclass cl, jlong rop, jlong state)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    gmp_randstate_t *stateptr = reinterpret_cast<gmp_randstate_t *>(state);

    return mpfr_urandomb(*ropptr, *stateptr);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1nextabove
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_nextabove(*xptr);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1nextbelow
  (JNIEnv *env, jclass cl, jlong x)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    mpfr_nextbelow(*xptr);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1nexttoward
  (JNIEnv *env, jclass cl, jlong x, jlong y)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);

    mpfr_nexttoward(*xptr, *yptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1pow
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_pow(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1pow_1si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_pow_si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1pow_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_pow_ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ui_1pow_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_ui_pow_ui(*ropptr, static_cast<unsigned long int>(op1), static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ui_1pow
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_ui_pow(*ropptr, static_cast<unsigned long int>(op1), *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1pow_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpz_t *op2ptr = reinterpret_cast<mpz_t *>(op2);

    return mpfr_pow_z(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sqrt
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sqrt(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sqrt_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_sqrt_ui(*ropptr, static_cast<unsigned long int>(op), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rec_1sqrt
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rec_sqrt(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_add(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_sub(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_mul(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_div(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_add_ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_sub_ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ui_1sub
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_ui_sub(*ropptr, static_cast<unsigned long int>(op1), *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ui_1div
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_ui_div(*ropptr, static_cast<unsigned long int>(op1), *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add_1si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_add_si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub_1si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_sub_si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1si_1sub
  (JNIEnv *env, jclass cl, jlong rop, jint op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_si_sub(*ropptr, op1, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_1si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_1si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1si_1div
  (JNIEnv *env, jclass cl, jlong rop, jint op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_si_div(*ropptr, op1, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add_1d
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jdouble op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_add_d(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub_1d
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jdouble op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_sub_d(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1d_1sub
  (JNIEnv *env, jclass cl, jlong rop, jdouble op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_d_sub(*ropptr, op1, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_1d
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jdouble op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_d(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_1d
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jdouble op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_d(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1d_1div
  (JNIEnv *env, jclass cl, jlong rop, jdouble op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_d_div(*ropptr, op1, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sqr
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sqr(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1const_1pi
  (JNIEnv *env, jclass cl, jlong rop, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_const_pi(*ropptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1const_1log2
  (JNIEnv *env, jclass cl, jlong rop, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_const_log2(*ropptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1const_1euler
  (JNIEnv *env, jclass cl, jlong rop, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_const_euler(*ropptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1const_1catalan
  (JNIEnv *env, jclass cl, jlong rop, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_const_catalan(*ropptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1agm
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_agm(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1log
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_log(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1log2
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_log2(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1log10
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_log10(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1log1p
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_log1p(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_exp(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1exp2
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_exp2(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1exp10
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_exp10(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1expm1
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_expm1(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1eint
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_eint(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1li2
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_li2(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_cmp(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1d
  (JNIEnv *env, jclass cl, jlong op1, jdouble op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_cmp_d(*op1ptr, op2);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmpabs
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_cmpabs(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1ui
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_cmp_ui(*op1ptr, static_cast<unsigned long int>(op2));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1si
  (JNIEnv *env, jclass cl, jlong op1, jint op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_cmp_si(*op1ptr, op2);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1ui_12exp
  (JNIEnv *env, jclass cl, jlong op1, jlong op2, jint e)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_cmp_ui_2exp(*op1ptr, static_cast<unsigned long int>(op2), e);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1si_12exp
  (JNIEnv *env, jclass cl, jlong op1, jint op2, jint e)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_cmp_si_2exp(*op1ptr, op2, e);
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1reldiff
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    mpfr_reldiff(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1eq
  (JNIEnv *env, jclass cl, jlong op1, jlong op2, jlong op3)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_eq(*op1ptr, *op2ptr, static_cast<unsigned long int>(op3));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sgn
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sgn(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_12exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_2exp(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_12exp
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_2exp(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_12ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_2ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_12ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_2ui(*ropptr, *op1ptr, static_cast<unsigned long int>(op2), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_12si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_mul_2si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_12si
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jint op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);

    return mpfr_div_2si(*ropptr, *op1ptr, op2, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rint
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rint(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1round
  (JNIEnv *env, jclass cl, jlong rop, jlong op)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_round(*ropptr, *opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1trunc
  (JNIEnv *env, jclass cl, jlong rop, jlong op)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_trunc(*ropptr, *opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ceil
  (JNIEnv *env, jclass cl, jlong rop, jlong op)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_ceil(*ropptr, *opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1floor
  (JNIEnv *env, jclass cl, jlong rop, jlong op)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_floor(*ropptr, *opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rint_1round
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rint_round(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rint_1trunc
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rint_trunc(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rint_1ceil
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rint_ceil(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1rint_1floor
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_rint_floor(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1frac
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_frac(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1modf
  (JNIEnv *env, jclass cl, jlong iop, jlong fop, jlong op, jint rnd)
{
    mpfr_t *iopptr = reinterpret_cast<mpfr_t *>(iop);
    mpfr_t *fopptr = reinterpret_cast<mpfr_t *>(fop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_modf(*iopptr, *fopptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1remainder
  (JNIEnv *env, jclass cl, jlong r, jlong x, jlong y, jint rnd)
{
    mpfr_t *rptr = reinterpret_cast<mpfr_t *>(r);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);

    return mpfr_remainder(*rptr, *xptr, *yptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fmod
  (JNIEnv *env, jclass cl, jlong r, jlong x, jlong y, jint rnd)
{
    mpfr_t *rptr = reinterpret_cast<mpfr_t *>(r);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);

    return mpfr_fmod(*rptr, *xptr, *yptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1ulong_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_ulong_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1slong_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_slong_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1uint_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_uint_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1sint_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_sint_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1ushort_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_ushort_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1sshort_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_sshort_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1uintmax_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_uintmax_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fits_1intmax_1p
  (JNIEnv *env, jclass cl, jlong op, jint rnd)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_fits_intmax_p(*opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1swap
  (JNIEnv *env, jclass cl, jlong x, jlong y)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);

    mpfr_swap(*xptr, *yptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1nan_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_nan_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1inf_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_inf_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1number_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_number_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1integer_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_integer_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1zero_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_zero_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1regular_1p
  (JNIEnv *env, jclass cl, jlong op)
{
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_regular_p(*opptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1greater_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_greater_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1greaterequal_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_greaterequal_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1less_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_less_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1lessequal_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_lessequal_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1lessgreater_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_lessgreater_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1equal_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_equal_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1unordered_1p
  (JNIEnv *env, jclass cl, jlong op1, jlong op2)
{
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_unordered_p(*op1ptr, *op2ptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1atanh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_atanh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1acosh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_acosh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1asinh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_asinh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cosh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_cosh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sinh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sinh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1tanh
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_tanh(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sinh_1cosh
  (JNIEnv *env, jclass cl, jlong sop, jlong cop, jlong op, jint rnd)
{
    mpfr_t *sopptr = reinterpret_cast<mpfr_t *>(sop);
    mpfr_t *copptr = reinterpret_cast<mpfr_t *>(cop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sinh_cosh(*sopptr, *copptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sech
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sech(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1csch
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_csch(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1coth
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_coth(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1acos
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_acos(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1asin
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_asin(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1atan
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_atan(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sin
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sin(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sin_1cos
  (JNIEnv *env, jclass cl, jlong sop, jlong cop, jlong op, jint rnd)
{
    mpfr_t *sopptr = reinterpret_cast<mpfr_t *>(sop);
    mpfr_t *copptr = reinterpret_cast<mpfr_t *>(cop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sin_cos(*sopptr, *copptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cos
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_cos(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1tan
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_tan(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1atan2
  (JNIEnv *env, jclass cl, jlong rop, jlong y, jlong x, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_atan2(*ropptr, *yptr, *xptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sec
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_sec(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1csc
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_csc(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cot
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_cot(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1hypot
  (JNIEnv *env, jclass cl, jlong rop, jlong x, jlong y, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpfr_t *yptr = reinterpret_cast<mpfr_t *>(y);

    return mpfr_hypot(*ropptr, *xptr, *yptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1erf
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_erf(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1erfc
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_erfc(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cbrt
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_cbrt(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1root
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jlong k, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_root(*ropptr, *opptr, static_cast<unsigned long int>(k), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1gamma
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_gamma(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1lngamma
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_lngamma(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1digamma
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_digamma(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1zeta
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_zeta(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1zeta_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_zeta_ui(*ropptr, static_cast<unsigned long int>(op), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fac_1ui
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);

    return mpfr_fac_ui(*ropptr, static_cast<unsigned long int>(op), static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1j0
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_j0(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1j1
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_j1(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1jn
  (JNIEnv *env, jclass cl, jlong rop, jint n, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_jn(*ropptr, n, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1y0
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_y0(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1y1
  (JNIEnv *env, jclass cl, jlong rop, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_y1(*ropptr, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1yn
  (JNIEnv *env, jclass cl, jlong rop, jint n, jlong op, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *opptr = reinterpret_cast<mpfr_t *>(op);

    return mpfr_yn(*ropptr, n, *opptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1ai
  (JNIEnv *env, jclass cl, jlong rop, jlong x, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_ai(*ropptr, *xptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1min
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_min(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1max
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_max(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1dim
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_dim(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpz_t *op2ptr = reinterpret_cast<mpz_t *>(op2);

    return mpfr_mul_z(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpz_t *op2ptr = reinterpret_cast<mpz_t *>(op2);

    return mpfr_div_z(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpz_t *op2ptr = reinterpret_cast<mpz_t *>(op2);

    return mpfr_add_z(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub_1z
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpz_t *op2ptr = reinterpret_cast<mpz_t *>(op2);

    return mpfr_sub_z(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1z_1sub
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpz_t *op1ptr = reinterpret_cast<mpz_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);

    return mpfr_z_sub(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1z
  (JNIEnv *env, jclass cl, jlong x, jlong y)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpz_t *yptr = reinterpret_cast<mpz_t *>(y);

    return mpfr_cmp_z(*xptr, *yptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1mul_1q
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpq_t *op2ptr = reinterpret_cast<mpq_t *>(op2);

    return mpfr_mul_q(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1div_1q
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpq_t *op2ptr = reinterpret_cast<mpq_t *>(op2);

    return mpfr_div_q(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1add_1q
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpq_t *op2ptr = reinterpret_cast<mpq_t *>(op2);

    return mpfr_add_q(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1sub_1q
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpq_t *op2ptr = reinterpret_cast<mpq_t *>(op2);

    return mpfr_sub_q(*ropptr, *op1ptr, *op2ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1q
  (JNIEnv *env, jclass cl, jlong x, jlong y)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpq_t *yptr = reinterpret_cast<mpq_t *>(y);

    return mpfr_cmp_q(*xptr, *yptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1cmp_1f
  (JNIEnv *env, jclass cl, jlong x, jlong y)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);
    mpf_t *yptr = reinterpret_cast<mpf_t *>(y);

    return mpfr_cmp_f(*xptr, *yptr);
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fma
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jlong op3, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);
    mpfr_t *op3ptr = reinterpret_cast<mpfr_t *>(op3);

    return mpfr_fma(*ropptr, *op1ptr, *op2ptr, *op3ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1fms
  (JNIEnv *env, jclass cl, jlong rop, jlong op1, jlong op2, jlong op3, jint rnd)
{
    mpfr_t *ropptr = reinterpret_cast<mpfr_t *>(rop);
    mpfr_t *op1ptr = reinterpret_cast<mpfr_t *>(op1);
    mpfr_t *op2ptr = reinterpret_cast<mpfr_t *>(op2);
    mpfr_t *op3ptr = reinterpret_cast<mpfr_t *>(op3);

    return mpfr_fms(*ropptr, *op1ptr, *op2ptr, *op3ptr, static_cast<mpfr_rnd_t>(rnd));
}

JNIEXPORT void JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1free_1cache
  (JNIEnv *env, jclass cl)
{

    mpfr_free_cache();
}

JNIEXPORT jint JNICALL Java_org_gmplib_gmpjni_MPFR_native_1mpfr_1subnormalize
  (JNIEnv *env, jclass cl, jlong x, jint t, jint rnd)
{
    mpfr_t *xptr = reinterpret_cast<mpfr_t *>(x);

    return mpfr_subnormalize(*xptr, t, static_cast<mpfr_rnd_t>(rnd));
}
