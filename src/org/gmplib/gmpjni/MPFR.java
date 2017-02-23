package org.gmplib.gmpjni;

import android.util.Log;

public class MPFR
{

    public static class MPFRException extends Exception
    {
	private int code;

	public static final int INVALID_HANDLE = 1;
	public static final int PARAMETER_OUT_OF_RANGE = 2;
	public static final int OUT_OF_MEMORY = 3;
	public static final int BAD_FORMAT = 4;
	public static final int IO_ERROR = 5;
	public static final int OPERATION_NOT_SUPPORTED = 6;
	public static final int INTERNAL_ERROR = 7;
	public static final int UNDERFLOW = 8;
	public static final int OVERFLOW = 9;
	public static final int DIVIDE_BY_ZERO = 10;
	public static final int NAN = 11;
	public static final int INEXACT = 12;
	public static final int RANGE_ERROR = 13;

	public MPFRException(int code, String msg)
	{
	    super(msg);
	    this.code = code;
	}

	public int getCode()
	{
	    return this.code;
	}
    }

    public static class mpfr_t
    {
	public mpfr_t() throws MPFRException
	{
	    handle = init();
	    if (handle == 0) {
		throw new MPFRException(MPFRException.INVALID_HANDLE, "Invalid handle");
	    }
	}

	public mpfr_t(int precision) throws MPFRException
	{
	    handle = init2(precision);
	    if (handle == 0) {
		throw new MPFRException(MPFRException.INVALID_HANDLE, "Invalid handle");
	    }
	}

	public void finalize()
	{
	    fini();
	    handle = 0;
	}

	protected long handle;

	private native long init();
	private native long init2(int precision);

	private native void fini();
    }

    public enum mpfr_rnd_t
    {
	MPFR_RNDNA,
	MPFR_RNDN,
	MPFR_RNDZ,
	MPFR_RNDU,
	MPFR_RNDD,
	MPFR_RNDA,
    }

    public static void init()
    {
	Log.d( "mpfrjni", "loading native libs" );
	System.loadLibrary("gmp");
	System.loadLibrary("mpfr");
	System.loadLibrary("mpfrjni");
    }

    private static void checkUnsignedIntRange(long y)
	throws MPFRException
    {
	if (y < 0 || y > GMP.MAX_UINT) {
	    throw new MPFRException(MPFRException.PARAMETER_OUT_OF_RANGE, "Parameter out of range");
	}
    }

    private static int mpfr_rnd_t_to_int(mpfr_rnd_t rnd)
    {
	int v = rnd.ordinal();
	return (v - 1);
    }

    private static mpfr_rnd_t[] mpfr_rnd_t_values = mpfr_rnd_t.values();

    private static mpfr_rnd_t int_to_mpfr_rnd_t(int val)
    {
	return mpfr_rnd_t_values[val + 1];
    }

    public static void checkMPFRException()
	throws MPFRException
    {
	if (mpfr_underflow_p() != 0) {
	    throw new MPFRException(MPFRException.UNDERFLOW, "Underflow");
	} else if (mpfr_overflow_p() != 0) {
	    throw new MPFRException(MPFRException.OVERFLOW, "Overflow");
	} else if (mpfr_divby0_p() != 0) {
	    throw new MPFRException(MPFRException.DIVIDE_BY_ZERO, "Divide by 0");
	} else if (mpfr_nanflag_p() != 0) {
	    throw new MPFRException(MPFRException.NAN, "NaN");
	} else if (mpfr_inexflag_p() != 0) {
	    throw new MPFRException(MPFRException.INEXACT, "Inexact");
	} else if (mpfr_erangeflag_p() != 0) {
	    throw new MPFRException(MPFRException.RANGE_ERROR, "Range error");
	}
    }

    private static native String native_mpfr_version();
    public static String getVersion()
    {
	return native_mpfr_version();
    }

    /**********************************************************
     * String functions
     **********************************************************/
    private static native void native_mpfr_set_str(long x, String str, int base, int rnd);
    public static void mpz_set_str(mpfr_t x, String str, int base, mpfr_rnd_t rnd)
	throws MPFRException
    {
	native_mpfr_set_str(x.handle, str, base, mpfr_rnd_t_to_int(rnd));
    }

    private static native String native_mpfr_get_str(GMP.MutableInteger exp, long x, int base, long n_digits, int rnd);
    public static String mpfr_get_str(GMP.MutableInteger exp, mpfr_t x, int base, long n_digits, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(n_digits);
	return native_mpfr_get_str(exp, x.handle, base, n_digits, mpfr_rnd_t_to_int(rnd));
    }

