default rel
	vcmpngepd xmm6, xmm7, oword [0x707]
	vcmpngepd xmm6, oword [0x707]
	vcmpngepd xmm5, xmm3, xmm6
	vcmpngepd ymm2, ymm3, ymm5
	vcmpngepd ymm2, ymm5
	vcmpngepd ymm1, ymm2, ymm2
	vcmpngepd k3, xmm1, oword [0x1ab]
	vcmpngepd k1, xmm2, xmm0
	vcmpngepd k2, ymm1, ymm4
	vcmpngepd k3, ymm4, ymm2
	vcmpngepd xmm12, xmm12, xmm11
	vcmpngepd ymm12, ymm10, ymm12
	vcmpngepd k6, xmm14, xmm15
	vcmpngepd k1, ymm10, ymm8
	vcmpngepd k6{k5}, xmm2, xmm5
	vcmpngepd k4{k1}, ymm0, ymm2
	vcmpngepd k1{k7}, zmm3, zword [0x744]
	vcmpngepd k4, zmm7, zmm6, {sae}
	vcmpngepd xmm5, xmm1, oword [eax+1]
	vcmpngepd xmm4, xmm2, oword [eax+64]
	vcmpngepd ymm4, ymm6, yword [eax+1]
	vcmpngepd ymm0, ymm7, yword [eax+64]
	vcmpngepd k1, xmm4, oword [eax+1]
	vcmpngepd k3, xmm3, oword [eax+64]
	vcmpngepd k2, ymm7, yword [eax+1]
	vcmpngepd k6, ymm6, yword [eax+64]
	vcmpngepd k3, zmm6, zword [eax+1]
	vcmpngepd k3, zmm4, zword [eax+64]
	vcmpngepd xmm2, xmm0, [0x78c]
	vcmpngepd ymm1, ymm6, [0xc52]
	vcmpngepd k4, xmm0, [0x756]
	vcmpngepd k7, ymm4, [0x236]
	vcmpngepd k3, zmm7, [0xd92]
