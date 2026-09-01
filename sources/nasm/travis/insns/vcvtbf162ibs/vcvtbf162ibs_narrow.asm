	vcvtbf162ibs xmm3, xmm3
	vcvtbf162ibs xmm7, oword [0xb80]
	vcvtbf162ibs ymm4, yword [0xa72]
	vcvtbf162ibs ymm4, ymm2
	vcvtbf162ibs zmm4, zmm5
	vcvtbf162ibs zmm2, zmm2

%ifdef ERROR
	vcvtbf162ibs xmm10, xmm10
	vcvtbf162ibs ymm15, ymm15
	vcvtbf162ibs zmm11, zmm14
	vcvtbf162ibs xmm16, xmm22
	vcvtbf162ibs ymm25, ymm20
	vcvtbf162ibs zmm31, zmm26
%endif
