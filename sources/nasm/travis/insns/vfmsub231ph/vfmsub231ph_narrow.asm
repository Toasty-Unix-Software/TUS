	vfmsub231ph xmm4, xmm6, xmm2
	vfmsub231ph xmm4, xmm2
	vfmsub231ph xmm1, xmm4, xmm3
	vfmsub231ph ymm2, ymm4, ymm3
	vfmsub231ph ymm2, ymm3
	vfmsub231ph ymm2, ymm5, yword [0xde6]
	vfmsub231ph zmm7, zmm2, zmm1
	vfmsub231ph zmm7, zmm1
	vfmsub231ph zmm0, zmm3, zmm4

%ifdef ERROR
	vfmsub231ph xmm15, xmm14, xmm8
	vfmsub231ph ymm10, ymm10, ymm14
	vfmsub231ph zmm13, zmm13, zmm14
	vfmsub231ph xmm16, xmm29, xmm20
	vfmsub231ph ymm17, ymm26, ymm29
	vfmsub231ph zmm26, zmm19, zmm28
%endif
