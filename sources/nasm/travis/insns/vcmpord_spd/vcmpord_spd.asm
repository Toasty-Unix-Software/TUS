default rel
	vcmpord_spd xmm5, xmm0, oword [0x1a2]
	vcmpord_spd xmm5, oword [0x1a2]
	vcmpord_spd xmm2, xmm2, xmm4
	vcmpord_spd ymm0, ymm2, ymm0
	vcmpord_spd ymm0, ymm0
	vcmpord_spd ymm6, ymm6, ymm0
	vcmpord_spd k6, xmm6, xmm3
	vcmpord_spd k6, xmm4, xmm4
	vcmpord_spd k3, ymm4, yword [0x74a]
	vcmpord_spd k1, ymm2, ymm3
	vcmpord_spd xmm10, xmm10, xmm10
	vcmpord_spd ymm9, ymm9, ymm8
	vcmpord_spd k6, xmm8, xmm11
	vcmpord_spd k4, ymm11, ymm11
	vcmpord_spd k2{k7}, xmm7, oword [0xa7c]
	vcmpord_spd k5{k4}, ymm0, ymm4
	vcmpord_spd k3{k7}, zmm2, zword [0x7e4]
	vcmpord_spd k2, zmm6, zmm5, {sae}
	vcmpord_spd xmm4, xmm1, oword [eax+1]
	vcmpord_spd xmm7, xmm4, oword [eax+64]
	vcmpord_spd ymm1, ymm3, yword [eax+1]
	vcmpord_spd ymm5, ymm0, yword [eax+64]
	vcmpord_spd k5, xmm5, oword [eax+1]
	vcmpord_spd k4, xmm3, oword [eax+64]
	vcmpord_spd k2, ymm0, yword [eax+1]
	vcmpord_spd k3, ymm0, yword [eax+64]
	vcmpord_spd k1, zmm2, zword [eax+1]
	vcmpord_spd k5, zmm7, zword [eax+64]
	vcmpord_spd xmm5, xmm5, [0xab4]
	vcmpord_spd ymm7, ymm3, [0x8b8]
	vcmpord_spd k3, xmm0, [0x181]
	vcmpord_spd k6, ymm7, [0xb75]
	vcmpord_spd k7, zmm3, [0xdd9]
