default rel
	vcmple_oqpd xmm5, xmm6, xmm0
	vcmple_oqpd xmm5, xmm0
	vcmple_oqpd xmm4, xmm0, oword [0x8c8]
	vcmple_oqpd ymm6, ymm1, ymm5
	vcmple_oqpd ymm6, ymm5
	vcmple_oqpd ymm2, ymm2, yword [0x76a]
	vcmple_oqpd k7, xmm3, xmm4
	vcmple_oqpd k7, xmm2, oword [0x83b]
	vcmple_oqpd k6, ymm5, yword [0x6d9]
	vcmple_oqpd k7, ymm6, yword [0x92e]
	vcmple_oqpd xmm12, xmm15, xmm12
	vcmple_oqpd ymm10, ymm13, ymm10
	vcmple_oqpd k6, xmm10, xmm8
	vcmple_oqpd k7, ymm14, ymm9
	vcmple_oqpd k3{k3}, xmm3, xmm3
	vcmple_oqpd k4{k4}, ymm6, ymm5
	vcmple_oqpd k1{k4}, zmm4, zword [0xca9]
	vcmple_oqpd k7, zmm5, zmm2, {sae}
	vcmple_oqpd xmm4, xmm2, oword [eax+1]
	vcmple_oqpd xmm7, xmm2, oword [eax+64]
	vcmple_oqpd ymm7, ymm4, yword [eax+1]
	vcmple_oqpd ymm6, ymm6, yword [eax+64]
	vcmple_oqpd k3, xmm6, oword [eax+1]
	vcmple_oqpd k5, xmm1, oword [eax+64]
	vcmple_oqpd k4, ymm2, yword [eax+1]
	vcmple_oqpd k1, ymm6, yword [eax+64]
	vcmple_oqpd k2, zmm4, zword [eax+1]
	vcmple_oqpd k2, zmm4, zword [eax+64]
	vcmple_oqpd xmm6, xmm2, [0x42b]
	vcmple_oqpd ymm0, ymm4, [0x29c]
	vcmple_oqpd k6, xmm1, [0xec9]
	vcmple_oqpd k1, ymm0, [0x86d]
	vcmple_oqpd k7, zmm4, [0x7df]
