	vfmsub312pd xmm7, xmm7, xmm0
	vfmsub312pd xmm1, xmm2, xmm7
	vfmsub312pd ymm6, ymm6, yword [0x491]
	vfmsub312pd ymm0, ymm7, yword [0x8bd]

%ifdef ERROR
	vfmsub312pd xmm10, xmm15, xmm8
	vfmsub312pd ymm8, ymm15, ymm11
%endif
