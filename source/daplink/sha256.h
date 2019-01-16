#ifndef SHA256_H_
#define SHA256_H_

#ifdef __cplusplus
 extern "C" {
#endif

    void calc_sha_256(uint8_t hash[32], const void *input, size_t len);

#ifdef __cplusplus
}
#endif

#endif // SHA256_H_
