	vfnmsub312ps xmm0, xmm2, xmm7
	vfnmsub312ps xmm1, xmm0, oword [0x251]
	vfnmsub312ps ymm5, ymm2, yword [0x23e]
	vfnmsub312ps ymm5, ymm1, ymm5

%ifdef ERROR
	vfnmsub312ps xmm15, xmm11, xmm11
	vfnmsub312ps ymm11, ymm15, ymm14
%endif
