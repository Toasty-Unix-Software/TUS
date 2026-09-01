	vextractf64x4 ymm0, zmm6, 0x72
	vextractf64x4 ymm4, zmm7, 0xca
	vextractf64x4 yword [0xd1d], zmm5, 0x86
	vextractf64x4 yword [0xd87], zmm6, 0x75

%ifdef ERROR
	vextractf64x4 ymm11, zmm15, 0x28
	vextractf64x4 yword [0xcdf], zmm13, 0xe5
	vextractf64x4 ymm24, zmm24, 0xb5
	vextractf64x4 yword [0x5e1], zmm17, 0xc9
%endif
