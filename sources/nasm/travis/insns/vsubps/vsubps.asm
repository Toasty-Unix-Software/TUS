default rel
	vsubps xmm0, xmm3, xmm7
	vsubps xmm0, xmm7
	vsubps xmm4, xmm5, xmm4
	vsubps ymm4, ymm6, ymm2
	vsubps ymm4, ymm2
	vsubps ymm6, ymm6, yword [0xfee]
	vsubps xmm2, xmm5, xmm0
	vsubps xmm2, xmm0
	vsubps xmm7, xmm2, xmm7
	vsubps ymm2, ymm3, yword [0x570]
	vsubps ymm2, yword [0x570]
	vsubps ymm7, ymm5, ymm3
	vsubps xmm10, xmm13, xmm13
	vsubps ymm15, ymm12, ymm15
	vsubps xmm12, xmm11, xmm8
	vsubps ymm12, ymm12, ymm11
	vsubps xmm24, xmm19, xmm27
	vsubps ymm17, ymm22, ymm22
	vsubps xmm25, xmm19, xmm21
	vsubps ymm16, ymm23, ymm18
	vsubps xmm2{k2}, xmm3, xmm4
	vsubps ymm5{k5}, ymm2, ymm6
	vsubps zmm1{k5}, zmm3, zmm0
	vsubps xmm5{k2}{z}, xmm2, xmm2
	vsubps ymm1{k2}{z}, ymm7, yword [0x786]
	vsubps zmm6{k5}{z}, zmm1, zmm0
	vsubps zmm5, zmm6, zmm4, {rd-sae}
	vsubps xmm5, xmm1, oword [eax+1]
	vsubps xmm1, xmm7, oword [eax+64]
	vsubps ymm3, ymm0, yword [eax+1]
	vsubps ymm6, ymm0, yword [eax+64]
	vsubps xmm7, xmm6, oword [eax+1]
	vsubps xmm2, xmm6, oword [eax+64]
	vsubps ymm5, ymm2, yword [eax+1]
	vsubps ymm6, ymm6, yword [eax+64]
	vsubps zmm1, zmm0, zword [eax+1]
	vsubps zmm2, zmm0, zword [eax+64]
	vsubps xmm5, xmm6, [0x114]
	vsubps ymm6, ymm6, [0xc5e]
	vsubps xmm6, xmm7, [0x855]
	vsubps ymm7, ymm5, [0xfe9]
	vsubps zmm4, zmm5, [0x9cd]
