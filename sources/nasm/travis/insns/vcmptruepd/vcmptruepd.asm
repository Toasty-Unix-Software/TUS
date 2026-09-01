default rel
	vcmptruepd xmm0, xmm4, xmm4
	vcmptruepd xmm0, xmm4
	vcmptruepd xmm0, xmm7, oword [0x8b0]
	vcmptruepd ymm1, ymm7, ymm5
	vcmptruepd ymm1, ymm5
	vcmptruepd ymm5, ymm0, ymm0
	vcmptruepd k2, xmm3, xmm3
	vcmptruepd k6, xmm5, oword [0xce5]
	vcmptruepd k2, ymm1, yword [0x514]
	vcmptruepd k6, ymm3, ymm4
	vcmptruepd xmm12, xmm13, xmm14
	vcmptruepd ymm11, ymm14, ymm8
	vcmptruepd k2, xmm8, xmm15
	vcmptruepd k3, ymm15, ymm11
	vcmptruepd k2{k6}, xmm7, oword [0x8f3]
	vcmptruepd k3{k1}, ymm2, ymm0
	vcmptruepd k7{k2}, zmm2, zmm1
	vcmptruepd k6, zmm1, zmm5, {sae}
	vcmptruepd xmm3, xmm3, oword [eax+1]
	vcmptruepd xmm1, xmm2, oword [eax+64]
	vcmptruepd ymm0, ymm5, yword [eax+1]
	vcmptruepd ymm5, ymm4, yword [eax+64]
	vcmptruepd k3, xmm3, oword [eax+1]
	vcmptruepd k3, xmm5, oword [eax+64]
	vcmptruepd k2, ymm1, yword [eax+1]
	vcmptruepd k2, ymm4, yword [eax+64]
	vcmptruepd k3, zmm2, zword [eax+1]
	vcmptruepd k4, zmm1, zword [eax+64]
	vcmptruepd xmm4, xmm2, [0x432]
	vcmptruepd ymm0, ymm5, [0xb0e]
	vcmptruepd k6, xmm1, [0x170]
	vcmptruepd k3, ymm2, [0xc09]
	vcmptruepd k7, zmm6, [0x406]
