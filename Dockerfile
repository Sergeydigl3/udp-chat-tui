# FROM alpine:latest

# # Install necessary dependencies
# RUN apk update && \
#     apk add --no-cache g++ sudo

# RUN apk add --no-cache make

# # Copy your C++ source code into the container
# COPY ChatClient.cpp /app/
# COPY Makefile /app/


# # Compile your C++ program
# WORKDIR /app
# RUN make

# # Set default values for environment variables
# ENV IP_ADDRESS="127.0.0.1"
# ENV PORT="8080"

# CMD ./chatclient $IP_ADDRESS $PORT


FROM ubuntu:latest

# Install necessary dependencies
RUN \
    --mount=type=cache,target=/var/cache/apt

RUN apt-get update && \
    apt-get install -y g++ sudo make git cmake

RUN apt-get install -y locales \
    && localedef -i en_US -c -f UTF-8 -A /usr/share/locale/locale.alias en_US.UTF-8



ENV DEBIAN_FRONTEND=noninteractive \
LANG=en_US.UTF-8 \
LANGUAGE=en_US.UTF-8 \
LC_ALL=en_US.UTF-8


# Compile ftxui for beauty

# Comment here to enable simple inteface
# Segment start
WORKDIR /tmp/external
RUN git clone -j8 https://github.com/ArthurSonzogni/ftxui
WORKDIR /tmp/external/ftxui
RUN mkdir build
WORKDIR /tmp/external/ftxui/build
RUN cmake .. && make && make install
ENV BEAUTY="TRUE"
# Segment end


COPY src/ /app/src
COPY CMakeLists.txt /app/


WORKDIR /app/build
RUN cmake .. && make

# Set default values for environment variables
ENV IP_ADDRESS="127.0.0.1"
ENV PORT="8080"

CMD ./chatclient $IP_ADDRESS $PORT

