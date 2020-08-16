#include <stdio.h>
#include <math.h>
#include "naivedsp/conversion.h"
#include "naivedsp/reverb.h"
#include "naivedsp/reverb_design.h"
#include "naivedsp/iir_design.h"

int main(int argc, char *argv[])
{
    int err = 0;

    if (argc < 2)
        return -1;

    FILE *fin = fopen(argv[1], "rb");
    FILE *fout = fopen("output.pcm", "wb");

    NaiveDefaultAllocator allocator;
    naive_default_allocator_init(&allocator);

    NaiveReverbStates reverb_states;
    NaiveReverbParams reverb_params;
    NaiveReverbParams params;

    err = naive_reverb_states_init(&reverb_states, &naive_default_alloc, &allocator, 44100, 16, 8192, 1, 2);
    if (err)
        goto end;

    err = naive_reverb_params_init(&reverb_params, &naive_default_alloc, &allocator, 16, 1, 2);
    if (err)
        goto end;

    err = naive_reverb_params_init(&params, &naive_default_alloc, &allocator, 16, 1, 2);
    if (err)
        goto end;

    err = naive_reverb_params_design(&params, 44100, 0.01, 16, 1, 10, 300.0f, 4000.0f, 1.2f, 1.0f, 1.0f, 1.0f / 16, 1.0f / 8);
    if (err)
        goto end;

    /* params.wet_eq_coeffs.num_bands = 1; */
    /* params.wet_eq_coeffs.band_coeffs[0].has_fos = NAIVE_TRUE; */
    /* err = naive_iir_1st_coeffs_lowpass(&params.wet_eq_coeffs.band_coeffs[0].fos_coeffs, 44100, 8000.0f); */
    /* if (err) */
    /*     goto end; */

    err = naive_reverb_set_params(&reverb_states, &reverb_params, &params);
    if (err)
        goto end;

    NaiveI16 *inout_i16 = naive_default_alloc(&allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveI16), sizeof(NaiveI16) * 256 * 2);
    NaiveF32 *left_f32 = naive_default_alloc(&allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * 256);
    NaiveF32 *right_f32 = naive_default_alloc(&allocator, NAIVE_MEM_STATE, NAIVE_ALIGNOF(NaiveF32), sizeof(NaiveF32) * 256);
    NaiveF32 *inout_f32[] = { left_f32, right_f32 };
    NaiveF32 *scratch = naive_default_alloc(&allocator, NAIVE_MEM_SCRATCH, NAIVE_ALIGNOF(NaiveF32) * 2, NAIVE_REVERB_SCRATCH_SIZE(16, 8192, 8192, 256));

    while (1) {
        NaiveU32 block_size = fread(inout_i16, sizeof(NaiveI16), 256 * 2, fin) / 2;
        naive_i16_q15_interleaved_to_f32_planar(inout_f32, inout_i16, 2, block_size);
        naive_reverb_process(&reverb_states, &reverb_params, left_f32, right_f32, block_size, scratch);
        naive_f32_planar_to_i16_q15_interleaved(inout_i16, inout_f32, 2, block_size);
        fwrite(inout_i16, sizeof(NaiveI16), block_size * 2, fout);
        if (block_size < 256)
            break;
    }

end:
    naive_default_allocator_finalize(&allocator);
    fclose(fout);
    fclose(fin);
    return err;
}
