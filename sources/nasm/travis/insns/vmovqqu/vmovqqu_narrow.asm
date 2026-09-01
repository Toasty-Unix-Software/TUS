	vmovqqu ymm7, ymm7
	vmovqqu ymm3, yword [0x3d6]
	vmovqqu yword [0xe07], ymm6
	vmovqqu yword [0x37d], ymm3

%ifdef ERROR
	vmovqqu ymm13, ymm13
	vmovqqu ymm13, ymm10
%endif
