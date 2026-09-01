default rel
	vmaxpd xmm4, xmm5, xmm0
	vmaxpd xmm4, xmm0
	vmaxpd xmm4, xmm5, xmm5
	vmaxpd ymm0, ymm2, ymm5
	vmaxpd ymm0, ymm5
	vmaxpd ymm2, ymm1, yword [0x330]
	vmaxpd xmm1, xmm3, xmm5
	vmaxpd xmm1, xmm5
	vmaxpd xmm6, xmm4, xmm2
	vmaxpd ymm0, ymm1, ymm6
	vmaxpd ymm0, ymm6
	vmaxpd ymm5, ymm4, yword [0x5d8]
	vmaxpd xmm13, xmm10, xmm11
	vmaxpd ymm12, ymm12, ymm8
	vmaxpd xmm12, xmm14, xmm8
	vmaxpd ymm14, ymm13, ymm10
	vmaxpd xmm16, xmm31, xmm25
	vmaxpd ymm17, ymm24, ymm20
	vmaxpd xmm19, xmm19, xmm18
	vmaxpd ymm28, ymm29, ymm19
	vmaxpd xmm7{k7}, xmm7, xmm0
	vmaxpd ymm7{k2}, ymm5, ymm7
	vmaxpd zmm2{k7}, zmm7, zword [0x261]
	vmaxpd xmm5{k2}{z}, xmm1, xmm7
	vmaxpd ymm2{k5}{z}, ymm6, ymm7
	vmaxpd zmm0{k6}{z}, zmm5, zmm0
	vmaxpd zmm6, zmm6, zmm2, {sae}
	vmaxpd xmm4, xmm0, oword [eax+1]
	vmaxpd xmm5, xmm5, oword [eax+64]
	vmaxpd ymm3, ymm6, yword [eax+1]
	vmaxpd ymm5, ymm6, yword [eax+64]
	vmaxpd xmm5, xmm4, oword [eax+1]
	vmaxpd xmm4, xmm2, oword [eax+64]
	vmaxpd ymm0, ymm5, yword [eax+1]
	vmaxpd ymm2, ymm0, yword [eax+64]
	vmaxpd zmm5, zmm6, zword [eax+1]
	vmaxpd zmm7, zmm2, zword [eax+64]
	vmaxpd xmm0, xmm3, [0x777]
	vmaxpd ymm3, ymm0, [0xaa6]
	vmaxpd xmm7, xmm4, [0x61a]
	vmaxpd ymm2, ymm0, [0x155]
	vmaxpd zmm7, zmm2, [0x1d9]
