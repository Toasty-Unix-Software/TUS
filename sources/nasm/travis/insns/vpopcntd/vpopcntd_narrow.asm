	vpopcntd xmm0, xmm2
	vpopcntd xmm0, oword [0x7fd]
	vpopcntd ymm4, ymm7
	vpopcntd ymm6, ymm3
	vpopcntd zmm5, zword [0x4c1]
	vpopcntd zmm4, zmm0

%ifdef ERROR
	vpopcntd xmm10, xmm15
	vpopcntd ymm10, ymm8
	vpopcntd zmm9, zmm10
	vpopcntd xmm30, xmm23
	vpopcntd ymm29, ymm26
	vpopcntd zmm20, zmm20
%endif
