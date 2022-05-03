#include <gtest/gtest.h>
#include "shoc/cipher/aes.h"
#include "shoc/mode/ecb.h"
#include "shoc/mode/cbc.h"
#include "shoc/mode/cfb.h"
#include "shoc/mode/ofb.h"
#include "shoc/mode/ctr.h"
#include "shoc/mode/ccm.h"
#include "shoc/mode/gcm.h"

using namespace shoc;

static const byte test_key[16] = {
    0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c, 
};

static const byte test_in[64] = {
    0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 
    0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 
    0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
    0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10,
};

static void compare(byte *out, const byte *exp, size_t len)
{
    ASSERT_TRUE(out);
    ASSERT_TRUE(exp);

    for (size_t i = 0; i < len; ++i) {
        ASSERT_EQ(out[i], exp[i]) << "At index " << i;
    }
}

TEST(Ecb, EncryptDecryptAes128)
{
    const byte exp[64] = {
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4,
    };
    byte out[64] = {};

    ecb_encrypt<Aes>(test_key, test_in, out, sizeof(test_in));
    compare(out, exp, sizeof(out));
    ecb_decrypt<Aes>(test_key, out, out, sizeof(out));
    compare(out, test_in, sizeof(test_in));
}

TEST(Cbc, EncryptDecryptAes128)
{
    const byte iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    const byte exp[64] = {
        0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
        0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
        0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
        0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7,
    };
    byte out[64] = {};

    cbc_encrypt<Aes>(test_key, iv, test_in, out, sizeof(test_in));
    compare(out, exp, sizeof(out));
    cbc_decrypt<Aes>(test_key, iv, out, out, sizeof(out));
    compare(out, test_in, sizeof(test_in));
}

TEST(Cfb, EncryptDecryptAes128)
{
    const byte iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    const byte exp[64] = {
        0x3b, 0x3f, 0xd9, 0x2e, 0xb7, 0x2d, 0xad, 0x20, 0x33, 0x34, 0x49, 0xf8, 0xe8, 0x3c, 0xfb, 0x4a,
        0xc8, 0xa6, 0x45, 0x37, 0xa0, 0xb3, 0xa9, 0x3f, 0xcd, 0xe3, 0xcd, 0xad, 0x9f, 0x1c, 0xe5, 0x8b,
        0x26, 0x75, 0x1f, 0x67, 0xa3, 0xcb, 0xb1, 0x40, 0xb1, 0x80, 0x8c, 0xf1, 0x87, 0xa4, 0xf4, 0xdf,
        0xc0, 0x4b, 0x05, 0x35, 0x7c, 0x5d, 0x1c, 0x0e, 0xea, 0xc4, 0xc6, 0x6f, 0x9f, 0xf7, 0xf2, 0xe6,
    };
    byte out[64] = {};

    cfb_encrypt<Aes>(test_key, iv, test_in, out, sizeof(test_in));
    compare(out, exp, sizeof(out));
    cfb_decrypt<Aes>(test_key, iv, out, out, sizeof(out));
    compare(out, test_in, sizeof(test_in));
}

