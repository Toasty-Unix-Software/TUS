	vfmsub312ps xmm7, xmm1, xmm7
	vfmsub312ps xmm4, xmm4, xmm3
	vfmsub312ps ymm6, ymm6, ymm4
	vfmsub312ps ymm4, ymm7, yword [0x36e]

%ifdef ERROR
	vfmsub312ps xmm8, xmm14, xmm15
	vfmsub312ps ymm11, ymm10, ymm13
%endif
