default rel
	vcmplt_ospd xmm3, xmm6, xmm2
	vcmplt_ospd xmm3, xmm2
	vcmplt_ospd xmm1, xmm1, xmm2
	vcmplt_ospd ymm1, ymm4, yword [0xd4c]
	vcmplt_ospd ymm1, yword [0xd4c]
	vcmplt_ospd ymm5, ymm1, ymm4
	vcmplt_ospd k7, xmm3, xmm6
	vcmplt_ospd k7, xmm0, oword [0xcb1]
	vcmplt_ospd k1, ymm3, ymm1
	vcmplt_ospd k2, ymm4, yword [0x64b]
	vcmplt_ospd xmm13, xmm9, xmm15
	vcmplt_ospd ymm15, ymm9, ymm13
	vcmplt_ospd k3, xmm8, xmm14
	vcmplt_ospd k4, ymm9, ymm11
	vcmplt_ospd k5{k5}, xmm4, xmm5
	vcmplt_ospd k1{k4}, ymm2, ymm4
	vcmplt_ospd k1{k2}, zmm4, zmm6
	vcmplt_ospd k4, zmm0, zmm1, {sae}
	vcmplt_ospd xmm1, xmm7, oword [eax+1]
	vcmplt_ospd xmm6, xmm3, oword [eax+64]
	vcmplt_ospd ymm2, ymm5, yword [eax+1]
	vcmplt_ospd ymm2, ymm5, yword [eax+64]
	vcmplt_ospd k1, xmm3, oword [eax+1]
	vcmplt_ospd k5, xmm1, oword [eax+64]
	vcmplt_ospd k1, ymm5, yword [eax+1]
	vcmplt_ospd k3, ymm6, yword [eax+64]
	vcmplt_ospd k2, zmm5, zword [eax+1]
	vcmplt_ospd k4, zmm1, zword [eax+64]
	vcmplt_ospd xmm3, xmm7, [0x61e]
	vcmplt_ospd ymm5, ymm7, [0x50c]
	vcmplt_ospd k3, xmm2, [0x82a]
	vcmplt_ospd k6, ymm5, [0x439]
	vcmplt_ospd k2, zmm2, [0x10b]
