default rel
	vpminud xmm0, xmm0, xmm1
	vpminud xmm0, xmm1
	vpminud xmm7, xmm1, xmm3
	vpminud ymm2, ymm4, ymm3
	vpminud ymm2, ymm3
	vpminud ymm1, ymm7, ymm4
	vpminud xmm0, xmm5, oword [0x7d7]
	vpminud xmm0, oword [0x7d7]
	vpminud xmm3, xmm0, xmm3
	vpminud ymm2, ymm6, yword [0x83e]
	vpminud ymm2, yword [0x83e]
	vpminud ymm2, ymm6, ymm0
	vpminud xmm9, xmm10, xmm9
	vpminud ymm13, ymm12, ymm12
	vpminud xmm15, xmm14, xmm9
	vpminud ymm11, ymm10, ymm15
	vpminud xmm22, xmm31, xmm23
	vpminud ymm26, ymm25, ymm31
	vpminud xmm25, xmm25, xmm27
	vpminud ymm29, ymm31, ymm24
	vpminud xmm2{k5}, xmm4, xmm2
	vpminud ymm5{k4}, ymm1, ymm5
	vpminud zmm3{k1}, zmm5, zword [0xdaf]
	vpminud xmm1{k7}{z}, xmm0, oword [0x688]
	vpminud ymm3{k4}{z}, ymm4, yword [0x922]
	vpminud zmm2{k3}{z}, zmm5, zmm2
	vpminud xmm3, xmm5, oword [eax+1]
	vpminud xmm4, xmm6, oword [eax+64]
	vpminud ymm4, ymm5, yword [eax+1]
	vpminud ymm4, ymm3, yword [eax+64]
	vpminud xmm5, xmm6, oword [eax+1]
	vpminud xmm7, xmm7, oword [eax+64]
	vpminud ymm1, ymm3, yword [eax+1]
	vpminud ymm4, ymm6, yword [eax+64]
	vpminud zmm5, zmm7, zword [eax+1]
	vpminud zmm6, zmm2, zword [eax+64]
	vpminud xmm1, xmm5, [0xf8e]
	vpminud ymm5, ymm7, [0x41b]
	vpminud xmm6, xmm1, [0x676]
	vpminud ymm5, ymm6, [0x7df]
	vpminud zmm4, zmm5, [0x277]
