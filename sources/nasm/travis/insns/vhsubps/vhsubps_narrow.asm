	vhsubps xmm7, xmm0, xmm0
	vhsubps xmm7, xmm0
	vhsubps xmm4, xmm0, xmm5
	vhsubps ymm5, ymm7, ymm2
	vhsubps ymm5, ymm2
	vhsubps ymm4, ymm0, ymm2

%ifdef ERROR
	vhsubps xmm13, xmm15, xmm15
	vhsubps ymm15, ymm15, ymm13
%endif
