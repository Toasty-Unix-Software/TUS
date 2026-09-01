	vcvtneoph2ps xmm7, oword [0xb80]
	vcvtneoph2ps xmm0, oword [0xcce]
	vcvtneoph2ps ymm7, yword [0xc29]
	vcvtneoph2ps ymm3, yword [0xc09]

%ifdef ERROR
	vcvtneoph2ps xmm8, oword [0x7d5]
	vcvtneoph2ps ymm15, yword [0xc8f]
%endif
