default rel
	vcmpeq_uqpd xmm3, xmm7, xmm2
	vcmpeq_uqpd xmm3, xmm2
	vcmpeq_uqpd xmm6, xmm3, xmm6
	vcmpeq_uqpd ymm6, ymm1, ymm4
	vcmpeq_uqpd ymm6, ymm4
	vcmpeq_uqpd ymm7, ymm3, yword [0xb81]
	vcmpeq_uqpd k5, xmm5, oword [0xc33]
	vcmpeq_uqpd k5, xmm4, xmm1
	vcmpeq_uqpd k6, ymm6, ymm7
	vcmpeq_uqpd k3, ymm3, ymm3
	vcmpeq_uqpd xmm11, xmm8, xmm10
	vcmpeq_uqpd ymm10, ymm11, ymm8
	vcmpeq_uqpd k7, xmm8, xmm12
	vcmpeq_uqpd k2, ymm11, ymm8
	vcmpeq_uqpd k2{k7}, xmm4, oword [0x342]
	vcmpeq_uqpd k1{k2}, ymm5, ymm5
	vcmpeq_uqpd k6{k4}, zmm2, zmm0
	vcmpeq_uqpd k5, zmm4, zmm2, {sae}
	vcmpeq_uqpd xmm6, xmm7, oword [eax+1]
	vcmpeq_uqpd xmm3, xmm5, oword [eax+64]
	vcmpeq_uqpd ymm1, ymm0, yword [eax+1]
	vcmpeq_uqpd ymm2, ymm2, yword [eax+64]
	vcmpeq_uqpd k2, xmm6, oword [eax+1]
	vcmpeq_uqpd k5, xmm2, oword [eax+64]
	vcmpeq_uqpd k7, ymm5, yword [eax+1]
	vcmpeq_uqpd k4, ymm0, yword [eax+64]
	vcmpeq_uqpd k4, zmm7, zword [eax+1]
	vcmpeq_uqpd k4, zmm4, zword [eax+64]
	vcmpeq_uqpd xmm2, xmm4, [0xe48]
	vcmpeq_uqpd ymm5, ymm3, [0xc84]
	vcmpeq_uqpd k7, xmm0, [0x3af]
	vcmpeq_uqpd k3, ymm1, [0xf98]
	vcmpeq_uqpd k1, zmm2, [0x290]
