default rel
	vandnpd xmm4, xmm1, xmm6
	vandnpd xmm4, xmm6
	vandnpd xmm2, xmm4, oword [0x1c7]
	vandnpd ymm5, ymm1, ymm2
	vandnpd ymm5, ymm2
	vandnpd ymm4, ymm7, yword [0xdac]
	vandnpd xmm7, xmm7, xmm2
	vandnpd xmm7, xmm2
	vandnpd xmm6, xmm0, xmm6
	vandnpd ymm4, ymm6, ymm0
	vandnpd ymm4, ymm0
	vandnpd ymm5, ymm3, ymm6
	vandnpd xmm11, xmm14, xmm10
	vandnpd ymm15, ymm10, ymm11
	vandnpd xmm11, xmm9, xmm8
	vandnpd ymm8, ymm10, ymm15
	vandnpd xmm16, xmm25, xmm17
	vandnpd ymm26, ymm19, ymm30
	vandnpd xmm19, xmm24, xmm24
	vandnpd ymm24, ymm16, ymm29
	vandnpd xmm1{k1}, xmm1, xmm6
	vandnpd ymm1{k7}, ymm4, yword [0x456]
	vandnpd zmm4{k2}, zmm3, zmm3
	vandnpd xmm7{k6}{z}, xmm3, oword [0xea3]
	vandnpd ymm5{k7}{z}, ymm2, ymm6
	vandnpd zmm5{k5}{z}, zmm6, zmm1
	vandnpd xmm1, xmm6, oword [eax+1]
	vandnpd xmm7, xmm0, oword [eax+64]
	vandnpd ymm7, ymm4, yword [eax+1]
	vandnpd ymm1, ymm3, yword [eax+64]
	vandnpd xmm7, xmm5, oword [eax+1]
	vandnpd xmm0, xmm0, oword [eax+64]
	vandnpd ymm5, ymm2, yword [eax+1]
	vandnpd ymm6, ymm4, yword [eax+64]
	vandnpd zmm0, zmm2, zword [eax+1]
	vandnpd zmm5, zmm0, zword [eax+64]
	vandnpd xmm2, xmm6, [0xc56]
	vandnpd ymm3, ymm4, [0x9fd]
	vandnpd xmm3, xmm7, [0x4df]
	vandnpd ymm0, ymm3, [0x81e]
	vandnpd zmm4, zmm6, [0x726]
