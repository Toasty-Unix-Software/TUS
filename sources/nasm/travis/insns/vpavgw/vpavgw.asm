default rel
	vpavgw xmm7, xmm7, oword [0xf85]
	vpavgw xmm7, oword [0xf85]
	vpavgw xmm7, xmm5, xmm7
	vpavgw ymm7, ymm0, ymm4
	vpavgw ymm7, ymm4
	vpavgw ymm5, ymm5, ymm4
	vpavgw xmm3, xmm6, xmm0
	vpavgw xmm3, xmm0
	vpavgw xmm7, xmm7, xmm2
	vpavgw ymm1, ymm6, ymm3
	vpavgw ymm1, ymm3
	vpavgw ymm2, ymm5, ymm7
	vpavgw xmm15, xmm9, xmm9
	vpavgw ymm10, ymm12, ymm14
	vpavgw xmm9, xmm15, xmm10
	vpavgw ymm9, ymm12, ymm9
	vpavgw xmm25, xmm28, xmm31
	vpavgw ymm23, ymm22, ymm22
	vpavgw xmm21, xmm21, xmm18
	vpavgw ymm20, ymm24, ymm27
	vpavgw xmm0{k1}, xmm5, xmm4
	vpavgw ymm0{k1}, ymm4, yword [0xdb2]
	vpavgw zmm0{k5}, zmm3, zword [0x2bf]
	vpavgw xmm3{k1}{z}, xmm6, oword [0xf5d]
	vpavgw ymm0{k4}{z}, ymm2, ymm7
	vpavgw zmm7{k5}{z}, zmm1, zword [0x312]
	vpavgw xmm1, xmm4, oword [eax+1]
	vpavgw xmm1, xmm4, oword [eax+64]
	vpavgw ymm3, ymm6, yword [eax+1]
	vpavgw ymm7, ymm0, yword [eax+64]
	vpavgw xmm5, xmm2, oword [eax+1]
	vpavgw xmm2, xmm2, oword [eax+64]
	vpavgw ymm0, ymm1, yword [eax+1]
	vpavgw ymm0, ymm2, yword [eax+64]
	vpavgw zmm2, zmm1, zword [eax+1]
	vpavgw zmm3, zmm3, zword [eax+64]
	vpavgw xmm2, xmm5, [0xb99]
	vpavgw ymm3, ymm1, [0x5ec]
	vpavgw xmm2, xmm3, [0xcb1]
	vpavgw ymm6, ymm7, [0x7e8]
	vpavgw zmm0, zmm4, [0xf51]
