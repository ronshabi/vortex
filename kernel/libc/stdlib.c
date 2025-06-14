#include "stdlib.h"
#include "ctype.h"

int atoi(const char *nptr)
{
    int sum        = 0;
    int multiplier = 1;
    int i          = 0;
    int coef       = 1;

    if (*nptr == '-' || *nptr == '+')
    {
        coef = (*nptr == '-') ? -1 : 1;
        nptr++;
    }

    // we need to go from the last digit back
    while (nptr[i] && isdigit(nptr[i]))
    {
        i++;
    }

    if (i == 0)
    {
        return 0; // not a number
    }

    i--;

    // now go back to nptr, and sum it up
    while (i >= 0)
    {
        const int BASE = 10;
        sum += multiplier * (nptr[i] - '0');
        multiplier *= BASE;
        i--;
    }

    return coef * sum;
}