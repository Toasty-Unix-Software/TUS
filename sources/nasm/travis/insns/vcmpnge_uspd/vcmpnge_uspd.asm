default rel
	vcmpnge_uspd xmm0, xmm1, xmm1
	vcmpnge_uspd xmm0, xmm1
	vcmpnge_uspd xmm6, xmm1, xmm4
	vcmpnge_uspd ymm4, ymm7, ymm2
	vcmpnge_uspd ymm4, ymm2
	vcmpnge_uspd ymm5, ymm4, yword [0xea0]
	vcmpnge_uspd k7, xmm1, xmm1
	vcmpnge_uspd k1, xmm6, oword [0x6bb]
	vcmpnge_uspd k7, ymm7, ymm4
	vcmpnge_uspd k3, ymm5, ymm3
	vcmpnge_uspd xmm10, xmm12, xmm10
	vcmpnge_uspd ymm13, ymm12, ymm14
	vcmpnge_uspd k3, xmm11, xmm15
	vcmpnge_uspd k5, ymm9, ymm9
	vcmpnge_uspd k6{k3}, xmm6, oword [0x40e]
	vcmpnge_uspd k2{k5}, ymm1, ymm3
	vcmpnge_uspd k3{k1}, zmm5, zmm7
	vcmpnge_uspd k3, zmm5, zmm5, {sae}
	vcmpnge_uspd xmm0, xmm7, oword [eax+1]
	vcmpnge_uspd xmm6, xmm2, oword [eax+64]
	vcmpnge_uspd ymm0, ymm7, yword [eax+1]
	vcmpnge_uspd ymm3, ymm5, yword [eax+64]
	vcmpnge_uspd k3, xmm3, oword [eax+1]
	vcmpnge_uspd k4, xmm2, oword [eax+64]
	vcmpnge_uspd k7, ymm3, yword [eax+1]
	vcmpnge_uspd k2, ymm6, yword [eax+64]
	vcmpnge_uspd k3, zmm7, zword [eax+1]
	vcmpnge_uspd k2, zmm0, zword [eax+64]
	vcmpnge_uspd xmm3, xmm6, [0xa0c]
	vcmpnge_uspd ymm1, ymm0, [0x54f]
	vcmpnge_uspd k3, xmm7, [0x8f4]
	vcmpnge_uspd k7, ymm6, [0xd01]
	vcmpnge_uspd k7, zmm5, [0x6d1]
