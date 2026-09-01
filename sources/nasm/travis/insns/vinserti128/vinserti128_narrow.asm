	vinserti128 ymm7, ymm1, oword [0x461], 0xfd
	vinserti128 ymm7, oword [0x461], 0xfd
	vinserti128 ymm1, ymm3, xmm0, 0xb

%ifdef ERROR
	vinserti128 ymm14, ymm11, xmm15, 0xa3
%endif