TEST(Ofb, EncryptDecryptAes128)
{
    const byte iv[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
    const byte exp[64] = {
        0x3b, 0x3f, 0xd9, 0x2e, 0xb7, 0x2d, 0xad, 0x20, 0x33, 0x34, 0x49, 0xf8, 0xe8, 0x3c, 0xfb, 0x4a,
        0x77, 0x89, 0x50, 0x8d, 0x16, 0x91, 0x8f, 0x03, 0xf5, 0x3c, 0x52, 0xda, 0xc5, 0x4e, 0xd8, 0x25,
        0x97, 0x40, 0x05, 0x1e, 0x9c, 0x5f, 0xec, 0xf6, 0x43, 0x44, 0xf7, 0xa8, 0x22, 0x60, 0xed, 0xcc,
        0x30, 0x4c, 0x65, 0x28, 0xf6, 0x59, 0xc7, 0x78, 0x66, 0xa5, 0x10, 0xd9, 0xc1, 0xd6, 0xae, 0x5e,
    };
    byte out[64] = {};

    ofb_encrypt<Aes>(test_key, iv, test_in, out, sizeof(test_in));
    compare(out, exp, sizeof(out));
    ofb_decrypt<Aes>(test_key, iv, out, out, sizeof(out));
    compare(out, test_in, sizeof(test_in));
}

TEST(Ctr, EncryptDecryptAes128)
{
    const byte iv[16] = {
        0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
    };
    const byte exp[64] = {
        0x87, 0x4d, 0x61, 0x91, 0xb6, 0x20, 0xe3, 0x26, 0x1b, 0xef, 0x68, 0x64, 0x99, 0x0d, 0xb6, 0xce,
        0x98, 0x06, 0xf6, 0x6b, 0x79, 0x70, 0xfd, 0xff, 0x86, 0x17, 0x18, 0x7b, 0xb9, 0xff, 0xfd, 0xff,
        0x5a, 0xe4, 0xdf, 0x3e, 0xdb, 0xd5, 0xd3, 0x5e, 0x5b, 0x4f, 0x09, 0x02, 0x0d, 0xb0, 0x3e, 0xab,
        0x1e, 0x03, 0x1d, 0xda, 0x2f, 0xbe, 0x03, 0xd1, 0x79, 0x21, 0x70, 0xa0, 0xf3, 0x00, 0x9c, 0xee,
    };
    byte out[64] = {};

    ctr_encrypt<Aes>(test_key, iv, test_in, out, sizeof(test_in));
    compare(out, exp, sizeof(exp));
    ctr_decrypt<Aes>(test_key, iv, out, out, sizeof(out));
    compare(out, test_in, sizeof(test_in));
}

TEST(Ccm, EncryptDecryptAes128)
{
    byte out[64];
    byte tag[64];

    const byte key[]     = { 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF };
    const byte nonce[]   = { 0x00, 0x00, 0x00, 0x03, 0x02, 0x01, 0x00, 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
    const byte aad[]     = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    const byte in[]      = { 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E };
    const byte exp_out[] = { 0x58, 0x8C, 0x97, 0x9A, 0x61, 0xC6, 0x63, 0xD2, 0xF0, 0x66, 0xD0, 0xC2, 0xC0, 0xF9, 0x89, 0x80, 0x6D, 0x5F, 0x6B, 0x61, 0xDA, 0xC3, 0x84 };
    const byte exp_tag[] = { 0x17, 0xE8, 0xD1, 0x2C, 0xFD, 0xF9, 0x26, 0xE0 };

    ASSERT_TRUE(ccm_encrypt<Aes>(key, nonce, aad, sizeof(aad), tag, 8, in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(ccm_decrypt<Aes>(key, nonce, aad, sizeof(aad), tag, 8, out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}

TEST(Gcm, EncryptDecryptAes128_1)
{
    byte tag[16];

    const byte key[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const byte iv[]         = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const byte exp_tag[]    = { 0x58, 0xe2, 0xfc, 0xce, 0xfa, 0x7e, 0x30, 0x61, 0x36, 0x7f, 0x1d, 0x57, 0xa4, 0xe7, 0x45, 0x5a };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), nullptr, nullptr, 0));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), nullptr, nullptr, 0));
}

TEST(Gcm, EncryptDecryptAes128_2)
{
    byte out[16];
    byte tag[16];

    const byte key[]        = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const byte in[]         = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const byte iv[]         = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const byte exp_out[]    = { 0x03, 0x88, 0xda, 0xce, 0x60, 0xb6, 0xa3, 0x92, 0xf3, 0x28, 0xc2, 0xb9, 0x71, 0xb2, 0xfe, 0x78 };
    const byte exp_tag[]    = { 0xab, 0x6e, 0x47, 0xd4, 0x2c, 0xec, 0x13, 0xbd, 0xf5, 0x3a, 0x67, 0xb2, 0x12, 0x57, 0xbd, 0xdf };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}

TEST(Gcm, EncryptDecryptAes128_3)
{
    byte out[64];
    byte tag[16];

    const byte key[]        = { 0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
    const byte in[]         = { 0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
                                0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
                                0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
                                0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39, 0x1a, 0xaf, 0xd2, 0x55 };
    const byte iv[]         = { 0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88 };
    const byte exp_out[]    = { 0x42, 0x83, 0x1e, 0xc2, 0x21, 0x77, 0x74, 0x24, 0x4b, 0x72, 0x21, 0xb7, 0x84, 0xd0, 0xd4, 0x9c,
                                0xe3, 0xaa, 0x21, 0x2f, 0x2c, 0x02, 0xa4, 0xe0, 0x35, 0xc1, 0x7e, 0x23, 0x29, 0xac, 0xa1, 0x2e,
                                0x21, 0xd5, 0x14, 0xb2, 0x54, 0x66, 0x93, 0x1c, 0x7d, 0x8f, 0x6a, 0x5a, 0xac, 0x84, 0xaa, 0x05,
                                0x1b, 0xa3, 0x0b, 0x39, 0x6a, 0x0a, 0xac, 0x97, 0x3d, 0x58, 0xe0, 0x91, 0x47, 0x3f, 0x59, 0x85 };
    const byte exp_tag[]    = { 0x4d, 0x5c, 0x2a, 0xf3, 0x27, 0xcd, 0x64, 0xa6, 0x2c, 0xf3, 0x5a, 0xbd, 0x2b, 0xa6, 0xfa, 0xb4 };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), nullptr, 0, tag, sizeof(exp_tag), out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}

TEST(Gcm, EncryptDecryptAes128_4)
{
    byte out[60];
    byte tag[16];

    const byte key[]        = { 0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
    const byte in[]         = { 0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
                                0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
                                0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
                                0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
    const byte aad[]        = { 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
                                0xab, 0xad, 0xda, 0xd2 };
    const byte iv[]         = { 0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad, 0xde, 0xca, 0xf8, 0x88 };
    const byte exp_out[]    = { 0x42, 0x83, 0x1e, 0xc2, 0x21, 0x77, 0x74, 0x24, 0x4b, 0x72, 0x21, 0xb7, 0x84, 0xd0, 0xd4, 0x9c,
                                0xe3, 0xaa, 0x21, 0x2f, 0x2c, 0x02, 0xa4, 0xe0, 0x35, 0xc1, 0x7e, 0x23, 0x29, 0xac, 0xa1, 0x2e,
                                0x21, 0xd5, 0x14, 0xb2, 0x54, 0x66, 0x93, 0x1c, 0x7d, 0x8f, 0x6a, 0x5a, 0xac, 0x84, 0xaa, 0x05,
                                0x1b, 0xa3, 0x0b, 0x39, 0x6a, 0x0a, 0xac, 0x97, 0x3d, 0x58, 0xe0, 0x91 };
    const byte exp_tag[]    = { 0x5b, 0xc9, 0x4f, 0xbc, 0x32, 0x21, 0xa5, 0xdb, 0x94, 0xfa, 0xe9, 0x5a, 0xe7, 0x12, 0x1a, 0x47 };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}

TEST(Gcm, EncryptDecryptAes128_5)
{
    byte out[60];
    byte tag[16];

    const byte key[]        = { 0xfe, 0xff, 0xe9, 0x92, 0x86, 0x65, 0x73, 0x1c, 0x6d, 0x6a, 0x8f, 0x94, 0x67, 0x30, 0x83, 0x08 };
    const byte in[]         = { 0xd9, 0x31, 0x32, 0x25, 0xf8, 0x84, 0x06, 0xe5, 0xa5, 0x59, 0x09, 0xc5, 0xaf, 0xf5, 0x26, 0x9a,
                                0x86, 0xa7, 0xa9, 0x53, 0x15, 0x34, 0xf7, 0xda, 0x2e, 0x4c, 0x30, 0x3d, 0x8a, 0x31, 0x8a, 0x72,
                                0x1c, 0x3c, 0x0c, 0x95, 0x95, 0x68, 0x09, 0x53, 0x2f, 0xcf, 0x0e, 0x24, 0x49, 0xa6, 0xb5, 0x25,
                                0xb1, 0x6a, 0xed, 0xf5, 0xaa, 0x0d, 0xe6, 0x57, 0xba, 0x63, 0x7b, 0x39 };
    const byte aad[]        = { 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef, 0xfe, 0xed, 0xfa, 0xce, 0xde, 0xad, 0xbe, 0xef,
                                0xab, 0xad, 0xda, 0xd2 };
    const byte iv[]         = { 0xca, 0xfe, 0xba, 0xbe, 0xfa, 0xce, 0xdb, 0xad };
    const byte exp_out[]    = { 0x61, 0x35, 0x3b, 0x4c, 0x28, 0x06, 0x93, 0x4a, 0x77, 0x7f, 0xf5, 0x1f, 0xa2, 0x2a, 0x47, 0x55,
                                0x69, 0x9b, 0x2a, 0x71, 0x4f, 0xcd, 0xc6, 0xf8, 0x37, 0x66, 0xe5, 0xf9, 0x7b, 0x6c, 0x74, 0x23,
                                0x73, 0x80, 0x69, 0x00, 0xe4, 0x9f, 0x24, 0xb2, 0x2b, 0x09, 0x75, 0x44, 0xd4, 0x89, 0x6b, 0x42,
                                0x49, 0x89, 0xb5, 0xe1, 0xeb, 0xac, 0x0f, 0x07, 0xc2, 0x3f, 0x45, 0x98 };
    const byte exp_tag[]    = { 0x36, 0x12, 0xd2, 0xe7, 0x9e, 0x3b, 0x07, 0x85, 0x56, 0x1b, 0xe1, 0x4a, 0xac, 0xa2, 0xfc, 0xcb };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}

TEST(Gcm, EncryptDecryptAes128_Special)
{
    byte out[51];
    byte tag[14];

    const byte key[]        = { 0x8d, 0x10, 0x04, 0x4f, 0x01, 0x4f, 0xc2, 0x45, 0xf6, 0xac, 0xd4, 0x61, 0x11, 0xa4, 0x07, 0x46 };
    const byte in[]         = { 0x4b, 0x04, 0x29, 0xbb, 0x8f, 0x76, 0x63, 0x52, 0x71, 0x5a, 0x24, 0xd4, 0xaa, 0xaa, 0x8b, 0x4d, 
                                0xa1, 0x25, 0xe1, 0x6c, 0xff, 0xf2, 0xb2, 0xe9, 0xb1, 0x8e, 0x5c, 0xba, 0x33, 0xed, 0xf5, 0x56, 
                                0x58, 0x01, 0x18, 0xa5, 0x9c, 0x51, 0xa9, 0xf1, 0xe9, 0x7a, 0x71, 0x7b, 0x40, 0x10, 0x32, 0xc9, 
                                0x4a, 0x34, 0x23 };
    const byte aad[]        = { 0xd5, 0xa4, 0xb4, 0x5c, 0xd2, 0x31, 0x2f, 0x20, 0x59, 0x55, 0xa2, 0x06, 0xe9, 0xdf, 0xaa, 0xad };
    const byte iv[]         = { 0x18, 0xf9, 0xfc, 0x16, 0x35, 0xdc, 0x5a, 0x43, 0xcc, 0xcd, 0x0d, 0xd9, 0x37, 0x96, 0x36, 0x04, 
                                0x04, 0xaa, 0x5c, 0x4d, 0x0d, 0x43, 0xc7, 0x76, 0x12, 0xe9, 0x77, 0x21, 0xc3, 0x42, 0x81, 0x33, 
                                0x42, 0xad, 0x7e, 0x24, 0x8c, 0xfd, 0xfd, 0x0e, 0x6c, 0x3a, 0xf1, 0xef, 0x51, 0x91, 0xaf, 0xa3, 
                                0x5f, 0x01, 0x73, 0x0c, 0x77, 0x98, 0xac, 0xc0, 0x3f, 0x21, 0xf9, 0x04, 0xbf, 0x4a, 0x1e, 0x44, 
                                0x26, 0x9e, 0xf6, 0x96, 0x3d, 0xff, 0x57, 0x83, 0x31, 0x2f, 0x63, 0xb5, 0xfd, 0x7e, 0x69, 0xc2,
                                0x2d, 0x2e, 0x6e, 0xd0, 0x14, 0xff, 0xa1, 0xa5, 0x62, 0x1c, 0xbd, 0xf8, 0x20, 0x57, 0x29, 0xab, 
                                0x71, 0x21, 0x82, 0x8b, 0xea, 0xd4, 0xa0, 0x91, 0x8c, 0xe2, 0xbe, 0xf7, 0x14, 0x9f, 0x3c, 0x60, 
                                0x86, 0x02, 0x86, 0x32, 0xa3, 0x17, 0x19, 0x61, 0xff, 0x94, 0x3d, 0xbf, 0xd1, 0xf9, 0x94, 0x43 };
    const byte exp_out[]    = { 0x11, 0x09, 0xfa, 0xf5, 0x10, 0x0b, 0x3c, 0x17, 0xfc, 0x43, 0x70, 0xc5, 0xae, 0xd2, 0xa4, 0xfe, 
                                0xbd, 0x4c, 0x08, 0x7f, 0xa8, 0x95, 0xe1, 0x34, 0x1c, 0x91, 0x00, 0xa9, 0xde, 0xbd, 0x3c, 0x89, 
                                0x88, 0x74, 0x28, 0x9e, 0x69, 0x18, 0x03, 0x58, 0x37, 0xb1, 0xa7, 0x7d, 0x9f, 0xab, 0x13, 0xc9, 
                                0x0f, 0x8d, 0x66 };
    const byte exp_tag[]    = { 0xd8, 0xee, 0x9e, 0xc0, 0xff, 0xef, 0x3d, 0xcc, 0xb9, 0xdc, 0x90, 0xb0, 0x82, 0x6a };

    ASSERT_TRUE(gcm_encrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), in, out, sizeof(in)));
    compare(out, exp_out, sizeof(exp_out));
    compare(tag, exp_tag, sizeof(exp_tag));
    ASSERT_TRUE(gcm_decrypt<Aes>(key, iv, sizeof(iv), aad, sizeof(aad), tag, sizeof(exp_tag), out, out, sizeof(in)));
    compare(out, in, sizeof(in));
}