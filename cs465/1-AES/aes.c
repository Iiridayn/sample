#include <stdlib.h>
#include <stdio.h>
#include <string.h> // memcpy
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <sys/stat.h>

#define BLOCK_LEN 128 // should never change from 128; analysis of other values is much weaker
#define AES_Nb (BLOCK_LEN / 32) // compiler should optimize this
#define MAX_KEY_WORDS 8

#define FFADD(x, y) (x) ^ (y)

// STRUCTURES

typedef union word {
	uint8_t byte[4];
	uint32_t word;
} word_t;

// HELPFUL CONSTANTS

// Rcon[] is 1-based, so the first entry is just a place holder
static const uint8_t rcon[] = {
	0x00,
	0x01, 0x02, 0x04, 0x08,
	0x10, 0x20, 0x40, 0x80,
	0x1B, 0x36, 0x6C, 0xD8,
	0xAB, 0x4D, 0x9A, 0x2F,
	0x5E, 0xBC, 0x63, 0xC6,
	0x97, 0x35, 0x6A, 0xD4,
	0xB3, 0x7D, 0xFA, 0xEF,
	0xC5, 0x91, 0x39, 0x72,
	0xE4, 0xD3, 0xBD, 0x61,
	0xC2, 0x9F, 0x25, 0x4A,
	0x94, 0x33, 0x66, 0xCC,
	0x83, 0x1D, 0x3A, 0x74,
	0xE8, 0xCB, 0x8D
};

static const uint8_t sbox[] = {
	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
	0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
	0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
	0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
	0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
	0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
	0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
	0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
	0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
	0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
	0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
	0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
	0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
	0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
	0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
	0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16,
};
static const uint8_t sbox_i[] = {
   0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
   0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
   0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
   0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
   0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
   0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
   0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
   0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
   0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
   0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
   0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
   0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
   0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
   0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
   0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
   0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d,
};

// MODULE LEVEL VARIABLES
static bool demo_mode = false;

// COMMON FUNCTIONS

// TODO table lookup possibly; generate at startup
// multiplies a polynomial in the GF by x
static const uint8_t xtime(uint8_t poly)
{
	uint8_t out = poly << 1;
	if (poly & 0x80)
		out ^= 0x1b;
	return out;
}

static const uint8_t poly_mult(uint8_t a, uint8_t b)
{
	uint8_t out = b & 0x01 ? a : 0;

	uint8_t mask = 0x02;
	uint8_t running = a;
	while (mask) {
		running = xtime(running);
		if (b & mask)
			out ^= running;
		mask <<= 1;
	}

	return out;
}

static word_t sub_word(word_t word)
{
	for (int i = 0; i < sizeof (word_t); i++) {
		word.byte[i] = sbox[word.byte[i]];
	}
	return word;
}

// LE arch; bitshift doesn't work
static word_t rot_word(word_t word)
{
	uint8_t tmp = word.byte[0];
	for (int i = 0; i < 3; i++)
		word.byte[i] = word.byte[i + 1];
	word.byte[3] = tmp;
	return word;
}

#define DEMO_BYTES(STATE, BYTES) do { \
	for (int i = 0; i < BYTES; i++) \
		printf("%02x", (STATE)[i]); \
	printf("\n"); \
} while (0);
#define DEMO(ROUND, LABEL, STATE) do { \
	if (!demo_mode) break; \
	printf("round[%2d].%-10s", ROUND, LABEL); \
	DEMO_BYTES(STATE, (AES_Nb * sizeof (word_t))); \
} while (0);

#define DUMP_WORD(W) printf("%02x%02x%02x%02x ", (W).byte[0], (W).byte[1], (W).byte[2], (W).byte[3]);

