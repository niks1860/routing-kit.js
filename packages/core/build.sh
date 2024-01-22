#! /bin/bash

set -e

export OPTIMIZE="-Os"
export LDFLAGS="${OPTIMIZE} Wall -DNDEBUG -march=native -ffast-math -std=c++11 -O3 -fPIC"
export CFLAGS="${OPTIMIZE}"
export CXXFLAGS="${OPTIMIZE}"

output_dir="wasm"
input_dir="bindings"

mkdir -p $output_dir

export EMCC_SKIP_SANITY_CHECK=1
export PEER=node_modules/routing-kit

echo "Compiling CCH"
(
em++ -I${PEER}/include -I$input_dir/include \
  ${OPTIMIZE} \
  --no-entry \
  --bind \
  -flto \
  -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
  -s STRICT=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s ASSERTIONS=0 \
  -s MALLOC=emmalloc \
  -s MODULARIZE=1 \
  -s EXPORT_ES6=1 \
  --extern-pre-js ./$output_dir/chh-pre.js \
  -o ./$output_dir/cch.js \
  ./$input_dir/cch.cpp \
  $PEER/src/timer.cpp \
  $PEER/src/graph_util.cpp \
  $PEER/src/bit_select.cpp \
  $PEER/src/id_mapper.cpp \
  $PEER/src/vector_io.cpp \
  $PEER/src/expect.cpp \
  $PEER/src/contraction_hierarchy.cpp \
  $PEER/src/bit_vector.cpp \
  $PEER/src/customizable_contraction_hierarchy.cpp \
  $PEER/src/nested_dissection.cpp 
)
echo "Compiling CCH"

echo "Compiling CH"
(
  em++ -I${PEER}/include -I$input_dir/include \
    ${OPTIMIZE} \
    --no-entry \
    --bind \
    -flto \
    -s ERROR_ON_UNDEFINED_SYMBOLS=0 \
    -s STRICT=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ASSERTIONS=0 \
    -s MALLOC=emmalloc \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    --extern-pre-js ./$output_dir/ch-pre.js \
    -o ./$output_dir/ch.js \
    ./$input_dir/ch.cpp \
    $PEER/src/timer.cpp \
    $PEER/src/graph_util.cpp \
    $PEER/src/bit_vector.cpp \
    $PEER/src/vector_io.cpp \
    $PEER/src/bit_select.cpp \
    $PEER/src/id_mapper.cpp \
    $PEER/src/expect.cpp \
    $PEER/src/nested_dissection.cpp \
    $PEER/src/contraction_hierarchy.cpp
)
echo "Compiling CH"
