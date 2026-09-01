	vfixupimmps xmm5, xmm2, xmm1, 0x6
	vfixupimmps xmm5, xmm1, 0x6
	vfixupimmps xmm7, xmm5, oword [0xf16], 0xd5
	vfixupimmps ymm1, ymm0, ymm0, 0x3c
	vfixupimmps ymm1, ymm0, 0x3c
	vfixupimmps ymm5, ymm1, yword [0x418], 0xe0
	vfixupimmps zmm2, zmm0, zmm5, 0x98
	vfixupimmps zmm2, zmm5, 0x98
	vfixupimmps zmm1, zmm7, zmm0, 0x4f

%ifdef ERROR
	vfixupimmps xmm11, xmm15, xmm9, 0x7f
	vfixupimmps ymm11, ymm10, ymm15, 0x9a
	vfixupimmps zmm15, zmm11, zmm11, 0xb
	vfixupimmps xmm23, xmm23, xmm17, 0xfd
	vfixupimmps ymm17, ymm25, ymm19, 0x57
	vfixupimmps zmm16, zmm18, zmm29, 0xe7
%endif
