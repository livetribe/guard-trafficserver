/*
 * Copyright 2014 the original author or authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the license at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
 */

#include "ltguard/utils.h"

#include <stdio.h>
#include <ts/ts.h>

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/evp.h>
#include <openssl/x509.h>

int
check_ts_version() {
	const char *ts_version = TSTrafficServerVersionGet();
	int result = 0;

	if (ts_version) {
		int major_ts_version = 0;
		int minor_ts_version = 0;
		int patch_ts_version = 0;

		if (sscanf(ts_version, "%d.%d.%d", &major_ts_version, &minor_ts_version, &patch_ts_version) != 3) {
			return 0;
  		}

		/* We need at least Traffic Server 2.0 */

		if (major_ts_version >= 2) {
			result = 1;
		}

	}

	return result;
}

RSA* getRsaFp(const char* rsapubKeyPath)
{
	LtGuardLogError("getRsaFp called");
	FILE* fp;
	fp = fopen( rsapubKeyPath, "r" );
	if ( fp == 0 ) {
		LtGuardLogError("Couldn't open RSA priv key: '%s'. %s", rsapubKeyPath, strerror(errno));
	    return NULL;
	}

	RSA *rsa = 0;
	rsa = RSA_new();
	if (rsa == 0) {
		LtGuardLogError("Couldn't create new RSA pub key obj.");
		unsigned long sslErr = ERR_get_error();
		if ( sslErr ) LtGuardLogError("%s", ERR_error_string(sslErr, 0));
		fclose( fp );
		return NULL;
	}

	rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
	if (rsa == 0) {
		LtGuardLogError("Couldn't use RSA pub keyfile.");
		unsigned long sslErr = ERR_get_error();
		if (sslErr) LtGuardLogError("%s", ERR_error_string(sslErr, 0));
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return rsa;
}

void
check_ssl_rv(const char *function_name,
             const int return_value,
             const int success_value)
{
     if (return_value != success_value) {
          fprintf(stderr,
                  "Error, return value of %s was %d",
                  function_name,
                  return_value);
          fprintf(stderr, " should have been %d\n", success_value);
          fprintf(stderr, "OpenSSL error messages:\n");
          ERR_print_errors_fp(stderr);
          exit(ERR_get_error());
     }
}
int
verify_data_evp(EVP_PKEY *key,
                FILE *data_file,
                FILE *signature_file)
{
     unsigned char *data;
     int data_len;

     unsigned char *sig;
     int sig_len;

     int result;
     int rv;

     EVP_MD_CTX *ctx = EVP_MD_CTX_create();

     const EVP_MD *md = EVP_get_digestbyname("SHA1");

     if (!md) {
          fprintf(stderr, "Error creating message digest");
          fprintf(stderr, " object, unknown name?\n");
          ERR_print_errors_fp(stderr);
          exit(1);
     }

     rv = EVP_VerifyInit_ex(ctx, md, NULL);
     check_ssl_rv("EVP_VerifyInit_ex", rv, 1);

     data = (unsigned char *)malloc(EVP_MD_size(md));
     data_len = fread(data, 1, EVP_MD_size(md), data_file);
     while (data_len > 0) {
          EVP_VerifyUpdate(ctx, data, data_len);
          data_len = fread(data, 1, 1024, data_file);
     }

     sig = (unsigned char *)malloc(512);
     sig_len = fread(sig, 1, 512, signature_file);

     result = EVP_VerifyFinal(ctx, sig, sig_len,  key);

     EVP_MD_CTX_destroy(ctx);

     free(data);
     free(sig);

     return result;
}
