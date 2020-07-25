# Huffman_Coding
Implementation of huffman code using huffman tree in C (not in canonical form)
Huffman coding produces optimal prefix binary code for any sequence of characters and helps to reduce size of encoded data to great extent.  
For example : optimal binary prefix code for string :  
 a_dead_dad_ceded_a_bad_babe_a_beaded_abaca_bed  
is <br> _ -> 00 <br> d -> 01 <br> a-> 10 <br> e -> 110 <br> c -> 1110 <br> b -> 1111 <br>
<br> 
<br>
gcc huffman.c priority_minheap.c -o huffmancoder (or anything) (optional : -fsanitize=address)<br>
to compile source code.
<br>
<br>
Note : This code does not do compression. Produced code should be packed in byte in the form of bits to produce compressed output. And it implements priority queue with lowerst priority in the form of minheap. And is completely memory safe (not buffer overflow safe though). Any buffer can be extended as per requirement. 
