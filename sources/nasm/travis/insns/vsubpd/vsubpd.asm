default rel
	vsubpd xmm0, xmm2, oword [0x89d]
	vsubpd xmm0, oword [0x89d]
	vsubpd xmm2, xmm5, xmm3
	vsubpd ymm2, ymm7, ymm3
	vsubpd ymm2, ymm3
	vsubpd ymm3, ymm6, yword [0xa51]
	vsubpd xmm0, xmm3, xmm1
	vsubpd xmm0, xmm1
	vsubpd xmm3, xmm0, xmm0
	vsubpd ymm5, ymm2, yword [0xb81]
	vsubpd ymm5, yword [0xb81]
	vsubpd ymm6, ymm5, ymm0
	vsubpd xmm10, xmm12, xmm12
	vsubpd ymm12, ymm11, ymm14
	vsubpd xmm12, xmm10, xmm9
	vsubpd ymm8, ymm15, ymm15
	vsubpd xmm24, xmm30, xmm30
	vsubpd ymm16, ymm23, ymm21
	vsubpd xmm23, xmm16, xmm25
	vsubpd ymm31, ymm19, ymm18
	vsubpd xmm2{k4}, xmm3, oword [0x6b9]
	vsubpd ymm4{k2}, ymm0, ymm2
	vsubpd zmm3{k2}, zmm1, zword [0x5a2]
	vsubpd xmm3{k5}{z}, xmm7, oword [0x22e]
	vsubpd ymm3{k3}{z}, ymm4, ymm7
	vsubpd zmm0{k1}{z}, zmm1, zmm0
	vsubpd zmm0, zmm3, zmm1, {rd-sae}
	vsubpd xmm3, xmm0, oword [eax+1]
	vsubpd xmm7, xmm2, oword [eax+64]
	vsubpd ymm7, ymm3, yword [eax+1]
	vsubpd ymm6, ymm4, yword [eax+64]
	vsubpd xmm4, xmm7, oword [eax+1]
	vsubpd xmm4, xmm4, oword [eax+64]
	vsubpd ymm3, ymm7, yword [eax+1]
	vsubpd ymm5, ymm0, yword [eax+64]
	vsubpd zmm0, zmm0, zword [eax+1]
	vsubpd zmm0, zmm5, zword [eax+64]
	vsubpd xmm6, xmm1, [0x21c]
	vsubpd ymm0, ymm0, [0xdde]
	vsubpd xmm5, xmm5, [0xd42]
	vsubpd ymm5, ymm3, [0x9b7]
	vsubpd zmm5, zmm2, [0xbf7]
