default rel
	vandpd xmm4, xmm4, oword [0xdfc]
	vandpd xmm4, oword [0xdfc]
	vandpd xmm4, xmm0, xmm6
	vandpd ymm2, ymm3, ymm3
	vandpd ymm2, ymm3
	vandpd ymm3, ymm4, ymm7
	vandpd xmm3, xmm5, oword [0x9f4]
	vandpd xmm3, oword [0x9f4]
	vandpd xmm1, xmm3, xmm1
	vandpd ymm2, ymm1, ymm6
	vandpd ymm2, ymm6
	vandpd ymm7, ymm5, ymm1
	vandpd xmm9, xmm10, xmm13
	vandpd ymm9, ymm11, ymm12
	vandpd xmm8, xmm14, xmm13
	vandpd ymm14, ymm9, ymm15
	vandpd xmm25, xmm24, xmm27
	vandpd ymm17, ymm26, ymm25
	vandpd xmm26, xmm31, xmm25
	vandpd ymm25, ymm30, ymm21
	vandpd xmm1{k4}, xmm5, xmm3
	vandpd ymm0{k3}, ymm1, ymm1
	vandpd zmm5{k1}, zmm3, zmm5
	vandpd xmm5{k3}{z}, xmm3, xmm0
	vandpd ymm1{k3}{z}, ymm1, ymm5
	vandpd zmm4{k1}{z}, zmm1, zmm6
	vandpd xmm5, xmm2, oword [eax+1]
	vandpd xmm5, xmm1, oword [eax+64]
	vandpd ymm5, ymm0, yword [eax+1]
	vandpd ymm5, ymm6, yword [eax+64]
	vandpd xmm5, xmm3, oword [eax+1]
	vandpd xmm6, xmm1, oword [eax+64]
	vandpd ymm7, ymm0, yword [eax+1]
	vandpd ymm7, ymm2, yword [eax+64]
	vandpd zmm5, zmm2, zword [eax+1]
	vandpd zmm7, zmm6, zword [eax+64]
	vandpd xmm3, xmm6, [0xf2d]
	vandpd ymm4, ymm0, [0x60e]
	vandpd xmm4, xmm0, [0x2b2]
	vandpd ymm4, ymm4, [0x7cf]
	vandpd zmm0, zmm6, [0xdb8]
