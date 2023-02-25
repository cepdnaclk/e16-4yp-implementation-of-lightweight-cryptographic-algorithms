lib_src=../../common/bytes.cpp ../../utils/hex_utils.c
xoodyak_src=xoodyak/xoodyak.c xoodyak/xoodoo.c
pre_process_src=pre_process.cpp cube.cpp $(xoodyak_src) $(lib_src) 


pre_process: $(pre_process_src) pre_process.hpp
	g++ -g -o $@ $(pre_process_src)

clean: pre_process.exe
	rm pre_process.exe
