default rel
	vunpcklpd xmm7, xmm1, xmm0
	vunpcklpd xmm7, xmm0
	vunpcklpd xmm7, xmm3, xmm4
	vunpcklpd ymm0, ymm3, yword [0x8d5]
	vunpcklpd ymm0, yword [0x8d5]
	vunpcklpd ymm2, ymm2, ymm2
	vunpcklpd xmm4, xmm2, oword [0xd2e]
	vunpcklpd xmm4, oword [0xd2e]
	vunpcklpd xmm4, xmm7, oword [0x639]
	vunpcklpd ymm4, ymm3, ymm0
	vunpcklpd ymm4, ymm0
	vunpcklpd ymm7, ymm7, ymm5
	vunpcklpd xmm9, xmm8, xmm15
	vunpcklpd ymm15, ymm8, ymm14
	vunpcklpd xmm13, xmm11, xmm13
	vunpcklpd ymm9, ymm14, ymm11
	vunpcklpd xmm19, xmm17, xmm23
	vunpcklpd ymm17, ymm16, ymm28
	vunpcklpd xmm20, xmm23, xmm18
	vunpcklpd ymm20, ymm20, ymm30
	vunpcklpd xmm7{k7}, xmm6, xmm5
	vunpcklpd ymm2{k4}, ymm0, ymm5
	vunpcklpd zmm3{k1}, zmm7, zmm1
	vunpcklpd xmm0{k4}{z}, xmm0, xmm0
	vunpcklpd ymm3{k1}{z}, ymm0, yword [0x3b9]
	vunpcklpd zmm4{k3}{z}, zmm2, zmm5
	vunpcklpd xmm1, xmm5, oword [eax+1]
	vunpcklpd xmm5, xmm7, oword [eax+64]
	vunpcklpd ymm3, ymm6, yword [eax+1]
	vunpcklpd ymm4, ymm2, yword [eax+64]
	vunpcklpd xmm3, xmm2, oword [eax+1]
	vunpcklpd xmm5, xmm7, oword [eax+64]
	vunpcklpd ymm1, ymm0, yword [eax+1]
	vunpcklpd ymm2, ymm4, yword [eax+64]
	vunpcklpd zmm1, zmm2, zword [eax+1]
	vunpcklpd zmm3, zmm1, zword [eax+64]
	vunpcklpd xmm6, xmm3, [0xcba]
	vunpcklpd ymm5, ymm6, [0xc0a]
	vunpcklpd xmm3, xmm0, [0xd3f]
	vunpcklpd ymm6, ymm4, [0x5e7]
	vunpcklpd zmm4, zmm6, [0xb8e]
