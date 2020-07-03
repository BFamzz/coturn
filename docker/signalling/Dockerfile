FROM bbox:latest

WORKDIR /usr/src/coturn
COPY . . 

WORKDIR /usr/src/coturn/build
RUN cmake \
-I/usr/local/include/ppxx \
-I/usr/local/include/scrypt-kdf.h \
-L/usr/local/lib \
-DPostgreSQL_TYPE_INCLUDE_DIR=/usr/include/postgresql \
-DPostgreSQL_INCLUDE_DIR=/usr/lib/x86_64-linux-gnu .. 
RUN make
EXPOSE 8080
CMD ["./zest-coturn"]