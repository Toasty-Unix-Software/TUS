default rel
	vpsubusw xmm5, xmm0, xmm5
	vpsubusw xmm5, xmm5
	vpsubusw xmm3, xmm2, xmm3
	vpsubusw ymm4, ymm6, ymm1
	vpsubusw ymm4, ymm1
	vpsubusw ymm1, ymm3, ymm5
	vpsubusw xmm4, xmm2, xmm7
	vpsubusw xmm4, xmm7
	vpsubusw xmm3, xmm6, xmm3
	vpsubusw ymm6, ymm0, yword [0xa24]
	vpsubusw ymm6, yword [0xa24]
	vpsubusw ymm2, ymm0, yword [0xf54]
	vpsubusw xmm12, xmm13, xmm13
	vpsubusw ymm12, ymm14, ymm11
	vpsubusw xmm9, xmm9, xmm8
	vpsubusw ymm12, ymm12, ymm13
	vpsubusw xmm21, xmm22, xmm29
	vpsubusw ymm27, ymm31, ymm29
	vpsubusw xmm24, xmm16, xmm28
	vpsubusw ymm23, ymm30, ymm30
	vpsubusw xmm6{k7}, xmm0, oword [0xfbe]
	vpsubusw ymm1{k2}, ymm7, yword [0xbc0]
	vpsubusw zmm5{k7}, zmm1, zmm1
	vpsubusw xmm3{k4}{z}, xmm5, oword [0x9ee]
	vpsubusw ymm4{k2}{z}, ymm3, yword [0x80c]
	vpsubusw zmm7{k3}{z}, zmm7, zmm2
	vpsubusw xmm2, xmm7, oword [eax+1]
	vpsubusw xmm6, xmm3, oword [eax+64]
	vpsubusw ymm5, ymm2, yword [eax+1]
	vpsubusw ymm0, ymm6, yword [eax+64]
	vpsubusw xmm4, xmm5, oword [eax+1]
	vpsubusw xmm5, xmm2, oword [eax+64]
	vpsubusw ymm5, ymm0, yword [eax+1]
	vpsubusw ymm0, ymm5, yword [eax+64]
	vpsubusw zmm1, zmm0, zword [eax+1]
	vpsubusw zmm4, zmm4, zword [eax+64]
	vpsubusw xmm6, xmm7, [0xb9d]
	vpsubusw ymm7, ymm0, [0x676]
	vpsubusw xmm0, xmm7, [0xef2]
	vpsubusw ymm2, ymm2, [0x643]
	vpsubusw zmm3, zmm4, [0xc75]
