# compilation variants

# Base compilation (your current Makefile)
make

# AddressSanitizer build
make CFLAGS="-Wall -Wextra -Werror -MMD -MP -fsanitize=address -fno-omit-frame-pointer -g3"

# UndefinedBehaviorSanitizer build
make CFLAGS="-Wall -Wextra -Werror -MMD -MP -fsanitize=undefined -fno-sanitize-recover=all -g3"

# Combined ASan + UBSan
make CFLAGS="-Wall -Wextra -Werror -MMD -MP -fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=all -g3"

# Debug build for Valgrind
make CFLAGS="-Wall -Wextra -Werror -MMD -MP -g3 -O0"



# valgrind analysis

# Memory leak detection (comprehensive)
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_memcheck.log ./cub3d maps/valid/simple.cub

# Invalid memory access detection
valgrind --tool=memcheck --track-origins=yes --read-var-info=yes --log-file=valgrind_access.log ./cub3d maps/valid/maze.cub

# Performance profiling
valgrind --tool=callgrind --callgrind-out-file=callgrind.out ./cub3d maps/valid/gallery.cub

# Cache miss analysis (performance)
valgrind --tool=cachegrind --cachegrind-out-file=cachegrind.out ./cub3d maps/valid/crazy.cub



# sanitiser runtime testing

# AddressSanitizer execution
ASAN_OPTIONS=verbosity=1:abort_on_error=1:detect_leaks=1 ./cub3d maps/valid/simple.cub

# UndefinedBehaviorSanitizer execution
UBSAN_OPTIONS=print_stacktrace=1:abort_on_error=1 ./cub3d maps/valid/maze.cub

# Combined sanitizers
ASAN_OPTIONS=verbosity=1:abort_on_error=1:detect_leaks=1 UBSAN_OPTIONS=print_stacktrace=1:abort_on_error=1 ./cub3d maps/valid/gallery.cub