default rel
	vcmpeqpd xmm7, xmm0, xmm3
	vcmpeqpd xmm7, xmm3
	vcmpeqpd xmm5, xmm1, xmm1
	vcmpeqpd ymm0, ymm2, ymm2
	vcmpeqpd ymm0, ymm2
	vcmpeqpd ymm5, ymm4, yword [0x199]
	vcmpeqpd k6, xmm4, xmm6
	vcmpeqpd k6, xmm5, xmm7
	vcmpeqpd k2, ymm0, ymm1
	vcmpeqpd k2, ymm5, ymm1
	vcmpeqpd xmm15, xmm12, xmm8
	vcmpeqpd ymm10, ymm12, ymm8
	vcmpeqpd k4, xmm15, xmm15
	vcmpeqpd k1, ymm11, ymm14
	vcmpeqpd k1{k7}, xmm2, oword [0xa3b]
	vcmpeqpd k1{k5}, ymm4, ymm1
	vcmpeqpd k7{k7}, zmm1, zmm2
	vcmpeqpd k3, zmm2, zmm7, {sae}
	vcmpeqpd xmm6, xmm1, oword [eax+1]
	vcmpeqpd xmm6, xmm5, oword [eax+64]
	vcmpeqpd ymm7, ymm0, yword [eax+1]
	vcmpeqpd ymm7, ymm6, yword [eax+64]
	vcmpeqpd k3, xmm0, oword [eax+1]
	vcmpeqpd k4, xmm1, oword [eax+64]
	vcmpeqpd k2, ymm1, yword [eax+1]
	vcmpeqpd k2, ymm7, yword [eax+64]
	vcmpeqpd k7, zmm7, zword [eax+1]
	vcmpeqpd k5, zmm3, zword [eax+64]
	vcmpeqpd xmm4, xmm1, [0x5f1]
	vcmpeqpd ymm4, ymm6, [0xb3e]
	vcmpeqpd k2, xmm0, [0x8f6]
	vcmpeqpd k2, ymm3, [0xae3]
	vcmpeqpd k1, zmm4, [0xa5a]
