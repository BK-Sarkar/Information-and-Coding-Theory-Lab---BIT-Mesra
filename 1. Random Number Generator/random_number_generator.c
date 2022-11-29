/**
 * @file random_number_generator.c
 * @brief
 * Compile Command:
 *  gcc random_number_generator.c -o random_number_generator
 * Run Command:
 *  ./random_number_generator
 */
#include <stdio.h>
#include <time.h>
#include <limits.h>
#define ll long long int

ll A = 102911, B = 103969, C = 1000025263, seed;

ll generate_random_number()
{
    ll r = ((A * seed) % C + B) % C;
    seed = r;
    return r;
}

int main()
{
    // initialize the seed
    seed = clock();
    ll rand_num = generate_random_number();
    printf("Random Number Generated: %ld\n", rand_num);
    return 0;
}
