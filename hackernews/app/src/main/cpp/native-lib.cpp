#include <jni.h>
#include <optional>
#include <string>

#include "http.h"
#include "java_interop.h"
#include "logging.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hackernews_MainActivity_getHackerNews(
        JNIEnv *env,
        jobject /* this */,
        jstring cacert_java) {
    if (cacert_java == nullptr) {
        hackernews::logging::FatalError(env, "cacert argument cannot be null");
    }

    const std::string cacert =
            hackernews::jni::Convert<std::string>::from(env, cacert_java);
    auto client = hackernews::http::Client(cacert);
    std::string error;
    auto result = client.get(
            "https://hacker-news.firebaseio.com/v0/beststories.json?print=pretty",
            &error);
    if (!result) {
        return env->NewStringUTF(error.c_str());
    }
    return env->NewStringUTF(result->c_str());
}