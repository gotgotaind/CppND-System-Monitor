PID=`ps aux | sort -nrk 3,3 | head -1 | awk '{print $2}'`
if [[ -e a.out ]]; then
rm a.out
fi
g++ -std=gnu++11 mytest.cpp
./a.out $PID
ps auxw | grep $PID
