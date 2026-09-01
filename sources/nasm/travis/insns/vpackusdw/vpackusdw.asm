default rel
	vpackusdw xmm1, xmm3, xmm2
	vpackusdw xmm1, xmm2
	vpackusdw xmm5, xmm3, xmm0
	vpackusdw ymm0, ymm2, yword [0x573]
	vpackusdw ymm0, yword [0x573]
	vpackusdw ymm6, ymm4, ymm7
	vpackusdw xmm4, xmm1, xmm5
	vpackusdw xmm4, xmm5
	vpackusdw xmm0, xmm2, oword [0xd58]
	vpackusdw ymm7, ymm5, ymm2
	vpackusdw ymm7, ymm2
	vpackusdw ymm3, ymm4, ymm4
	vpackusdw xmm15, xmm15, xmm9
	vpackusdw ymm13, ymm11, ymm13
	vpackusdw xmm15, xmm13, xmm14
	vpackusdw ymm10, ymm15, ymm9
	vpackusdw xmm23, xmm28, xmm21
	vpackusdw ymm28, ymm16, ymm27
	vpackusdw xmm29, xmm26, xmm25
	vpackusdw ymm26, ymm23, ymm27
	vpackusdw xmm6{k5}, xmm4, xmm2
	vpackusdw ymm5{k1}, ymm5, ymm6
	vpackusdw zmm5{k1}, zmm1, zmm4
	vpackusdw xmm3{k6}{z}, xmm4, xmm1
	vpackusdw ymm6{k1}{z}, ymm3, ymm6
	vpackusdw zmm5{k7}{z}, zmm2, zmm5
	vpackusdw xmm5, xmm2, oword [eax+1]
	vpackusdw xmm3, xmm1, oword [eax+64]
	vpackusdw ymm2, ymm7, yword [eax+1]
	vpackusdw ymm7, ymm0, yword [eax+64]
	vpackusdw xmm7, xmm4, oword [eax+1]
	vpackusdw xmm4, xmm7, oword [eax+64]
	vpackusdw ymm4, ymm2, yword [eax+1]
	vpackusdw ymm5, ymm3, yword [eax+64]
	vpackusdw zmm5, zmm2, zword [eax+1]
	vpackusdw zmm4, zmm1, zword [eax+64]
	vpackusdw xmm2, xmm1, [0xace]
	vpackusdw ymm2, ymm0, [0xe3e]
	vpackusdw xmm4, xmm1, [0xc1d]
	vpackusdw ymm2, ymm4, [0x8f0]
	vpackusdw zmm4, zmm3, [0x37f]
