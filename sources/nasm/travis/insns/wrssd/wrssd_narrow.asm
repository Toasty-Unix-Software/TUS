	wrssd dword [0x7d7], ecx
	wrssd dword [0xa62], edi

%ifdef ERROR
	wrssd dword [0xfd8], r11d
	wrssd dword [0xb5d], r18d
%endif
