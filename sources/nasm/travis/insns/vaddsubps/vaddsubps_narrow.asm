	vaddsubps xmm1, xmm7, oword [0x112]
	vaddsubps xmm1, oword [0x112]
	vaddsubps xmm3, xmm4, oword [0xb70]
	vaddsubps ymm7, ymm0, ymm2
	vaddsubps ymm7, ymm2
	vaddsubps ymm6, ymm2, yword [0xe5c]

%ifdef ERROR
	vaddsubps xmm9, xmm15, xmm11
	vaddsubps ymm13, ymm9, ymm13
%endif
