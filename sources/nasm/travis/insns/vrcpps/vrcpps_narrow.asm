	vrcpps xmm5, xmm5
	vrcpps xmm5, oword [0xedd]
	vrcpps ymm2, ymm2
	vrcpps ymm1, ymm5

%ifdef ERROR
	vrcpps xmm15, xmm13
	vrcpps ymm8, ymm12
%endif
