default rel
	vcmptrue_uqpd xmm0, xmm4, oword [0x4a6]
	vcmptrue_uqpd xmm0, oword [0x4a6]
	vcmptrue_uqpd xmm0, xmm4, xmm5
	vcmptrue_uqpd ymm7, ymm4, yword [0x4f0]
	vcmptrue_uqpd ymm7, yword [0x4f0]
	vcmptrue_uqpd ymm0, ymm2, yword [0x4a5]
	vcmptrue_uqpd k3, xmm5, xmm7
	vcmptrue_uqpd k4, xmm6, oword [0x6f3]
	vcmptrue_uqpd k5, ymm0, yword [0x1bf]
	vcmptrue_uqpd k3, ymm6, ymm0
	vcmptrue_uqpd xmm9, xmm9, xmm12
	vcmptrue_uqpd ymm15, ymm9, ymm14
	vcmptrue_uqpd k5, xmm12, xmm12
	vcmptrue_uqpd k3, ymm14, ymm8
	vcmptrue_uqpd k4{k3}, xmm4, oword [0xb90]
	vcmptrue_uqpd k1{k2}, ymm4, ymm7
	vcmptrue_uqpd k6{k5}, zmm1, zmm0
	vcmptrue_uqpd k7, zmm4, zmm3, {sae}
	vcmptrue_uqpd xmm1, xmm2, oword [eax+1]
	vcmptrue_uqpd xmm0, xmm0, oword [eax+64]
	vcmptrue_uqpd ymm2, ymm0, yword [eax+1]
	vcmptrue_uqpd ymm0, ymm2, yword [eax+64]
	vcmptrue_uqpd k5, xmm5, oword [eax+1]
	vcmptrue_uqpd k4, xmm5, oword [eax+64]
	vcmptrue_uqpd k3, ymm6, yword [eax+1]
	vcmptrue_uqpd k3, ymm5, yword [eax+64]
	vcmptrue_uqpd k7, zmm4, zword [eax+1]
	vcmptrue_uqpd k7, zmm0, zword [eax+64]
	vcmptrue_uqpd xmm2, xmm2, [0xcff]
	vcmptrue_uqpd ymm4, ymm4, [0x49d]
	vcmptrue_uqpd k2, xmm1, [0xfc3]
	vcmptrue_uqpd k3, ymm1, [0x914]
	vcmptrue_uqpd k2, zmm0, [0x268]
