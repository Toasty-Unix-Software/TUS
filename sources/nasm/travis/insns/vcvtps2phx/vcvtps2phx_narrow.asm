	vcvtps2phx xmm5, oword [0xb67]
	vcvtps2phx xmm6, xmm4
	vcvtps2phx xmm4, yword [0x163]
	vcvtps2phx xmm6, yword [0xb3c]
	vcvtps2phx ymm2, zword [0x5a2]
	vcvtps2phx ymm4, zword [0x3e9]

%ifdef ERROR
	vcvtps2phx xmm9, xmm8
	vcvtps2phx xmm13, ymm15
	vcvtps2phx ymm14, zmm12
	vcvtps2phx xmm24, xmm24
	vcvtps2phx xmm31, ymm28
	vcvtps2phx ymm16, zmm31
%endif
