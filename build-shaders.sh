PIDS=()

for filename in ./bin/linux-gcc-64/Debug/data/**/shaders/*.fx; do
    echo Compiling $filename...
    wine /usr/bin/efb.exe /I ./bin/linux-gcc-64/Debug/data/shaders/ $filename $filename'b'
    PIDS+=$!' '
done

for pid in ${PIDS[*]}; do
    wait $pid
done
