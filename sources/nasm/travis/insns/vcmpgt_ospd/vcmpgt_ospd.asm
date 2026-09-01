default rel
	vcmpgt_ospd xmm0, xmm5, xmm7
	vcmpgt_ospd xmm0, xmm7
	vcmpgt_ospd xmm6, xmm0, xmm5
	vcmpgt_ospd ymm7, ymm4, ymm0
	vcmpgt_ospd ymm7, ymm0
	vcmpgt_ospd ymm3, ymm5, ymm0
	vcmpgt_ospd k5, xmm7, xmm3
	vcmpgt_ospd k7, xmm6, oword [0xf99]
	vcmpgt_ospd k2, ymm4, yword [0x457]
	vcmpgt_ospd k3, ymm5, ymm5
	vcmpgt_ospd xmm14, xmm14, xmm9
	vcmpgt_ospd ymm14, ymm10, ymm8
	vcmpgt_ospd k4, xmm15, xmm11
	vcmpgt_ospd k4, ymm13, ymm14
	vcmpgt_ospd k5{k4}, xmm4, xmm0
	vcmpgt_ospd k4{k7}, ymm5, ymm2
	vcmpgt_ospd k3{k6}, zmm1, zmm1
	vcmpgt_ospd k6, zmm3, zmm2, {sae}
	vcmpgt_ospd xmm4, xmm7, oword [eax+1]
	vcmpgt_ospd xmm3, xmm1, oword [eax+64]
	vcmpgt_ospd ymm6, ymm2, yword [eax+1]
	vcmpgt_ospd ymm7, ymm4, yword [eax+64]
	vcmpgt_ospd k6, xmm7, oword [eax+1]
	vcmpgt_ospd k1, xmm2, oword [eax+64]
	vcmpgt_ospd k5, ymm1, yword [eax+1]
	vcmpgt_ospd k7, ymm0, yword [eax+64]
	vcmpgt_ospd k4, zmm7, zword [eax+1]
	vcmpgt_ospd k6, zmm5, zword [eax+64]
	vcmpgt_ospd xmm4, xmm6, [0xad8]
	vcmpgt_ospd ymm5, ymm7, [0x55a]
	vcmpgt_ospd k6, xmm5, [0x923]
	vcmpgt_ospd k4, ymm3, [0x856]
	vcmpgt_ospd k5, zmm5, [0xa1b]
