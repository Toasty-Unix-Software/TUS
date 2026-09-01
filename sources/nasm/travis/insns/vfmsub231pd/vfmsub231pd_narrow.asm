	vfmsub231pd xmm3, xmm7, xmm0
	vfmsub231pd xmm4, xmm4, xmm5
	vfmsub231pd ymm4, ymm5, ymm3
	vfmsub231pd ymm1, ymm5, ymm2
	vfmsub231pd xmm5, xmm3, xmm7
	vfmsub231pd xmm4, xmm5, xmm4
	vfmsub231pd ymm4, ymm7, ymm2
	vfmsub231pd ymm3, ymm3, ymm7

%ifdef ERROR
	vfmsub231pd xmm12, xmm15, xmm8
	vfmsub231pd ymm10, ymm10, ymm13
	vfmsub231pd xmm8, xmm8, xmm12
	vfmsub231pd ymm12, ymm11, ymm13
	vfmsub231pd xmm16, xmm21, xmm22
	vfmsub231pd ymm31, ymm16, ymm25
	vfmsub231pd xmm24, xmm20, xmm17
	vfmsub231pd ymm29, ymm28, ymm27
%endif
