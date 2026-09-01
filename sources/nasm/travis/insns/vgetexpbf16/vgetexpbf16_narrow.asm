	vgetexpbf16 xmm4, xmm1
	vgetexpbf16 xmm5, xmm0
	vgetexpbf16 ymm0, yword [0x4a1]
	vgetexpbf16 ymm7, ymm1
	vgetexpbf16 zmm1, zword [0x5e1]
	vgetexpbf16 zmm2, zmm4

%ifdef ERROR
	vgetexpbf16 xmm10, xmm14
	vgetexpbf16 ymm10, ymm13
	vgetexpbf16 zmm14, zmm14
	vgetexpbf16 xmm29, xmm25
	vgetexpbf16 ymm20, ymm19
	vgetexpbf16 zmm17, zmm19
%endif
