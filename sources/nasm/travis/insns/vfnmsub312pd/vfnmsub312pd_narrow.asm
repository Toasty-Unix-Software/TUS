	vfnmsub312pd xmm6, xmm6, xmm3
	vfnmsub312pd xmm3, xmm7, xmm5
	vfnmsub312pd ymm3, ymm5, ymm0
	vfnmsub312pd ymm4, ymm7, ymm5

%ifdef ERROR
	vfnmsub312pd xmm9, xmm11, xmm8
	vfnmsub312pd ymm15, ymm11, ymm14
%endif