//#define APPENDIX_A
#ifdef APPENDIX_A
#define DEBUG_A(W) DUMP_WORD(W)
#else
#define DEBUG_A(W)
#endif
static void expand_key(uint8_t *key, uint8_t key_bytes, word_t *out, int rounds)
{
	memcpy(out, key, key_bytes); // First is just the key
#ifdef APPENDIX_A
	for (int i = 0; i < (key_bytes / sizeof (uint32_t)); i++)
		DUMP_WORD(out[i]);
	printf("\n");
#endif

	word_t tmp;
	int key_words = key_bytes / sizeof (uint32_t);
	// operates a word at a time
	for (int i = key_words; i < (AES_Nb * (rounds + 1)); i++) {
		tmp = out[i - 1];
		DEBUG_A(tmp);

		if (i % key_words == 0) {
			tmp = rot_word(tmp);
			DEBUG_A(tmp);

			tmp = sub_word(tmp);
			DEBUG_A(tmp);
#ifdef APPENDIX_A
			printf("%02x000000 ", rcon[i / key_words]);
#endif

			tmp.byte[0] = tmp.byte[0] ^ rcon[i / key_words];
			DEBUG_A(tmp);
		} else if (key_words > 6 && i % key_words == 4) {
			tmp = sub_word(tmp);
		}
		DEBUG_A(out[i - key_words]);

		out[i].word = out[i - key_words].word ^ tmp.word;
		DEBUG_A(tmp);
#ifdef APPENDIX_A
		printf("\n");
#endif
	}
}

// PAIRED SUBROUTINES

static void sub_bytes(uint8_t *state)
{
	for (int i = 0; i < (AES_Nb * sizeof (uint32_t)); i++) {
		state[i] = sbox[state[i]];
	}
}
static void sub_bytes_i(uint8_t *state)
{
	for (int i = 0; i < (AES_Nb * sizeof (uint32_t)); i++) {
		state[i] = sbox_i[state[i]];
	}
}

// state is being stored _column major_; so it's shift columns 1-3 by 1-3 adding to bottom
// NOTE only defined for Nb == 4
static void shift_rows(uint8_t *state)
{
	// 1; no shift for first row
	for (int i = 1; i < sizeof (uint32_t); i++) {
		word_t tmp;
		for (int j = 0; j < sizeof (uint32_t); j++)
			tmp.byte[j] = state[i + j * sizeof (uint32_t)];

		for (int k = 0; k < i; k++)
			tmp = rot_word(tmp);

		for (int j = 0; j < sizeof (uint32_t); j++)
			state[i + j * sizeof (uint32_t)] = tmp.byte[j];
	}
}

static void shift_rows_i(uint8_t *state)
{
	// 1; no shift for first row
	for (int i = 1; i < sizeof (uint32_t); i++) {
		word_t tmp;
		for (int j = 0; j < sizeof (uint32_t); j++)
			tmp.byte[j] = state[i + j * sizeof (uint32_t)];

		// forward shift by 3 is the same as reverse shift by 1 in mod 4 space,
		// and we're already not concerned about efficiency w/this code as is
		for (int k = 3; k >= i; k--)
			tmp = rot_word(tmp);

		for (int j = 0; j < sizeof (uint32_t); j++)
			state[i + j * sizeof (uint32_t)] = tmp.byte[j];
	}
}

// state is column major; so this is rows
static void mix_columns(uint8_t *state)
{
	uint8_t base[sizeof (uint32_t)];
	for (int i = 0; i < AES_Nb; i++) {
		uint8_t *row = state + (i * sizeof (uint32_t));
		memcpy(base, state + (i * sizeof (uint32_t)), sizeof (base));

		row[0] = xtime(base[0]) ^ poly_mult(base[1], 3) ^ base[2] ^ base[3];
		row[1] = base[0] ^ xtime(base[1]) ^ poly_mult(base[2], 3) ^ base[3];
		row[2] = base[0] ^ base[1] ^ xtime(base[2]) ^ poly_mult(base[3], 3);
		row[3] = poly_mult(base[0], 3) ^ base[1] ^ base[2] ^ xtime(base[3]);
	}
}

// as above, column major; mostly to make this code easier
static void mix_columns_i(uint8_t *state)
{
	uint8_t b[sizeof (uint32_t)]; // base
	for (int i = 0; i < AES_Nb; i++) {
		uint8_t *row = state + (i * sizeof (uint32_t));
		memcpy(b, state + (i * sizeof (uint32_t)), sizeof (b));

		row[0] = poly_mult(b[0], 0x0e) ^ poly_mult(b[1], 0x0b) ^ poly_mult(b[2], 0x0d) ^ poly_mult(b[3], 0x09);
		row[1] = poly_mult(b[0], 0x09) ^ poly_mult(b[1], 0x0e) ^ poly_mult(b[2], 0x0b) ^ poly_mult(b[3], 0x0d);
		row[2] = poly_mult(b[0], 0x0d) ^ poly_mult(b[1], 0x09) ^ poly_mult(b[2], 0x0e) ^ poly_mult(b[3], 0x0b);
		row[3] = poly_mult(b[0], 0x0b) ^ poly_mult(b[1], 0x0d) ^ poly_mult(b[2], 0x09) ^ poly_mult(b[3], 0x0e);
	}
}

