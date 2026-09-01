default rel
	vcmpeq_oqpd k3, xmm5, xmm6
	vcmpeq_oqpd k5, xmm2, xmm0
	vcmpeq_oqpd k1, ymm2, ymm0
	vcmpeq_oqpd k6, ymm4, ymm7
	vcmpeq_oqpd k1, zmm5, zmm4
	vcmpeq_oqpd k4, zmm0, zmm4
	vcmpeq_oqpd k5, xmm11, xmm10
	vcmpeq_oqpd k1, ymm12, ymm12
	vcmpeq_oqpd k6, zmm13, zmm9
	vcmpeq_oqpd k4, xmm20, xmm19
	vcmpeq_oqpd k6, ymm18, ymm16
	vcmpeq_oqpd k1, zmm22, zmm16
	vcmpeq_oqpd k3{k6}, xmm1, oword [0x9af]
	vcmpeq_oqpd k2{k5}, ymm5, yword [0x1b9]
	vcmpeq_oqpd k6{k1}, zmm5, zword [0xcdd]
	vcmpeq_oqpd k7, zmm6, zmm7, {sae}
	vcmpeq_oqpd k7, xmm5, oword [eax+1]
	vcmpeq_oqpd k4, xmm7, oword [eax+64]
	vcmpeq_oqpd k6, ymm3, yword [eax+1]
	vcmpeq_oqpd k5, ymm4, yword [eax+64]
	vcmpeq_oqpd k6, zmm5, zword [eax+1]
	vcmpeq_oqpd k5, zmm4, zword [eax+64]
	vcmpeq_oqpd k5, xmm3, [0x79e]
	vcmpeq_oqpd k1, ymm3, [0x4c6]
	vcmpeq_oqpd k3, zmm4, [0x282]
