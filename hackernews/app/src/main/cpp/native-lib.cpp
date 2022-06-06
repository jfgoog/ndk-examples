#include <jni.h>
#include <optional>
#include <string>

#include "http.h"
#include "java_interop.h"
#include "json/json.h"
#include "logging.h"

namespace hackernews {
namespace {

std::vector<std::string> GetTitles(const std::string& cacert) {
  auto client = http::Client(cacert);
  std::string error;
  auto result = client.get(
            "https://hacker-news.firebaseio.com/v0/beststories.json",
            &error);
  if (!result) {
    return {error.c_str()};
  }

  Json::Value root;
  std::istringstream(result.value()) >> root;
  std::vector<std::string> ids;
  int i = 0;
  for (const auto& id : root) {
      if (++i > 10) break;
      result = client.get(
                "https://hacker-news.firebaseio.com/v0/item/" + id.asString() + ".json",
                &error);
      if (!result) {
        return {error.c_str()};
      }
      Json::Value item;
      std::istringstream(result.value()) >> item;
      ids.push_back(item["title"].asString());
  }
  return ids;
}

}  // namespace
}  // namespace hackernews

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_example_hackernews_MainActivity_getHackerNews(
        JNIEnv *env,
        jobject /* this */,
        jstring cacert_java) {
    if (cacert_java == nullptr) {
        hackernews::logging::FatalError(env, "cacert argument cannot be null");
    }

    const std::string cacert =
            hackernews::jni::Convert<std::string>::from(env, cacert_java);
    return hackernews::jni::Convert<jobjectArray, jstring>::from(env,
                                                   hackernews::GetTitles(cacert));

}