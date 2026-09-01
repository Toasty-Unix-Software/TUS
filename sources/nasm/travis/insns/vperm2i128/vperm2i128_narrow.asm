	vperm2i128 ymm4, ymm0, ymm6, 0x7e
	vperm2i128 ymm4, ymm6, 0x7e
	vperm2i128 ymm7, ymm6, ymm0, 0x62

%ifdef ERROR
	vperm2i128 ymm11, ymm13, ymm11, 0x6e
%endif
