	vextracti64x4 ymm0, zmm7, 0xa
	vextracti64x4 ymm5, zmm5, 0x47
	vextracti64x4 yword [0x9bc], zmm5, 0x8
	vextracti64x4 yword [0x3f2], zmm2, 0xe6

%ifdef ERROR
	vextracti64x4 ymm15, zmm12, 0xf9
	vextracti64x4 yword [0xe75], zmm15, 0x23
	vextracti64x4 ymm23, zmm25, 0x4b
	vextracti64x4 yword [0x596], zmm16, 0xc1
%endif
