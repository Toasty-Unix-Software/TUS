	vhaddpd xmm7, xmm4, oword [0x9cc]
	vhaddpd xmm7, oword [0x9cc]
	vhaddpd xmm4, xmm0, xmm3
	vhaddpd ymm1, ymm2, ymm4
	vhaddpd ymm1, ymm4
	vhaddpd ymm1, ymm2, ymm6

%ifdef ERROR
	vhaddpd xmm11, xmm14, xmm12
	vhaddpd ymm8, ymm15, ymm15
%endif
