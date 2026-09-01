default rel
	vpmaddubsw xmm5, xmm6, oword [0xbd7]
	vpmaddubsw xmm5, oword [0xbd7]
	vpmaddubsw xmm5, xmm5, oword [0x73d]
	vpmaddubsw ymm1, ymm5, ymm2
	vpmaddubsw ymm1, ymm2
	vpmaddubsw ymm3, ymm6, ymm0
	vpmaddubsw xmm5, xmm1, oword [0xabb]
	vpmaddubsw xmm5, oword [0xabb]
	vpmaddubsw xmm6, xmm6, xmm6
	vpmaddubsw ymm6, ymm4, yword [0x521]
	vpmaddubsw ymm6, yword [0x521]
	vpmaddubsw ymm4, ymm4, ymm5
	vpmaddubsw xmm9, xmm13, xmm15
	vpmaddubsw ymm11, ymm8, ymm9
	vpmaddubsw xmm9, xmm11, xmm12
	vpmaddubsw ymm9, ymm9, ymm14
	vpmaddubsw xmm27, xmm17, xmm26
	vpmaddubsw ymm21, ymm18, ymm29
	vpmaddubsw xmm16, xmm24, xmm17
	vpmaddubsw ymm24, ymm28, ymm24
	vpmaddubsw xmm6{k6}, xmm1, oword [0xc36]
	vpmaddubsw ymm2{k6}, ymm7, yword [0x286]
	vpmaddubsw zmm6{k6}, zmm0, zmm5
	vpmaddubsw xmm3{k5}{z}, xmm3, xmm3
	vpmaddubsw ymm2{k7}{z}, ymm5, ymm1
	vpmaddubsw zmm1{k3}{z}, zmm2, zword [0x888]
	vpmaddubsw xmm7, xmm7, oword [eax+1]
	vpmaddubsw xmm0, xmm2, oword [eax+64]
	vpmaddubsw ymm2, ymm4, yword [eax+1]
	vpmaddubsw ymm3, ymm0, yword [eax+64]
	vpmaddubsw xmm0, xmm5, oword [eax+1]
	vpmaddubsw xmm1, xmm3, oword [eax+64]
	vpmaddubsw ymm4, ymm4, yword [eax+1]
	vpmaddubsw ymm6, ymm1, yword [eax+64]
	vpmaddubsw zmm1, zmm4, zword [eax+1]
	vpmaddubsw zmm5, zmm0, zword [eax+64]
	vpmaddubsw xmm7, xmm0, [0xb4a]
	vpmaddubsw ymm0, ymm1, [0x9ea]
	vpmaddubsw xmm6, xmm7, [0x5f6]
	vpmaddubsw ymm6, ymm6, [0x2a8]
	vpmaddubsw zmm0, zmm1, [0xa06]
