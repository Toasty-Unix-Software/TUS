	vextractf64x2 xmm1, ymm6, 0x2b
	vextractf64x2 xmm1, ymm7, 0xfc
	vextractf64x2 xmm3, zmm2, 0xe3
	vextractf64x2 xmm2, zmm5, 0x7a
	vextractf64x2 oword [0x605], ymm5, 0x9
	vextractf64x2 oword [0xb70], ymm7, 0x3d
	vextractf64x2 oword [0x268], zmm6, 0x8d
	vextractf64x2 oword [0xd36], zmm5, 0x2

%ifdef ERROR
	vextractf64x2 xmm8, ymm14, 0x7
	vextractf64x2 xmm11, zmm11, 0x43
	vextractf64x2 oword [0x2ee], ymm14, 0xdf
	vextractf64x2 oword [0xd81], zmm12, 0xe1
	vextractf64x2 xmm17, ymm16, 0x6f
	vextractf64x2 xmm25, zmm25, 0x7e
	vextractf64x2 oword [0x3c7], ymm28, 0xe7
	vextractf64x2 oword [0x410], zmm22, 0xef
%endif
