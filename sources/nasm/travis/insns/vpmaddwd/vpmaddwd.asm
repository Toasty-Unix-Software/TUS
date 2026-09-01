default rel
	vpmaddwd xmm0, xmm1, xmm7
	vpmaddwd xmm0, xmm7
	vpmaddwd xmm6, xmm5, xmm7
	vpmaddwd ymm4, ymm6, ymm7
	vpmaddwd ymm4, ymm7
	vpmaddwd ymm5, ymm5, ymm6
	vpmaddwd xmm7, xmm6, xmm0
	vpmaddwd xmm7, xmm0
	vpmaddwd xmm1, xmm1, oword [0xc6e]
	vpmaddwd ymm0, ymm2, yword [0xc2b]
	vpmaddwd ymm0, yword [0xc2b]
	vpmaddwd ymm6, ymm6, ymm4
	vpmaddwd xmm9, xmm9, xmm13
	vpmaddwd ymm9, ymm10, ymm8
	vpmaddwd xmm14, xmm12, xmm10
	vpmaddwd ymm15, ymm13, ymm12
	vpmaddwd xmm21, xmm28, xmm23
	vpmaddwd ymm28, ymm21, ymm20
	vpmaddwd xmm16, xmm27, xmm22
	vpmaddwd ymm31, ymm29, ymm29
	vpmaddwd xmm3{k1}, xmm3, oword [0x4c3]
	vpmaddwd ymm3{k2}, ymm5, ymm6
	vpmaddwd zmm2{k3}, zmm4, zword [0xb57]
	vpmaddwd xmm1{k4}{z}, xmm3, oword [0x692]
	vpmaddwd ymm2{k2}{z}, ymm3, ymm0
	vpmaddwd zmm7{k6}{z}, zmm7, zword [0xc9a]
	vpmaddwd xmm0, xmm4, oword [eax+1]
	vpmaddwd xmm3, xmm0, oword [eax+64]
	vpmaddwd ymm6, ymm3, yword [eax+1]
	vpmaddwd ymm2, ymm6, yword [eax+64]
	vpmaddwd xmm6, xmm7, oword [eax+1]
	vpmaddwd xmm2, xmm5, oword [eax+64]
	vpmaddwd ymm2, ymm1, yword [eax+1]
	vpmaddwd ymm3, ymm2, yword [eax+64]
	vpmaddwd zmm6, zmm4, zword [eax+1]
	vpmaddwd zmm0, zmm6, zword [eax+64]
	vpmaddwd xmm4, xmm4, [0x42f]
	vpmaddwd ymm7, ymm1, [0xef2]
	vpmaddwd xmm5, xmm4, [0xaa2]
	vpmaddwd ymm2, ymm0, [0xbe5]
	vpmaddwd zmm6, zmm3, [0x66c]
