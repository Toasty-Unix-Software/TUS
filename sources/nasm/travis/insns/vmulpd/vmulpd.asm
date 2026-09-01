default rel
	vmulpd xmm3, xmm3, xmm6
	vmulpd xmm3, xmm6
	vmulpd xmm2, xmm3, xmm1
	vmulpd ymm5, ymm7, yword [0x8aa]
	vmulpd ymm5, yword [0x8aa]
	vmulpd ymm1, ymm6, ymm1
	vmulpd xmm5, xmm7, oword [0xf73]
	vmulpd xmm5, oword [0xf73]
	vmulpd xmm7, xmm3, oword [0xc19]
	vmulpd ymm0, ymm0, ymm6
	vmulpd ymm0, ymm6
	vmulpd ymm2, ymm6, ymm5
	vmulpd xmm10, xmm14, xmm8
	vmulpd ymm10, ymm9, ymm14
	vmulpd xmm14, xmm8, xmm15
	vmulpd ymm14, ymm8, ymm10
	vmulpd xmm25, xmm18, xmm26
	vmulpd ymm23, ymm20, ymm23
	vmulpd xmm21, xmm31, xmm22
	vmulpd ymm21, ymm28, ymm22
	vmulpd xmm3{k3}, xmm7, xmm6
	vmulpd ymm5{k7}, ymm2, ymm1
	vmulpd zmm7{k2}, zmm5, zmm5
	vmulpd xmm7{k1}{z}, xmm4, oword [0x9f2]
	vmulpd ymm1{k1}{z}, ymm0, ymm0
	vmulpd zmm6{k2}{z}, zmm7, zmm6
	vmulpd zmm1, zmm1, zmm2, {rz-sae}
	vmulpd xmm1, xmm5, oword [eax+1]
	vmulpd xmm6, xmm6, oword [eax+64]
	vmulpd ymm2, ymm1, yword [eax+1]
	vmulpd ymm3, ymm5, yword [eax+64]
	vmulpd xmm2, xmm5, oword [eax+1]
	vmulpd xmm3, xmm7, oword [eax+64]
	vmulpd ymm2, ymm5, yword [eax+1]
	vmulpd ymm6, ymm5, yword [eax+64]
	vmulpd zmm2, zmm3, zword [eax+1]
	vmulpd zmm6, zmm7, zword [eax+64]
	vmulpd xmm4, xmm3, [0x9af]
	vmulpd ymm0, ymm1, [0x8ce]
	vmulpd xmm0, xmm1, [0xa75]
	vmulpd ymm5, ymm7, [0x79d]
	vmulpd zmm1, zmm6, [0xcb9]
