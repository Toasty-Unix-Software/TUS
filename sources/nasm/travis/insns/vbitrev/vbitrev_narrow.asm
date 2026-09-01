	vbitrev xmm3, xmm4
	vbitrev xmm5, oword [0xa32]
	vbitrev ymm2, ymm4
	vbitrev ymm3, yword [0xc0e]
	vbitrev zmm1, zword [0x352]
	vbitrev zmm2, zmm5

%ifdef ERROR
	vbitrev xmm13, xmm11
	vbitrev ymm11, ymm8
	vbitrev zmm12, zmm12
	vbitrev xmm22, xmm20
	vbitrev ymm25, ymm22
	vbitrev zmm25, zmm25
%endif
