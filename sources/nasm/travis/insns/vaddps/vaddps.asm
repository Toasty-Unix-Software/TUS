default rel
	vaddps xmm3, xmm1, xmm4
	vaddps xmm3, xmm4
	vaddps xmm3, xmm4, oword [0x9bd]
	vaddps ymm5, ymm5, ymm3
	vaddps ymm5, ymm3
	vaddps ymm7, ymm7, ymm6
	vaddps xmm6, xmm0, oword [0xa06]
	vaddps xmm6, oword [0xa06]
	vaddps xmm0, xmm1, xmm6
	vaddps ymm3, ymm1, ymm4
	vaddps ymm3, ymm4
	vaddps ymm0, ymm0, yword [0x704]
	vaddps xmm14, xmm8, xmm10
	vaddps ymm15, ymm9, ymm8
	vaddps xmm15, xmm14, xmm12
	vaddps ymm15, ymm13, ymm14
	vaddps xmm18, xmm20, xmm17
	vaddps ymm23, ymm29, ymm31
	vaddps xmm20, xmm26, xmm23
	vaddps ymm27, ymm20, ymm27
	vaddps xmm7{k1}, xmm0, oword [0x452]
	vaddps ymm4{k3}, ymm1, ymm4
	vaddps zmm7{k1}, zmm7, zmm2
	vaddps xmm0{k6}{z}, xmm3, xmm2
	vaddps ymm0{k5}{z}, ymm6, ymm3
	vaddps zmm4{k4}{z}, zmm1, zword [0x808]
	vaddps zmm2, zmm1, zmm1, {rn-sae}
	vaddps xmm6, xmm6, oword [eax+1]
	vaddps xmm5, xmm3, oword [eax+64]
	vaddps ymm6, ymm2, yword [eax+1]
	vaddps ymm0, ymm2, yword [eax+64]
	vaddps xmm3, xmm7, oword [eax+1]
	vaddps xmm7, xmm5, oword [eax+64]
	vaddps ymm6, ymm1, yword [eax+1]
	vaddps ymm3, ymm7, yword [eax+64]
	vaddps zmm3, zmm6, zword [eax+1]
	vaddps zmm3, zmm0, zword [eax+64]
	vaddps xmm6, xmm7, [0x657]
	vaddps ymm0, ymm4, [0xc18]
	vaddps xmm6, xmm1, [0xf15]
	vaddps ymm3, ymm3, [0x5ef]
	vaddps zmm1, zmm7, [0xb26]
