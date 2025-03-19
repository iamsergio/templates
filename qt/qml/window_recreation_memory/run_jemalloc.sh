  MALLOC_CONF=""background_thread:true,dirty_decay_ms:0,muzzy_decay_ms:0" LD_PRELOAD=/usr/lib/libjemalloc.so ./build-dev/test $*
