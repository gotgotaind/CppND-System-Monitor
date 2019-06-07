if [[ -e a.out ]]; then
rm a.out
fi
g++ -std=gnu++11 mytest.cpp
./a.out
ps auxw | grep 640
