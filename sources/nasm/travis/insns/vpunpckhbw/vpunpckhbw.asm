default rel
	vpunpckhbw xmm6, xmm1, xmm4
	vpunpckhbw xmm6, xmm4
	vpunpckhbw xmm1, xmm3, xmm1
	vpunpckhbw ymm4, ymm2, yword [0x830]
	vpunpckhbw ymm4, yword [0x830]
	vpunpckhbw ymm0, ymm3, ymm1
	vpunpckhbw xmm4, xmm7, xmm6
	vpunpckhbw xmm4, xmm6
	vpunpckhbw xmm1, xmm3, xmm0
	vpunpckhbw ymm7, ymm3, ymm1
	vpunpckhbw ymm7, ymm1
	vpunpckhbw ymm3, ymm1, yword [0x5f8]
	vpunpckhbw xmm11, xmm15, xmm9
	vpunpckhbw ymm14, ymm13, ymm8
	vpunpckhbw xmm13, xmm14, xmm13
	vpunpckhbw ymm8, ymm15, ymm10
	vpunpckhbw xmm27, xmm23, xmm20
	vpunpckhbw ymm29, ymm23, ymm24
	vpunpckhbw xmm22, xmm21, xmm21
	vpunpckhbw ymm16, ymm27, ymm20
	vpunpckhbw xmm4{k6}, xmm3, oword [0xe63]
	vpunpckhbw ymm7{k7}, ymm0, ymm2
	vpunpckhbw zmm5{k5}, zmm1, zmm2
	vpunpckhbw xmm1{k5}{z}, xmm4, oword [0x48e]
	vpunpckhbw ymm7{k7}{z}, ymm5, yword [0x209]
	vpunpckhbw zmm5{k1}{z}, zmm4, zmm3
	vpunpckhbw xmm4, xmm5, oword [eax+1]
	vpunpckhbw xmm5, xmm7, oword [eax+64]
	vpunpckhbw ymm3, ymm4, yword [eax+1]
	vpunpckhbw ymm3, ymm5, yword [eax+64]
	vpunpckhbw xmm7, xmm1, oword [eax+1]
	vpunpckhbw xmm4, xmm3, oword [eax+64]
	vpunpckhbw ymm3, ymm2, yword [eax+1]
	vpunpckhbw ymm6, ymm7, yword [eax+64]
	vpunpckhbw zmm0, zmm7, zword [eax+1]
	vpunpckhbw zmm2, zmm3, zword [eax+64]
	vpunpckhbw xmm6, xmm0, [0xc1e]
	vpunpckhbw ymm7, ymm7, [0x49b]
	vpunpckhbw xmm3, xmm0, [0xffe]
	vpunpckhbw ymm5, ymm1, [0xa31]
	vpunpckhbw zmm7, zmm1, [0x307]
