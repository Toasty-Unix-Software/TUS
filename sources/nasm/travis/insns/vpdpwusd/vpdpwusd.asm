default rel
	vpdpwusd xmm0, xmm4, xmm4
	vpdpwusd xmm1, xmm1, xmm2
	vpdpwusd ymm4, ymm5, ymm1
	vpdpwusd ymm2, ymm3, yword [0x253]
	vpdpwusd xmm4, xmm1, xmm4
	vpdpwusd xmm6, xmm0, xmm7
	vpdpwusd ymm1, ymm0, yword [0xac8]
	vpdpwusd ymm4, ymm0, ymm1
	vpdpwusd xmm13, xmm12, xmm14
	vpdpwusd ymm13, ymm14, ymm8
	vpdpwusd xmm14, xmm10, xmm12
	vpdpwusd ymm10, ymm12, ymm8
	vpdpwusd xmm24, xmm18, xmm19
	vpdpwusd ymm20, ymm23, ymm26
	vpdpwusd xmm21, xmm16, xmm28
	vpdpwusd ymm28, ymm18, ymm27
	vpdpwusd xmm7{k2}, xmm4, oword [0x8be]
	vpdpwusd ymm3{k4}, ymm7, ymm1
	vpdpwusd zmm2{k1}, zmm2, zmm5
	vpdpwusd xmm7{k4}{z}, xmm2, xmm5
	vpdpwusd ymm3{k6}{z}, ymm0, ymm7
	vpdpwusd zmm7{k6}{z}, zmm0, zmm5
	vpdpwusd xmm3, xmm3, oword [eax+1]
	vpdpwusd xmm7, xmm2, oword [eax+64]
	vpdpwusd ymm4, ymm6, yword [eax+1]
	vpdpwusd ymm6, ymm0, yword [eax+64]
	vpdpwusd xmm4, xmm3, oword [eax+1]
	vpdpwusd xmm4, xmm7, oword [eax+64]
	vpdpwusd ymm0, ymm5, yword [eax+1]
	vpdpwusd ymm1, ymm5, yword [eax+64]
	vpdpwusd zmm6, zmm7, zword [eax+1]
	vpdpwusd zmm2, zmm7, zword [eax+64]
	vpdpwusd xmm2, xmm6, [0x4ad]
	vpdpwusd ymm2, ymm6, [0xab2]
	vpdpwusd xmm2, xmm4, [0x10d]
	vpdpwusd ymm1, ymm3, [0xb0f]
	vpdpwusd zmm4, zmm4, [0xb8e]
