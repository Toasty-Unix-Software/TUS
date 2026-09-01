	wrussd dword [0xb39], esi
	wrussd dword [0xa52], edx

%ifdef ERROR
	wrussd dword [0x27e], r10d
	wrussd dword [0x968], r16d
%endif
