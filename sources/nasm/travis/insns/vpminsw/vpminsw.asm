default rel
	vpminsw xmm6, xmm1, xmm4
	vpminsw xmm6, xmm4
	vpminsw xmm2, xmm1, oword [0x4ca]
	vpminsw ymm1, ymm5, ymm1
	vpminsw ymm1, ymm1
	vpminsw ymm4, ymm4, ymm7
	vpminsw xmm0, xmm4, xmm0
	vpminsw xmm0, xmm0
	vpminsw xmm7, xmm4, xmm7
	vpminsw ymm3, ymm4, yword [0x571]
	vpminsw ymm3, yword [0x571]
	vpminsw ymm1, ymm5, yword [0xf04]
	vpminsw xmm13, xmm11, xmm12
	vpminsw ymm15, ymm15, ymm15
	vpminsw xmm11, xmm8, xmm13
	vpminsw ymm15, ymm11, ymm14
	vpminsw xmm27, xmm20, xmm24
	vpminsw ymm30, ymm28, ymm28
	vpminsw xmm27, xmm21, xmm23
	vpminsw ymm21, ymm17, ymm19
	vpminsw xmm1{k5}, xmm7, oword [0x4ad]
	vpminsw ymm4{k3}, ymm2, ymm6
	vpminsw zmm3{k7}, zmm3, zmm4
	vpminsw xmm2{k7}{z}, xmm6, xmm4
	vpminsw ymm6{k6}{z}, ymm4, yword [0xe42]
	vpminsw zmm4{k4}{z}, zmm0, zword [0xb56]
	vpminsw xmm6, xmm0, oword [eax+1]
	vpminsw xmm6, xmm7, oword [eax+64]
	vpminsw ymm2, ymm6, yword [eax+1]
	vpminsw ymm6, ymm5, yword [eax+64]
	vpminsw xmm4, xmm5, oword [eax+1]
	vpminsw xmm3, xmm7, oword [eax+64]
	vpminsw ymm7, ymm6, yword [eax+1]
	vpminsw ymm1, ymm7, yword [eax+64]
	vpminsw zmm5, zmm7, zword [eax+1]
	vpminsw zmm0, zmm6, zword [eax+64]
	vpminsw xmm7, xmm4, [0xd8f]
	vpminsw ymm6, ymm1, [0x2df]
	vpminsw xmm6, xmm0, [0xca1]
	vpminsw ymm2, ymm3, [0x4db]
	vpminsw zmm1, zmm7, [0x5a4]
