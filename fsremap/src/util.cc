/*
 * util.cc
 *
 *  Created on: Mar 9, 2011
 *      Author: max
 */

#include "first.hh"

#include <cstdlib>    // for srandom(), random()
#include <ctime>      // for time()

#include "util.hh"    // for ff_pretty_size(), ff_strtoull()

FT_NAMESPACE_BEGIN

static const ft_ull max_n_div_10 = (ft_ull)-1 / 10;
const char max_n_rem_10 = (char)((ft_ull)-1 % 10);

static int ff_str2ull_(const char *& str, ft_ull * ret_n)
{
    ft_ull n = 0;
    char ch;
    while ((ch = *str) >= '0' && ch <= '9') {
        str++;
        ch -= '0';
        if (n < max_n_div_10 || (n == max_n_div_10 && ch <= max_n_rem_10)) {
            n *= 10;
            n += ch;
        } else
            return EOVERFLOW;
    }
    * ret_n = n;
    return 0;
}

int ff_str2ull(const char * str, ft_ull * ret_n)
{
    ft_ull n = 0;
    int err = ff_str2ull_(str, & n);
    if (err == 0 && *str != '\0')
        return EINVAL;
    if (err == 0)
        * ret_n = n;
    return err;
}


/** convert string with optional [k|M|G|T|P|E|Z|Y] scale to unsigned number */
int ff_str2ull_scaled(const char * str, ft_ull * ret_n)
{
    ft_ull scale, n = 0;
    int err;
    do {
        if ((err = ff_str2ull_(str, & n)) != 0)
            break;

        switch (*str) {
            case '\0': scale = 0; break;
            case 'k': scale = 10; break;
            case 'M': scale = 20; break;
            case 'G': scale = 30; break;
            case 'T': scale = 40; break;
            case 'P': scale = 50; break;
            case 'E': scale = 60; break;
            case 'Z': scale = 70; break;
            case 'Y': scale = 80; break;
            default: err = EINVAL; break;
        }
        if (err != 0)
            break;

        /* no benefit in scaling 0 */
        if (n != 0) {
            /* overflow? */
            if (scale >= 8*sizeof(ft_ull) || n > (ft_ull)-1 >> scale) {
                err = EOVERFLOW;
                break;
            }
            n <<= scale;
        }
        * ret_n = n;
    } while (0);
    return err;
}





/** return a random number in the range [0,n] */
ft_ull ff_random(ft_ull n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        srandom(time(NULL));
    }

    ft_ull r;
    if (n == 0)
        return 0;
    if (n < RAND_MAX) {
        ft_ull max = RAND_MAX - (RAND_MAX % (n + 1));
        do {
            r = random();
        } while (r > max);
        return r / (max / (n + 1));
    }
    if (n == RAND_MAX)
        return random();
    const ft_ull max_p_1 = (ft_ull)RAND_MAX + 1;
    const ft_ull n_hi = (n + RAND_MAX) / max_p_1;
    do {
        r = ff_random(n_hi) * max_p_1 + random();
    } while (r > n);
    return r;
}







static char const* fc_pretty_size_unit[] = {
    "", "kilo", "mega", "giga", "tera", "peta", "exa", "zeta", "yotta",
};

enum { fc_pretty_size_len = sizeof(fc_pretty_size_unit)/sizeof(fc_pretty_size_unit[0]) };

/**
 * return human-readable representation of len,
 * with [kilo|mega|giga|tera|peta|exa|zeta|yotta] scale as appropriate
 */
const char * ff_pretty_size(ft_uoff len, double * ret_pretty_len)
{
    double pretty_len = (double) len;
    ft_size i;
    for (i = 0; i < fc_pretty_size_len-1 && pretty_len >= 1024.0; i++)
        pretty_len *= .0009765625; // exactly 1/1024
    * ret_pretty_len = pretty_len;
    return fc_pretty_size_unit[i];
}






static char const* fc_pretty_time_unit[] = {
    "second", "minute", "hour", "day", "month", "year",
};
static const double fc_pretty_time[] = {
    1.0, 60.0, 3600.0, 86400.0, 86400.0 * 30, 86400.0 * 365,
};

enum { fc_pretty_time_len = sizeof(fc_pretty_time)/sizeof(fc_pretty_time[0]) };

/**
 * return human-readable representation of time,
 * with [second|minute|hour|day|month|year] scale as appropriate
 */
char const * ff_pretty_time(double time, double * ret_pretty_time)
{
    ft_size i = 0;
    for (; i < fc_pretty_time_len - 1; i++) {
        if (time < fc_pretty_time[i+1])
            break;
    }
    * ret_pretty_time = time / fc_pretty_time[i];
    return fc_pretty_time_unit[i];
}

/**
 * return human-readable representation of time,
 * with [second|minute|hour|day|month|year] scale as appropriate
 */
void ff_pretty_time2(double time,
		ft_ull * ret_pretty_time1, char const ** ret_pretty_label1,
		ft_ull * ret_pretty_time2, char const ** ret_pretty_label2)
{
    ft_size i = 0;
    for (; i < fc_pretty_time_len - 1; i++) {
        if (time < fc_pretty_time[i+1])
            break;
    }
    double time1 = time / fc_pretty_time[i];

    * ret_pretty_time1 = ff_pretty_number(time1);
    * ret_pretty_label1 = fc_pretty_time_unit[i];
    if (ret_pretty_time2 == NULL || ret_pretty_label2 == NULL)
    	return;

	* ret_pretty_time2 = 0.0;
	* ret_pretty_label2 = NULL;

	if (i == 0 || time1 <= 1.0 || time1 > 1.9)
		return;

    time -= (ft_ull) time1 * fc_pretty_time[i];
    if (time <= 0.0)
    	return;

    ft_size j = 0;
    for (j = 0; j < fc_pretty_time_len - 1; j++) {
    	if (time < fc_pretty_time[j+1])
    		break;
    }
    if (j + 1 == i) {
    	* ret_pretty_time1 = 1;
		* ret_pretty_time2 = ff_pretty_number(time / fc_pretty_time[j]);
		* ret_pretty_label2 = fc_pretty_time_unit[j];
    }
}

/**
 * return approximate number, rounding to "one-and-a-half" significant digits.
 * if t <= 10, return (ft_ull)(t + 0.5)
 * if t <= 30, return ((ft_ull)(t/5 + 0.5)) * 5
 * if t <= 100, return ((ft_ull)(t/10 + 0.5)) * 10
 * if t <= 300, return ((ft_ull)(t/50 + 0.5)) * 50
 * otherwise return ((ft_ull)(t/100 + 0.5)) * 100
 */
ft_ull ff_pretty_number(double t) {
	ft_ull n;
	if (t <= 10.0)
		n = t + 0.5;
	else if (t <= 30.0)
		n = 5 * (ft_ull)(t*0.2 + 0.5);
	else if (t <= 100.0)
		n = 10 * (ft_ull)(t*0.1 + 0.5);
	else if (t <= 300.0)
		n = 50 * (ft_ull)(t*0.02 + 0.5);
	else
		n = 100 * (ft_ull)(t*0.01 + 0.5);
	return n;
}

FT_NAMESPACE_END