// Takes AES_Nb words
static void add_round_key(uint8_t *state, word_t *round_key)
{
	for (int i = 0; i < AES_Nb; i++) {
		for (int j = 0; j < sizeof (uint32_t); j++) {
			state[i * sizeof (uint32_t) + j] ^= round_key[i].byte[j];
		}
	}
}


// PUBLIC INTERFACE

#define DUMP_STATE(S) do { \
	for (int i = 0; i < sizeof (uint32_t); i++) { \
		printf("%02x", (S)[i]); \
		for (int j = i + sizeof (uint32_t); j < (AES_Nb * sizeof (uint32_t)); j += sizeof (uint32_t)) { \
			printf(" %02x", (S)[j]); \
		} \
		printf("\n"); \
	} \
	printf("\n"); \
} while(0);

//#define APPENDIX_B
#ifdef APPENDIX_B
#define DEBUG_B(S) DUMP_STATE(S)
// TODO: just cast it as bytes, as w/DEMO
#define DEBUG_B_KEY(K) do {\
	for (int i = 0; i < sizeof (uint32_t); i++) { \
		printf("%02x", (K)[0].byte[i]); \
		for (int j = 1; j < AES_Nb; j++) { \
			printf(" %02x", (K)[j].byte[i]); \
		} \
		printf("\n"); \
	} \
	printf("\n"); \
} while(0);
#else
#define DEBUG_B(S)
#define DEBUG_B_KEY(K)
#endif
// byte in[4*Nb], byte out[4*Nb], word w[Nb*(Nr+1)]
void cipher(uint8_t *state, word_t *round_keys, uint8_t nr)
{
	if (demo_mode)
		printf("\nCIPHER (ENCRYPT):\n");

	DEBUG_B(state);
	DEMO(0, "input", state);

	DEBUG_B_KEY(round_keys);
	DEMO(0, "k_sch", (uint8_t *) round_keys);
	add_round_key(state, round_keys);

	for (int round = 1; round < nr; round++) {
#ifdef APPENDIX_B
		printf("Round %d\n", round);
#endif
		DEBUG_B(state);
		DEMO(round, "start", state);

		sub_bytes(state);
		DEBUG_B(state);
		DEMO(round, "s_box", state);

		shift_rows(state);
		DEBUG_B(state);
		DEMO(round, "s_row", state);

		mix_columns(state);
		DEBUG_B(state);
		DEMO(round, "m_col", state);

		DEBUG_B_KEY(&round_keys[round * AES_Nb]);
		DEMO(round, "k_sch", (uint8_t *) &round_keys[round * AES_Nb]);
		add_round_key(state, &round_keys[round * AES_Nb]);
	}

#ifdef APPENDIX_B
	printf("Round %d\n", nr);
#endif
	DEBUG_B(state);
	DEMO(nr, "start", state);

	sub_bytes(state);
	DEBUG_B(state);
	DEMO(nr, "s_box", state);

	shift_rows(state);
	DEBUG_B(state);
	DEMO(nr, "s_row", state);

	DEBUG_B_KEY(&round_keys[nr * AES_Nb]);
	DEMO(nr, "k_sch", (uint8_t *) &round_keys[nr * AES_Nb]);
	add_round_key(state, &round_keys[nr * AES_Nb]);

	DEBUG_B(state);
	DEMO(nr, "output", state);
}

void decipher(uint8_t *state, word_t *round_keys, uint8_t nr)
{
	if (demo_mode)
		printf("\nINVERSE CIPHER (DECRYPT):\n");

	DEMO(0, "iinput", state);
	DEMO(0, "ik_sch", (uint8_t *) &round_keys[nr * AES_Nb]);
	add_round_key(state, &round_keys[nr * AES_Nb]);

	for (int round = nr - 1; round > 0; round--) {
		DEMO(nr - round, "istart", state);

		shift_rows_i(state);
		DEMO(nr - round, "is_row", state);

		sub_bytes_i(state);
		DEMO(nr - round, "is_box", state);

		DEMO(nr - round, "ik_sch", (uint8_t *) &round_keys[round * AES_Nb]);
		add_round_key(state, &round_keys[round * AES_Nb]);
		DEMO(nr - round, "ik_add", state);

		mix_columns_i(state);
	}

	DEMO(nr, "istart", state);

	shift_rows_i(state);
	DEMO(nr, "is_row", state);

	sub_bytes_i(state);
	DEMO(nr, "is_sbox", state);

	DEMO(nr, "ik_sch", (uint8_t *) round_keys);
	add_round_key(state, round_keys);

	DEMO(nr, "ioutput", state);
}

