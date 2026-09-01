default rel
	vpdpwsud xmm6, xmm0, xmm7
	vpdpwsud xmm2, xmm1, xmm2
	vpdpwsud ymm6, ymm5, ymm2
	vpdpwsud ymm7, ymm2, ymm6
	vpdpwsud xmm0, xmm4, xmm3
	vpdpwsud xmm2, xmm1, xmm6
	vpdpwsud ymm5, ymm3, ymm4
	vpdpwsud ymm5, ymm4, ymm6
	vpdpwsud xmm9, xmm9, xmm13
	vpdpwsud ymm12, ymm13, ymm13
	vpdpwsud xmm8, xmm8, xmm14
	vpdpwsud ymm15, ymm8, ymm14
	vpdpwsud xmm21, xmm27, xmm18
	vpdpwsud ymm30, ymm23, ymm22
	vpdpwsud xmm29, xmm21, xmm17
	vpdpwsud ymm18, ymm21, ymm20
	vpdpwsud xmm5{k4}, xmm0, oword [0xb12]
	vpdpwsud ymm5{k5}, ymm0, yword [0x721]
	vpdpwsud zmm7{k4}, zmm7, zword [0x99f]
	vpdpwsud xmm0{k1}{z}, xmm6, xmm2
	vpdpwsud ymm6{k5}{z}, ymm0, ymm1
	vpdpwsud zmm4{k4}{z}, zmm1, zmm5
	vpdpwsud xmm4, xmm2, oword [eax+1]
	vpdpwsud xmm1, xmm3, oword [eax+64]
	vpdpwsud ymm1, ymm2, yword [eax+1]
	vpdpwsud ymm1, ymm6, yword [eax+64]
	vpdpwsud xmm6, xmm0, oword [eax+1]
	vpdpwsud xmm5, xmm1, oword [eax+64]
	vpdpwsud ymm6, ymm1, yword [eax+1]
	vpdpwsud ymm5, ymm4, yword [eax+64]
	vpdpwsud zmm1, zmm3, zword [eax+1]
	vpdpwsud zmm3, zmm6, zword [eax+64]
	vpdpwsud xmm0, xmm1, [0x2c9]
	vpdpwsud ymm6, ymm6, [0x5e3]
	vpdpwsud xmm6, xmm0, [0x7ef]
	vpdpwsud ymm0, ymm6, [0xf2f]
	vpdpwsud zmm3, zmm0, [0xb66]
