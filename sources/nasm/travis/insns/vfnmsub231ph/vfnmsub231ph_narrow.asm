	vfnmsub231ph xmm4, xmm5, oword [0x25c]
	vfnmsub231ph xmm4, oword [0x25c]
	vfnmsub231ph xmm5, xmm6, xmm3
	vfnmsub231ph ymm4, ymm3, yword [0xfd5]
	vfnmsub231ph ymm4, yword [0xfd5]
	vfnmsub231ph ymm2, ymm7, yword [0x1cb]
	vfnmsub231ph zmm3, zmm5, zword [0xc12]
	vfnmsub231ph zmm3, zword [0xc12]
	vfnmsub231ph zmm0, zmm1, zmm4

%ifdef ERROR
	vfnmsub231ph xmm15, xmm10, xmm15
	vfnmsub231ph ymm8, ymm13, ymm9
	vfnmsub231ph zmm15, zmm14, zmm14
	vfnmsub231ph xmm26, xmm24, xmm27
	vfnmsub231ph ymm27, ymm29, ymm27
	vfnmsub231ph zmm21, zmm31, zmm31
%endif
