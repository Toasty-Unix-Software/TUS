default rel
	vpexpandw xmm2, xmm3
	vpexpandw xmm1, oword [0x632]
	vpexpandw ymm3, ymm2
	vpexpandw ymm5, ymm0
	vpexpandw zmm6, zword [0xf22]
	vpexpandw zmm3, zmm1
	vpexpandw xmm9, xmm13
	vpexpandw ymm15, ymm8
	vpexpandw zmm14, zmm15
	vpexpandw xmm25, xmm29
	vpexpandw ymm24, ymm25
	vpexpandw zmm28, zmm27
	vpexpandw xmm0{k3}, oword [0xc77]
	vpexpandw ymm1{k4}, ymm6
	vpexpandw zmm6{k5}, zmm3
	vpexpandw xmm6{k4}{z}, oword [0x99e]
	vpexpandw ymm7{k4}{z}, ymm6
	vpexpandw zmm2{k1}{z}, zmm7
	vpexpandw xmm1, oword [eax+1]
	vpexpandw xmm7, oword [eax+64]
	vpexpandw ymm6, yword [eax+1]
	vpexpandw ymm7, yword [eax+64]
	vpexpandw zmm0, zword [eax+1]
	vpexpandw zmm6, zword [eax+64]
	vpexpandw xmm4, [0x2fd]
	vpexpandw ymm1, [0x67a]
	vpexpandw zmm0, [0x239]
