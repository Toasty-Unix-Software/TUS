default rel
	vgetexpps xmm0, xmm4
	vgetexpps xmm3, xmm1
	vgetexpps ymm3, yword [0xd51]
	vgetexpps ymm3, ymm5
	vgetexpps zmm5, zword [0xf49]
	vgetexpps zmm0, zmm6
	vgetexpps xmm12, xmm13
	vgetexpps ymm15, ymm13
	vgetexpps zmm11, zmm13
	vgetexpps xmm21, xmm24
	vgetexpps ymm28, ymm29
	vgetexpps zmm18, zmm28
	vgetexpps xmm3{k5}, oword [0xb64]
	vgetexpps ymm3{k1}, ymm7
	vgetexpps zmm5{k3}, zmm2
	vgetexpps xmm1{k7}{z}, oword [0xe6b]
	vgetexpps ymm2{k5}{z}, ymm1
	vgetexpps zmm7{k1}{z}, zword [0x370]
	vgetexpps zmm3, zmm5, {sae}
	vgetexpps xmm1, oword [eax+1]
	vgetexpps xmm4, oword [eax+64]
	vgetexpps ymm3, yword [eax+1]
	vgetexpps ymm5, yword [eax+64]
	vgetexpps zmm3, zword [eax+1]
	vgetexpps zmm6, zword [eax+64]
	vgetexpps xmm7, [0xc12]
	vgetexpps ymm7, [0x3e2]
	vgetexpps zmm5, [0xf59]
