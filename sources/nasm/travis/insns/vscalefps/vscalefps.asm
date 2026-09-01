default rel
	vscalefps xmm2, xmm7, xmm1
	vscalefps xmm2, xmm1
	vscalefps xmm6, xmm2, oword [0xf66]
	vscalefps ymm3, ymm4, yword [0xe65]
	vscalefps ymm3, yword [0xe65]
	vscalefps ymm6, ymm0, ymm6
	vscalefps zmm5, zmm6, zword [0x959]
	vscalefps zmm5, zword [0x959]
	vscalefps zmm6, zmm4, zmm6
	vscalefps xmm8, xmm12, xmm8
	vscalefps ymm11, ymm13, ymm11
	vscalefps zmm15, zmm14, zmm14
	vscalefps xmm25, xmm18, xmm20
	vscalefps ymm21, ymm29, ymm27
	vscalefps zmm27, zmm16, zmm29
	vscalefps xmm7{k4}, xmm7, xmm0
	vscalefps ymm4{k2}, ymm3, ymm3
	vscalefps zmm3{k4}, zmm5, zmm2
	vscalefps xmm1{k2}{z}, xmm2, xmm7
	vscalefps ymm3{k6}{z}, ymm4, ymm4
	vscalefps zmm6{k6}{z}, zmm2, zmm4
	vscalefps zmm1, zmm6, zmm4, {ru-sae}
	vscalefps xmm6, xmm6, oword [eax+1]
	vscalefps xmm7, xmm6, oword [eax+64]
	vscalefps ymm2, ymm1, yword [eax+1]
	vscalefps ymm3, ymm1, yword [eax+64]
	vscalefps zmm3, zmm6, zword [eax+1]
	vscalefps zmm3, zmm2, zword [eax+64]
	vscalefps xmm7, xmm3, [0x7fa]
	vscalefps ymm0, ymm4, [0x18c]
	vscalefps zmm4, zmm7, [0x8a1]
