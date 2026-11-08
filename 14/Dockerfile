FROM ubuntu:22.04

RUN apt-get update && apt-get install -y clang make

WORKDIR /app

COPY . .

RUN clang++ -O3 -std=c++14 -pthread 14_server.cpp hashtable.cpp zset.cpp avl.cpp heap.cpp thread_pool.cpp -o my_redis_server

EXPOSE 6379

CMD ["./my_redis_server"]