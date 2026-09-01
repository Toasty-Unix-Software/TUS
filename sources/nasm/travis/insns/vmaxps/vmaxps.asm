default rel
	vmaxps xmm1, xmm0, xmm1
	vmaxps xmm1, xmm1
	vmaxps xmm1, xmm1, xmm0
	vmaxps ymm5, ymm2, ymm0
	vmaxps ymm5, ymm0
	vmaxps ymm3, ymm5, ymm3
	vmaxps xmm0, xmm5, oword [0x5a2]
	vmaxps xmm0, oword [0x5a2]
	vmaxps xmm0, xmm3, xmm3
	vmaxps ymm6, ymm0, ymm3
	vmaxps ymm6, ymm3
	vmaxps ymm2, ymm3, ymm0
	vmaxps xmm11, xmm14, xmm9
	vmaxps ymm11, ymm8, ymm8
	vmaxps xmm14, xmm12, xmm13
	vmaxps ymm8, ymm12, ymm13
	vmaxps xmm23, xmm17, xmm27
	vmaxps ymm30, ymm20, ymm30
	vmaxps xmm20, xmm26, xmm23
	vmaxps ymm16, ymm29, ymm16
	vmaxps xmm0{k6}, xmm1, xmm2
	vmaxps ymm4{k3}, ymm1, ymm0
	vmaxps zmm3{k1}, zmm6, zmm6
	vmaxps xmm7{k5}{z}, xmm6, xmm6
	vmaxps ymm5{k4}{z}, ymm1, ymm2
	vmaxps zmm3{k1}{z}, zmm4, zword [0xab6]
	vmaxps zmm2, zmm3, zmm4, {sae}
	vmaxps xmm7, xmm4, oword [eax+1]
	vmaxps xmm1, xmm0, oword [eax+64]
	vmaxps ymm6, ymm4, yword [eax+1]
	vmaxps ymm2, ymm6, yword [eax+64]
	vmaxps xmm6, xmm5, oword [eax+1]
	vmaxps xmm0, xmm4, oword [eax+64]
	vmaxps ymm6, ymm1, yword [eax+1]
	vmaxps ymm4, ymm4, yword [eax+64]
	vmaxps zmm6, zmm7, zword [eax+1]
	vmaxps zmm0, zmm1, zword [eax+64]
	vmaxps xmm7, xmm5, [0xd7c]
	vmaxps ymm6, ymm2, [0xf63]
	vmaxps xmm6, xmm2, [0x262]
	vmaxps ymm3, ymm0, [0x9f2]
	vmaxps zmm4, zmm5, [0x4a8]
