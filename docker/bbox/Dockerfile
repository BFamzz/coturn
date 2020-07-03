FROM gcc:9.2.0

RUN printf "deb https://deb.debian.org/debian buster main\ndeb-src https://deb.debian.org/debian buster main\ndeb https://deb.debian.org/debian-security/ buster/updates main\ndeb-src https://deb.debian.org/debian-security/ buster/updates main\ndeb https://deb.debian.org/debian buster-updates main\ndeb-src https://deb.debian.org/debian buster-updates main" > /etc/apt/sources.list

RUN apt-get -qq update
RUN apt-get -qq upgrade

RUN cd /usr/bin && mkdir gfortran && cd ../..

RUN apt-get -qq -y install gfortran

RUN apt-get -qq install cmake

WORKDIR /usr/src

RUN wget https://www.tarsnap.com/scrypt/scrypt-1.3.0.tgz && tar -xzvf scrypt-1.3.0.tgz \
	&& cd scrypt-1.3.0 && ./configure --enable-libscrypt-kdf && make install && cd .. && \
	rm -rf scrypt-1.3.0/ && rm scrypt-1.3.0.tgz

RUN git clone https://github.com/curl/curl.git && cd curl/ \
	&& ./buildconf && ./configure && cd .. && rm -rf curl/

RUN git clone https://github.com/Thalhammer/jwt-cpp.git && cd jwt-cpp && mkdir build && \
	cd build && cmake -DBUILD_TESTS=OFF .. && make && make install && cd ../.. && \
	rm -rf jwt-cpp/

RUN apt-get -qq install libboost-all-dev
RUN apt-get -qq install build-essential libtcmalloc-minimal4 && \
	ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

RUN git clone https://github.com/acl-dev/acl.git \
	&& cd acl/lib_acl && make && ldconfig /usr/src/acl/lib_acl/lib \
	&& cd ../lib_protocol && make && ldconfig /usr/src/acl/lib_protocol/lib \
	&& cd ../lib_acl_cpp && make && ldconfig /usr/src/acl/lib_acl_cpp/lib \
	&& cd ../.. 

RUN git clone https://github.com/jtv/libpqxx.git \
--branch 6.4 --depth 1 \ 
&& cd libpqxx/ && mkdir build && cd build/ && cmake \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_INSTALL_PREFIX=/usr/local \
-DPQXX_DIR=/usr/local/lib \
-DPostgreSQL_DIR=/usr/lib/x86_64-linux-gnu \
-DPQXX_TYPE_INCLUDE_DIR=/usr/local/include/pqxx \
-DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql \
-DCMAKE_MODULE_PATH=/usr/src/libpqxx-r6.4/cmake .. \
&& make && make install && ldconfig /usr/local/lib && \
cd ../.. && rm -rf libpqxx/