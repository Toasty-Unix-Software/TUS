default rel
	vcmpngt_uspd xmm3, xmm4, oword [0xf28]
	vcmpngt_uspd xmm3, oword [0xf28]
	vcmpngt_uspd xmm0, xmm4, xmm2
	vcmpngt_uspd ymm2, ymm4, ymm5
	vcmpngt_uspd ymm2, ymm5
	vcmpngt_uspd ymm4, ymm5, yword [0xc0b]
	vcmpngt_uspd k5, xmm2, xmm5
	vcmpngt_uspd k5, xmm1, xmm0
	vcmpngt_uspd k4, ymm5, ymm3
	vcmpngt_uspd k1, ymm1, yword [0xad9]
	vcmpngt_uspd xmm12, xmm13, xmm13
	vcmpngt_uspd ymm8, ymm11, ymm10
	vcmpngt_uspd k5, xmm12, xmm11
	vcmpngt_uspd k1, ymm9, ymm8
	vcmpngt_uspd k7{k7}, xmm5, xmm7
	vcmpngt_uspd k3{k6}, ymm2, yword [0x7b4]
	vcmpngt_uspd k6{k3}, zmm7, zword [0x9e1]
	vcmpngt_uspd k4, zmm6, zmm6, {sae}
	vcmpngt_uspd xmm0, xmm5, oword [eax+1]
	vcmpngt_uspd xmm0, xmm6, oword [eax+64]
	vcmpngt_uspd ymm2, ymm1, yword [eax+1]
	vcmpngt_uspd ymm3, ymm7, yword [eax+64]
	vcmpngt_uspd k3, xmm4, oword [eax+1]
	vcmpngt_uspd k1, xmm2, oword [eax+64]
	vcmpngt_uspd k5, ymm2, yword [eax+1]
	vcmpngt_uspd k5, ymm6, yword [eax+64]
	vcmpngt_uspd k5, zmm6, zword [eax+1]
	vcmpngt_uspd k5, zmm6, zword [eax+64]
	vcmpngt_uspd xmm1, xmm7, [0xbd6]
	vcmpngt_uspd ymm7, ymm3, [0x4bf]
	vcmpngt_uspd k3, xmm1, [0x5ae]
	vcmpngt_uspd k4, ymm2, [0x5b3]
	vcmpngt_uspd k7, zmm0, [0x24b]
