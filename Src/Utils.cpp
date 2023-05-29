/*
 * crow.cpp
 *
 *  Created on: 25 апр. 2023 г.
 *      Author: mr_s
 */

#define CROW_MAIN
#include "crow.h"
#include "crow/json.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <curl/curl.h>
#include <string>
#include <Utils.h>

namespace NFB::Atd {

CURL *libCurl = nullptr;

Initializer::Initializer()
{
	libCurl = curl_easy_init();
	if(!libCurl)
		ERROR_RN << "Ошибка инициализации libCurl";

}

Initializer::~Initializer()
{
	if(libCurl)
		curl_easy_cleanup(libCurl);
}

//------------------------------ HTTP ------------------------------
size_t curlWriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	std::string* str = (std::string*) userdata;
	str->append(ptr, size * nmemb);
	return size * nmemb;
}

std::string makeRequestGet(const std::string &url)
{
	std::string data;
	curl_easy_setopt(libCurl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(libCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(libCurl, CURLOPT_WRITEFUNCTION, &curlWriteCallback);
	curl_easy_setopt(libCurl, CURLOPT_WRITEDATA, &data);
	CURLcode res = curl_easy_perform(libCurl);
	if(res != CURLE_OK)
		std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

	return data;
}

std::string base64Encode(const std::string &input)
{
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    bio = BIO_new(BIO_s_mem());
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(bio, input.c_str(), input.length());
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    return result;
}

std::string base64Decode(const std::string &input)
{
    BIO *bio, *b64;
    int decodedLength = 0;
    char *decodedBuffer;

    bio = BIO_new_mem_buf(input.c_str(), -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    decodedBuffer = (char*)malloc(input.length()*4/3);
    decodedLength = BIO_read(bio, decodedBuffer, input.length());
    BIO_free_all(bio);
    std::string result(decodedBuffer, decodedLength);
    free(decodedBuffer);

    return result;
}

//char* base64(const unsigned char *input, int length) {
//	const auto pl = 4*((length+2)/3);
//	auto output = reinterpret_cast<char *>(calloc(pl+1, 1)); //+1 for the terminating null that EVP_EncodeBlock adds on
//	const auto ol = EVP_EncodeBlock(reinterpret_cast<unsigned char *>(output), input, length);
//	if (pl != ol) { std::cerr << "Whoops, encode predicted " << pl << " but we got " << ol << "\n"; }
//	return output;
//}
//
//unsigned char* decode64(const char *input, int length) {
//	const auto pl = 3*length/4;
//	auto output = reinterpret_cast<unsigned char *>(calloc(pl+1, 1));
//	const auto ol = EVP_DecodeBlock(output, reinterpret_cast<const unsigned char *>(input), length);
//	if (pl != ol) { std::cerr << "Whoops, decode predicted " << pl << " but we got " << ol << "\n"; }
//	return output;
//}

size_t stringTimeToSeconds(const std::string &str)
{
	std::tm datetime = {};

	std::istringstream ss(str);
	ss >> std::get_time(&datetime, "%Y-%m-%dT%H:%M:%SZ%z");

	std::time_t time = std::mktime(&datetime);
	double seconds = std::difftime(time, 0);

	return seconds;
}

//------------------------------ UUID ------------------------------
std::string generateUuidV4()
{
#ifdef ATTEND_DEBUG
	static size_t Count = 0;
	return std::to_string(Count++);
#else
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<int> distribution(0, 255);

    std::array<unsigned char, 16> bytes;
    for (auto& byte : bytes)
        byte = static_cast<unsigned char>(distribution(generator));

    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    bytes[8] = (bytes[8] & 0x3F) | 0x80;

    std::stringstream stream;
    stream << std::hex;// << std::uppercase;

    for (size_t i = 0; i < bytes.size(); i++)
    {
        if (i == 4 || i == 6 || i == 8 || i == 10)
            stream << "-";

        stream << std::setw(2) << std::setfill('0') << int(bytes[i]);
    }

    return stream.str();
#endif
}

std::string generateUuidV4_time_track() {
    std::string uuid;
    std::array<int, 16> bits;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    for (int i = 0; i < 16; i++) {
        bits[i] = dis(gen);
    }
    bits[6] = (bits[6] & 0x0f) | 0x40;
    bits[8] = (bits[8] & 0x3f) | 0x80;
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    uint64_t nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    uint64_t timestamp = nanoseconds / 100 + 122192928000000000;
    for (int i = 0; i < 16; i++) {
        if (i == 6) {
            uuid += "4";
            continue;
        }
        if (i == 8) {
            uuid += "8";
            continue;
        }
        int index = i * 4;
        int value = bits[i];
        char hex[4];
        sprintf(hex, "%02x", value);
        uuid += hex[0];
        uuid += hex[1];
    }
    uuid.insert(8, "-");
    uuid.insert(13, "-");
    uuid.insert(18, "-");
    uuid.insert(23, "-");
    return uuid;
}

}
