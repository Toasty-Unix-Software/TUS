default rel
	vpsubw xmm5, xmm2, xmm7
	vpsubw xmm5, xmm7
	vpsubw xmm5, xmm5, oword [0xbb6]
	vpsubw ymm3, ymm4, yword [0x7a3]
	vpsubw ymm3, yword [0x7a3]
	vpsubw ymm1, ymm5, ymm4
	vpsubw xmm3, xmm1, xmm6
	vpsubw xmm3, xmm6
	vpsubw xmm5, xmm4, xmm5
	vpsubw ymm3, ymm7, ymm1
	vpsubw ymm3, ymm1
	vpsubw ymm1, ymm4, ymm1
	vpsubw xmm15, xmm11, xmm13
	vpsubw ymm15, ymm9, ymm15
	vpsubw xmm15, xmm13, xmm11
	vpsubw ymm11, ymm15, ymm8
	vpsubw xmm28, xmm30, xmm27
	vpsubw ymm31, ymm16, ymm25
	vpsubw xmm28, xmm29, xmm29
	vpsubw ymm22, ymm30, ymm26
	vpsubw xmm3{k6}, xmm5, xmm0
	vpsubw ymm2{k5}, ymm2, ymm0
	vpsubw zmm3{k2}, zmm1, zmm4
	vpsubw xmm4{k3}{z}, xmm3, oword [0xa8b]
	vpsubw ymm0{k4}{z}, ymm0, ymm6
	vpsubw zmm2{k6}{z}, zmm5, zword [0x452]
	vpsubw xmm7, xmm2, oword [eax+1]
	vpsubw xmm5, xmm7, oword [eax+64]
	vpsubw ymm1, ymm3, yword [eax+1]
	vpsubw ymm4, ymm0, yword [eax+64]
	vpsubw xmm4, xmm1, oword [eax+1]
	vpsubw xmm1, xmm3, oword [eax+64]
	vpsubw ymm6, ymm0, yword [eax+1]
	vpsubw ymm5, ymm5, yword [eax+64]
	vpsubw zmm1, zmm3, zword [eax+1]
	vpsubw zmm4, zmm0, zword [eax+64]
	vpsubw xmm7, xmm2, [0xf13]
	vpsubw ymm2, ymm6, [0x6eb]
	vpsubw xmm3, xmm2, [0x97b]
	vpsubw ymm2, ymm3, [0xaf8]
	vpsubw zmm0, zmm1, [0x110]