// TEST CODE

// Unguarded comma literals, sigh
#define _UNPAREN(...) __VA_ARGS__
#define INVOKE(expr) expr
#define UNPAREN(args) INVOKE(_UNPAREN args)

#define ASSERT(x) if (!(x)) printf("Failed " # x "\n");
#define ASSERTEQ(x, y) if ((x) != (y)) printf("Failed " # x " = " # y "; 0x%x\n", (x));

#define ASSERT_WORD_MATCH(FUNC, X, Y) do { \
	word_t in = {{ UNPAREN(X) }}; \
	word_t out = FUNC(in); \
	word_t expected = {{ UNPAREN(Y) }}; \
	if (memcmp(&out, &expected, sizeof(word_t)) != 0) \
		printf( \
			"Failed: " #FUNC "(" #X ") != " #Y "; 0x%02x%02x%02x%02x\n", \
			out.byte[0], out.byte[1], out.byte[2], out.byte[3] \
		); \
} while(0);

#define ASSERTINVSAME(FUNC, X, Y) do { \
	FUNC(X); \
	FUNC ## _i(X); \
	if (memcmp((X), (Y), sizeof (X)) != 0) { \
		printf("Failed " #FUNC " inversion\n"); \
		DUMP_STATE(X); \
	} \
} while (0);

static void test()
{
	ASSERTEQ(FFADD(0x57, 0x83), 0xd4);

	ASSERTEQ(xtime(0x57), 0xae);
	ASSERTEQ(xtime(0xae), 0x47);
	ASSERTEQ(xtime(0x47), 0x8e);
	ASSERTEQ(xtime(0x8e), 0x07);

	ASSERTEQ(poly_mult(0x57, 0x13), 0xfe);

	ASSERT_WORD_MATCH(sub_word, (0x00, 0x10, 0x20, 0x30), (0x63, 0xca, 0xb7, 0x04));
	ASSERT_WORD_MATCH(sub_word, (0x40, 0x50, 0x60, 0x70), (0x09, 0x53, 0xd0, 0x51));
	ASSERT_WORD_MATCH(sub_word, (0x80, 0x90, 0xa0, 0xb0), (0xcd, 0x60, 0xe0, 0xe7));
	ASSERT_WORD_MATCH(sub_word, (0xc0, 0xd0, 0xe0, 0xf0), (0xba, 0x70, 0xe1, 0x8c));

	ASSERT_WORD_MATCH(rot_word, (0x09, 0xcf, 0x4f, 0x3c), (0xcf, 0x4f, 0x3c, 0x09));
	ASSERT_WORD_MATCH(rot_word, (0x09, 0xcf, 0x4f, 0x3c), (0xcf, 0x4f, 0x3c, 0x09));
	ASSERT_WORD_MATCH(rot_word, (0x2a, 0x6c, 0x76, 0x05), (0x6c, 0x76, 0x05, 0x2a));

	uint8_t base[AES_Nb * sizeof (word_t)] = {
		0x00, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b,
		0x0c, 0x0d, 0x0e, 0x0f,
	};
	uint8_t test[sizeof (base)];
	memcpy(test, base, sizeof (test));

	ASSERTINVSAME(mix_columns, test, base);
	ASSERTINVSAME(sub_bytes, test, base);
	ASSERTINVSAME(shift_rows, test, base);

	uint8_t key[16] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
		0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
	};
	uint8_t expanded[] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,
		0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05,
		0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f,
		0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b,
		0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00,
		0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2, 0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc,
		0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41, 0xca, 0x00, 0x93, 0xfd,
		0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f,
		0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f,
		0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e,
		0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6,
	};
	word_t w[sizeof (expanded) / sizeof (word_t)] = {0};
	int nr = (sizeof (key) / sizeof (word_t)) + 6;
	expand_key(key, sizeof (key), w, nr);
	ASSERT(memcmp(expanded, w, sizeof(expanded)) == 0);

	uint8_t b_in[16] = {
		0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d,
		0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34,
	};
	uint8_t b_expected[16] = {
		0x39, 0x25, 0x84, 0x1d,
		0x02, 0xdc, 0x09, 0xfb,
		0xdc, 0x11, 0x85, 0x97,
		0x19, 0x6a, 0x0b, 0x32,
	};
	uint8_t state[sizeof (b_in)];
	memcpy(state, b_in, sizeof (state));
	cipher(state, w, nr);
	ASSERT(memcmp(state, b_expected, sizeof (state)) == 0);

	decipher(state, w, nr);
	ASSERT(memcmp(state, b_in, sizeof (state)) == 0);
}