    /**********************************************************
     * MutableInteger functions
     **********************************************************/
    private static native double native_mpfr_get_d_2exp(/*signed long int **/ GMP.MutableInteger exp, long op, int rnd);
    public static double mpfr_get_d_2exp(/*signed long int **/ GMP.MutableInteger exp, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_d_2exp(exp, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_frexp(/*mpfr_exp_t **/ GMP.MutableInteger exp, long y, long x, int rnd);
    public static int mpfr_frexp(/*mpfr_exp_t **/ GMP.MutableInteger exp, mpfr_t y, mpfr_t x, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_frexp(exp, y.handle, x.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_lgamma(long rop, GMP.MutableInteger signp, long op, int rnd);
    public static int mpfr_lgamma(mpfr_t rop, GMP.MutableInteger signp, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_lgamma(rop.handle, signp, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_remquo(long r, GMP.MutableInteger q, long x, long y, int rnd);
    public static int mpfr_remquo(mpfr_t r, GMP.MutableInteger q, mpfr_t x, mpfr_t y, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_remquo(r.handle, q, x.handle, y.handle, mpfr_rnd_t_to_int(rnd));
    }

    /**********************************************************
     * Auto-generated functions
     **********************************************************/

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emin();
    public static /*mpfr_exp_t*/ int mpfr_get_emin()
	throws MPFRException
    {
	return native_mpfr_get_emin();
    }

    private static native int native_mpfr_set_emin(/*mpfr_exp_t*/ int exp);
    public static int mpfr_set_emin(/*mpfr_exp_t*/ int exp)
	throws MPFRException
    {
	return native_mpfr_set_emin(exp);
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emin_min();
    public static /*mpfr_exp_t*/ int mpfr_get_emin_min()
	throws MPFRException
    {
	return native_mpfr_get_emin_min();
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emin_max();
    public static /*mpfr_exp_t*/ int mpfr_get_emin_max()
	throws MPFRException
    {
	return native_mpfr_get_emin_max();
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emax();
    public static /*mpfr_exp_t*/ int mpfr_get_emax()
	throws MPFRException
    {
	return native_mpfr_get_emax();
    }

    private static native int native_mpfr_set_emax(/*mpfr_exp_t*/ int exp);
    public static int mpfr_set_emax(/*mpfr_exp_t*/ int exp)
	throws MPFRException
    {
	return native_mpfr_set_emax(exp);
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emax_min();
    public static /*mpfr_exp_t*/ int mpfr_get_emax_min()
	throws MPFRException
    {
	return native_mpfr_get_emax_min();
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_emax_max();
    public static /*mpfr_exp_t*/ int mpfr_get_emax_max()
	throws MPFRException
    {
	return native_mpfr_get_emax_max();
    }

    private static native void native_mpfr_set_default_rounding_mode(int rnd);
    public static void mpfr_set_default_rounding_mode(mpfr_rnd_t rnd)
	throws MPFRException
    {
	native_mpfr_set_default_rounding_mode(mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_get_default_rounding_mode();
    public static mpfr_rnd_t mpfr_get_default_rounding_mode()
	throws MPFRException
    {
	return int_to_mpfr_rnd_t(native_mpfr_get_default_rounding_mode());
    }

    private static native void native_mpfr_clear_flags();
    public static void mpfr_clear_flags()
	throws MPFRException
    {
	native_mpfr_clear_flags();
    }

    private static native void native_mpfr_clear_underflow();
    public static void mpfr_clear_underflow()
	throws MPFRException
    {
	native_mpfr_clear_underflow();
    }

    private static native void native_mpfr_clear_overflow();
    public static void mpfr_clear_overflow()
	throws MPFRException
    {
	native_mpfr_clear_overflow();
    }

    private static native void native_mpfr_clear_divby0();
    public static void mpfr_clear_divby0()
	throws MPFRException
    {
	native_mpfr_clear_divby0();
    }

    private static native void native_mpfr_clear_nanflag();
    public static void mpfr_clear_nanflag()
	throws MPFRException
    {
	native_mpfr_clear_nanflag();
    }

    private static native void native_mpfr_clear_inexflag();
    public static void mpfr_clear_inexflag()
	throws MPFRException
    {
	native_mpfr_clear_inexflag();
    }

    private static native void native_mpfr_clear_erangeflag();
    public static void mpfr_clear_erangeflag()
	throws MPFRException
    {
	native_mpfr_clear_erangeflag();
    }

    private static native void native_mpfr_set_underflow();
    public static void mpfr_set_underflow()
	throws MPFRException
    {
	native_mpfr_set_underflow();
    }

    private static native void native_mpfr_set_overflow();
    public static void mpfr_set_overflow()
	throws MPFRException
    {
	native_mpfr_set_overflow();
    }

    private static native void native_mpfr_set_divby0();
    public static void mpfr_set_divby0()
	throws MPFRException
    {
	native_mpfr_set_divby0();
    }

    private static native void native_mpfr_set_nanflag();
    public static void mpfr_set_nanflag()
	throws MPFRException
    {
	native_mpfr_set_nanflag();
    }

    private static native void native_mpfr_set_inexflag();
    public static void mpfr_set_inexflag()
	throws MPFRException
    {
	native_mpfr_set_inexflag();
    }

    private static native void native_mpfr_set_erangeflag();
    public static void mpfr_set_erangeflag()
	throws MPFRException
    {
	native_mpfr_set_erangeflag();
    }

    private static native int native_mpfr_underflow_p();
    public static int mpfr_underflow_p()
	throws MPFRException
    {
	return native_mpfr_underflow_p();
    }

    private static native int native_mpfr_overflow_p();
    public static int mpfr_overflow_p()
	throws MPFRException
    {
	return native_mpfr_overflow_p();
    }

    private static native int native_mpfr_divby0_p();
    public static int mpfr_divby0_p()
	throws MPFRException
    {
	return native_mpfr_divby0_p();
    }

    private static native int native_mpfr_nanflag_p();
    public static int mpfr_nanflag_p()
	throws MPFRException
    {
	return native_mpfr_nanflag_p();
    }

    private static native int native_mpfr_inexflag_p();
    public static int mpfr_inexflag_p()
	throws MPFRException
    {
	return native_mpfr_inexflag_p();
    }

    private static native int native_mpfr_erangeflag_p();
    public static int mpfr_erangeflag_p()
	throws MPFRException
    {
	return native_mpfr_erangeflag_p();
    }

    private static native int native_mpfr_check_range(long x, int t, int rnd);
    public static int mpfr_check_range(mpfr_t x, int t, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_check_range(x.handle, t, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_prec_round(long x, /*mpfr_prec_t*/ int prec, int rnd);
    public static int mpfr_prec_round(mpfr_t x, /*mpfr_prec_t*/ int prec, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_prec_round(x.handle, prec, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_can_round(long b, /*mpfr_exp_t*/ int err, int rnd1, int rnd2, /*mpfr_prec_t*/ int prec);
    public static int mpfr_can_round(mpfr_t b, /*mpfr_exp_t*/ int err, mpfr_rnd_t rnd1, mpfr_rnd_t rnd2, /*mpfr_prec_t*/ int prec)
	throws MPFRException
    {
	return native_mpfr_can_round(b.handle, err, mpfr_rnd_t_to_int(rnd1), mpfr_rnd_t_to_int(rnd2), prec);
    }

    private static native /*mpfr_prec_t*/ int native_mpfr_min_prec(long x);
    public static /*mpfr_prec_t*/ int mpfr_min_prec(mpfr_t x)
	throws MPFRException
    {
	return native_mpfr_min_prec(x.handle);
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_exp(long x);
    public static /*mpfr_exp_t*/ int mpfr_get_exp(mpfr_t x)
	throws MPFRException
    {
	return native_mpfr_get_exp(x.handle);
    }

    private static native int native_mpfr_set_exp(long x, /*mpfr_exp_t*/ int e);
    public static int mpfr_set_exp(mpfr_t x, /*mpfr_exp_t*/ int e)
	throws MPFRException
    {
	return native_mpfr_set_exp(x.handle, e);
    }

    private static native /*mpfr_prec_t*/ int native_mpfr_get_prec(long x);
    public static /*mpfr_prec_t*/ int mpfr_get_prec(mpfr_t x)
	throws MPFRException
    {
	return native_mpfr_get_prec(x.handle);
    }

    private static native void native_mpfr_set_prec(long x, /*mpfr_prec_t*/ int prec);
    public static void mpfr_set_prec(mpfr_t x, /*mpfr_prec_t*/ int prec)
	throws MPFRException
    {
	native_mpfr_set_prec(x.handle, prec);
    }

    private static native void native_mpfr_set_prec_raw(long x, /*mpfr_prec_t*/ int prec);
    public static void mpfr_set_prec_raw(mpfr_t x, /*mpfr_prec_t*/ int prec)
	throws MPFRException
    {
	native_mpfr_set_prec_raw(x.handle, prec);
    }

    private static native void native_mpfr_set_default_prec(/*mpfr_prec_t*/ int prec);
    public static void mpfr_set_default_prec(/*mpfr_prec_t*/ int prec)
	throws MPFRException
    {
	native_mpfr_set_default_prec(prec);
    }

    private static native /*mpfr_prec_t*/ int native_mpfr_get_default_prec();
    public static /*mpfr_prec_t*/ int mpfr_get_default_prec()
	throws MPFRException
    {
	return native_mpfr_get_default_prec();
    }

    private static native int native_mpfr_set_d(long rop, double op, int rnd);
    public static int mpfr_set_d(mpfr_t rop, double op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_d(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_flt(long rop, float op, int rnd);
    public static int mpfr_set_flt(mpfr_t rop, float op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_flt(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_z(long rop, long op, int rnd);
    public static int mpfr_set_z(mpfr_t rop, GMP.mpz_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_z(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_z_2exp(long rop, long op, /*mpfr_exp_t*/ int e, int rnd);
    public static int mpfr_set_z_2exp(mpfr_t rop, GMP.mpz_t op, /*mpfr_exp_t*/ int e, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_z_2exp(rop.handle, op.handle, e, mpfr_rnd_t_to_int(rnd));
    }

    private static native void native_mpfr_set_nan(long x);
    public static void mpfr_set_nan(mpfr_t x)
	throws MPFRException
    {
	native_mpfr_set_nan(x.handle);
    }

    private static native void native_mpfr_set_inf(long x, int sign);
    public static void mpfr_set_inf(mpfr_t x, int sign)
	throws MPFRException
    {
	native_mpfr_set_inf(x.handle, sign);
    }

    private static native void native_mpfr_set_zero(long x, int sign);
    public static void mpfr_set_zero(mpfr_t x, int sign)
	throws MPFRException
    {
	native_mpfr_set_zero(x.handle, sign);
    }

    private static native int native_mpfr_set_f(long rop, long op, int rnd);
    public static int mpfr_set_f(mpfr_t rop, GMP.mpf_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_f(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_get_f(long rop, long op, int rnd);
    public static int mpfr_get_f(GMP.mpf_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_f(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_si(long rop, /*long*/ int op, int rnd);
    public static int mpfr_set_si(mpfr_t rop, /*long*/ int op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_si(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_ui(long rop, /*unsigned long*/ long op, int rnd);
    public static int mpfr_set_ui(mpfr_t rop, /*unsigned long*/ long op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op);
	return native_mpfr_set_ui(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_si_2exp(long rop, /*long*/ int op, /*mpfr_exp_t*/ int e, int rnd);
    public static int mpfr_set_si_2exp(mpfr_t rop, /*long*/ int op, /*mpfr_exp_t*/ int e, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_si_2exp(rop.handle, op, e, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_ui_2exp(long rop, /*unsigned long*/ long op, /*mpfr_exp_t*/ int e, int rnd);
    public static int mpfr_set_ui_2exp(mpfr_t rop, /*unsigned long*/ long op, /*mpfr_exp_t*/ int e, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op);
	return native_mpfr_set_ui_2exp(rop.handle, op, e, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set_q(long rop, long op, int rnd);
    public static int mpfr_set_q(mpfr_t rop, GMP.mpq_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set_q(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_abs(long rop, long op, int rnd);
    public static int mpfr_abs(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_abs(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_set(long rop, long op, int rnd);
    public static int mpfr_set(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_set(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_neg(long rop, long op, int rnd);
    public static int mpfr_neg(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_neg(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_signbit(long op);
    public static int mpfr_signbit(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_signbit(op.handle);
    }

    private static native int native_mpfr_setsign(long rop, long op, int s, int rnd);
    public static int mpfr_setsign(mpfr_t rop, mpfr_t op, int s, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_setsign(rop.handle, op.handle, s, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_copysign(long rop, long op1, long op2, int rnd);
    public static int mpfr_copysign(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_copysign(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native /*mpfr_exp_t*/ int native_mpfr_get_z_2exp(long rop, long op);
    public static /*mpfr_exp_t*/ int mpfr_get_z_2exp(GMP.mpz_t rop, mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_get_z_2exp(rop.handle, op.handle);
    }

    private static native float native_mpfr_get_flt(long op, int rnd);
    public static float mpfr_get_flt(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_flt(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native double native_mpfr_get_d(long op, int rnd);
    public static double mpfr_get_d(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_d(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native double native_mpfr_get_d1(long op);
    public static double mpfr_get_d1(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_get_d1(op.handle);
    }

    private static native /*long*/ int native_mpfr_get_si(long op, int rnd);
    public static /*long*/ int mpfr_get_si(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_si(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native /*unsigned long*/ long native_mpfr_get_ui(long op, int rnd);
    public static /*unsigned long*/ long mpfr_get_ui(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_ui(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_get_z(long rop, long op, int rnd);
    public static int mpfr_get_z(GMP.mpz_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_get_z(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_urandom(long rop, long state, int rnd);
    public static int mpfr_urandom(mpfr_t rop, GMP.randstate_t state, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_urandom(rop.handle, state.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_grandom(long rop1, long rop2, long state, int rnd);
    public static int mpfr_grandom(mpfr_t rop1, mpfr_t rop2, GMP.randstate_t state, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_grandom(rop1.handle, rop2.handle, state.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_urandomb(long rop, long state);
    public static int mpfr_urandomb(mpfr_t rop, GMP.randstate_t state)
	throws MPFRException
    {
	return native_mpfr_urandomb(rop.handle, state.handle);
    }

    private static native void native_mpfr_nextabove(long x);
    public static void mpfr_nextabove(mpfr_t x)
	throws MPFRException
    {
	native_mpfr_nextabove(x.handle);
    }

    private static native void native_mpfr_nextbelow(long x);
    public static void mpfr_nextbelow(mpfr_t x)
	throws MPFRException
    {
	native_mpfr_nextbelow(x.handle);
    }

    private static native void native_mpfr_nexttoward(long x, long y);
    public static void mpfr_nexttoward(mpfr_t x, mpfr_t y)
	throws MPFRException
    {
	native_mpfr_nexttoward(x.handle, y.handle);
    }

    private static native int native_mpfr_pow(long rop, long op1, long op2, int rnd);
    public static int mpfr_pow(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_pow(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_pow_si(long rop, long op1, /*long int*/ int op2, int rnd);
    public static int mpfr_pow_si(mpfr_t rop, mpfr_t op1, /*long int*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_pow_si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_pow_ui(long rop, long op1, /*unsigned long int*/ long op2, int rnd);
    public static int mpfr_pow_ui(mpfr_t rop, mpfr_t op1, /*unsigned long int*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_pow_ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_ui_pow_ui(long rop, /*unsigned long int*/ long op1, /*unsigned long int*/ long op2, int rnd);
    public static int mpfr_ui_pow_ui(mpfr_t rop, /*unsigned long int*/ long op1, /*unsigned long int*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op1);
	checkUnsignedIntRange(op2);
	return native_mpfr_ui_pow_ui(rop.handle, op1, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_ui_pow(long rop, /*unsigned long int*/ long op1, long op2, int rnd);
    public static int mpfr_ui_pow(mpfr_t rop, /*unsigned long int*/ long op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op1);
	return native_mpfr_ui_pow(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_pow_z(long rop, long op1, long op2, int rnd);
    public static int mpfr_pow_z(mpfr_t rop, mpfr_t op1, GMP.mpz_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_pow_z(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sqrt(long rop, long op, int rnd);
    public static int mpfr_sqrt(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sqrt(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sqrt_ui(long rop, /*unsigned long*/ long op, int rnd);
    public static int mpfr_sqrt_ui(mpfr_t rop, /*unsigned long*/ long op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op);
	return native_mpfr_sqrt_ui(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_rec_sqrt(long rop, long op, int rnd);
    public static int mpfr_rec_sqrt(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rec_sqrt(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add(long rop, long op1, long op2, int rnd);
    public static int mpfr_add(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_add(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub(long rop, long op1, long op2, int rnd);
    public static int mpfr_sub(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sub(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul(long rop, long op1, long op2, int rnd);
    public static int mpfr_mul(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div(long rop, long op1, long op2, int rnd);
    public static int mpfr_div(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add_ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_add_ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_add_ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub_ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_sub_ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_sub_ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_ui_sub(long rop, /*unsigned long*/ long op1, long op2, int rnd);
    public static int mpfr_ui_sub(mpfr_t rop, /*unsigned long*/ long op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op1);
	return native_mpfr_ui_sub(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_mul_ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_mul_ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_div_ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_div_ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_ui_div(long rop, /*unsigned long*/ long op1, long op2, int rnd);
    public static int mpfr_ui_div(mpfr_t rop, /*unsigned long*/ long op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op1);
	return native_mpfr_ui_div(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add_si(long rop, long op1, /*long int*/ int op2, int rnd);
    public static int mpfr_add_si(mpfr_t rop, mpfr_t op1, /*long int*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_add_si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub_si(long rop, long op1, /*long int*/ int op2, int rnd);
    public static int mpfr_sub_si(mpfr_t rop, mpfr_t op1, /*long int*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sub_si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_si_sub(long rop, /*long int*/ int op1, long op2, int rnd);
    public static int mpfr_si_sub(mpfr_t rop, /*long int*/ int op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_si_sub(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_si(long rop, long op1, /*long int*/ int op2, int rnd);
    public static int mpfr_mul_si(mpfr_t rop, mpfr_t op1, /*long int*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul_si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_si(long rop, long op1, /*long int*/ int op2, int rnd);
    public static int mpfr_div_si(mpfr_t rop, mpfr_t op1, /*long int*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div_si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_si_div(long rop, /*long int*/ int op1, long op2, int rnd);
    public static int mpfr_si_div(mpfr_t rop, /*long int*/ int op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_si_div(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add_d(long rop, long op1, double op2, int rnd);
    public static int mpfr_add_d(mpfr_t rop, mpfr_t op1, double op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_add_d(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub_d(long rop, long op1, double op2, int rnd);
    public static int mpfr_sub_d(mpfr_t rop, mpfr_t op1, double op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sub_d(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_d_sub(long rop, double op1, long op2, int rnd);
    public static int mpfr_d_sub(mpfr_t rop, double op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_d_sub(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_d(long rop, long op1, double op2, int rnd);
    public static int mpfr_mul_d(mpfr_t rop, mpfr_t op1, double op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul_d(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_d(long rop, long op1, double op2, int rnd);
    public static int mpfr_div_d(mpfr_t rop, mpfr_t op1, double op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div_d(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_d_div(long rop, double op1, long op2, int rnd);
    public static int mpfr_d_div(mpfr_t rop, double op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_d_div(rop.handle, op1, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sqr(long rop, long op, int rnd);
    public static int mpfr_sqr(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sqr(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_const_pi(long rop, int rnd);
    public static int mpfr_const_pi(mpfr_t rop, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_const_pi(rop.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_const_log2(long rop, int rnd);
    public static int mpfr_const_log2(mpfr_t rop, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_const_log2(rop.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_const_euler(long rop, int rnd);
    public static int mpfr_const_euler(mpfr_t rop, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_const_euler(rop.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_const_catalan(long rop, int rnd);
    public static int mpfr_const_catalan(mpfr_t rop, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_const_catalan(rop.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_agm(long rop, long op1, long op2, int rnd);
    public static int mpfr_agm(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_agm(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_log(long rop, long op, int rnd);
    public static int mpfr_log(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_log(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_log2(long rop, long op, int rnd);
    public static int mpfr_log2(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_log2(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_log10(long rop, long op, int rnd);
    public static int mpfr_log10(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_log10(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_log1p(long rop, long op, int rnd);
    public static int mpfr_log1p(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_log1p(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_exp(long rop, long op, int rnd);
    public static int mpfr_exp(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_exp(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_exp2(long rop, long op, int rnd);
    public static int mpfr_exp2(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_exp2(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_exp10(long rop, long op, int rnd);
    public static int mpfr_exp10(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_exp10(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_expm1(long rop, long op, int rnd);
    public static int mpfr_expm1(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_expm1(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_eint(long rop, long op, int rnd);
    public static int mpfr_eint(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_eint(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_li2(long rop, long op, int rnd);
    public static int mpfr_li2(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_li2(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cmp(long op1, long op2);
    public static int mpfr_cmp(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_cmp(op1.handle, op2.handle);
    }

    private static native int native_mpfr_cmp_d(long op1, double op2);
    public static int mpfr_cmp_d(mpfr_t op1, double op2)
	throws MPFRException
    {
	return native_mpfr_cmp_d(op1.handle, op2);
    }

    private static native int native_mpfr_cmpabs(long op1, long op2);
    public static int mpfr_cmpabs(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_cmpabs(op1.handle, op2.handle);
    }

    private static native int native_mpfr_cmp_ui(long op1, /*unsigned long*/ long op2);
    public static int mpfr_cmp_ui(mpfr_t op1, /*unsigned long*/ long op2)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_cmp_ui(op1.handle, op2);
    }

    private static native int native_mpfr_cmp_si(long op1, /*long*/ int op2);
    public static int mpfr_cmp_si(mpfr_t op1, /*long*/ int op2)
	throws MPFRException
    {
	return native_mpfr_cmp_si(op1.handle, op2);
    }

    private static native int native_mpfr_cmp_ui_2exp(long op1, /*unsigned long*/ long op2, /*mpfr_exp_t*/ int e);
    public static int mpfr_cmp_ui_2exp(mpfr_t op1, /*unsigned long*/ long op2, /*mpfr_exp_t*/ int e)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_cmp_ui_2exp(op1.handle, op2, e);
    }

    private static native int native_mpfr_cmp_si_2exp(long op1, /*long*/ int op2, /*mpfr_exp_t*/ int e);
    public static int mpfr_cmp_si_2exp(mpfr_t op1, /*long*/ int op2, /*mpfr_exp_t*/ int e)
	throws MPFRException
    {
	return native_mpfr_cmp_si_2exp(op1.handle, op2, e);
    }

    private static native void native_mpfr_reldiff(long rop, long op1, long op2, int rnd);
    public static void mpfr_reldiff(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	native_mpfr_reldiff(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_eq(long op1, long op2, /*unsigned long*/ long op3);
    public static int mpfr_eq(mpfr_t op1, mpfr_t op2, /*unsigned long*/ long op3)
	throws MPFRException
    {
	checkUnsignedIntRange(op3);
	return native_mpfr_eq(op1.handle, op2.handle, op3);
    }

    private static native int native_mpfr_sgn(long op);
    public static int mpfr_sgn(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_sgn(op.handle);
    }

    private static native int native_mpfr_mul_2exp(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_mul_2exp(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_mul_2exp(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_2exp(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_div_2exp(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_div_2exp(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_2ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_mul_2ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_mul_2ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_2ui(long rop, long op1, /*unsigned long*/ long op2, int rnd);
    public static int mpfr_div_2ui(mpfr_t rop, mpfr_t op1, /*unsigned long*/ long op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op2);
	return native_mpfr_div_2ui(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_2si(long rop, long op1, /*long*/ int op2, int rnd);
    public static int mpfr_mul_2si(mpfr_t rop, mpfr_t op1, /*long*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul_2si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_2si(long rop, long op1, /*long*/ int op2, int rnd);
    public static int mpfr_div_2si(mpfr_t rop, mpfr_t op1, /*long*/ int op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div_2si(rop.handle, op1.handle, op2, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_rint(long rop, long op, int rnd);
    public static int mpfr_rint(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rint(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_round(long rop, long op);
    public static int mpfr_round(mpfr_t rop, mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_round(rop.handle, op.handle);
    }

    private static native int native_mpfr_trunc(long rop, long op);
    public static int mpfr_trunc(mpfr_t rop, mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_trunc(rop.handle, op.handle);
    }

    private static native int native_mpfr_ceil(long rop, long op);
    public static int mpfr_ceil(mpfr_t rop, mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_ceil(rop.handle, op.handle);
    }

    private static native int native_mpfr_floor(long rop, long op);
    public static int mpfr_floor(mpfr_t rop, mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_floor(rop.handle, op.handle);
    }

    private static native int native_mpfr_rint_round(long rop, long op, int rnd);
    public static int mpfr_rint_round(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rint_round(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_rint_trunc(long rop, long op, int rnd);
    public static int mpfr_rint_trunc(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rint_trunc(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_rint_ceil(long rop, long op, int rnd);
    public static int mpfr_rint_ceil(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rint_ceil(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_rint_floor(long rop, long op, int rnd);
    public static int mpfr_rint_floor(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_rint_floor(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_frac(long rop, long op, int rnd);
    public static int mpfr_frac(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_frac(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_modf(long iop, long fop, long op, int rnd);
    public static int mpfr_modf(mpfr_t iop, mpfr_t fop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_modf(iop.handle, fop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_remainder(long r, long x, long y, int rnd);
    public static int mpfr_remainder(mpfr_t r, mpfr_t x, mpfr_t y, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_remainder(r.handle, x.handle, y.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fmod(long r, long x, long y, int rnd);
    public static int mpfr_fmod(mpfr_t r, mpfr_t x, mpfr_t y, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fmod(r.handle, x.handle, y.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_ulong_p(long op, int rnd);
    public static int mpfr_fits_ulong_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_ulong_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_slong_p(long op, int rnd);
    public static int mpfr_fits_slong_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_slong_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_uint_p(long op, int rnd);
    public static int mpfr_fits_uint_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_uint_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_sint_p(long op, int rnd);
    public static int mpfr_fits_sint_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_sint_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_ushort_p(long op, int rnd);
    public static int mpfr_fits_ushort_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_ushort_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_sshort_p(long op, int rnd);
    public static int mpfr_fits_sshort_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_sshort_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_uintmax_p(long op, int rnd);
    public static int mpfr_fits_uintmax_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_uintmax_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fits_intmax_p(long op, int rnd);
    public static int mpfr_fits_intmax_p(mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fits_intmax_p(op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native void native_mpfr_swap(long x, long y);
    public static void mpfr_swap(mpfr_t x, mpfr_t y)
	throws MPFRException
    {
	native_mpfr_swap(x.handle, y.handle);
    }

    private static native int native_mpfr_nan_p(long op);
    public static int mpfr_nan_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_nan_p(op.handle);
    }

    private static native int native_mpfr_inf_p(long op);
    public static int mpfr_inf_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_inf_p(op.handle);
    }

    private static native int native_mpfr_number_p(long op);
    public static int mpfr_number_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_number_p(op.handle);
    }

    private static native int native_mpfr_integer_p(long op);
    public static int mpfr_integer_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_integer_p(op.handle);
    }

    private static native int native_mpfr_zero_p(long op);
    public static int mpfr_zero_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_zero_p(op.handle);
    }

    private static native int native_mpfr_regular_p(long op);
    public static int mpfr_regular_p(mpfr_t op)
	throws MPFRException
    {
	return native_mpfr_regular_p(op.handle);
    }

    private static native int native_mpfr_greater_p(long op1, long op2);
    public static int mpfr_greater_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_greater_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_greaterequal_p(long op1, long op2);
    public static int mpfr_greaterequal_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_greaterequal_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_less_p(long op1, long op2);
    public static int mpfr_less_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_less_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_lessequal_p(long op1, long op2);
    public static int mpfr_lessequal_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_lessequal_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_lessgreater_p(long op1, long op2);
    public static int mpfr_lessgreater_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_lessgreater_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_equal_p(long op1, long op2);
    public static int mpfr_equal_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_equal_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_unordered_p(long op1, long op2);
    public static int mpfr_unordered_p(mpfr_t op1, mpfr_t op2)
	throws MPFRException
    {
	return native_mpfr_unordered_p(op1.handle, op2.handle);
    }

    private static native int native_mpfr_atanh(long rop, long op, int rnd);
    public static int mpfr_atanh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_atanh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_acosh(long rop, long op, int rnd);
    public static int mpfr_acosh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_acosh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_asinh(long rop, long op, int rnd);
    public static int mpfr_asinh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_asinh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cosh(long rop, long op, int rnd);
    public static int mpfr_cosh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_cosh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sinh(long rop, long op, int rnd);
    public static int mpfr_sinh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sinh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_tanh(long rop, long op, int rnd);
    public static int mpfr_tanh(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_tanh(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sinh_cosh(long sop, long cop, long op, int rnd);
    public static int mpfr_sinh_cosh(mpfr_t sop, mpfr_t cop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sinh_cosh(sop.handle, cop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sech(long rop, long op, int rnd);
    public static int mpfr_sech(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sech(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_csch(long rop, long op, int rnd);
    public static int mpfr_csch(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_csch(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_coth(long rop, long op, int rnd);
    public static int mpfr_coth(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_coth(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_acos(long rop, long op, int rnd);
    public static int mpfr_acos(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_acos(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_asin(long rop, long op, int rnd);
    public static int mpfr_asin(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_asin(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_atan(long rop, long op, int rnd);
    public static int mpfr_atan(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_atan(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sin(long rop, long op, int rnd);
    public static int mpfr_sin(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sin(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sin_cos(long sop, long cop, long op, int rnd);
    public static int mpfr_sin_cos(mpfr_t sop, mpfr_t cop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sin_cos(sop.handle, cop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cos(long rop, long op, int rnd);
    public static int mpfr_cos(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_cos(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_tan(long rop, long op, int rnd);
    public static int mpfr_tan(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_tan(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_atan2(long rop, long y, long x, int rnd);
    public static int mpfr_atan2(mpfr_t rop, mpfr_t y, mpfr_t x, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_atan2(rop.handle, y.handle, x.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sec(long rop, long op, int rnd);
    public static int mpfr_sec(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sec(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_csc(long rop, long op, int rnd);
    public static int mpfr_csc(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_csc(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cot(long rop, long op, int rnd);
    public static int mpfr_cot(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_cot(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_hypot(long rop, long x, long y, int rnd);
    public static int mpfr_hypot(mpfr_t rop, mpfr_t x, mpfr_t y, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_hypot(rop.handle, x.handle, y.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_erf(long rop, long op, int rnd);
    public static int mpfr_erf(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_erf(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_erfc(long rop, long op, int rnd);
    public static int mpfr_erfc(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_erfc(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cbrt(long rop, long op, int rnd);
    public static int mpfr_cbrt(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_cbrt(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_root(long rop, long op, /*unsigned long*/ long k, int rnd);
    public static int mpfr_root(mpfr_t rop, mpfr_t op, /*unsigned long*/ long k, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(k);
	return native_mpfr_root(rop.handle, op.handle, k, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_gamma(long rop, long op, int rnd);
    public static int mpfr_gamma(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_gamma(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_lngamma(long rop, long op, int rnd);
    public static int mpfr_lngamma(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_lngamma(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_digamma(long rop, long op, int rnd);
    public static int mpfr_digamma(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_digamma(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_zeta(long rop, long op, int rnd);
    public static int mpfr_zeta(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_zeta(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_zeta_ui(long rop, /*unsigned long*/ long op, int rnd);
    public static int mpfr_zeta_ui(mpfr_t rop, /*unsigned long*/ long op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op);
	return native_mpfr_zeta_ui(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fac_ui(long rop, /*unsigned long int*/ long op, int rnd);
    public static int mpfr_fac_ui(mpfr_t rop, /*unsigned long int*/ long op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	checkUnsignedIntRange(op);
	return native_mpfr_fac_ui(rop.handle, op, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_j0(long rop, long op, int rnd);
    public static int mpfr_j0(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_j0(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_j1(long rop, long op, int rnd);
    public static int mpfr_j1(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_j1(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_jn(long rop, /*long*/ int n, long op, int rnd);
    public static int mpfr_jn(mpfr_t rop, /*long*/ int n, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_jn(rop.handle, n, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_y0(long rop, long op, int rnd);
    public static int mpfr_y0(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_y0(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_y1(long rop, long op, int rnd);
    public static int mpfr_y1(mpfr_t rop, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_y1(rop.handle, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_yn(long rop, /*long*/ int n, long op, int rnd);
    public static int mpfr_yn(mpfr_t rop, /*long*/ int n, mpfr_t op, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_yn(rop.handle, n, op.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_ai(long rop, long x, int rnd);
    public static int mpfr_ai(mpfr_t rop, mpfr_t x, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_ai(rop.handle, x.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_min(long rop, long op1, long op2, int rnd);
    public static int mpfr_min(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_min(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_max(long rop, long op1, long op2, int rnd);
    public static int mpfr_max(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_max(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_dim(long rop, long op1, long op2, int rnd);
    public static int mpfr_dim(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_dim(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_mul_z(long rop, long op1, long op2, int rnd);
    public static int mpfr_mul_z(mpfr_t rop, mpfr_t op1, GMP.mpz_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul_z(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_z(long rop, long op1, long op2, int rnd);
    public static int mpfr_div_z(mpfr_t rop, mpfr_t op1, GMP.mpz_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div_z(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add_z(long rop, long op1, long op2, int rnd);
    public static int mpfr_add_z(mpfr_t rop, mpfr_t op1, GMP.mpz_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_add_z(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub_z(long rop, long op1, long op2, int rnd);
    public static int mpfr_sub_z(mpfr_t rop, mpfr_t op1, GMP.mpz_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sub_z(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_z_sub(long rop, long op1, long op2, int rnd);
    public static int mpfr_z_sub(mpfr_t rop, GMP.mpz_t op1, mpfr_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_z_sub(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cmp_z(long x, long y);
    public static int mpfr_cmp_z(mpfr_t x, GMP.mpz_t y)
	throws MPFRException
    {
	return native_mpfr_cmp_z(x.handle, y.handle);
    }

    private static native int native_mpfr_mul_q(long rop, long op1, long op2, int rnd);
    public static int mpfr_mul_q(mpfr_t rop, mpfr_t op1, GMP.mpq_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_mul_q(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_div_q(long rop, long op1, long op2, int rnd);
    public static int mpfr_div_q(mpfr_t rop, mpfr_t op1, GMP.mpq_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_div_q(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_add_q(long rop, long op1, long op2, int rnd);
    public static int mpfr_add_q(mpfr_t rop, mpfr_t op1, GMP.mpq_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_add_q(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_sub_q(long rop, long op1, long op2, int rnd);
    public static int mpfr_sub_q(mpfr_t rop, mpfr_t op1, GMP.mpq_t op2, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_sub_q(rop.handle, op1.handle, op2.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_cmp_q(long x, long y);
    public static int mpfr_cmp_q(mpfr_t x, GMP.mpq_t y)
	throws MPFRException
    {
	return native_mpfr_cmp_q(x.handle, y.handle);
    }

    private static native int native_mpfr_cmp_f(long x, long y);
    public static int mpfr_cmp_f(mpfr_t x, GMP.mpf_t y)
	throws MPFRException
    {
	return native_mpfr_cmp_f(x.handle, y.handle);
    }

    private static native int native_mpfr_fma(long rop, long op1, long op2, long op3, int rnd);
    public static int mpfr_fma(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_t op3, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fma(rop.handle, op1.handle, op2.handle, op3.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native int native_mpfr_fms(long rop, long op1, long op2, long op3, int rnd);
    public static int mpfr_fms(mpfr_t rop, mpfr_t op1, mpfr_t op2, mpfr_t op3, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_fms(rop.handle, op1.handle, op2.handle, op3.handle, mpfr_rnd_t_to_int(rnd));
    }

    private static native void native_mpfr_free_cache();
    public static void mpfr_free_cache()
	throws MPFRException
    {
	native_mpfr_free_cache();
    }

    private static native int native_mpfr_subnormalize(long x, int t, int rnd);
    public static int mpfr_subnormalize(mpfr_t x, int t, mpfr_rnd_t rnd)
	throws MPFRException
    {
	return native_mpfr_subnormalize(x.handle, t, mpfr_rnd_t_to_int(rnd));
    }
}
