#include "naivedsp/iir_states.h"

void naive_iir_1st_df1_states_init(NaiveIir1stDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
}

void naive_iir_2nd_df1_states_init(NaiveIir2ndDf1States *self)
{
    self->s0 = 0;
    self->s1 = 0;
    self->s2 = 0;
    self->s3 = 0;
}
