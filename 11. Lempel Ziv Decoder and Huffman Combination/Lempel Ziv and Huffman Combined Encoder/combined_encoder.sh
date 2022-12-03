#! /bin/bash

# Run Command:
#   chmod a+x combined_encoder.sh
#   ./combined_encoder.sh
#
# This script first applies Huffman Encoding on the data, then applies Lempel-Ziv Encoding on Huffman's Output
# Files Required:
#   input.txt - This file contains the message to be encoded
#
# Files generated:
#   huffman_mapping.txt - This file contains the mapping used during Huffman Encoding
#   lempel_ziv_mapping.txt - This file contains the mapping used during Lempel-Ziv Enconding
#   encoded.txt - This file contains the final encoded data
# Both mapping files are important as they would be used in decoding the data too

gcc -lm huffman_encoder.c -o huffman_encoder
./huffman_encoder
mv input.txt initial_input.txt
mv mapping.txt huffman_mapping.txt
mv encoded.txt input.txt
rm huffman_encoder
gcc -lm lempel_ziv_encoder.c -o lempel_ziv_encoder
./lempel_ziv_encoder
mv mapping.txt lempel_ziv_mapping.txt
rm input.txt lempel_ziv_encoder
mv initial_input.txt input.txt
