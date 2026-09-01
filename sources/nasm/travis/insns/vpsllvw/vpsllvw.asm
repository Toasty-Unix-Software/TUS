default rel
	vpsllvw xmm0, xmm4, xmm6
	vpsllvw xmm0, xmm6
	vpsllvw xmm3, xmm7, xmm4
	vpsllvw ymm4, ymm2, ymm1
	vpsllvw ymm4, ymm1
	vpsllvw ymm6, ymm3, ymm7
	vpsllvw zmm5, zmm1, zmm5
	vpsllvw zmm5, zmm5
	vpsllvw zmm0, zmm3, zmm6
	vpsllvw xmm13, xmm11, xmm10
	vpsllvw ymm8, ymm14, ymm12
	vpsllvw zmm8, zmm13, zmm11
	vpsllvw xmm31, xmm25, xmm24
	vpsllvw ymm22, ymm25, ymm26
	vpsllvw zmm19, zmm23, zmm24
	vpsllvw xmm6{k1}, xmm7, xmm3
	vpsllvw ymm6{k2}, ymm5, yword [0x36c]
	vpsllvw zmm0{k1}, zmm3, zword [0x99e]
	vpsllvw xmm0{k3}{z}, xmm6, xmm0
	vpsllvw ymm2{k2}{z}, ymm3, ymm3
	vpsllvw zmm1{k1}{z}, zmm0, zword [0x6a2]
	vpsllvw xmm3, xmm1, oword [eax+1]
	vpsllvw xmm3, xmm7, oword [eax+64]
	vpsllvw ymm2, ymm2, yword [eax+1]
	vpsllvw ymm4, ymm3, yword [eax+64]
	vpsllvw zmm4, zmm6, zword [eax+1]
	vpsllvw zmm5, zmm4, zword [eax+64]
	vpsllvw xmm2, xmm3, [0x218]
	vpsllvw ymm6, ymm7, [0xc34]
	vpsllvw zmm6, zmm2, [0xf7f]
