default rel
	vpdpbssd xmm5, xmm3, xmm0
	vpdpbssd xmm0, xmm4, xmm6
	vpdpbssd ymm4, ymm2, ymm7
	vpdpbssd ymm4, ymm1, ymm2
	vpdpbssd xmm7, xmm3, xmm1
	vpdpbssd xmm2, xmm7, xmm5
	vpdpbssd ymm6, ymm0, ymm5
	vpdpbssd ymm2, ymm4, yword [0xe57]
	vpdpbssd xmm12, xmm14, xmm15
	vpdpbssd ymm13, ymm8, ymm14
	vpdpbssd xmm11, xmm8, xmm9
	vpdpbssd ymm14, ymm11, ymm15
	vpdpbssd xmm23, xmm28, xmm23
	vpdpbssd ymm17, ymm24, ymm19
	vpdpbssd xmm20, xmm31, xmm22
	vpdpbssd ymm20, ymm23, ymm20
	vpdpbssd xmm6{k1}, xmm4, oword [0x1f7]
	vpdpbssd ymm1{k1}, ymm0, yword [0x137]
	vpdpbssd zmm6{k3}, zmm1, zmm3
	vpdpbssd xmm7{k5}{z}, xmm6, oword [0x641]
	vpdpbssd ymm4{k6}{z}, ymm3, ymm1
	vpdpbssd zmm3{k3}{z}, zmm2, zmm5
	vpdpbssd xmm6, xmm6, oword [eax+1]
	vpdpbssd xmm5, xmm0, oword [eax+64]
	vpdpbssd ymm4, ymm0, yword [eax+1]
	vpdpbssd ymm6, ymm0, yword [eax+64]
	vpdpbssd xmm2, xmm2, oword [eax+1]
	vpdpbssd xmm2, xmm1, oword [eax+64]
	vpdpbssd ymm7, ymm4, yword [eax+1]
	vpdpbssd ymm5, ymm3, yword [eax+64]
	vpdpbssd zmm7, zmm2, zword [eax+1]
	vpdpbssd zmm6, zmm6, zword [eax+64]
	vpdpbssd xmm7, xmm2, [0x307]
	vpdpbssd ymm5, ymm5, [0x275]
	vpdpbssd xmm4, xmm2, [0x389]
	vpdpbssd ymm4, ymm3, [0xb28]
	vpdpbssd zmm3, zmm4, [0x9ad]
