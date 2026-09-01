	vcvtps2iubs xmm7, xmm1
	vcvtps2iubs xmm0, oword [0x9fc]
	vcvtps2iubs ymm7, ymm5
	vcvtps2iubs ymm0, ymm6
	vcvtps2iubs zmm0, zmm4
	vcvtps2iubs zmm7, zmm4

%ifdef ERROR
	vcvtps2iubs xmm8, xmm15
	vcvtps2iubs ymm14, ymm13
	vcvtps2iubs zmm15, zmm15
	vcvtps2iubs xmm20, xmm28
	vcvtps2iubs ymm29, ymm19
	vcvtps2iubs zmm25, zmm20
%endif
