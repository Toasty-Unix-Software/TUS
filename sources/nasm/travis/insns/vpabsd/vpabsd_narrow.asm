	vpabsd xmm3, oword [0xfec]
	vpabsd xmm1, xmm4
	vpabsd ymm1, ymm6
	vpabsd ymm4, ymm2
	vpabsd xmm5, oword [0xc6c]
	vpabsd xmm0, xmm0
	vpabsd ymm0, ymm3
	vpabsd ymm4, ymm1

%ifdef ERROR
	vpabsd xmm11, xmm14
	vpabsd ymm12, ymm10
	vpabsd xmm13, xmm15
	vpabsd ymm12, ymm9
	vpabsd xmm22, xmm22
	vpabsd ymm16, ymm24
	vpabsd xmm22, xmm26
	vpabsd ymm20, ymm17
%endif
