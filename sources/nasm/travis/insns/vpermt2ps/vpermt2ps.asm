default rel
	vpermt2ps xmm2, xmm5, xmm6
	vpermt2ps xmm6, xmm6, xmm2
	vpermt2ps ymm1, ymm3, ymm0
	vpermt2ps ymm2, ymm0, ymm1
	vpermt2ps zmm5, zmm3, zword [0x165]
	vpermt2ps zmm7, zmm6, zmm4
	vpermt2ps xmm10, xmm11, xmm12
	vpermt2ps ymm11, ymm9, ymm12
	vpermt2ps zmm11, zmm14, zmm10
	vpermt2ps xmm22, xmm27, xmm30
	vpermt2ps ymm28, ymm28, ymm22
	vpermt2ps zmm31, zmm26, zmm25
	vpermt2ps xmm6{k2}, xmm7, xmm0
	vpermt2ps ymm0{k2}, ymm3, ymm3
	vpermt2ps zmm0{k2}, zmm2, zword [0x9b9]
	vpermt2ps xmm6{k1}{z}, xmm1, xmm4
	vpermt2ps ymm1{k6}{z}, ymm2, ymm4
	vpermt2ps zmm3{k5}{z}, zmm5, zmm2
	vpermt2ps xmm1, xmm6, oword [eax+1]
	vpermt2ps xmm3, xmm1, oword [eax+64]
	vpermt2ps ymm5, ymm2, yword [eax+1]
	vpermt2ps ymm6, ymm7, yword [eax+64]
	vpermt2ps zmm7, zmm5, zword [eax+1]
	vpermt2ps zmm3, zmm3, zword [eax+64]
	vpermt2ps xmm4, xmm1, [0x5b6]
	vpermt2ps ymm3, ymm6, [0xeac]
	vpermt2ps zmm5, zmm2, [0xd00]
