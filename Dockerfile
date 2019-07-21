FROM ancients/rigado:latest as builder
COPY Software/ /build/
WORKDIR /build
RUN make
#Artifact produced:
# /build/_build/lasertheremin.out

FROM alpine
COPY --from=builder /build/_build/lasertheremin.out /lasertheremin.out
