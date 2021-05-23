#ifndef _BASE64_H_

#define _BASE64_H_

size_t b64_encoded_size(size_t inlen);

char *b64_encode(const unsigned char *in, size_t len);

#endif //_BASE64_H_

