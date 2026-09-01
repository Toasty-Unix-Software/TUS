	vperm2f128 ymm1, ymm7, ymm4, 0x13
	vperm2f128 ymm1, ymm4, 0x13
	vperm2f128 ymm5, ymm7, ymm2, 0xee

%ifdef ERROR
	vperm2f128 ymm14, ymm12, ymm15, 0x3
%endif
