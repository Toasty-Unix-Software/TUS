default rel
	vpunpckhwd xmm3, xmm0, xmm6
	vpunpckhwd xmm3, xmm6
	vpunpckhwd xmm0, xmm2, oword [0x417]
	vpunpckhwd ymm3, ymm6, ymm5
	vpunpckhwd ymm3, ymm5
	vpunpckhwd ymm1, ymm6, ymm3
	vpunpckhwd xmm3, xmm5, oword [0xc7d]
	vpunpckhwd xmm3, oword [0xc7d]
	vpunpckhwd xmm3, xmm5, xmm0
	vpunpckhwd ymm3, ymm5, ymm2
	vpunpckhwd ymm3, ymm2
	vpunpckhwd ymm1, ymm5, ymm1
	vpunpckhwd xmm12, xmm9, xmm13
	vpunpckhwd ymm14, ymm9, ymm11
	vpunpckhwd xmm8, xmm8, xmm12
	vpunpckhwd ymm8, ymm15, ymm8
	vpunpckhwd xmm23, xmm28, xmm27
	vpunpckhwd ymm31, ymm29, ymm28
	vpunpckhwd xmm20, xmm21, xmm24
	vpunpckhwd ymm30, ymm28, ymm20
	vpunpckhwd xmm0{k2}, xmm3, xmm6
	vpunpckhwd ymm2{k5}, ymm4, ymm0
	vpunpckhwd zmm3{k7}, zmm7, zword [0x79e]
	vpunpckhwd xmm0{k6}{z}, xmm3, xmm5
	vpunpckhwd ymm6{k4}{z}, ymm3, ymm0
	vpunpckhwd zmm1{k1}{z}, zmm6, zmm0
	vpunpckhwd xmm0, xmm6, oword [eax+1]
	vpunpckhwd xmm2, xmm5, oword [eax+64]
	vpunpckhwd ymm4, ymm2, yword [eax+1]
	vpunpckhwd ymm0, ymm7, yword [eax+64]
	vpunpckhwd xmm6, xmm5, oword [eax+1]
	vpunpckhwd xmm1, xmm5, oword [eax+64]
	vpunpckhwd ymm4, ymm2, yword [eax+1]
	vpunpckhwd ymm5, ymm4, yword [eax+64]
	vpunpckhwd zmm1, zmm3, zword [eax+1]
	vpunpckhwd zmm6, zmm2, zword [eax+64]
	vpunpckhwd xmm2, xmm5, [0xaf9]
	vpunpckhwd ymm1, ymm6, [0x7b3]
	vpunpckhwd xmm7, xmm5, [0xa9a]
	vpunpckhwd ymm1, ymm5, [0x23a]
	vpunpckhwd zmm4, zmm3, [0x67c]
