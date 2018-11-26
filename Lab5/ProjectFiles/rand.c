#include "rand.h"
#include "cmsis_os.h"
#include "TM4C129.h"                    // Device header
#include <stdbool.h>
//*****************************************************************************
//! Set the random number generator seed.
//!
//! \param ulSeed is the new seed value to use for the random number generator.
//!
//! This function is very similar to the C library <tt>srand()</tt> function.
//! It will set the seed value used in the <tt>urand()</tt> function.
//!
//! \return None
//*****************************************************************************
void usrand(unsigned int ulSeed)
{
    g_ulRandomSeed = (ulSeed-50)/10;
}

//*****************************************************************************
//! Generate a new (pseudo) random number
//!
//! This function is very similar to the C library <tt>rand()</tt> function.
//! It will generate a pseudo-random number sequence based on the seed value.
//!
//! \return A pseudo-random number will be returned.
//*****************************************************************************
int urand(void)

{

    // Generate a new pseudo-random number with a linear congruence random
    // number generator.  This new random number becomes the seed for the next
    // random number.

g_ulRandomSeed = ((g_ulRandomSeed * 32) + 100)/65;

    // Return the new random number.

return((int)g_ulRandomSeed);
}