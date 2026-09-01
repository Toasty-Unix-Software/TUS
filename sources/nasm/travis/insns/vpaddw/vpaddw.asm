default rel
	vpaddw xmm5, xmm1, xmm5
	vpaddw xmm5, xmm5
	vpaddw xmm4, xmm4, oword [0xe56]
	vpaddw ymm0, ymm0, ymm5
	vpaddw ymm0, ymm5
	vpaddw ymm4, ymm0, ymm0
	vpaddw xmm5, xmm4, oword [0x9ca]
	vpaddw xmm5, oword [0x9ca]
	vpaddw xmm5, xmm7, xmm7
	vpaddw ymm3, ymm0, ymm4
	vpaddw ymm3, ymm4
	vpaddw ymm1, ymm7, yword [0x3b2]
	vpaddw xmm11, xmm15, xmm8
	vpaddw ymm14, ymm13, ymm15
	vpaddw xmm14, xmm10, xmm11
	vpaddw ymm10, ymm15, ymm13
	vpaddw xmm23, xmm16, xmm18
	vpaddw ymm22, ymm31, ymm16
	vpaddw xmm30, xmm23, xmm17
	vpaddw ymm31, ymm17, ymm18
	vpaddw xmm3{k7}, xmm6, xmm6
	vpaddw ymm5{k3}, ymm4, ymm4
	vpaddw zmm7{k1}, zmm6, zmm1
	vpaddw xmm6{k3}{z}, xmm5, oword [0x98c]
	vpaddw ymm3{k6}{z}, ymm5, yword [0x688]
	vpaddw zmm3{k2}{z}, zmm4, zmm6
	vpaddw xmm7, xmm0, oword [eax+1]
	vpaddw xmm5, xmm0, oword [eax+64]
	vpaddw ymm7, ymm7, yword [eax+1]
	vpaddw ymm3, ymm6, yword [eax+64]
	vpaddw xmm3, xmm3, oword [eax+1]
	vpaddw xmm7, xmm5, oword [eax+64]
	vpaddw ymm2, ymm2, yword [eax+1]
	vpaddw ymm4, ymm4, yword [eax+64]
	vpaddw zmm0, zmm6, zword [eax+1]
	vpaddw zmm3, zmm3, zword [eax+64]
	vpaddw xmm5, xmm4, [0x974]
	vpaddw ymm5, ymm1, [0x218]
	vpaddw xmm2, xmm6, [0x234]
	vpaddw ymm1, ymm3, [0x25e]
	vpaddw zmm0, zmm0, [0xd7e]
