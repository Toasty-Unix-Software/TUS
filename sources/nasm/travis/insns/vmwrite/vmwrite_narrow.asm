	vmwrite edx, ebx
	vmwrite esi, ebx

%ifdef ERROR
	vmwrite rdx, rdx
	vmwrite rdx, rsi
%endif
