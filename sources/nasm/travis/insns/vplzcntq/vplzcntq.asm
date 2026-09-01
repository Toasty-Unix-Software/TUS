default rel
	vplzcntq xmm1, xmm0
	vplzcntq xmm2, oword [0x2d3]
	vplzcntq ymm0, yword [0xa12]
	vplzcntq ymm6, ymm7
	vplzcntq zmm0, zmm2
	vplzcntq zmm7, zmm5
	vplzcntq xmm14, xmm11
	vplzcntq ymm11, ymm15
	vplzcntq zmm14, zmm15
	vplzcntq xmm25, xmm17
	vplzcntq ymm23, ymm30
	vplzcntq zmm17, zmm24
	vplzcntq xmm4{k3}, xmm2
	vplzcntq ymm3{k3}, yword [0x8b0]
	vplzcntq zmm4{k2}, zword [0x436]
	vplzcntq xmm7{k3}{z}, xmm4
	vplzcntq ymm7{k1}{z}, yword [0xa31]
	vplzcntq zmm0{k6}{z}, zword [0x1cb]
	vplzcntq xmm2, oword [eax+1]
	vplzcntq xmm0, oword [eax+64]
	vplzcntq ymm3, yword [eax+1]
	vplzcntq ymm6, yword [eax+64]
	vplzcntq zmm0, zword [eax+1]
	vplzcntq zmm1, zword [eax+64]
	vplzcntq xmm2, [0x924]
	vplzcntq ymm7, [0x81e]
	vplzcntq zmm5, [0x268]