static void print_key_state(uint8_t *key, uint8_t key_bytes, uint8_t *state)
{
	printf("PLAINTEXT:          ");
	DEMO_BYTES(state, (AES_Nb * sizeof (word_t)));
	printf("KEY:                ");
	DEMO_BYTES(key, key_bytes);
}

static void demo()
{
	demo_mode = true;

	uint8_t key[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
	};
	uint8_t state[16] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
	};

	uint8_t nr;
	word_t w[sizeof (key) * (1 + (sizeof (key) / sizeof (word_t)))] = {0};

	nr = 10;
	expand_key(key, 16, w, nr);
	print_key_state(key, 16, state);
	cipher(state, w, nr);
	decipher(state, w, nr);
	printf("\n\n");

	nr = 12;
	expand_key(key, 24, w, nr);
	print_key_state(key, 24, state);
	cipher(state, w, nr);
	decipher(state, w, nr);
	printf("\n\n");

	nr = 14;
	expand_key(key, sizeof (key), w, nr);
	print_key_state(key, sizeof (key), state);
	cipher(state, w, nr);
	decipher(state, w, nr);

	demo_mode = false;
}

// SETUP

// input: key (size inferred, not CLI... file based), input over stdin, output over stdout
int main(int argc, char *argv[])
{
	if (argc == 1) {
		demo();
		return 0;
	}

	uint8_t key_bytes = 0;
	uint8_t key[sizeof (word_t) * 8]; // we only support 4/6/8

	bool reverse = false;
	char opt;
	while (-1 != (opt = getopt(argc, argv, "dk:rt"))) {
		switch (opt) {
			case 't':
				test();
				break;
			case 'd':
				demo_mode = true;
				break;
			case 'r':
				reverse = true;
				break;
			case 'k': {
				struct stat st;
				stat(optarg, &st);
				switch (st.st_size / sizeof (word_t)) {
					// only allow keys w/4/6/8 words
					case 4:
					case 6:
					case 8:
						key_bytes = st.st_size;
						FILE *fp = fopen(optarg, "rb");
						fread(key, st.st_size, 1, fp);
						if (EOF == fclose(fp)) {
							printf("Error _closing_ file, terminating\n");
							return -1;
						}
						break;
					default:
						printf("Key must be 4, 6, or 8 words long\n");
						return 1;
				}
				break;
			}
			case '?':
			default:
				printf("Unhandled argument %c\n", optopt);
				return 1;
		}
	}

	if (key_bytes == 0)
		return 0;

	uint8_t nr = key_bytes / sizeof (word_t) + 6; // = words in key + 6; might not generalize outside of 4/6/8

	// max size buffer; can always use less
	word_t round_keys[AES_Nb * MAX_KEY_WORDS * (1 + MAX_KEY_WORDS)] = {0};
	expand_key(key, key_bytes, round_keys, nr);

	uint8_t state[AES_Nb * sizeof (word_t)] = {0}; // I give up - column major it is
	freopen(NULL, "rb", stdin);
	size_t read = fread(state, sizeof (state), 1, stdin);
	if (read != 1) {
		printf("Input too short\n");
		return 2;
	}
	// TODO: check if input is too long; fail if is

	if (demo_mode) {
		print_key_state(key, key_bytes, state);
	}

	if (reverse)
		decipher(state, round_keys, nr);
	else
		cipher(state, round_keys, nr);

	for (int i = 0; i < sizeof (state); i++)
		printf("%02x", state[i]);
	printf("\n");

	return 0;
}
