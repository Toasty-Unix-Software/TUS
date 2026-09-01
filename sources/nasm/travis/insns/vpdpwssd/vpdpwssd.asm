default rel
	vpdpwssd xmm3, xmm5, oword [0xd33]
	vpdpwssd xmm0, xmm0, oword [0xa6b]
	vpdpwssd ymm2, ymm1, ymm7
	vpdpwssd ymm7, ymm2, yword [0xd0d]
	vpdpwssd xmm7, xmm7, xmm5
	vpdpwssd xmm7, xmm5
	vpdpwssd xmm0, xmm4, xmm4
	vpdpwssd ymm4, ymm6, yword [0x686]
	vpdpwssd ymm4, yword [0x686]
	vpdpwssd ymm0, ymm5, ymm4
	vpdpwssd xmm13, xmm11, xmm12
	vpdpwssd ymm10, ymm8, ymm13
	vpdpwssd xmm14, xmm9, xmm9
	vpdpwssd ymm13, ymm12, ymm11
	vpdpwssd xmm21, xmm22, xmm26
	vpdpwssd ymm30, ymm30, ymm28
	vpdpwssd xmm30, xmm19, xmm30
	vpdpwssd ymm27, ymm18, ymm23
	vpdpwssd xmm4{k3}, xmm6, xmm6
	vpdpwssd ymm4{k6}, ymm6, ymm7
	vpdpwssd zmm2{k5}, zmm4, zmm6
	vpdpwssd xmm1{k7}{z}, xmm7, xmm4
	vpdpwssd ymm1{k5}{z}, ymm6, yword [0x907]
	vpdpwssd zmm4{k1}{z}, zmm5, zmm4
	vpdpwssd xmm7, xmm7, oword [eax+1]
	vpdpwssd xmm5, xmm6, oword [eax+64]
	vpdpwssd ymm5, ymm5, yword [eax+1]
	vpdpwssd ymm7, ymm7, yword [eax+64]
	vpdpwssd xmm0, xmm5, oword [eax+1]
	vpdpwssd xmm4, xmm3, oword [eax+64]
	vpdpwssd ymm2, ymm6, yword [eax+1]
	vpdpwssd ymm5, ymm2, yword [eax+64]
	vpdpwssd zmm1, zmm0, zword [eax+1]
	vpdpwssd zmm1, zmm4, zword [eax+64]
	vpdpwssd xmm1, xmm3, [0x6a5]
	vpdpwssd ymm5, ymm3, [0xe61]
	vpdpwssd xmm6, xmm6, [0xab6]
	vpdpwssd ymm3, ymm6, [0x595]
	vpdpwssd zmm5, zmm3, [0xe5a]
