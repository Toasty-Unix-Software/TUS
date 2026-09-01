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

%ifdef ERROR
	vsubps xmm10, xmm13, xmm13
	vsubps ymm15, ymm12, ymm15
	vsubps xmm12, xmm11, xmm8
	vsubps ymm12, ymm12, ymm11
	vsubps xmm24, xmm19, xmm27
	vsubps ymm17, ymm22, ymm22
	vsubps xmm25, xmm19, xmm21
	vsubps ymm16, ymm23, ymm18
%endif
