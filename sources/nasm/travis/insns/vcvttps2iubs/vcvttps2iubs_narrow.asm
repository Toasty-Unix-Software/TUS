	vcvttps2iubs xmm5, xmm2
	vcvttps2iubs xmm3, oword [0xce7]
	vcvttps2iubs ymm4, ymm1
	vcvttps2iubs ymm1, ymm0
	vcvttps2iubs zmm2, zmm5
	vcvttps2iubs zmm7, zmm3

%ifdef ERROR
	vcvttps2iubs xmm9, xmm9
	vcvttps2iubs ymm14, ymm8
	vcvttps2iubs zmm15, zmm15
	vcvttps2iubs xmm22, xmm28
	vcvttps2iubs ymm22, ymm26
	vcvttps2iubs zmm25, zmm31
%endif
