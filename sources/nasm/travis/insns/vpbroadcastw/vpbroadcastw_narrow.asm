	vpbroadcastw xmm0, word [0x686]
	vpbroadcastw xmm0, word [0x8eb]
	vpbroadcastw xmm3, xmm1
	vpbroadcastw xmm6, xmm6
	vpbroadcastw ymm1, word [0x508]
	vpbroadcastw ymm0, word [0x8fd]
	vpbroadcastw ymm1, xmm7
	vpbroadcastw ymm7, xmm0

%ifdef ERROR
	vpbroadcastw xmm15, word [0x7be]
	vpbroadcastw xmm14, xmm9
	vpbroadcastw ymm13, word [0x5db]
	vpbroadcastw ymm8, xmm10
	vpbroadcastw xmm31, word [0x249]
	vpbroadcastw xmm20, xmm31
	vpbroadcastw ymm26, word [0x34e]
	vpbroadcastw ymm22, xmm18
%endif
