Sizes: 8, 12, 16, 20, 24

I wrote a quick wrapper script in bash which passes 40 bytes from /dev/urandom
to sha1sum.

## Preimage - hash of leading 0's

Has applications to bitcoin/proof-of-work systems. Should take roughly 2^n
tries; n is bits.

### 8 bits: 00

* Expected hashes per sample: 2^8 = 256. Average: 240.66
* Expected total hashes: 50 * 2^8 = 12800. Actual: 12033
* Took 00:07:10.
* About 28 hashes per second
* Thinking bash is a bit slow.

### 16 bits: 0000

* At 28 hashes per second, 50 samples should take 32.5 hours… Avg of 39 minutes
  each.
* Inlined wrapper script. Improved speed to around 170.5 hashes per second;
  about 5 hours 20 minutes expected.
* Completed in about 4 hours 16 minutes
* Expected per sample: 2^16 = 65536. Actual: 68442.8
* Expected total hashes: 5 * 2^16 = 3,276,800. Actual: 3,422,139.
* About 224 hashes per second in long-running execution.
* Too slow, time to refactor again.

### Refactor

Inlining the wrapper script brought it up to 170.5 hashes per second. It's
using about 10% of a CPU core (including the base64 encoding/decoding, etc) -
possibly implementing on an interpreted language w/a lot of IPC was unwise.

Looks like to get 50 24-bit hash preimages w/in an hour I should target 233,017
hashes per second.

Looks like with cuda I'd need to reimplement sha1 on the GPU; let's just use C
w/openssl instead.

* Repeated 2 byte attack in 15 seconds; (50 3560988 71219.8) - that's 237,399.2 hashes/second

### 12 bits: 000

* Expected hashes per sample: 2^12 = 4096. Average: 3635.46
* Expected total hashes: 50 * 2^12 = 204,800. Actual: 181,773

### 20 bits: 0000

* Expected hashes per sample: 2^20 = 1,048,576. Average: 1,154,503.70
* Expected total hashes: 50 * 2^20 = 52,428,800. Actual: 57,725,185

### 24 bits: 00000

* Expected hashes per sample: 2^24 = 16,777,216. Average: 18,169,611.28
* Expected total hashes: 50 * 2^24 = 838,860,800. Actual: 908,480,564 

## Collision

### 8 bits:

* Expected: 2^(8/2) = 16. Average: 19.44
* Expected total: 50 * 2^(8/2) = 800. Actual: 972

### 12 bits:

* Expected: 2^(12/2) = 64. Average: 91.76
* Expected total: 50 * 2^(12/2) = 3200. Actual: 4588 

### 16 bits:

* Expected: 2^(16/2) = 256. Average: 312.42
* Expected total: 50 * 2^(16/2) = 12,800. Actual: 15,621 

### 20 bits:

* Expected: 2^(20/2) = 1024. Average: 1375.70
* Expected total: 50 * 2^(20/2) = 51,200. Actual: 68,785

### 24 bits:

* Expected: 2^(24/2) = 4096. Average: 4961.96
* Expected total: 50 * 2^(24/2) = 204,800. Actual: 248,098

## Conclusions

Redoing the 16 bit collision 5000 times produced an average of 321.74 hashes
per collision - substantially greater than the 256 predicted - but still well
below the next power of 2.

Redoing the 8 bit preimage 5000 times produced an average of 263.50. Redoing it
500,000 times produced an average of 255.95. Pretty close to the theoretical
value.

After the success of the 500,000 times 8 bit preimage, I re-ran the 16 bit
collision 500,000 times to get a more precise estimate of the average. Took
about 5.5 minutes - avg number of hashes was 321.20. We can therefore conclude
that the true average number of hashes is not precisely 2^(n/2), but rather
closer to 2^(n / 1.9214) (or 2^(n * 0.5205)).

I don't much like the sample sizes required to reach the theoretical value.
