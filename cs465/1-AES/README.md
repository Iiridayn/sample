DO NOT USE THIS IN PRODUCTION CODE! THERE IS NO DEFENSE AGAINST SIDE-CHANNEL ATTACKS. THIS WAS DONE FOR LEARNING PURPOSES _ONLY_!

How to compile and run the code:
* Compile: `make`; requires gcc, linux system (we use `sys/stat.h`)
* Prep - make a key: `echo '000102030405060708090a0b0c0d0e0f' | xxd -r -p > inc.key`
* Run: `echo '00112233445566778899aabbccddeeff' | xxd -r -p | ./aes -d -k inc.key`
* Decrypt: `echo '69c4e0d86a7b0430d8cdb78070b4c55a' | xxd -r -p | ./aes -d -r -k inc.key`
* Simple test for sameness: `echo '00112233445566778899aabbccddeeff' | xxd -r -p | ./aes -k inc.key | xxd -r -p | ./aes -r -k inc.key`

I only used the resources listed at https://cs465.internet.byu.edu/fall-2019/projects/project1 to understand/implement AES; primarily the FIPS-197 spec.

I pass all test cases (128, 192, 256) in Appendix C of the FIPS specification.
