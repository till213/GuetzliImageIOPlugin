DEPENDPATH += $$PWD/guetzli/guetzli \
              $$PWD/guetzli/third_party/butteraugli

# Guetzli

HEADERS   += $$PWD/guetzli/guetzli/processor.h \
             $$PWD/guetzli/guetzli/butteraugli_comparator.h \
             $$PWD/guetzli/guetzli/comparator.h \
             $$PWD/guetzli/guetzli/debug_print.h \
             $$PWD/guetzli/guetzli/fast_log.h \
             $$PWD/guetzli/guetzli/jpeg_data.h \
             $$PWD/guetzli/guetzli/jpeg_data_decoder.h \
             $$PWD/guetzli/guetzli/jpeg_data_encoder.h \
             $$PWD/guetzli/guetzli/jpeg_data_reader.h \
             $$PWD/guetzli/guetzli/jpeg_data_writer.h \
             $$PWD/guetzli/guetzli/jpeg_huffman_decode.h \
             $$PWD/guetzli/guetzli/output_image.h \
             $$PWD/guetzli/guetzli/quantize.h \
             $$PWD/guetzli/guetzli/preprocess_downsample.h \
             $$PWD/guetzli/guetzli/dct_double.h \
             $$PWD/guetzli/guetzli/fdct.h \
             $$PWD/guetzli/guetzli/entropy_encode.h \
             $$PWD/guetzli/guetzli/gamma_correct.h \
             $$PWD/guetzli/guetzli/quality.h \
             $$PWD/guetzli/guetzli/score.h \
             $$PWD/guetzli/guetzli/idct.h

SOURCES   += $$PWD/guetzli/guetzli/processor.cc \
             $$PWD/guetzli/guetzli/butteraugli_comparator.cc \
             $$PWD/guetzli/guetzli/debug_print.cc \
             $$PWD/guetzli/guetzli/jpeg_data.cc \
             $$PWD/guetzli/guetzli/jpeg_data_decoder.cc \
             $$PWD/guetzli/guetzli/jpeg_data_encoder.cc \
             $$PWD/guetzli/guetzli/jpeg_data_reader.cc \
             $$PWD/guetzli/guetzli/jpeg_data_writer.cc \
             $$PWD/guetzli/guetzli/jpeg_huffman_decode.cc \
             $$PWD/guetzli/guetzli/output_image.cc \
             $$PWD/guetzli/guetzli/quantize.cc \
             $$PWD/guetzli/guetzli/preprocess_downsample.cc \
             $$PWD/guetzli/guetzli/dct_double.cc \
             $$PWD/guetzli/guetzli/fdct.cc \
             $$PWD/guetzli/guetzli/entropy_encode.cc \
             $$PWD/guetzli/guetzli/gamma_correct.cc \
             $$PWD/guetzli/guetzli/quality.cc \
             $$PWD/guetzli/guetzli/score.cc \
             $$PWD/guetzli/guetzli/idct.cc

# Butteraugli

HEADERS   += $$PWD/guetzli/third_party/butteraugli/butteraugli/butteraugli.h
SOURCES   += $$PWD/guetzli/third_party/butteraugli/butteraugli/butteraugli.cc


