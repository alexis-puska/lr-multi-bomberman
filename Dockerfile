# syntax=docker/dockerfile:1
ARG BASE_IMAGE=debian:oldoldstable-slim
FROM $BASE_IMAGE AS builder
RUN apt-get update && apt-get install -y build-essential git libsdl2*-dev libdrm-dev libgbm-dev
WORKDIR /build
COPY . ./
ARG TARGET_PLATFORM
ARG TARGET_SUBVARIANT
ARG TARGET_ID
ARG VERSION
RUN <<EOF
  set -eu
  case "${TARGET_SUBVARIANT-}" in
    rpi?) sed -i "/#platform =/a platform = ${TARGET_SUBVARIANT}" Makefile.libretro ;;
  esac
  make clean -f Makefile.libretro
  make -f Makefile.libretro
  sed -i "s/^display_version = \".*\"/display_version = \"${VERSION}\"/" *.info
  tar c -zvf "${TARGET_ID:-$(uname -m)${TARGET_SUBVARIANT:+-${TARGET_SUBVARIANT}}}.tar.gz" \
    *.so \
    *.info \
    emulationStationTheme/ \
    -C resources/image img_splash_screen.png
EOF

FROM scratch
COPY --from=builder /build/*.tar.gz ./
