clang++ main.cpp -o drm_example -fsanitize=address -fsanitize=undefined -lgbm -ldrm -O0 -g -ggdb3 -I /usr/include/libdrm
