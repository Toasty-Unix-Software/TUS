	vhaddps xmm4, xmm4, xmm5
	vhaddps xmm4, xmm5
	vhaddps xmm3, xmm6, xmm3
	vhaddps ymm2, ymm5, yword [0xa8f]
	vhaddps ymm2, yword [0xa8f]
	vhaddps ymm7, ymm6, ymm3

%ifdef ERROR
	vhaddps xmm13, xmm13, xmm11
	vhaddps ymm11, ymm15, ymm10
%endif
