FROM ancients/rigado:latest as builder
COPY Software/ /build/
WORKDIR /build
RUN mkdir /root/dev
RUN ln -s /sdk /root/dev/nordic-sdk15
RUN make
RUN arm-none-eabi-objcopy -v -O ihex _build/lasertheremin.out _build/lasertheremin.hex
#Artifact produced:
# /build/_build/lasertheremin.out

FROM alpine
COPY --from=builder /build/_build/lasertheremin.hex /lasertheremin.hex
