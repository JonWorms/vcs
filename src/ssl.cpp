#include "ssl.hpp"
#include <openssl/ssl.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/x509.h>

using namespace web;
using namespace std;

RSA* generate_rsa() {
	int err = 1;
	BIGNUM *ex = BN_new();
	RSA *rsa = RSA_new();

	if(err == 1) {
		err = BN_set_word(ex, RSA_F4);
	}

	if(err == 1) {
		err = RSA_generate_key_ex(rsa, 2048, ex, NULL);
	}
	BN_free(ex);
	if(err == 1) {
		return rsa;
	}

	RSA_free(rsa);
	return NULL;
}

EVP_PKEY* create_pkey() {
	RSA *rsa = generate_rsa();
	EVP_PKEY *pkey;
	
	if(EVP_PKEY_assign_RSA(pkey, rsa) == 1) {
		return pkey;
	} else {
		RSA_free(rsa);
		EVP_PKEY_free(pkey);
		return NULL;
	}
}


X509* create_cert(const unsigned char *c, const unsigned char *on, const unsigned char *cn, const unsigned char *dnsn, EVP_PKEY **pkey_addr) {

	EVP_PKEY *pkey = *pkey_addr;
	X509 *cert = X509_new();

	ASN1_INTEGER_set(X509_get_serialNumber(cert), 1);
	X509_gmtime_adj(X509_get_notBefore(cert), 0);
	X509_gmtime_adj(X509_get_notAfter(cert), 3155760000); // 100 years

	if(X509_set_pubkey(cert, pkey) != 1) {
		X509_free(cert);
		return NULL;
	}

	pkey = NULL;

	
	X509_NAME *name = X509_get_subject_name(cert);
	X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, c, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, cn, -1, -1, 0);
	X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, on, -1, -1, 0);

	X509_set_issuer_name(cert, name);
	X509_sign(cert, pkey, EVP_sha1());
	return cert;


} 



void ssl::generate_self_signed_cert() {

	const unsigned char *c  = reinterpret_cast<const unsigned char*>("United States of America");
	const unsigned char *cn = reinterpret_cast<const unsigned char*>("USA");
	const unsigned char *on = reinterpret_cast<const unsigned char*>("vcs");
	//const unsigned char *cert_file = reinterpret_cast<const unsigned char*>("cert.pem");
	//const unsigned char *key_file = reinterpret_cast<const unsigned char*>("key.pem");
	unsigned char *pw = (unsigned char*) "";
		
	
	EVP_PKEY *pkey = create_pkey();
	if(pkey == NULL) {
		return;
	}

	FILE *key_pem = fopen("key.pem", "wb");
	PEM_write_PrivateKey(key_pem, pkey, EVP_des_ede3_cbc(), pw, 0, NULL, NULL);
	fclose(key_pem);

	
	X509 *cert = create_cert(c, on, cn, NULL, &pkey);
	if(cert == NULL) {
		EVP_PKEY_free(pkey);
		return;
	}
	
	FILE *cert_pem = fopen("cert.pem", "wb");
	PEM_write_X509(cert_pem, cert);
	fclose(cert_pem);
	
	

	
		
	//BN_free(ex);
	//x509_free(cert);

}

bool ssl::ssl_directory_exists() {
	//SSL_CERT_DIRECTORY
	return filesystem::exists(SSL_CERT_DIRECTORY);
}

bool ssl::ssl_file_exists(string filename) {
	filesystem::path filepath(SSL_CERT_DIRECTORY);
	filepath /= filename;
	return filesystem::exists(filepath); //(to_string(SSL_CERT_DIRECTORY) + filename)
}

void ssl::get_certificates() {
	if(!ssl_directory_exists()) {
		std::cout << "cert directory does not exist" << std::endl;
	} 

	if(!(ssl_file_exists("vcs.crt") && ssl_file_exists("vcs.key"))) {
		std::cout << "generating self-signed ssl certificate" << std::endl;
		generate_self_signed_cert();
	}
	
	std::cout << "getting ssl cert files" << std::endl;
}