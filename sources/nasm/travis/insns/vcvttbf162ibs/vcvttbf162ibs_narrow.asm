	vcvttbf162ibs xmm0, xmm4
	vcvttbf162ibs xmm6, oword [0x14a]
	vcvttbf162ibs ymm6, ymm5
	vcvttbf162ibs ymm3, ymm0
	vcvttbf162ibs zmm5, zmm2
	vcvttbf162ibs zmm0, zmm3

%ifdef ERROR
	vcvttbf162ibs xmm15, xmm15
	vcvttbf162ibs ymm9, ymm10
	vcvttbf162ibs zmm8, zmm12
	vcvttbf162ibs xmm17, xmm21
	vcvttbf162ibs ymm18, ymm19
	vcvttbf162ibs zmm29, zmm29
%endif
