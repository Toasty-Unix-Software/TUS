	vscalefbf16 xmm6, xmm6, oword [0xfbc]
	vscalefbf16 xmm4, xmm5, xmm3
	vscalefbf16 ymm6, ymm0, ymm6
	vscalefbf16 ymm1, ymm3, ymm4
	vscalefbf16 zmm5, zmm4, zword [0x422]
	vscalefbf16 zmm1, zmm0, zword [0xb1c]

%ifdef ERROR
	vscalefbf16 xmm13, xmm8, xmm12
	vscalefbf16 ymm9, ymm13, ymm10
	vscalefbf16 zmm11, zmm14, zmm8
	vscalefbf16 xmm27, xmm29, xmm28
	vscalefbf16 ymm18, ymm30, ymm23
	vscalefbf16 zmm18, zmm19, zmm17
%endif
