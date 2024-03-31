FROM debian:bullseye

RUN apt-get update && apt-get install -y \
  cppdb-dev \
  jwt-cpp-dev \
  libmysqlclient-dev

COPY . /app

WORKDIR /app

RUN g++ -std=c++17 -o auth-feed main.cpp -lcppdb -ljwt-cpp -lmysqlclient

CMD ["./auth-feed"]