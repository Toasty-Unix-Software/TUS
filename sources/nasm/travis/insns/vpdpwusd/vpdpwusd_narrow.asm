	vpdpwusd xmm0, xmm4, xmm4
	vpdpwusd xmm1, xmm1, xmm2
	vpdpwusd ymm4, ymm5, ymm1
	vpdpwusd ymm2, ymm3, yword [0x253]
	vpdpwusd xmm4, xmm1, xmm4
	vpdpwusd xmm6, xmm0, xmm7
	vpdpwusd ymm1, ymm0, yword [0xac8]
	vpdpwusd ymm4, ymm0, ymm1

%ifdef ERROR
	vpdpwusd xmm13, xmm12, xmm14
	vpdpwusd ymm13, ymm14, ymm8
	vpdpwusd xmm14, xmm10, xmm12
	vpdpwusd ymm10, ymm12, ymm8
	vpdpwusd xmm24, xmm18, xmm19
	vpdpwusd ymm20, ymm23, ymm26
	vpdpwusd xmm21, xmm16, xmm28
	vpdpwusd ymm28, ymm18, ymm27
%endif
